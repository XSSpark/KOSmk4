/* Copyright (c) 2019 Griefer@Work                                            *
 *                                                                            *
 * This software is provided 'as-is', without any express or implied          *
 * warranty. In no event will the authors be held liable for any damages      *
 * arising from the use of this software.                                     *
 *                                                                            *
 * Permission is granted to anyone to use this software for any purpose,      *
 * including commercial applications, and to alter it and redistribute it     *
 * freely, subject to the following restrictions:                             *
 *                                                                            *
 * 1. The origin of this software must not be misrepresented; you must not    *
 *    claim that you wrote the original software. If you use this software    *
 *    in a product, an acknowledgement in the product documentation would be  *
 *    appreciated but is not required.                                        *
 * 2. Altered source versions must be plainly marked as such, and must not be *
 *    misrepresented as being the original software.                          *
 * 3. This notice may not be removed or altered from any source distribution. *
 */
#ifndef GUARD_LIBDL_MODULE_C
#define GUARD_LIBDL_MODULE_C 1
#define _KOS_SOURCE 1
#define _KOS_KERNEL_SOURCE 1
#define _GNU_SOURCE 1

/* Keep this one the first */
#include "elf.h"
/**/

#include <kos/debugtrap.h>
#include <kos/syscalls.h>

#include <errno.h>
#include <fcntl.h>
#include <malloc.h>
#include <signal.h>
#include <string.h>
#include <unistd.h>

DECL_BEGIN

INTERN ElfW(Shdr) empty_shdr[1] = { 0 };

/* DlModule functions */
INTERN NONNULL((1)) void CC
DlModule_Destroy(DlModule *__restrict self) {
	uint16_t i;
	uintptr_t fini_func        = 0;
	uintptr_t *fini_array_base = NULL;
	size_t fini_array_size     = 0;

	/* Unbind the module from the list of all modules. */
	atomic_rwlock_write(&DlModule_AllLock);
	DlModule_RemoveFromAll(self);
	atomic_rwlock_endwrite(&DlModule_AllLock);

	/* Trigger the trap informing a debugger of the change in loaded libraries. */
	if (!sys_debugtrap_disabled) {
		struct debugtrap_reason r;
		r.dtr_signo  = SIGTRAP;
		r.dtr_reason = DEBUGTRAP_REASON_LIBRARY;
		if (sys_debugtrap(NULL, &r) == -ENOENT)
			sys_debugtrap_disabled = true;
	}

	/* Unbind the module from the global symbol table. */
	assert(LLIST_ISBOUND(self, dm_globals) ==
	       ((self->dm_flags & RTLD_GLOBAL) != 0));
	if (LLIST_ISBOUND(self, dm_globals)) {
		atomic_rwlock_write(&DlModule_GlobalLock);
		DlModule_RemoveFromGlobals(self);
		LLIST_UNBIND(self, dm_globals);
		atomic_rwlock_endwrite(&DlModule_GlobalLock);
	}

	/* Skip finalizers if the library . */
	if unlikely(self->dm_flags & RTLD_NOINIT)
		goto done_fini;
	for (i = 0; i < self->dm_dyncnt; ++i) {
		switch (self->dm_dynhdr[i].d_tag) {

		case DT_NULL:
			goto done_dyntag;

		case DT_FINI:
			fini_func = (uintptr_t)self->dm_dynhdr[i].d_un.d_ptr;
			break;

		case DT_FINI_ARRAY:
			fini_array_base = (uintptr_t *)(self->dm_loadaddr +
			                                self->dm_dynhdr[i].d_un.d_ptr);
			break;

		case DT_FINI_ARRAYSZ:
			fini_array_size = (size_t)self->dm_dynhdr[i].d_un.d_val / sizeof(void (*)(void));
			break;

		default: break;
		}
	}
done_dyntag:
	/* Service fini-array functions in reverse order. */
	while (fini_array_size--)
		(*(void (*)(void))(fini_array_base[fini_array_size] /* + self->dm_loadaddr*/))();
	/* Service a fini function, if one was specified. */
	if (fini_func)
		(*(void (*)(void))(fini_func + self->dm_loadaddr))();
done_fini:
	/* Delete memory mappings of the module. */
	COMPILER_BARRIER();
	for (i = 0; i < self->dm_phnum; ++i) {
		if (self->dm_phdr[i].p_type != PT_LOAD)
			continue;
		sys_munmap((void *)(self->dm_phdr[i].p_vaddr + self->dm_loadaddr),
		           self->dm_phdr[i].p_memsz);
	}
	COMPILER_BARRIER();
	/* Drop references from dependent modules. */
	for (i = 0; i < self->dm_depcnt; ++i) {
		DlModule *temp;
		temp = self->dm_depvec[i];
		if (!(temp->dm_flags & RTLD_NODELETE))
			DlModule_Decref(temp);
	}
	if (self->dm_file > 0)
		sys_close(self->dm_file);
	/* Free dynamically allocated heap-memory. */
	if (self->dm_shdr != empty_shdr)
		free(self->dm_shdr);
	if (self->dm_sections) {
again_free_sections:
		atomic_rwlock_write(&self->dm_sections_lock);
		for (i = 0; i < self->dm_shnum; ++i) {
			DlSection *sect;
			sect = self->dm_sections[i];
			if (!sect)
				continue;
			if (!atomic_rwlock_trywrite(&sect->ds_module_lock)) {
				bool hasref;
				hasref = DlSection_TryIncref(sect);
				atomic_rwlock_endwrite(&self->dm_sections_lock);
				if (hasref) {
					atomic_rwlock_write(&sect->ds_module_lock);
					atomic_rwlock_endwrite(&sect->ds_module_lock);
					DlSection_Decref(sect);
				}
				goto again_free_sections;
			}
			assertf(sect->ds_flags & ELF_DLSECTION_FLAG_OWNED,
			        "If this was true, then the section should have kept a reference to us!");
			sect->ds_module = NULL;
			atomic_rwlock_endwrite(&sect->ds_module_lock);
#ifndef NDEBUG
			memset(&self->dm_sections[i], 0xcc, sizeof(self->dm_sections[i]));
#endif /* !NDEBUG */
		}
#ifndef CONFIG_NO_DANGLING_DL_SECTIONS
		{
			REF DlSection *dangle, *next;
			dangle                     = self->dm_sections_dangling;
			self->dm_sections_dangling = NULL;
			atomic_rwlock_endwrite(&self->dm_sections_lock);
			while (dangle) {
				next = dangle->ds_dangling;
				DlSection_Decref(dangle);
				dangle = next;
			}
		}
#else /* !CONFIG_NO_DANGLING_DL_SECTIONS */
		atomic_rwlock_endwrite(&self->dm_sections_lock);
#endif /* CONFIG_NO_DANGLING_DL_SECTIONS */
		free(self->dm_sections);
	}
	if (self->dm_tlsmsize) {
		if (self->dm_tlsstoff == 0)
			DlModule_RemoveTLSExtension(self);
		if (self->dm_tlsfsize)
			free((void *)self->dm_tlsinit);
	} else {
		assert(!self->dm_tlsfsize);
	}
	free(self->dm_shstrtab);
	free(self->dm_depvec);
	free(self->dm_filename);
	free(self);
}


INTDEF WUNUSED fd_t CC reopen_bigfs(fd_t fd);

/* Lazily allocate if necessary, and return the file descriptor for `self' */
INTERN WUNUSED NONNULL((1)) fd_t CC
DlModule_GetFd(DlModule *__restrict self) {
	fd_t result = self->dm_file;
	if (result < 0) {
		fd_t newresult;
		assert(result == -1);
		result = sys_open(self->dm_filename, O_RDONLY | O_CLOEXEC, 0);
		if (E_ISERR(result))
			goto err;
		/* Make sure to only use big file descriptor indices, so-as
		 * to prevent use of reserved file numbers, as used by the
		 * standard I/O handles (aka. `STD(IN|OUT|ERR)_FILENO') */
		result    = reopen_bigfs(result);
		newresult = ATOMIC_CMPXCH_VAL(self->dm_file, -1, result);
		if unlikely(newresult != -1) {
			sys_close(result);
			result = newresult;
		}
	}
	return result;
err:
	elf_setdlerrorf("%q: Failed to open module for reading",
	                self->dm_filename);
	return -1;
}

/* Lazily allocate if necessary, and return the vector of section headers for `self'
 * NOTE: On success, this function guaranties that the following fields have been initialized:
 *  - self->dm_shnum
 *  - self->dm_shoff
 *  - self->dm_shstrndx
 *  - self->dm_shdr
 * @return: NULL: Error */
INTERN WUNUSED NONNULL((1)) ElfW(Shdr) *CC
DlModule_GetShdrs(DlModule *__restrict self) {
	fd_t fd;
	ElfW(Shdr) *result;
	result = self->dm_shdr;
	if (result != NULL)
		return result;
	/* Must load the section headers. */
	fd = DlModule_GetFd(self);
	if unlikely(fd < 0)
		goto err;
	if (self->dm_shnum == (ElfW(Half))-1 ||
	    self->dm_shoff == 0 ||
	    self->dm_shstrndx == (ElfW(Half))-1) {
		ElfW(Ehdr) ehdr;
		/* Must initialize information about section headers. */
		if (preadall(fd, &ehdr, sizeof(ehdr), 0) <= 0)
			goto err_read_ehdr;
		/* Quickly verify that the ehdr is valid. */
		if unlikely(DlModule_VerifyEhdr(&ehdr, self->dm_filename, false))
			goto err;
		if unlikely(ehdr.e_shstrndx >= ehdr.e_shnum) {
			elf_setdlerrorf("%q: Invalid `e_shstrndx=%u' is greater than or equal to `e_shnum=%u'",
			                self->dm_filename, ehdr.e_shstrndx, ehdr.e_shnum);
			goto err;
		}
		ATOMIC_CMPXCH(self->dm_shnum, (uint16_t)-1, ehdr.e_shnum);
		ATOMIC_CMPXCH(self->dm_shoff, 0, ehdr.e_shoff);
		ATOMIC_CMPXCH(self->dm_shstrndx, (ElfW(Half))-1, ehdr.e_shstrndx);
	}
	if unlikely(!self->dm_shnum) {
		ATOMIC_CMPXCH(self->dm_shdr, NULL, (ElfW(Shdr) *)empty_shdr);
		return empty_shdr;
	}
	/* Allocate the section header vector. */
	result = (ElfW(Shdr) *)malloc(self->dm_shnum * sizeof(ElfW(Shdr)));
	if unlikely(!result)
		goto err_nomem;
	if (preadall(fd, result, self->dm_shnum * sizeof(ElfW(Shdr)), self->dm_shoff) <= 0)
		goto err_read_shdr;
	{
		ElfW(Shdr) *new_result;
		/* Save the newly loaded section header vector. */
		new_result = ATOMIC_CMPXCH_VAL(self->dm_shdr,
		                               NULL,
		                               result);
		if unlikely(new_result != NULL) {
			free(result);
			result = new_result;
		}
	}
	return result;
err_read_shdr:
	free(result);
	elf_setdlerrorf("%q: Failed to read section header vector",
	                self->dm_filename);
	goto err;
err_nomem:
	elf_setdlerrorf("%q: Failed to allocate section header vector",
	                self->dm_filename);
	goto err;
err_read_ehdr:
	elf_setdlerrorf("%q: Failed to read ElfW(Ehdr)",
	                self->dm_filename);
err:
	return NULL;
}

/* Lazily allocate if necessary, and return the section header string table for `self'
 * @return: NULL: Error (s.a. elf_dlerror_message) */
INTERN WUNUSED NONNULL((1)) char *CC
DlModule_GetShstrtab(DlModule *__restrict self) {
	char *result;
	ElfW(Shdr) *shdrs;
	result = self->dm_shstrtab;
	if (result)
		return result;
	shdrs = DlModule_GetShdrs(self);
	if unlikely(!shdrs)
		goto err;
	shdrs += self->dm_shstrndx;
	if unlikely(shdrs->sh_type == SHT_NOBITS) {
		elf_setdlerrorf("%q: Section `e_shstrndx=%u' has type `SHT_NOBITS'",
		                self->dm_filename, self->dm_shstrndx);
		goto err;
	}
	/* Allocate the section header string table. */
	result = (char *)malloc(shdrs->sh_size + 1);
	if unlikely(!result)
		goto err_nomem;
	result[shdrs->sh_size] = '\0'; /* Ensure NUL-termination */
	/* Read the string table into memory. */
	if (preadall(self->dm_file, result, shdrs->sh_size, shdrs->sh_offset) <= 0)
		goto err_read_shstrtab;
	{
		char *new_result;
		/* Save the newly loaded section header string table. */
		new_result = ATOMIC_CMPXCH_VAL(self->dm_shstrtab,
		                               NULL,
		                               result);
		if unlikely(new_result != NULL) {
			free(result);
			result = new_result;
		}
	}
	return result;
err_read_shstrtab:
	elf_setdlerrorf("%q: Failed to read contents of `e_shstrndx=%u' (`sh_offset=%I64u')",
	                self->dm_filename, self->dm_shstrndx, (u64)shdrs->sh_offset);
	goto err;
err_nomem:
	elf_setdlerrorf("%q: Failed to allocate section header string table",
	                self->dm_filename);
err:
	return NULL;
}


INTERN ATTR_COLD NONNULL((1, 2)) int CC
elf_setdlerror_nosect(DlModule *__restrict self,
                      char const *__restrict name) {
	return elf_setdlerrorf("%q: No such section %q",
	                       self->dm_filename, name);
}

INTERN WUNUSED NONNULL((1, 2)) ElfW(Shdr) *CC
DlModule_GetSection(DlModule *__restrict self,
                    char const *__restrict name) {
	ElfW(Shdr) *result;
	uint16_t i;
	char *strtab = DlModule_GetShstrtab(self);
	if unlikely(!strtab)
		goto err;
	result = self->dm_shdr;
	assert(result);
	for (i = 0; i < self->dm_shnum; ++i, ++result) {
		if (strcmp(strtab + result->sh_name, name) == 0)
			return result; /* Got it! */
	}
	elf_setdlerror_nosect(self, name);
err:
	return NULL;
}




DECL_END

#endif /* !GUARD_LIBDL_MODULE_C */
