/* Copyright (c) 2019-2021 Griefer@Work                                       *
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
 *    in a product, an acknowledgement (see the following) in the product     *
 *    documentation is required:                                              *
 *    Portions Copyright (c) 2019-2021 Griefer@Work                           *
 * 2. Altered source versions must be plainly marked as such, and must not be *
 *    misrepresented as being the original software.                          *
 * 3. This notice may not be removed or altered from any source distribution. *
 */
#ifndef GUARD_KERNEL_SRC_MEMORY_DRIVER_C
#define GUARD_KERNEL_SRC_MEMORY_DRIVER_C 1
#define __WANT_MODULE_SECTION__ms_dead
#define __WANT_MODULE__md_mmlop
#define __WANT_DRIVER_SECTION_ds_sect
#define __WANT_DRIVER_d_module
#define __WANT_MNODE__mn_dead
#define _KOS_SOURCE 1

#include <kernel/compiler.h>

#ifdef CONFIG_USE_NEW_DRIVER
#include <debugger/config.h>
#include <debugger/hook.h>
#include <debugger/io.h>
#include <debugger/rt.h>
#include <fs/node.h>
#include <fs/vfs.h>
#include <kernel/arch/driver.h>
#include <kernel/debugtrap.h>
#include <kernel/heap.h>
#include <kernel/mman.h>
#include <kernel/mman/cache.h>
#include <kernel/mman/driver.h>
#include <kernel/mman/kram.h>
#include <kernel/mman/map.h>
#include <kernel/mman/mfile.h>
#include <kernel/mman/mnode.h>
#include <kernel/mman/module.h>
#include <kernel/mman/mpart.h>
#include <kernel/mman/nopf.h>
#include <kernel/mman/rw.h>
#include <kernel/mman/sync.h>
#include <kernel/mman/unmapped.h>
#include <kernel/paging.h>
#include <kernel/panic.h>
#include <kernel/printk.h>
#include <kernel/syslog.h>
#include <kernel/vboxgdb.h>
#include <sched/signal.h>

#include <hybrid/atomic.h>
#include <hybrid/overflow.h>
#include <hybrid/sequence/bsearch.h>
#include <hybrid/sequence/rbtree.h>
#include <hybrid/typecore.h>

#include <kos/except.h>
#include <kos/except/reason/noexec.h>

#include <alloca.h>
#include <assert.h>
#include <elf.h>
#include <malloca.h>
#include <signal.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>

#include <libcmdline/decode.h>
#include <libunwind/eh_frame.h>
#include <libunwind/unwind.h>
#include <libzlib/inflate.h>


#if __SIZEOF_POINTER__ == 4
#define BADPOINTER UINT32_C(0xcccccccc)
#elif __SIZEOF_POINTER__ == 8
#define BADPOINTER UINT64_C(0xcccccccccccccccc)
#elif __SIZEOF_POINTER__ == 2
#define BADPOINTER UINT16_C(0xcccc)
#elif __SIZEOF_POINTER__ == 1
#define BADPOINTER UINT8_C(0xcc)
#else /* __SIZEOF_POINTER__ == ... */
#error "Unsupported sizeof(void *)"
#endif /* __SIZEOF_POINTER__ != ... */

#define HINT_ADDR(x, y) x
#define HINT_MODE(x, y) y
#define HINT_GETADDR(x) HINT_ADDR x
#define HINT_GETMODE(x) HINT_MODE x

#ifndef NDEBUG
#define DBG_memset(dst, byte, num_bytes) memset(dst, byte, num_bytes)
#else /* !NDEBUG */
#define DBG_memset(dst, byte, num_bytes) (void)0
#endif /* NDEBUG */

/* Max # of section headers accepted for driver modules.
 * A limit must be imposed to prevent malicious programs
 * from flooding kernel-space memory. */
#ifndef ELF_ARCH_MAXSHCOUNT
#define ELF_ARCH_MAXSHCOUNT 128
#endif /* !ELF_ARCH_MAXSHCOUNT */

DECL_BEGIN

#ifndef CONFIG_USE_NEW_FS
#define mfile_read(self, dst, num_bytes, pos)    inode_readk((struct inode *)(self), dst, num_bytes, pos)
#define mfile_readall(self, dst, num_bytes, pos) inode_readallk((struct inode *)(self), dst, num_bytes, pos)
#endif /* !CONFIG_USE_NEW_FS */

/************************************************************************/
/* Loaded driver enumeration (lockless!)                                */
/************************************************************************/

PRIVATE struct driver_loadlist default_loaded_drivers = {
	.dll_refcnt  = 2, /* +1:default_loaded_drivers, +1:drivers */
	.dll_count   = 1, /* Upon boot, only the kernel core driver exists. */
	.dll_drivers = { &kernel_driver }
};

/* [1..1] A descriptor for the set of currently loaded drivers.
 * NOTE: To retrieve this list, don't use `arref_get(&drivers)',
 *       but make use  of `get_driver_loadlist()' instead.  This
 *       must be done since the later will automatically try  to
 *       get rid of drivers that  have been destroyed, but  were
 *       unable to remove themselves from the load-list. */
PUBLIC struct driver_loadlist_arref drivers = ARREF_INIT(&default_loaded_drivers);
#ifdef CONFIG_VBOXGDB
DEFINE_PUBLIC_ALIAS(_vboxgdb_kos_driver_state, drivers);
#endif /* CONFIG_VBOXGDB */

/* Set to true if `drivers' may contain drivers
 * that  have been destroyed,  but could not be
 * removed due to lack of memory. */
PRIVATE bool loaded_drivers_contains_dead = false;



PUBLIC NOBLOCK NONNULL((1)) void
NOTHROW(FCALL driver_loadlist_destroy)(struct driver_loadlist *__restrict self) {
	size_t i;
	for (i = 0; i < self->dll_count; ++i)
		weakdecref(self->dll_drivers[i]);
	kfree(self);
}

/* Return the list of currently loaded drivers.
 * Note that this function is  NOBLOCK+NOTHROW! */
PUBLIC NOBLOCK ATTR_RETNONNULL WUNUSED REF struct driver_loadlist *
NOTHROW(FCALL get_driver_loadlist)(void) {
	REF struct driver_loadlist *result;
again:
	result = arref_get(&drivers);

	/* Only reap dead drivers from the load-list if the kernel
	 * hasn't been poisoned,  and debugger-mode isn't  active.
	 *
	 * iow: Only do this when the system is running "normal" */
	if (unlikely(loaded_drivers_contains_dead) &&
	    likely(!kernel_poisoned() && !dbg_active)) {
		bool cmpxch_ok;
		size_t i, newcount;
		REF struct driver_loadlist *new_dll;
		COMPILER_BARRIER();
		loaded_drivers_contains_dead = false;

		/* Try to construct a new driver load-list that doesn't
		 * contain any of the drivers that have been destroyed.
		 *
		 * For this purpose, figure out the set of drivers  that
		 * aren't already dead. This can easily be done by doing
		 * a tryincref() on every driver from the resulting set,
		 * and counting how many we got.
		 *
		 * After that point, we'll continue to hold a reference
		 * to every non-destroyed driver, meaning that the # of
		 * non-destroyed drivers in the resulting set will stay
		 * the same until we release those reference again. */
		for (i = 0, newcount = 0; i < result->dll_count; ++i) {
			if (tryincref(result->dll_drivers[i]))
				++newcount;
		}
		assert(newcount <= result->dll_count);

		/* Check for special case: all drivers are still alive.
		 * This  might  happen due  to race  conditions, etc... */
		if unlikely(newcount == result->dll_count) {
			/* Simply drop all of the references we acquired. */
			for (i = 0; i < result->dll_count; ++i)
				decref_unlikely(result->dll_drivers[i]);
			goto done;
		}

		if unlikely(newcount == 1) {
			/* Special  case:  only 1  driver remains,
			 * which has to be the kernel core driver! */
			new_dll = incref(&default_loaded_drivers);
		} else {
			size_t dst;
			/* Try to allocate a new driver load-list.
			 * If this fails, drop all of our references, change the
			 * `loaded_drivers_contains_dead' flag back to true, and
			 * silently return  the original  list (containing  some
			 * dead drivers), thus letting the next caller deal with
			 * the removal of dead drivers. */
			new_dll = (REF struct driver_loadlist *)kmalloc_nx(offsetof(struct driver_loadlist, dll_drivers) +
			                                                   newcount * sizeof(WEAK REF struct driver *),
			                                                   GFP_ATOMIC | GFP_LOCKED | GFP_PREFLT);
			if unlikely(!new_dll) {
				/* Failed to allocate a  new loadlist, and there  are
				 * still some more dead drivers within the old one...
				 *
				 * In this case, leave the work to-be done up for the
				 * next caller of this function. */
				loaded_drivers_contains_dead = true;
				COMPILER_WRITE_BARRIER();

				for (i = 0; i < result->dll_count; ++i) {
					if (!wasdestroyed(result->dll_drivers[i]))
						decref_unlikely(result->dll_drivers[i]);
				}
				goto done;
			}

			/* Fill in the new driver load-list with all of the non-
			 * destroyed   drivers   from    the   original    list. */
			new_dll->dll_refcnt = 2; /* +1:return, +1:arref_cmpxch_inherit_new() */
			new_dll->dll_count  = newcount;
			for (i = 0, dst = 0; i < result->dll_count; ++i) {
				if (wasdestroyed(result->dll_drivers[i]))
					continue;
				new_dll->dll_drivers[dst] = weakincref(result->dll_drivers[i]);
				++dst;
			}
			assert(dst == newcount);
		}

		/* Try to exchange the  old driver load-list (which  contains
		 * some dead drivers) with the new one (which doesn't contain
		 * any dead drivers at the moment, since we're still  holding
		 * full references to all drivers that would have been  dead) */
		COMPILER_BARRIER();
		cmpxch_ok = arref_cmpxch_inherit_new(&drivers, result, new_dll);
		COMPILER_BARRIER();

		/* Drop all of  our references  to drivers  apart of  the set  of
		 * ones that haven't been destroyed yet. Note that by doing this,
		 * more  drivers may end up being destroyed, but that's ok, since
		 * those  drivers will  once again  be able  to remove themselves
		 * normally from the driver-load-list.
		 *
		 * Also note that we can  simply `decref()' every driver  from
		 * `new_dll' here, since that list contains only those drivers
		 * which weren't destroyed earlier, which  is the same set  as
		 * those which we were able to `tryincref()' above!
		 *
		 * As far as logic ordering goes, the only thing import here is
		 * that these decref() operations happen _after_ the CMPXCH  of
		 * the current driver load-list above! */
		for (i = 0; i < new_dll->dll_count; ++i)
			decref_unlikely(new_dll->dll_drivers[i]);

		/* Drop our reference to the old set of loaded drivers. */
		decref_likely(result);

		if (!cmpxch_ok) {
			/* The driver load-list changed in the mean time...
			 * Handle this case by trying again... */
			loaded_drivers_contains_dead = true;
			decref_nokill(new_dll); /* The would-be inherited-by-drivers reference */
			decref_likely(new_dll); /* The would-be returned reference */
			COMPILER_WRITE_BARRIER();
			goto again;
		}

		/* Success. - Simply return the (now smaller)
		 * set  of  (probably)  still-alive  drivers. */
		result = new_dll;
		COMPILER_BARRIER();
	}
done:
	return result;
}






/************************************************************************/
/* Driver section operator table                                        */
/************************************************************************/

PRIVATE NOBLOCK NONNULL((1)) void
NOTHROW(FCALL driver_section_destroy)(struct driver_section *__restrict self) {
	REF struct driver *drv;
	drv = (REF struct driver *)self->ds_sect.ms_module;
	if (tryincref(drv)) {
		/* Try to remove `self' from the module's section vector. */
		uint16_t myindex;
		myindex = (uint16_t)(size_t)(self->ds_shdr - drv->d_shdr);
		assert(myindex < drv->d_shnum);
		/* Replace a pointer to `self' with `NULL'.
		 * There is the (very) unlikely case that someone reloaded
		 * the section before we were able to delete out  pointer,
		 * in which case that new section already took our spot! */
		awref_cmpxch(&drv->d_sections[myindex], self, NULL);
		decref_unlikely(drv);
	}
	weakdecref(drv);
	if (self->ds_infladdr != (KERNEL byte_t *)-1 &&
	    self->ds_infladdr != self->ds_addr) {
		mman_unmap_kram(self->ds_infladdr,
		                self->ds_inflsize);
	}
	if (!(self->ds_sect.ms_flags & SHF_ALLOC) &&
	    self->ds_addr != (KERNEL byte_t *)-1) {
		mman_unmap_kram_and_kfree(self->ds_addr,
		                          self->ds_sect.ms_size,
		                          self);
	} else {
		kfree(self);
	}
}

PRIVATE /*ATTR_PURE*/ WUNUSED NONNULL((1)) char const *FCALL
driver_section_getname(struct driver_section *__restrict self) {
	struct driver *drv;
	drv = (struct driver *)self->ds_sect.ms_module;
	assert(!wasdestroyed(drv));
	return drv->d_shstrtab + self->ds_shdr->sh_name;
}

PRIVATE WUNUSED NONNULL((1)) KERNEL byte_t *FCALL
driver_section_create_kernaddr_ex(struct driver_section *__restrict self,
                                  struct driver *__restrict drv) {
	struct mfile *file = driver_getfile(drv);
	if (!file)
		THROW(E_NO_SUCH_OBJECT);
	return (byte_t *)mman_map(/* self:        */ &mman_kernel,
	                          /* hint:        */ HINT_GETADDR(KERNEL_VMHINT_TEMPORARY),
	                          /* num_bytes:   */ self->ds_sect.ms_size,
	                          /* prot:        */ (PROT_READ | PROT_WRITE) & ~PROT_SHARED,
	                          /* flags:       */ HINT_GETMODE(KERNEL_VMHINT_TEMPORARY),
	                          /* file:        */ file,
	                          /* file_fspath: */ drv->d_module.md_fspath,
	                          /* file_fsname: */ drv->d_module.md_fsname,
	                          /* file_pos:    */ (pos_t)self->ds_shdr->sh_offset);
}

PRIVATE WUNUSED NONNULL((1)) KERNEL byte_t *FCALL
driver_section_create_kernaddr(struct driver_section *__restrict self) {
	REF struct driver *drv;
	drv = (REF struct driver *)self->ds_sect.ms_module;
	if (!tryincref(drv))
		THROW(E_NO_SUCH_OBJECT);
	FINALLY_DECREF_UNLIKELY(drv);
	return driver_section_create_kernaddr_ex(self, drv);
}



PRIVATE WUNUSED NONNULL((1)) KERNEL byte_t *FCALL
driver_section_getaddr(struct driver_section *__restrict self) {
	byte_t *result;
	if (self->ds_addr != (KERNEL byte_t *)-1)
		return self->ds_addr;
	result = driver_section_create_kernaddr(self);
	if unlikely(!ATOMIC_CMPXCH(self->ds_addr, (KERNEL byte_t *)-1, result)) {
		/* Race condition: some other thread already did this in the mean time... */
		mman_unmap_kram(result, self->ds_sect.ms_size);
	}
	return self->ds_addr;
}

PRIVATE WUNUSED NONNULL((1, 2)) KERNEL byte_t *FCALL
driver_section_getaddr_inflate(struct driver_section *__restrict self,
                               size_t *__restrict psize) {
	size_t dst_size, src_size;
	KERNEL byte_t *src_data, *dst_data;
	void *src_data_freeme_cookie;
	REF struct driver *drv;
	if (self->ds_infladdr != (KERNEL byte_t *)-1) {
		*psize = self->ds_inflsize;
		return self->ds_infladdr;
	}
	if (!(self->ds_sect.ms_flags & SHF_COMPRESSED)) {
		/* Section isn't actually compressed! */
		dst_data = driver_section_getaddr(self);
		ATOMIC_WRITE(self->ds_inflsize, self->ds_sect.ms_size);
		ATOMIC_CMPXCH(self->ds_infladdr, (KERNEL byte_t *)-1, dst_data);
		*psize = self->ds_sect.ms_size;
		return dst_data;
	}
	drv = (REF struct driver *)self->ds_sect.ms_module;
	if (!tryincref(drv))
		THROW(E_NO_SUCH_OBJECT);
	FINALLY_DECREF_UNLIKELY(drv);
	if unlikely(OVERFLOW_USUB(self->ds_sect.ms_size, sizeof(ElfW(Chdr)), &src_size))
		THROW(E_INVALID_ARGUMENT);
	src_data = self->ds_addr;
	src_data_freeme_cookie = NULL;
	if (src_data == (KERNEL byte_t *)-1) {
		/* Allocate a  cookie that  we can  later use  to
		 * asynchronously unmap the compressed data blob. */
		src_data_freeme_cookie = kmalloc(sizeof(struct mman_unmap_kram_job),
		                                 GFP_LOCKED | GFP_PREFLT);
		TRY {
			src_data = driver_section_create_kernaddr_ex(self, drv);
		} EXCEPT {
			kfree(src_data_freeme_cookie);
			RETHROW();
		}
	}
	TRY {
		ElfW(Chdr) *chdr;
		chdr = (ElfW(Chdr) *)src_data;
		if unlikely(chdr->ch_type != ELFCOMPRESS_ZLIB)
			THROW(E_INVALID_ARGUMENT);
		dst_size = chdr->ch_size;
		dst_data = (byte_t *)mman_map_kram(NULL, dst_size, GFP_PREFLT);
		TRY {
			ssize_t error;
			struct zlib_reader reader;
			zlib_reader_init(&reader, src_data, src_size);
			/* Decompress data. */
#ifdef CONFIG_HAVE_DEBUGGER
			if (dbg_active) {
				/* Display an on-screen message that we're decompressing stuff... */
				void *screen_buffer;
				char const *section_name;
				dbg_pprinter_arg_t pp;
				section_name = driver_section_getname(self);
				if (!section_name)
					section_name = DBGSTR("?");
				screen_buffer = alloca(dbg_screen_width * dbg_screen_cellsize);
				dbg_getscreendata(0, dbg_screen_height - 1, dbg_screen_width, 1, screen_buffer);
				dbg_beginupdate();
				dbg_savecolor();
				dbg_setcolor(ANSITTY_CL_BLACK, ANSITTY_CL_LIGHT_GRAY);
				dbg_hline(0, dbg_screen_height - 1, dbg_screen_width, ' ');
				dbg_pprinter_arg_init(&pp, 0, dbg_screen_height - 1);
				dbg_pprinter(&pp, DBGSTR("Decompressing "), 14);
				if (module_haspath_or_name(&drv->d_module)) {
					module_printpath_or_name(&drv->d_module, &dbg_pprinter, &pp);
				} else {
					dbg_pprinter(&pp, DBGSTR("?"), 1);
				}
				dbg_pprinter(&pp, DBGSTR(":"), 1);
				dbg_pprinter(&pp, section_name, strlen(section_name));
				dbg_pprinter(&pp, DBGSTR("..."), 1);
				dbg_loadcolor();
				dbg_endupdate();
				error = zlib_reader_read(&reader, dst_data, dst_size);
				dbg_setscreendata(0, dbg_screen_height - 1, dbg_screen_width, 1, screen_buffer);
			} else {
				error = zlib_reader_read(&reader, dst_data, dst_size);
			}
#else /* CONFIG_HAVE_DEBUGGER */
			error = zlib_reader_read(&reader, dst_data, dst_size);
#endif /* !CONFIG_HAVE_DEBUGGER */
			zlib_reader_fini(&reader);
			if unlikely(error < 0)
				THROW(E_INVALID_ARGUMENT);
			/* clear all trailing data that could not be read. */
			if (dst_size > (size_t)error) {
				memset((byte_t *)dst_data + (size_t)error,
				       0, dst_size - error);
			}
		} EXCEPT {
			mman_unmap_kram(dst_data, dst_size);
			RETHROW();
		}
	} EXCEPT {
		if (src_data_freeme_cookie)
			mman_unmap_kram_and_kfree(src_data, self->ds_sect.ms_size, src_data_freeme_cookie);
		RETHROW();
	}
	if (src_data_freeme_cookie)
		mman_unmap_kram_and_kfree(src_data, self->ds_sect.ms_size, src_data_freeme_cookie);
	ATOMIC_WRITE(self->ds_inflsize, dst_size);
	ATOMIC_CMPXCH(self->ds_infladdr, (KERNEL byte_t *)-1, dst_data);
	*psize = dst_size;
	return dst_data;
}


PRIVATE struct module_section_ops const driver_section_ops = {
	.ms_destroy         = (void (FCALL *)(struct module_section *__restrict))&driver_section_destroy,
	.ms_getname         = (char const *(FCALL *)(struct module_section *__restrict))&driver_section_getname,
	.ms_getaddr         = (byte_t *(FCALL *)(struct module_section *__restrict))&driver_section_getaddr,
	.ms_getaddr_alias   = (byte_t *(FCALL *)(struct module_section *__restrict))&driver_section_getaddr,
	.ms_getaddr_inflate = (byte_t *(FCALL *)(struct module_section *__restrict, size_t *__restrict))&driver_section_getaddr_inflate,
};





/************************************************************************/
/* Custom kernel driver data structures                                 */
/************************************************************************/
#ifdef __INTELLISENSE__
struct kernel_shstrtab { char zero; };
INTERN_CONST ATTR_COLDRODATA struct kernel_shstrtab const kernel_shstrtab_data = { 0 };
enum { KERNEL_SECTIONS_COUNT };
INTERN_CONST ATTR_COLDRODATA ElfW(Shdr) const kernel_shdr[KERNEL_SECTIONS_COUNT] = {};
INTERN_CONST struct driver_section_awref kernel_sections[KERNEL_SECTIONS_COUNT] = {};
#else /* __INTELLISENSE__ */
#define SECTION_DESCRIPTOR_INDEX     PP_CAT2(KERNEL_SECTION_INDEX_, __LINE__)
#define SECTION_DESCRIPTOR_SHSTRNAME PP_CAT2(ks_name_, __LINE__)

/* Define globals. */
#define INTERN_SECTION(symbol_name, name, type, flags, start, size, entsize, link, info) /* nothing */
#include "../../fs/kernel_sections.def"

/* Define the kernel's .shstrtab section */
struct kernel_shstrtab {
#define INTERN_SECTION(symbol_name, name, type, flags, start, size, entsize, link, info) \
	char SECTION_DESCRIPTOR_SHSTRNAME[sizeof(name)];
#include "../../fs/kernel_sections.def"
	char zero;
};

INTERN_CONST ATTR_COLDRODATA struct kernel_shstrtab const kernel_shstrtab_data = {
#define INTERN_SECTION(symbol_name, name, type, flags, start, size, entsize, link, info) \
	/* .SECTION_DESCRIPTOR_SHSTRNAME = */ name,
#include "../../fs/kernel_sections.def"
	/* .zero = */ 0
};



/* Define section indices */
enum {
#define INTERN_SECTION(symbol_name, name, type, flags, start, size, entsize, link, info) \
	SECTION_DESCRIPTOR_INDEX,
#include "../../fs/kernel_sections.def"
	KERNEL_SECTIONS_COUNT
};


/* Define the ELF section header vector. */
INTERN_CONST ATTR_COLDRODATA ElfW(Shdr) const kernel_shdr[KERNEL_SECTIONS_COUNT] = {
#define INTERN_SECTION(symbol_name, name, type, flags, start, size, entsize, link, info) \
	/* [SECTION_DESCRIPTOR_INDEX] = */ {                                                 \
		.sh_name      = offsetof(struct kernel_shstrtab, SECTION_DESCRIPTOR_SHSTRNAME),  \
		.sh_type      = (type),                                                          \
		.sh_flags     = (flags),                                                         \
		.sh_addr      = (uintptr_t)(start),                                              \
		.sh_offset    = 0,                                                               \
		.sh_size      = (uintptr_t)(size),                                               \
		.sh_link      = link,                                                            \
		.sh_info      = info,                                                            \
		.sh_addralign = 1,                                                               \
		.sh_entsize   = entsize,                                                         \
	},
#include "../../fs/kernel_sections.def"
};



/* Define the driver section descriptors. */
#define _SECTION(decl_name, name, type, flags, start, size, entsize, link, info) \
	struct driver_section decl_name = {                                          \
		.ds_sect = {                                                             \
			.ms_refcnt  = 1,                                                     \
			.ms_ops     = &driver_section_ops,                                   \
			.ms_module  = &kernel_driver,                                        \
			.ms_size    = (size_t)(size),                                        \
			.ms_entsize = (size_t)(entsize),                                     \
			.ms_type    = (type),                                                \
			.ms_flags   = (flags),                                               \
			.ms_link    = (link),                                                \
			.ms_info    = (info),                                                \
			.ms_cache   = LIST_ENTRY_UNBOUND_INITIALIZER,                        \
		},                                                                       \
		.ds_shdr     = &kernel_shdr[SECTION_DESCRIPTOR_INDEX],                   \
		.ds_addr     = (void *)(start),                                          \
		.ds_infladdr = (flags) & SHF_COMPRESSED ? (void *)-1 : (void *)(start),  \
		.ds_inflsize = (flags) & SHF_COMPRESSED ? 0 : (size_t)(size),            \
	};
#define INTERN_SECTION(intern_name, name, type, flags, start, size, entsize, link, info) \
	INTERN _SECTION(intern_name, name, type, flags, start, size, entsize, link, info)
#define EXPORT_SECTION(export_name, name, type, flags, start, size, entsize, link, info) \
	PUBLIC _SECTION(export_name, name, type, flags, start, size, entsize, link, info)
#include "../../fs/kernel_sections.def"


/* Define the driver sections vector. */
INTERN_CONST struct driver_section_awref kernel_sections[KERNEL_SECTIONS_COUNT] = {
#define INTERN_SECTION(symbol_name, ...) /* [SECTION_DESCRIPTOR_INDEX] = */ AWREF_INIT(&symbol_name),
#include "../../fs/kernel_sections.def"
};
#endif /* !__INTELLISENSE__ */



extern byte_t __kernel_start[];
extern byte_t __kernel_end[];
extern byte_t __kernel_size[];
extern byte_t __kernel_size_nofree[];
extern byte_t __kernel_free_start[];
extern byte_t __kernel_free_size[];
extern byte_t __kernel_eh_frame_start[];
extern byte_t __kernel_eh_frame_end[];

PUBLIC NOBLOCK NONNULL((1)) void
NOTHROW(FCALL driver_free_)(struct driver *__restrict self) ASMNAME("driver_free");

PRIVATE WUNUSED NONNULL((1)) REF struct driver_section *FCALL
kernel_locksection_index(struct driver *__restrict UNUSED(self),
                         unsigned int section_index) {
	REF struct driver_section *result;
	if (section_index >= KERNEL_SECTIONS_COUNT)
		return NULL;
	result = awref_get(&kernel_sections[section_index]);
	return result;
}

PRIVATE WUNUSED NONNULL((1)) REF struct driver_section *FCALL
kernel_locksection(struct driver *__restrict self,
                   USER CHECKED char const *section_name) {
	unsigned int i;
	for (i = 0; i < KERNEL_SECTIONS_COUNT; ++i) {
		char const *name;
		name = (char *)&kernel_shstrtab_data + kernel_shdr[i].sh_name;
		if (strcmp(name, section_name) == 0)
			break; /* Found it! */
	}
	return kernel_locksection_index(self, i);
}

PRIVATE WUNUSED NONNULL((1, 3)) bool FCALL
driver_sectinfo(struct driver *__restrict self,
                uintptr_t module_relative_addr,
                struct module_sectinfo *__restrict info);

#ifndef KERNEL_DRIVER_NAME
#define KERNEL_DRIVER_NAME "kernel"
#endif /* !KERNEL_DRIVER_NAME */

#ifndef KERNEL_DRIVER_FILENAME
#define KERNEL_DRIVER_FILENAME "/os/kernel.bin"
#endif /* !KERNEL_DRIVER_FILENAME */

PRIVATE struct module_ops const kernel_module_ops = {
	.mo_free              = (void(FCALL *)(struct module *__restrict))&driver_free_,
	.mo_destroy           = (typeof(((struct module_ops *)0)->mo_destroy))BADPOINTER, /* Must never be called! */
	.mo_nonodes           = (typeof(((struct module_ops *)0)->mo_nonodes))BADPOINTER, /* Must never be called! */
	.mo_locksection       = (REF struct module_section *(FCALL *)(struct module *__restrict, USER CHECKED char const *))&kernel_locksection,
	.mo_locksection_index = (REF struct module_section *(FCALL *)(struct module *__restrict, unsigned int))&kernel_locksection_index,
	.mo_sectinfo          = (bool (FCALL *)(struct module *__restrict, uintptr_t, struct module_sectinfo *__restrict))&driver_sectinfo
};

PRIVATE ATTR_COLDRODATA char const kernel_driver_name[] = KERNEL_DRIVER_NAME;
PRIVATE ATTR_COLDDATA struct directory_entry kernel_driver_fsname = {
	.de_refcnt   = 1, /* +1: kernel_driver.d_module.md_fsname */
	.de_heapsize = offsetof(struct directory_entry, de_name) + sizeof(KERNEL_DRIVER_FILENAME),
	.de_next     = NULL,
	.de_bypos    = LIST_ENTRY_UNBOUND_INITIALIZER,
	.de_fsdata   = {},
	.de_pos      = 0,
	.de_ino      = 0,
	.de_hash     = 0,
	.de_namelen  = COMPILER_STRLEN(KERNEL_DRIVER_FILENAME),
	.de_type     = DT_REG,
	.de_name     = KERNEL_DRIVER_FILENAME
};

PUBLIC struct driver kernel_driver = {
	.d_module = {
		.md_refcnt     = 2, /* +1: kernel_driver, +1: d_state == DRIVER_STATE_LOADED */
		.md_weakrefcnt = 1 + KERNEL_SECTIONS_COUNT,
		.md_nodecount  = 1, /* TODO */
		.md_ops        = &kernel_module_ops,
		.md_mman       = &mman_kernel,
		.md_loadaddr   = 0,
		.md_loadmin    = __kernel_start,
		.md_loadmax    = __kernel_end - 1,
		.md_file       = NULL,
		{{ NULL, &kernel_driver_fsname, }},
#ifdef _MODULE_HAVE_SIZEOF_POINTER
		.md_sizeof_pointer = sizeof(void *),
#endif /* _MODULE_HAVE_SIZEOF_POINTER */
	},
	.d_state               = DRIVER_STATE_LOADED,
	.d_name                = kernel_driver_name,
	.d_cmdline             = NULL,
	.d_argc                = 0,
	.d_argv                = NULL,
	.d_depcnt              = 0,
	.d_depvec              = NULL,
	.d_eh_frame_start      = __kernel_eh_frame_start,
	.d_eh_frame_end        = __kernel_eh_frame_end,
	.d_eh_frame_cache      = NULL,
	.d_eh_frame_cache_lock = ATOMIC_RWLOCK_INIT,
	.d_dyncnt              = 0,
	.d_dynhdr              = NULL,
	.d_dynsym_tab          = NULL,
	.d_dynsym_cnt          = 0,
	.d_gnuhashtab          = NULL,
	.d_hashtab             = NULL,
	.d_dynstr              = NULL,
	.d_dynstr_end          = NULL,
	.d_shstrndx            = KERNEL_SECTIONS_COUNT - 1,
	.d_shnum               = KERNEL_SECTIONS_COUNT,
	.d_shdr                = (ElfW(Shdr) *)kernel_shdr,
	.d_sections            = kernel_sections,
	.d_shstrtab            = (char *)&kernel_shstrtab_data,
	.d_shstrsiz            = sizeof(kernel_shstrtab_data),
	.d_phnum               = 2,
	.d_phdr                = {
		/* [0] = */ ELFW(PHDR_INIT)(
			/* .p_type   = */ PT_LOAD,
			/* .p_offset = */ 0, /* Doesn't matter... */
			/* .p_vaddr  = */ (uintptr_t)__kernel_start,
			/* .p_paddr  = */ (uintptr_t)__kernel_start - KERNEL_CORE_BASE,
			/* .p_filesz = */ (uintptr_t)__kernel_size_nofree,
			/* .p_memsz  = */ (uintptr_t)__kernel_size_nofree,
			/* .p_flags  = */ PF_X | PF_W | PF_R,
			/* .p_align  = */ PAGESIZE
		),
		/* [1] = */ ELFW(PHDR_INIT)(
			/* .p_type   = */ PT_LOAD,
			/* .p_offset = */ 0, /* Doesn't matter... */
			/* .p_vaddr  = */ (uintptr_t)__kernel_free_start,
			/* .p_paddr  = */ (uintptr_t)__kernel_free_start - KERNEL_CORE_BASE,
			/* .p_filesz = */ (uintptr_t)__kernel_free_size,
			/* .p_memsz  = */ (uintptr_t)__kernel_free_size,
			/* .p_flags  = */ PF_X | PF_W | PF_R,
			/* .p_align  = */ PAGESIZE
		)
	}
};




/************************************************************************/
/* Driver enumeration/lookup functions                                  */
/************************************************************************/
PUBLIC NOBLOCK WUNUSED REF struct driver *
NOTHROW(FCALL driver_fromaddr)(void const *addr) {
	REF struct driver_loadlist *ll;
	REF struct driver *result;
	size_t i;
	/* If possible, try to make use of the mnode->module  self-pointers!
	 * Note  however that this  requires acquiring a  lock to the kernel
	 * mman, which can't be done consistently without blocking. As such,
	 * we have a fallback further  below that uses the driver  load-list
	 * in order to find the relevant driver as a fall-back. */
	if (mman_lock_tryacquire(&mman_kernel)) {
		struct mnode *node;
		result = NULL;
		node   = mman_mappings_locate(&mman_kernel, addr);
		if (node) {
			result = (struct driver *)node->mn_module;
			/* NOTE: Any module we  find here should  _always_ be a  driver
			 *       module, but better be save than sorry, and check  that
			 *       this is the case, only constructing the to-be returned
			 *       reference if that is truly the case. */
			if (result && (!module_isdriver((struct module *)result) ||
			               !tryincref(result)))
				result = NULL;
		}
		mman_lock_release(&mman_kernel);
		return result;
	}

	/* Try to do a binary search for `addr'.
	 * We take advantage of the fact that the driver list is
	 * sorted by the address  ranges used by the  individual
	 * drivers. */
	ll = get_driver_loadlist();
	assert_assume(ll->dll_count >= 1);
	BSEARCHR (i, ll->dll_drivers, ll->dll_count,
	          ->d_module.md_loadmin,
	          ->d_module.md_loadmax,
	          (byte_t const *)addr) {
		uint16_t phdr;
		uintptr_t driver_reladdr;
		result = ll->dll_drivers[i];
		/* Check  if the driver-relative address is being
		 * mapped by one of the driver's program headers!
		 * Note  that we can access the phdr-vector, even
		 * after the driver's been destroyed just fine! */
		driver_reladdr = (uintptr_t)addr - result->d_module.md_loadaddr;
		for (phdr = 0; phdr < result->d_phnum; ++phdr) {
			if (result->d_phdr[phdr].p_type != PT_LOAD)
				continue; /* Not a load-header! */
			if (driver_reladdr < result->d_phdr[phdr].p_vaddr)
				continue; /* Not this one! */
			if (driver_reladdr >= (result->d_phdr[phdr].p_vaddr +
			                       result->d_phdr[phdr].p_memsz))
				continue; /* Not this one! */
			/* Found it! */
			if unlikely(!tryincref(result))
				goto nope;
			decref_unlikely(ll);
			return result;
		}
		goto nope;
	}
nope:
	decref_unlikely(ll);
	return NULL;
}

PUBLIC NOBLOCK WUNUSED REF struct driver *
NOTHROW(FCALL driver_aboveaddr)(void const *addr) {
	REF struct driver_loadlist *ll;
	REF struct driver *result;
	size_t i;
	ll = get_driver_loadlist();
	assert_assume(ll->dll_count >= 1);
	for (i = 0; i < ll->dll_count; ++i) {
		if (addr >= result->d_module.md_loadmin) {
			if likely(tryincref(result)) {
				decref_unlikely(ll);
				return result;
			}
		}
	}
	decref_unlikely(ll);
	return NULL;
}

PUBLIC NOBLOCK WUNUSED REF struct driver *
NOTHROW(FCALL driver_next)(struct module *prev) {
	size_t i;
	REF struct driver *result;
	REF struct driver_loadlist *ll;
	ll = get_driver_loadlist();
	assert_assume(ll->dll_count >= 1);
	i = 0;
	if (prev != NULL) {
		/* Find the index of the successor of `prev' */
		for (;; ++i) {
			if (i >= ll->dll_count - 1)
				return NULL; /* No successor... */
			if (&ll->dll_drivers[i]->d_module == prev) {
				++i;
				break;
			}
		}
	}
	do {
		result = ll->dll_drivers[i];
		if (tryincref(result))
			break;
		result = NULL;
	} while (++i < ll->dll_count);
	decref_unlikely(ll);
	return result;
}

/* Lookup an already-loaded driver, given its name or file. */
PUBLIC WUNUSED REF struct driver *FCALL
driver_fromname(USER CHECKED char const *driver_name)
		THROWS(E_SEGFAULT) {
	size_t i;
	REF struct driver *result;
	REF struct driver_loadlist *ll;
	ll = get_driver_loadlist();
	FINALLY_DECREF_UNLIKELY(ll);
	assert_assume(ll->dll_count >= 1);
	for (i = 0; i < ll->dll_count; ++i) {
		result = ll->dll_drivers[i];
		if (!tryincref(result))
			continue;
		if (strcmp(result->d_name, driver_name) == 0)
			return result;
		decref_unlikely(result);
	}
	return NULL;
}

PUBLIC WUNUSED REF struct driver *FCALL
driver_fromname_with_len(USER CHECKED char const *driver_name,
                         size_t driver_name_len)
		THROWS(E_SEGFAULT) {
	size_t i;
	REF struct driver *result;
	REF struct driver_loadlist *ll;
	ll = get_driver_loadlist();
	FINALLY_DECREF_UNLIKELY(ll);
	assert_assume(ll->dll_count >= 1);
	for (i = 0; i < ll->dll_count; ++i) {
		result = ll->dll_drivers[i];
		if (!tryincref(result))
			continue;
		if (strcmpz(result->d_name, driver_name, driver_name_len) == 0)
			return result;
		decref_unlikely(result);
	}
	return NULL;
}

PUBLIC NOBLOCK WUNUSED NONNULL((1)) REF struct driver *
NOTHROW(FCALL driver_fromfile)(struct mfile *__restrict driver_file) {
	size_t i;
	REF struct driver *result;
	REF struct driver_loadlist *ll;
	ll = get_driver_loadlist();
	assert_assume(ll->dll_count >= 1);
	for (i = 0; i < ll->dll_count; ++i) {
		result = ll->dll_drivers[i];
		if (result->d_module.md_file == driver_file) {
			if (tryincref(result)) {
				decref_unlikely(ll);
				return result;
			}
			break;
		}
	}
	decref_unlikely(ll);
	return NULL;
}



/************************************************************************/
/* Driver helper functions                                              */
/************************************************************************/

/* Return the mem-file belonging to the given driver.
 * If the driver  wasn't loaded form  a file, try  to search for  that
 * file within the root filesystem within paths from `driver_libpath'.
 * If the file still can't be found, return `NULL'. */
PUBLIC WUNUSED NONNULL((1)) struct mfile *FCALL
driver_getfile(struct driver *__restrict self)
		THROWS(E_IOERROR, E_WOULDBLOCK, E_BADALLOC) {
	if (!self->d_module.md_file) {
		/* TODO */
		COMPILER_IMPURE();
	}
	return self->d_module.md_file;
}




/************************************************************************/
/* Driver symbol lookup functions                                       */
/************************************************************************/
PRIVATE WUNUSED NONNULL((1, 2)) bool FCALL
driver_dlsym_drv(struct driver *__restrict self,
                 struct driver_syminfo *__restrict info)
		THROWS(E_SEGFAULT, ...) {
	USER CHECKED char const *name = info->dsi_name + 4;
	if (strcmp(name, "self") == 0) {
		info->dsi_addr = self;
		info->dsi_size = sizeof(*self);
		goto ok;
	}
	if (name[0] == 'l' && name[1] == 'o' &&
	    name[2] == 'a' && name[3] == 'd') {
		USER CHECKED char const *load_name = name + 4;
		if (strcmp(load_name, "addr") == 0) {
			info->dsi_addr = (void *)self->d_module.md_loadaddr;
			goto ok_size0;
		}
		if (strcmp(load_name, "min") == 0) {
			info->dsi_addr = self->d_module.md_loadmin;
			goto ok_size0;
		}
		if (strcmp(load_name, "max") == 0) {
			info->dsi_addr = self->d_module.md_loadmax;
			goto ok_size0;
		}
	}
	if (strcmp(name, "name") == 0) {
		info->dsi_addr = (void *)self->d_name;
		info->dsi_size = (strlen(self->d_name) + 1) * sizeof(char);
		goto ok;
	}
	if (strcmp(name, "file") == 0) {
		info->dsi_addr = driver_getfile(self);
		info->dsi_size = sizeof(void *);
		goto ok;
	}
	if (strcmp(name, "cmdline") == 0) {
		info->dsi_addr = (void *)self->d_cmdline;
		info->dsi_size = (strlen(self->d_cmdline) + 1) * sizeof(char);sizeof(void *);
		goto ok;
	}
	if (name[0] == 'a' && name[1] == 'r' && name[2] == 'g') {
		USER CHECKED char const *arg_name = name + 3;
		if (strcmp(arg_name, "c") == 0) {
			info->dsi_addr = &self->d_argc;
			info->dsi_size = sizeof(self->d_argc);
			goto ok;
		}
		if (strcmp(arg_name, "v") == 0) {
			info->dsi_addr = self->d_argv;
			info->dsi_size = self->d_argc * sizeof(char *);
			goto ok;
		}
		if (arg_name[0] == '$') {
			/* TODO */
		}
	}
	return false;
ok_size0:
	info->dsi_size = 0;
ok:
	info->dsi_bind = STB_GLOBAL;
	return true;
}

LOCAL ATTR_PURE WUNUSED NONNULL((1)) u32 FCALL
gnu_symhash(USER CHECKED char const *name) {
	u32 h = 5381;
	for (; *name; ++name) {
		h = h * 33 + (u8)*name;
	}
	return h;
}

LOCAL ATTR_PURE WUNUSED NONNULL((1)) u32 FCALL
elf_symhash(USER CHECKED char const *name) {
	u32 h = 0;
	while (*name) {
		u32 g;
		h = (h << 4) + *name++;
		g = h & 0xf0000000;
		if (g)
			h ^= g >> 24;
		h &= ~g;
	}
	return h;
}


struct kernel_syment {
	char const *ks_name; /* [0..1] Symbol name (NULL for sentinel) */
	void       *ks_addr; /* Symbol address */
	u32         ks_size; /* Symbol size */
	u32         ks_hash; /* Symbol hash (s.a. `elf_symhash()') */
};
struct kernel_symtab {
	uintptr_t                                     ds_mask;  /* Hash mask. */
	COMPILER_FLEXIBLE_ARRAY(struct kernel_syment, ds_list); /* Symbol map. */
};

/* The kernel symbol table (containing everything marked as PUBLIC) */
extern struct kernel_symtab kernel_symbol_table;

/* Lookup a symbol within the kernel core's symbol table. */
PRIVATE WUNUSED NONNULL((1)) bool FCALL
kernel_dlsym(struct driver_syminfo *__restrict info)
		THROWS(E_SEGFAULT, ...) {
	uint32_t hash = info->dsi_elfhash;
	uint32_t i, perturb;
	if (hash == (uint32_t)-1)
		hash = info->dsi_elfhash = elf_symhash(info->dsi_name);
	perturb = i = hash & kernel_symbol_table.ds_mask;
	/* NOTE: Changes to this algorithm must be mirrored in /src/kernel/core/.sources */
	for (;; i = ((i << 2) + i + perturb + 1), perturb >>= 5) {
		uint32_t index;
		index = i & kernel_symbol_table.ds_mask;
		if (!kernel_symbol_table.ds_list[index].ks_name)
			break; /* Sentinel */
		if (kernel_symbol_table.ds_list[index].ks_hash != hash)
			continue; /* Different hash */
		if (strcmp(kernel_symbol_table.ds_list[index].ks_name, info->dsi_name) != 0)
			continue; /* Different name */
		/* Found it! */
		info->dsi_addr = kernel_symbol_table.ds_list[index].ks_addr;
		info->dsi_size = kernel_symbol_table.ds_list[index].ks_size;
		info->dsi_bind = STB_GLOBAL;
		return true;
	}
	return false;
}

#if ELF_ARCH_CLASS == ELFCLASS32
#define ELF_CLASSBITS 32
#elif ELF_ARCH_CLASS == ELFCLASS64
#define ELF_CLASSBITS 64
#else /* ELF_ARCH_CLASS == ... */
#define ELF_CLASSBITS (__SIZEOF_POINTER__ * 8)
#endif /* ELF_ARCH_CLASS != ... */


PRIVATE ATTR_COLD ATTR_NOINLINE NONNULL((1, 2)) void FCALL
print_corrupted_symbol_table_error_message(struct driver *__restrict self,
                                           char const *__restrict kind) {
	printk(KERN_ERR "[mod] %s symbol hash table of ", kind);
	module_printpath_or_name(&self->d_module, &syslog_printer, SYSLOG_LEVEL_ERR);
	printk(KERN_ERR "is corrupt\n");
}

/* Lookup a symbol from the ELF .dynsym section. */
PRIVATE WUNUSED NONNULL((1, 2)) ElfW(Sym) const *FCALL
driver_dlsym_elf(struct driver *__restrict self,
                 struct driver_syminfo *__restrict info)
		THROWS(E_SEGFAULT, ...) {
	ElfW(Sym) const *result;
	/************************************************************************/
	/* GNU hash table support                                               */
	/************************************************************************/
	{
		ElfW(GnuHashTable) const *gnu_ht;
		if ((gnu_ht = self->d_gnuhashtab) != NULL) {
			/* This implementation is derived from:
			 * https://flapenguin.me/2017/05/10/elf-lookup-dt-gnu-hash/
			 * https://sourceware.org/ml/binutils/2006-10/msg00377.html
			 */
			ElfW(Word) symid, gh_symoffset;
			ElfW(Word) const *gh_buckets;
			ElfW(Word) const *gh_chains;
			ElfW(Addr) bloom_word, bloom_mask;
			uint32_t hash = info->dsi_gnuhash;
			if (hash == (uint32_t)-1)
				hash = info->dsi_gnuhash = gnu_symhash(info->dsi_name);
			if unlikely(!gnu_ht->gh_bloom_size || !gnu_ht->gh_bloom_size)
				goto nosym_no_gnu_ht;
			gh_symoffset = gnu_ht->gh_symoffset;
			gh_buckets   = (ElfW(Word) const *)(gnu_ht->gh_bloom + gnu_ht->gh_bloom_size);
			gh_chains    = (ElfW(Word) const *)(gh_buckets + gnu_ht->gh_nbuckets);
			bloom_word   = gnu_ht->gh_bloom[(hash / ELF_CLASSBITS) % gnu_ht->gh_bloom_size];
			bloom_mask   = ((ElfW(Addr))1 << (hash % ELF_CLASSBITS)) |
			               ((ElfW(Addr))1 << ((hash >> gnu_ht->gh_bloom_shift) % ELF_CLASSBITS));
			if ((bloom_word & bloom_mask) != bloom_mask)
				goto nosym;
			symid = gh_buckets[hash % gnu_ht->gh_nbuckets];
			if unlikely(symid < gh_symoffset)
				goto nosym;
			/* Search for the symbol. */
			for (;; ++symid) {
				ElfW(Word) enthash;
				result  = self->d_dynsym_tab + symid;
				enthash = gh_chains[symid - gh_symoffset];
				if likely((hash | 1) == (enthash | 1)) {
					if likely(strcmp(info->dsi_name, self->d_dynstr + result->st_name) == 0)
						return result; /* Found it! */
				}
				if unlikely(enthash & 1)
					break; /* End of chain */
			}
			goto nosym;
		}
	}

search_elf_table:
	/************************************************************************/
	/* ELF hash table support                                               */
	/************************************************************************/
	{
		ElfW(HashTable) const *elf_ht;
		if ((elf_ht = self->d_hashtab) != NULL) {
			ElfW(Word) const *ht_chains;
			ElfW(Word) max_attempts, chain;
			uint32_t hash = info->dsi_elfhash;
			if (hash == (uint32_t)-1)
				hash = info->dsi_elfhash = elf_symhash(info->dsi_name);
			if unlikely(!elf_ht->ht_nbuckts || !elf_ht->ht_nchains)
				goto nosym_no_elf_ht;
			max_attempts = elf_ht->ht_nchains;
			ht_chains    = elf_ht->ht_table + elf_ht->ht_nbuckts;
			chain        = elf_ht->ht_table[hash % elf_ht->ht_nbuckts];
			do {
				if unlikely(chain == STN_UNDEF)
					break; /* End of chain. */
				if unlikely(chain >= elf_ht->ht_nchains)
					goto nosym_no_elf_ht; /* Corrupted hash-table */
				result = self->d_dynsym_tab + chain;
				if likely(strcmp(info->dsi_name, self->d_dynstr + result->st_name) == 0)
					return result; /* Found it! */
				/* Load the next chain entry. */
				chain = ht_chains[chain];
			} while likely(--max_attempts);
			goto nosym;
		}
	}

search_dynsym:
	/************************************************************************/
	/* Do a linear search over the symbol table.                            */
	/************************************************************************/
	{
		ElfW(Sym) const *dynsym;
		if ((dynsym = self->d_dynsym_tab) != NULL) {
			size_t i, dyncnt;
			dyncnt = self->d_dynsym_cnt;
			for (i = 0; i < dyncnt; ++i) {
				char const *symname;
				symname = self->d_dynstr + dynsym[i].st_name;
				if (strcmp(info->dsi_name, symname) != 0)
					continue;
				/* Found it! */
				return &dynsym[i];
			}
		}
	}
nosym:
	return NULL;
nosym_no_gnu_ht:
	print_corrupted_symbol_table_error_message(self, "GNU");
	self->d_gnuhashtab = NULL;
	goto search_elf_table;
nosym_no_elf_ht:
	print_corrupted_symbol_table_error_message(self, "ELF");
	self->d_hashtab = NULL;
	goto search_dynsym;
}



/* Lookup a symbol  within a driver,  given its  name.
 * Note that this function will _not_ look into driver
 * dependencies in order to resolve that symbol.
 * NOTE: This function also takes care of the special
 *       set of `drv_*' symbols!
 * @return: true:  Found a symbol matching the given name.
 * @return: false: No symbol matching the given name found. */
PUBLIC WUNUSED NONNULL((1, 2)) bool FCALL
driver_dlsym_local(struct driver *__restrict self,
                   struct driver_syminfo *__restrict info)
		THROWS(E_SEGFAULT, ...) {
	ElfW(Sym) const *symbol;
	/* Check for special symbols from the `drv_*' namespace. */
	if (info->dsi_name[0] == 'd' && info->dsi_name[1] == 'r' &&
	    info->dsi_name[2] == 'v' && info->dsi_name[3] == '_') {
		if (driver_dlsym_drv(self, info))
			return true;
	}
	/* Check for the special case with the kernel core. */
	if (self == &kernel_driver)
		return kernel_dlsym(info);

	/* Do a generic ELF .dynsym lookup. */
	symbol = driver_dlsym_elf(self, info);
	if (symbol && symbol->st_shndx != SHN_UNDEF) {
		uintptr_t st_addr;
		unsigned char st_info;
		st_info = ATOMIC_READ(symbol->st_info);
		st_addr = symbol->st_value;
		if (symbol->st_shndx != SHN_ABS)
			st_addr += self->d_module.md_loadaddr;
		if (ELFW(ST_TYPE)(st_info) == STT_GNU_IFUNC ||
		    ELFW(ST_TYPE)(st_info) == STT_KOS_IDATA) {
			/* Resolve dynamic address. */
			st_addr = (*(uintptr_t(*)(void))st_addr)();
			/* TODO: Write-back the resolved address, and change
			 *       the typing of  the symbol to  no longer  be
			 *       indirect! */
		}
		info->dsi_addr = (void *)st_addr;
		info->dsi_size = symbol->st_size;
		info->dsi_bind = ELFW(ST_BIND)(st_info);
		return true;
	}
	return false;
}



#define DRIVER_DLSYM_EX_NOENT 0 /* Symbol not found */
#define DRIVER_DLSYM_EX_OK    1 /* Success */
#define DRIVER_DLSYM_EX_EOF   2 /* No drivers found at `depth' */

PRIVATE WUNUSED NONNULL((1, 2)) unsigned int FCALL
driver_dlsym_ex(struct driver *__restrict self,
                struct driver_syminfo *__restrict info,
                unsigned int depth)
		THROWS(E_SEGFAULT, ...) {
	unsigned int result, status;
	size_t i;
	if (depth == 0) {
		if (driver_dlsym_local(self, info))
			return DRIVER_DLSYM_EX_OK;
		return DRIVER_DLSYM_EX_NOENT;
	}
	--depth;
	result = DRIVER_DLSYM_EX_EOF;
	for (i = 0; i < self->d_depcnt; ++i) {
		REF struct driver *dep;
		dep = axref_get(&self->d_depvec[i]);
		if (!dep)
			continue;
		{
			FINALLY_DECREF_UNLIKELY(dep);
			status = driver_dlsym_ex(dep, info, depth);
		}
		if (status != DRIVER_DLSYM_EX_EOF) {
			result = status;
			if (result == DRIVER_DLSYM_EX_OK)
				break;
		}
	}
	return result;
}

/* Same  as `driver_dlsym_local()', but if that function fails,
 * or  returns a weak  symbol, scan all  of the dependencies of
 * `self'  for another symbol with the same name. If a non-weak
 * symbol is found return it. Otherwise, return the first  weak
 * symbol encountered during the search, and if all that fails,
 * return `false' to indicate failure.
 * @return: true:  Found a symbol matching the given name.
 * @return: false: No symbol matching the given name found. */
PUBLIC WUNUSED NONNULL((1, 2)) bool FCALL
driver_dlsym(struct driver *__restrict self,
             struct driver_syminfo *__restrict info)
		THROWS(E_SEGFAULT, ...) {
	unsigned int status, depth, max_depth;
	bool result = driver_dlsym_local(self, info);
	/* All drivers have an implicit dependency on the kernel core! */
	if (!result)
		result = kernel_dlsym(info);
	if (!result) {
		REF struct driver_loadlist *ll;
		ll        = get_driver_loadlist();
		max_depth = (unsigned int)ll->dll_count;
		decref_unlikely(ll);
		for (depth = 0; depth < max_depth; ++depth) {
			status = driver_dlsym_ex(self, info, depth);
			if (status != DRIVER_DLSYM_EX_NOENT) {
				if (status == DRIVER_DLSYM_EX_OK) {
					result = true;
					if (info->dsi_bind == STB_WEAK) {
						++depth;
						goto find_nonweak_after;
					}
				}
				break;
			}
		}
	} else if (info->dsi_bind == STB_WEAK) {
		REF struct driver_loadlist *ll;
		struct driver_syminfo info2;
		ll        = get_driver_loadlist();
		max_depth = (unsigned int)ll->dll_count;
		decref_unlikely(ll);
		depth = 0;
find_nonweak_after:
		info2.dsi_name    = info->dsi_name;
		info2.dsi_elfhash = info->dsi_elfhash;
		info2.dsi_gnuhash = info->dsi_gnuhash;
		for (; depth < max_depth; ++depth) {
			status = driver_dlsym_ex(self, &info2, depth);
			if (status != DRIVER_DLSYM_EX_NOENT) {
				if (status == DRIVER_DLSYM_EX_OK && info2.dsi_bind == STB_WEAK)
					continue; /* Another weak symbol (skip; we only ever use the first one found!) */
				info->dsi_addr = info2.dsi_addr;
				info->dsi_size = info2.dsi_size;
				info->dsi_bind = info2.dsi_bind;
				break;
			}
		}
	}
	return result;
}

/* Search for a symbol in all loaded drivers, following the order
 * of drivers, as they appear returned by `get_driver_loadlist()' */
PUBLIC WUNUSED NONNULL((1)) bool FCALL
driver_dlsym_global(struct driver_syminfo *__restrict info)
		THROWS(E_SEGFAULT, ...) {
	size_t i;
	REF struct driver_loadlist *ll;
	ll = get_driver_loadlist();
	FINALLY_DECREF_UNLIKELY(ll);
	assert_assume(ll->dll_count >= 1);
	for (i = 0; i < ll->dll_count; ++i) {
		bool ok;
		REF struct driver *d;
		d = ll->dll_drivers[i];
		if (!tryincref(d))
			continue;
		{
			FINALLY_DECREF_UNLIKELY(d);
			ok = driver_dlsym_local(d, info);
		}
		if (!ok)
			continue;
		if (info->dsi_bind == STB_WEAK) {
			/* Try to find another driver that may define this symbol as non-weak! */
			struct driver_syminfo info2;
			info2.dsi_name    = info->dsi_name;
			info2.dsi_elfhash = info->dsi_elfhash;
			info2.dsi_gnuhash = info->dsi_gnuhash;
			for (++i; i < ll->dll_count; ++i) {
				d = ll->dll_drivers[i];
				if (!tryincref(d))
					continue;
				{
					FINALLY_DECREF_UNLIKELY(d);
					ok = driver_dlsym_local(d, info);
				}
				/* Only accept non-weak symbols at this point! */
				if (!ok || info2.dsi_bind == STB_WEAK)
					continue;
				/* Found a non-weak symbol! */
				info->dsi_addr = info2.dsi_addr;
				info->dsi_size = info2.dsi_size;
				info->dsi_bind = info2.dsi_bind;
				break;
			}
		}
		return true;
	}
	return false;
}

/* Helper wrappers for the above functions that simply take the
 * name of the symbol and will return its address, or `NULL' if
 * no such symbol could be found. */
PUBLIC WUNUSED NONNULL((1)) void *FCALL
driver_dlsym_local_f(struct driver *__restrict self,
                     USER CHECKED char const *name)
		THROWS(E_SEGFAULT, ...) {
	struct driver_syminfo info;
	driver_syminfo_init(&info, name);
	if (!driver_dlsym_local(self, &info))
		info.dsi_addr = NULL;
	return info.dsi_addr;
}

PUBLIC WUNUSED NONNULL((1)) void *FCALL
driver_dlsym_f(struct driver *__restrict self,
               USER CHECKED char const *name)
		THROWS(E_SEGFAULT, ...) {
	struct driver_syminfo info;
	driver_syminfo_init(&info, name);
	if (!driver_dlsym(self, &info))
		info.dsi_addr = NULL;
	return info.dsi_addr;
}

PUBLIC WUNUSED NONNULL((1)) void *FCALL
driver_dlsym_global_f(USER CHECKED char const *name)
		THROWS(E_SEGFAULT, ...) {
	struct driver_syminfo info;
	driver_syminfo_init(&info, name);
	if (!driver_dlsym_global(&info))
		info.dsi_addr = NULL;
	return info.dsi_addr;
}



PRIVATE WUNUSED NONNULL((1)) bool
NOTHROW(FCALL kernel_dladdr)(struct driver_symaddr *__restrict info,
                             void const *addr) {
	uintptr_t i;
	for (i = 0; i <= kernel_symbol_table.ds_mask; ++i) {
		void *end;
		if ((byte_t *)kernel_symbol_table.ds_list[i].ks_addr > (byte_t *)addr)
			continue;
		end = (void *)((uintptr_t)kernel_symbol_table.ds_list[i].ks_addr +
		               kernel_symbol_table.ds_list[i].ks_size);
		if ((byte_t *)addr >= (byte_t *)end)
			continue;
		/* Found it! */
		info->dsa_addr = kernel_symbol_table.ds_list[i].ks_addr;
		info->dsa_size = kernel_symbol_table.ds_list[i].ks_size;
		info->dsa_name = kernel_symbol_table.ds_list[i].ks_name;
		info->dsa_bind = STB_GLOBAL;
		return true;
	}
	return false;
}

PRIVATE NOBLOCK WUNUSED NONNULL((1)) ElfW(Sym) const *
NOTHROW(FCALL driver_dladdr_elf)(struct driver *__restrict self,
                                 uintptr_t driver_reladdr) {
	size_t i, count;
	ElfW(Sym) const *result;
	uintptr_t prev_symbol_end;
	count = self->d_dynsym_cnt;

	/* First pass: Search for a symbol that contains the given address exactly. */
	for (i = 0; i < count; ++i) {
		result = &self->d_dynsym_tab[i];
		if (result->st_shndx == SHN_UNDEF ||
		    result->st_shndx == SHN_ABS)
			continue;
		if (result->st_value > driver_reladdr)
			continue;
		if (result->st_value == driver_reladdr)
			return result; /* Exact match */
		if (result->st_size != 0) {
			/* Symbol has a defined size value. */
			if (driver_reladdr < (result->st_value + result->st_size))
				return result; /* The given address is contained within this one. */
		}
	}

	/* Second pass: Search for the nearest open-ended symbol defined below `driver_reladdr' */
	prev_symbol_end = 0;
	result = NULL;
	for (i = 0; i < count; ++i) {
		ElfW(Sym) const *sym;
		sym = &self->d_dynsym_tab[i];
		if (sym->st_shndx == SHN_UNDEF ||
		    sym->st_shndx == SHN_ABS)
			continue;
		if (sym->st_value > driver_reladdr)
			continue;
		if (result->st_size != 0) {
			uintptr_t symend;
			symend = result->st_value + result->st_size;
			assertf(symend <= driver_reladdr,
			        "Otherwise, the symbol would be a perfect match, which "
			        "we would have already detected during the first pass");
			if (symend > prev_symbol_end) {
				/* Closer matching symbol. */
				prev_symbol_end = symend;
				result          = NULL; /* Closed end */
			}
		} else if (sym->st_value > prev_symbol_end) {
			/* Open-ended symbol above previous symbol.
			 * Use this one unless we find another one that's closer. */
			prev_symbol_end = sym->st_value;
			result          = sym;
		}
	}
	assert(!result || (result->st_shndx != SHN_UNDEF &&
	                   result->st_shndx != SHN_ABS));
	assert(!result || result->st_size == 0);
	assert(!result || result->st_value < driver_reladdr);
	return result;
}

/* Do the reverse of the `dlsym()' functions above, and resolve
 * a  driver-relative `driver_reladdr' to try to determine info
 * about the symbol that contains that address.
 * @return: true:  Success.
 * @return: false: Failure. */
PUBLIC NOBLOCK WUNUSED NONNULL((1, 2)) bool
NOTHROW(FCALL driver_dladdr_local)(struct driver *__restrict self,
                                   struct driver_symaddr *__restrict info,
                                   uintptr_t driver_reladdr) {
	ElfW(Sym) const *symbol;
	/* Special case for the kernel core. */
	if (self == &kernel_driver)
		return kernel_dladdr(info, (void *)driver_reladdr);

	/* Do a generic ELF .dynsym lookup. */
	symbol = driver_dladdr_elf(self, driver_reladdr);
	if (symbol) {
		info->dsa_addr = (void *)(self->d_module.md_loadaddr + symbol->st_value);
		info->dsa_size = symbol->st_size;
		info->dsa_name = self->d_dynstr + symbol->st_name;
		info->dsa_bind = ELFW(ST_BIND)(symbol->st_info);
		return true;
	}
	return false;
}

/* Similar to `driver_dladdr_local()', but uses `driver_fromaddr()'
 * to lookup the module containing  `addr', and follows this up  by
 * using `driver_dladdr_local()' */
PUBLIC NOBLOCK WUNUSED NONNULL((1)) REF struct driver *
NOTHROW(FCALL driver_dladdr)(struct driver_symaddr *__restrict info,
                             void const *addr) {
	REF struct driver *drv;
	drv = driver_fromaddr(addr);
	if (!drv)
		return NULL;
	if (!driver_dladdr_local(drv, info,
	                         (uintptr_t)addr -
	                         drv->d_module.md_loadaddr)) {
		decref_unlikely(drv);
		drv = NULL;
	}
	return drv;
}


/* FDE Cache API */
struct driver_fde_cache {
	unwind_fde_t                           dfc_fde;    /* Cached FDE */
	size_t                                 dfc_heapsz; /* Allocated heap size (least significant bit is R/B-bit; 0:black; 1:red) */
	LLRBTREE_NODE(struct driver_fde_cache) dfc_node;   /* R/B-node */
};
#define driver_fde_cache_heapsz(self) ((self)->dfc_heapsz & ~1)

DECL_END

#define RBTREE_LEFT_LEANING
#define RBTREE_OMIT_REMOVE
#define RBTREE_OMIT_REMOVENODE
#define RBTREE_OMIT_INSERT
#define RBTREE_WANT_TRYINSERT
#define RBTREE_DECL                PRIVATE
#define RBTREE_IMPL                PRIVATE
#define RBTREE(name)               dfc_##name
#define RBTREE_T                   struct driver_fde_cache
#define RBTREE_Tkey                void const *
#define RBTREE_CC                  FCALL
#define RBTREE_NOTHROW             NOTHROW
#define RBTREE_NODEPATH            dfc_node
#define RBTREE_ISRED(self)         ((self)->dfc_heapsz & 1)
#define RBTREE_SETRED(self)        (void)((self)->dfc_heapsz |= 1)
#define RBTREE_SETBLACK(self)      (void)((self)->dfc_heapsz &= ~1)
#define RBTREE_FLIPCOLOR(self)     (void)((self)->dfc_heapsz ^= 1)
#define RBTREE_COPYCOLOR(dst, src) (void)((dst)->dfc_heapsz &= ~1, (dst)->dfc_heapsz |= ((src)->dfc_heapsz & 1))
#define RBTREE_GETNODE(self)       (self)->dfc_node
#define RBTREE_GETMINKEY(node)     (node)->dfc_fde.f_pcstart
#define RBTREE_GETMAXKEY(node)     ((byte_t *)(node)->dfc_fde.f_pcend - 1)
#include <hybrid/sequence/rbtree-abi.h>

DECL_BEGIN

PRIVATE ATTR_USED NOBLOCK NONNULL((1)) size_t
NOTHROW(FCALL dfc_freetree)(struct driver_fde_cache *__restrict tree) {
	size_t result, temp;
	struct driver_fde_cache *mi, *ma;
	result = 0;
again:
	mi   = tree->dfc_node.rb_lhs;
	ma   = tree->dfc_node.rb_rhs;
	temp = driver_fde_cache_heapsz(tree);
	result += temp;
	heap_free_untraced(&kernel_locked_heap, tree, temp, GFP_LOCKED);
	if (mi) {
		if (ma)
			result += dfc_freetree(ma);
		tree = mi;
		goto again;
	}
	if (ma) {
		tree = ma;
		goto again;
	}
	return result;
}

/* Try to clear the FDE cache of the given, or of all loaded drivers. */
PRIVATE ATTR_USED NOBLOCK NONNULL((1)) size_t
NOTHROW(FCALL clear_fde_cache)(struct driver *__restrict self) {
	size_t result = 0;
	if (atomic_rwlock_trywrite(&self->d_eh_frame_cache_lock)) {
		struct driver_fde_cache *tree;
		tree = self->d_eh_frame_cache;
		self->d_eh_frame_cache = NULL;
		atomic_rwlock_endwrite(&self->d_eh_frame_cache_lock);
		if (tree)
			result = dfc_freetree(tree);
	}
	return result;
}

DEFINE_SYSTEM_CACHE_CLEAR(clear_fde_caches);
PRIVATE ATTR_USED NOBLOCK size_t
NOTHROW(KCALL clear_fde_caches)(void) {
	size_t i, result = 0;
	REF struct driver_loadlist *ll;
	ll = get_driver_loadlist();
	for (i = 0; i < ll->dll_count; ++i) {
		REF struct driver *drv;
		drv = ll->dll_drivers[i];
		if (!tryincref(drv))
			continue;
		result += clear_fde_cache(drv);
		decref_unlikely(drv);
	}
	decref_unlikely(ll);
	return result;
}


/* Lookup the FDE descriptor for a given `absolute_pc', whilst trying to
 * make use of the FDE cache of `self'.
 * @return: * : One of `UNWIND_*' from <libunwind/api.h> */
PUBLIC NOBLOCK NONNULL((1)) unsigned int
NOTHROW(FCALL driver_findfde)(struct driver *__restrict self, void const *absolute_pc,
                              unwind_fde_t *__restrict result) {
	unsigned int status;

	/* Try to look through the driver's FDE cache for a descriptor. */
	if (atomic_rwlock_tryread(&self->d_eh_frame_cache_lock)) {
		struct driver_fde_cache *cache;
		cache = dfc_locate(self->d_eh_frame_cache, absolute_pc);
		if (cache) {
			memcpy(result, &cache->dfc_fde, sizeof(unwind_fde_t));
			atomic_rwlock_endread(&self->d_eh_frame_cache_lock);
			return UNWIND_SUCCESS;
		}
		atomic_rwlock_endread(&self->d_eh_frame_cache_lock);
	}

	/* Scan the .eh_frame section of the driver. */
	status = unwind_fde_scan(self->d_eh_frame_start,
	                         self->d_eh_frame_end,
	                         absolute_pc, result,
	                         sizeof(void *));

	/* Try to cache the results  upon success, but don't do  so
	 * if the kernel's been poisoned, or trying to the lock the
	 * cache for writing would most likely block. */
	if likely(status == UNWIND_SUCCESS && !kernel_poisoned() &&
	          atomic_rwlock_canwrite(&self->d_eh_frame_cache_lock)) {
		struct heapptr cp;
		assert(absolute_pc >= result->f_pcstart);
		assert(absolute_pc < result->f_pcend);
		cp = heap_alloc_untraced_nx(&kernel_locked_heap,
		                            sizeof(struct driver_fde_cache),
		                            GFP_ATOMIC | GFP_LOCKED | GFP_PREFLT);
		if (cp.hp_siz != 0) {
			struct driver_fde_cache *cache;
			cache = (struct driver_fde_cache *)cp.hp_ptr;
			memcpy(&cache->dfc_fde, result, sizeof(unwind_fde_t));
			cache->dfc_heapsz = cp.hp_siz;
			if likely(atomic_rwlock_trywrite(&self->d_eh_frame_cache_lock)) {
				bool ok;
				/* Try to insert the entry into the cache. */
				ok = dfc_tryinsert(&self->d_eh_frame_cache, cache);
				atomic_rwlock_endwrite(&self->d_eh_frame_cache_lock);
				if unlikely(!ok)
					goto cannot_cache;
			} else {
cannot_cache:
				heap_free_untraced(&kernel_locked_heap,
				                   cp.hp_ptr, cp.hp_siz,
				                   GFP_LOCKED);
			}
		}
	}
	return status;
}



/* NOTE: This function is normally part of libunwind, but in kernel-space
 *       is implemented here so-as to allow for per-driver caching of FDE
 *       descriptors. */

/* Lookup FDE information  associated with a  given program counter  position.
 * Using integration with KOS's DL extension APIs, this function automatically
 * accesses the `.eh_frame' sections of  the module associated with the  given
 * address, as well as  keep track of a  lazily allocated address-tree of  FDE
 * caches for quick (O(1))  repeated access to an  FDE located within a  known
 * function. */
INTERN NOBLOCK NONNULL((2)) unsigned int
NOTHROW_NCX(KCALL libuw_unwind_fde_find)(void const *absolute_pc,
                                         unwind_fde_t *__restrict result) {
	unsigned int error;
	REF struct driver *d;
	/* Figure out which driver is mapped at the given `absolute_pc' */
	d = driver_fromaddr(absolute_pc);
	if unlikely(!d)
		return UNWIND_NO_FRAME;
	/* Lookup the FDE descriptor within the driver */
	error = driver_findfde(d, absolute_pc, result);
	decref_unlikely(d);
	return error;
}
DEFINE_PUBLIC_ALIAS(unwind_fde_find, libuw_unwind_fde_find);








/************************************************************************/
/* Driver load/unload functions                                         */
/************************************************************************/

/* Signal broadcast whenever the `d_state' of any loaded driver changes. */
PUBLIC struct sig driver_state_changed = SIG_INIT;

#define THROW_FAULTY_ELF_ERROR(...)           \
	THROW(E_NOT_EXECUTABLE_FAULTY,            \
	      E_NOT_EXECUTABLE_FAULTY_FORMAT_ELF, \
	      __VA_ARGS__)

INTDEF NOBLOCK NONNULL((1, 2)) Tobpostlockop(struct mpart) * /* from "mnode.c" */
NOTHROW(FCALL mnode_unlink_from_part_lockop)(Toblockop(struct mpart) *__restrict self,
                                             struct mpart *__restrict part);
INTDEF NOBLOCK NONNULL((1)) void /* from "mnode.c" */
NOTHROW(FCALL mpart_maybe_clear_mlock)(struct mpart *__restrict self);

/* Free a given mem-node that was destroyed when unmapping a driver. */
PRIVATE NOBLOCK NONNULL((1)) void
NOTHROW(FCALL driver_deadnodes_free)(struct mnode *__restrict self) {
	REF struct mpart *part;
	xdecref(self->mn_fspath);
	xdecref(self->mn_fsname);
	if ((part = self->mn_part) != NULL) {
		/* Try to unlink the node from the copy- or share-chain of the associated part. */
		if (mpart_lock_tryacquire(part)) {
			LIST_REMOVE(self, mn_link);
			if ((self->mn_flags & (MNODE_F_MLOCK)) &&
			    (part->mp_flags & (MPART_F_MLOCK | MPART_F_MLOCK_FROZEN)) == MPART_F_MLOCK)
				mpart_maybe_clear_mlock(part);
			mpart_lock_release(part);
			decref_unlikely(part);
		} else {
			Toblockop(struct mpart) *lop;
			/* Must insert the node into the part's list of deleted nodes. */
			weakincref(self->mn_mman); /* A weak reference here is required by the ABI */
			DBG_memset(&self->mn_part, 0xcc, sizeof(self->mn_part));

			/* Insert into the  lock-operations list of  `part'
			 * The act of doing this is what essentially causes
			 * ownership of our node to be transfered to `part' */
			lop = (Toblockop(struct mpart) *)self;
			lop->olo_func = &mnode_unlink_from_part_lockop;
			SLIST_ATOMIC_INSERT(&part->mp_lockops, lop, olo_link);

			/* Try to reap dead nodes. */
			_mpart_lockops_reap(part);

			/* Drop our old reference to the associated part. */
			decref(part);
			return;
		}
	}
	mnode_free(self);
}

/* Free all nodes from the given list of dead nodes. */
PRIVATE NOBLOCK NONNULL((1)) void
NOTHROW(FCALL driver_deadnodes_freelist)(struct mnode_slist *__restrict deadnodes) {
	while (!SLIST_EMPTY(deadnodes)) {
		struct mnode *node;
		node = SLIST_FIRST(deadnodes);
		SLIST_REMOVE_HEAD(deadnodes, _mn_dead);
		driver_deadnodes_free(node);
	}
}

/* While holding a lock to the kernel mman, unmap all of  the
 * mappings that are associated with the given driver `self'.
 * Destroyed nodes are added to `deadlist' */
PRIVATE NOBLOCK NONNULL((1)) void
NOTHROW(FCALL mman_remove_driver_nodes)(struct driver *__restrict self,
                                        struct mnode_slist *__restrict deadlist) {
	/* Go over all mem-nodes that cover the address range of `self',
	 * and  remove+unmap+unprepare all that have module-pointers set
	 * to point back at `self'.
	 * Doing this should result in exactly `self->d_module.md_nodecount'
	 * nodes being removed! */
	struct mnode_tree_minmax mima;
again_find_nodes:
	mman_mappings_minmaxlocate(&mman_kernel,
	                           self->d_module.md_loadmin,
	                           self->d_module.md_loadmax,
	                           &mima);
	if (mima.mm_min != NULL) {
		struct mnode *node;
		for (node = mima.mm_min;;) {
			if likely(node->mn_module == &self->d_module) {
#ifndef NDEBUG
				assert(self->d_module.md_nodecount != 0);
				--self->d_module.md_nodecount;
#endif /* !NDEBUG */
				/* Driver nodes _must_ have the NOMERGE+PREPARED flags set.
				 * Otherwise, we'd  be unable  to  unmap them  as  NOTHROW! */
				assert(node->mn_flags & MNODE_F_NOMERGE);
				assert(node->mn_flags & MNODE_F_MPREPARED);

				/* Must get rid of this node! */
				mman_mappings_removenode(&mman_kernel, node);

				/* Unmap+sync+unprepare the node. */
				mnode_pagedir_unmap(node);
				mnode_mman_supersync(node);
				mnode_pagedir_kernelunprepare(node);

				/* Set the UNMAPPED flag for the node */
				ATOMIC_OR(node->mn_flags, MNODE_F_UNMAPPED);

				/* Add the node to the list of dead nodes. */
				SLIST_INSERT(&self->_d_deadnodes, node, _mn_dead);
				goto again_find_nodes;
			}
			if (node == mima.mm_max)
				break;
			node = mnode_tree_nextnode(node);
			assert(node);
		}
	}
#ifdef NDEBUG
	assert(self->d_module.md_nodecount == 0);
#else /* NDEBUG */
	self->d_module.md_nodecount = 0;
#endif /* !NDEBUG */
}

/* While holding a lock to the kernel mman, unmap all of  the
 * mappings that are associated with the given driver `self'.
 * Destroyed nodes are added to `self->_d_deadnodes' */
#define driver_unmap_text(self)                               \
	(mman_remove_driver_nodes((self), &(self)->_d_deadnodes), \
	 (*(self)->d_module.md_ops->mo_nonodes)(&(self)->d_module))

PRIVATE NOBLOCK NONNULL((1)) void
NOTHROW(FCALL driver_finitext_postlop)(Tobpostlockop(struct mman) *__restrict lop,
                                       struct mman *__restrict UNUSED(mm)) {
	REF struct driver *me;
	me = container_of(lop, struct driver, _d_mm_postlop);
	sig_broadcast(&driver_state_changed);
	/* Destroy all of the dead mem-nodes. */
	driver_deadnodes_freelist(&me->_d_deadnodes);
	/* The reference inherited by the state-transition to `DRIVER_STATE_KILL' */
	decref(me);
}

PRIVATE NOBLOCK NONNULL((1)) Tobpostlockop(struct mman) *
NOTHROW(FCALL driver_finitext_lop)(Toblockop(struct mman) *__restrict lop,
                                   struct mman *__restrict UNUSED(mm)) {
	struct driver *me;
	me = container_of(lop, struct driver, _d_mm_lop);
	/* Unmap driver text sections. */
	driver_unmap_text(me);
	/* Setup a post lock operation to broadcast the state-change,
	 * as well as drop the self-reference we've inherited as  the
	 * result of inheriting */
	ATOMIC_WRITE(me->d_state, DRIVER_STATE_KILL);
	me->_d_mm_postlop.oplo_func = &driver_finitext_postlop;
	return &me->_d_mm_postlop;
}

/* Perform the necessary actions after the state of
 * `self' has been set to `DRIVER_STATE_FINI_TEXT'. */
PRIVATE NOBLOCK NONNULL((1)) void
NOTHROW(FCALL driver_runstate_fini_text)(struct driver *__restrict self) {
	/* Initialize the list of dead mem-nodes. */
	SLIST_INIT(&self->_d_deadnodes);

	/* Initiate the unmapping of  */
	if (mman_lock_tryacquire(&mman_kernel)) {
		driver_unmap_text(self);
		mman_lock_release_f(&mman_kernel);
		ATOMIC_WRITE(self->d_state, DRIVER_STATE_KILL);
		decref_nokill(self); /* The self-reference dropped by the transition to `DRIVER_STATE_KILL'! */
		sig_broadcast(&driver_state_changed);
		/* Destroy all of the dead mem-noeds. */
		driver_deadnodes_freelist(&self->_d_deadnodes);
		mman_lockops_reap(&mman_kernel);
	} else {
		self->_d_mm_lop.olo_func = &driver_finitext_lop;
		SLIST_ATOMIC_INSERT(&mman_kernel_lockops, &self->_d_mm_lop, olo_link);
		_mman_lockops_reap(&mman_kernel);
	}
}

/* Perform the necessary actions after the state of
 * `self' has been set to `DRIVER_STATE_FINI_DEPS'. */
PRIVATE NOBLOCK NONNULL((1)) void
NOTHROW(FCALL driver_runstate_fini_deps)(struct driver *__restrict self) {
	size_t i;
	/* Go over all dependencies and clear them. */
	for (i = 0; i < self->d_depcnt; ++i)
		axref_clear(&self->d_depvec[i]);

	/* Next up: finalize driver text. */
	ATOMIC_WRITE(self->d_state, DRIVER_STATE_FINI_TEXT);
	sig_broadcast(&driver_state_changed);
	driver_runstate_fini_text(self);
}


/* Initialize driver dependencies. */
PRIVATE NONNULL((1)) void FCALL
driver_runstate_init_deps(struct driver *__restrict self) {
	ElfW(Dyn) const *elf_dynamic;
	size_t i, elf_dynsize, dst = 0;
	elf_dynamic = self->d_dynhdr;
	elf_dynsize = self->d_dyncnt;
	for (i = 0; i < elf_dynsize; ++i) {
		char const *filename;
		REF struct driver *dependency;
		if (elf_dynamic[i].d_tag != DT_NEEDED)
			continue; /* We only care about dependencies! */
		if unlikely(dst >= self->d_depcnt)
			THROW_FAULTY_ELF_ERROR(E_NOT_EXECUTABLE_FAULTY_REASON_ELF_BAD_NEEDED, dst);
		filename = self->d_dynstr + elf_dynamic[i].d_un.d_ptr;
		if unlikely(filename < self->d_dynstr ||
		            filename >= self->d_dynstr_end)
			THROW_FAULTY_ELF_ERROR(E_NOT_EXECUTABLE_FAULTY_REASON_ELF_BAD_NEEDED, dst);
		/* Check  for  special case:  loading driver  dependencies before
		 * the root filesystem has been mounted (i.e. bootloader drivers) */
		if unlikely(!ATOMIC_READ(vfs_kernel.p_inode)) {
			dependency = driver_fromname(filename);
			if unlikely(!dependency) {
				char const *name;
				/* We  get here if  a driver loaded as  a bootloader module required
				 * another driver as a dependency, with that other driver not having
				 * been provided in the same manner.
				 * In this case, tell the user that they've booted KOS in an impossible
				 * configuration,   also   informing  them   of  the   missing  driver.
				 * Note that when  built with  the builtin debugger  enabled, the  user
				 * will  even  be  prompted  with  a  really  nice  error  message!  ;) */
				name = strchr(filename, '/');
				name = name ? name + 1 : filename;
				dependency = driver_fromname(name);
				if unlikely(!dependency) {
					kernel_panic("Cannot load dependency %q of driver %q\n"
					             "Consider starting KOS with the dependency as a boot-module",
					             name, self->d_name);
				}
			}
		} else {
			/* Recursively load the dependent driver. */
			dependency = driver_insmod(filename);
		}
		/* Write the dependency back into the vector of dependencies. */
		axref_set_inherit(&self->d_depvec[dst], dependency);
		++dst;
	}
	if unlikely(dst < self->d_depcnt)
		THROW_FAULTY_ELF_ERROR(E_NOT_EXECUTABLE_FAULTY_REASON_ELF_BAD_NEEDED, dst);
}


/* Initialize driver relocations, but stop if the driver's state is altered. */
PRIVATE NONNULL((1)) bool FCALL
driver_runstate_init_relo(struct driver *__restrict self) {
	/* TODO */
}


typedef void (*driver_initfini_t)(int argc, char *argv[]);
struct driver_constructor_info {
	driver_initfini_t  dci_init;             /* [0..1] Initializer callback */
	driver_initfini_t *dci_init_array;       /* [1..1][dci_fini_array_sz] Constructors. */
	size_t             dci_init_array_sz;    /* # of constructors. */
	driver_initfini_t *dci_preinit_array;    /* [1..1][dci_fini_array_sz] Pre-constructors. */
	size_t             dci_preinit_array_sz; /* # of pre-constructors. */
};
struct driver_destructor_info {
	driver_initfini_t  ddi_fini;          /* [0..1] Finalizer callback */
	driver_initfini_t *ddi_fini_array;    /* [1..1][ddi_fini_array_sz] Destructors. */
	size_t             ddi_fini_array_sz; /* # of destructors. */
};

struct driver_initfini_savestate {
	pflag_t difss_preemption; /* Saved preemption state. */
	inline driver_initfini_savestate() noexcept
	    : difss_preemption(PREEMPTION_PUSH()) { }
	inline ~driver_initfini_savestate() noexcept {
		PREEMPTION_POP(difss_preemption);
	}
};

PRIVATE NONNULL((1, 3)) void FCALL
run_driver_initfini_cb(struct driver *__restrict self,
                       driver_initfini_t cb, char const *__restrict kind) {
	if unlikely(*(void **)&cb < self->d_module.md_loadmin ||
	            *(void **)&cb > self->d_module.md_loadmax) {
		printk(KERN_ERR "[mod] Bad %s-function in driver %q (%p is out-of-bounds of %p...%p, loadaddr:%p)\n",
		       kind, self->d_name, cb, self->d_module.md_loadmin, self->d_module.md_loadmax, self->d_module.md_loadaddr);
		THROW_FAULTY_ELF_ERROR(E_NOT_EXECUTABLE_FAULTY_REASON_ELF_BAD_INIT_FUNC,
		                       (uintptr_t)(void *)cb - self->d_module.md_loadaddr);
	} else {
		/* Preserve a couple of thread context variables when invoking an init/fini function. */
		struct driver_initfini_savestate savestate;
		/* Invoke the actual function. */
		(*cb)(self->d_argc, self->d_argv);
	}
}

PRIVATE NOBLOCK NONNULL((1, 4)) void
NOTHROW(FCALL log_bad_initfini_vector)(struct driver *__restrict self,
                                       driver_initfini_t *vector,
                                       size_t count, char const *__restrict kind) {
	printk(KERN_ERR "[mod] Bad %s-function-vector in driver %q "
	                "(%p...%p is out-of-bounds of %p...%p, loadaddr:%p)\n",
	       kind, self->d_name, (void *)(vector), (byte_t *)((void *)(vector + count)) - 1,
	       self->d_module.md_loadmin, self->d_module.md_loadmax, self->d_module.md_loadaddr);
}

/* Run driver constructors, but stop if the driver's state is altered. */
PRIVATE NONNULL((1, 4)) bool FCALL
run_driver_init_vector_cb(struct driver *__restrict self,
                          driver_initfini_t *vector,
                          size_t count, char const *__restrict kind) {
	if (!count)
		return; /* Nothing to do here! */
	if unlikely((void *)(vector) < self->d_module.md_loadmin ||
	            (void *)(vector + count) > self->d_module.md_loadmax + 1 ||
	            (void *)(vector + count) < (void *)(vector)) {
		log_bad_initfini_vector(self, vector, count, kind);
		THROW_FAULTY_ELF_ERROR(E_NOT_EXECUTABLE_FAULTY_REASON_ELF_BAD_INIT_FUNC,
		                       (uintptr_t)(void *)(vector),
		                       (uintptr_t)(void *)(vector + count));
	} else {
		size_t i;
		for (i = 0; i < count; ++i) {
			run_driver_initfini_cb(self, vector[i], kind);
			if (ATOMIC_READ(self->d_state) != DRIVER_STATE_INIT_CTRS)
				return false;
		}
	}
	return true;
}

PRIVATE NOBLOCK NONNULL((1, 2)) void
NOTHROW(FCALL dump_driver_initfini_exception)(struct driver *__restrict self,
                                              char const *__restrict kind) {
	error_printf("running %s functions for driver %s", kind, self->d_name);
}

PRIVATE NONNULL((1, 3)) void
NOTHROW(FCALL run_driver_initfini_cb_nx)(struct driver *__restrict self,
                                         driver_initfini_t cb,
                                         char const *__restrict kind) {
	TRY {
		run_driver_initfini_cb(self, cb, kind);
	} EXCEPT {
		dump_driver_initfini_exception(self, kind);
	}
}

PRIVATE NONNULL((1, 4)) void
NOTHROW(FCALL run_driver_initfini_vector_cb_nx)(struct driver *__restrict self,
                                                driver_initfini_t *vector,
                                                size_t count, char const *__restrict kind) {
	if (!count)
		return; /* Nothing to do here! */
	if unlikely((void *)(vector) < self->d_module.md_loadmin ||
	            (void *)(vector + count) > self->d_module.md_loadmax + 1 ||
	            (void *)(vector + count) < (void *)(vector)) {
		log_bad_initfini_vector(self, vector, count, kind);
	} else {
		size_t i;
		for (i = 0; i < count; ++i) {
			TRY {
				run_driver_initfini_cb(self, vector[i], kind);
			} EXCEPT {
				dump_driver_initfini_exception(self, kind);
			}
		}
	}
}


/* Fill in information about constructors of `self' */
PRIVATE NONNULL((1, 2)) void FCALL
driver_getctors(struct driver *__restrict self,
                struct driver_constructor_info *__restrict result) {
	ElfW(Dyn) const *elf_dynamic;
	size_t i, elf_dynsize;
	elf_dynamic = self->d_dynhdr;
	elf_dynsize = self->d_dyncnt;
	memset(result, 0, sizeof(*result));
	for (i = 0; i < elf_dynsize; ++i) {
		switch (elf_dynamic[i].d_tag) {

		case DT_INIT:
			*(void **)&result->dci_init = (void *)(self->d_module.md_loadaddr +
			                                       self->d_dynhdr[i].d_un.d_ptr);
			break;

		case DT_PREINIT_ARRAY:
			result->dci_preinit_array = (driver_initfini_t *)(self->d_module.md_loadaddr +
			                                                  self->d_dynhdr[i].d_un.d_ptr);
			break;

		case DT_PREINIT_ARRAYSZ:
			result->dci_preinit_array_sz = self->d_dynhdr[i].d_un.d_val /
			                               sizeof(driver_initfini_t);
			break;

		case DT_INIT_ARRAY:
			result->dci_init_array = (driver_initfini_t *)(self->d_module.md_loadaddr +
			                                               self->d_dynhdr[i].d_un.d_ptr);
			break;

		case DT_INIT_ARRAYSZ:
			result->dci_init_array_sz = self->d_dynhdr[i].d_un.d_val /
			                            sizeof(driver_initfini_t);
			break;

		default:
			break;
		}
	}
}

/* Fill in information about destructors of `self' */
PRIVATE NONNULL((1, 2)) void FCALL
driver_getdtors(struct driver *__restrict self,
                struct driver_destructor_info *__restrict result) {
	ElfW(Dyn) const *elf_dynamic;
	size_t i, elf_dynsize;
	elf_dynamic = self->d_dynhdr;
	elf_dynsize = self->d_dyncnt;
	memset(result, 0, sizeof(*result));
	for (i = 0; i < elf_dynsize; ++i) {
		switch (elf_dynamic[i].d_tag) {

		case DT_FINI:
			*(void **)&result->ddi_fini = (void *)(self->d_module.md_loadaddr +
			                                       self->d_dynhdr[i].d_un.d_ptr);
			break;

		case DT_FINI_ARRAY:
			result->ddi_fini_array = (driver_initfini_t *)(self->d_module.md_loadaddr +
			                                               self->d_dynhdr[i].d_un.d_ptr);
			break;

		case DT_FINI_ARRAYSZ:
			result->ddi_fini_array_sz = self->d_dynhdr[i].d_un.d_val /
			                            sizeof(driver_initfini_t);
			break;

		default:
			break;
		}
	}
}

PRIVATE NONNULL((1)) void
NOTHROW(FCALL run_driver_fini_vector)(struct driver *__restrict self) {
	struct driver_destructor_info dtrs;
	NESTED_EXCEPTION;
	TRY {
		driver_getdtors(self, &dtrs);
	} EXCEPT {
		dump_driver_initfini_exception(self, "DT_FINI_ARRAY");
		return;
	}
	run_driver_initfini_vector_cb_nx(self,
	                                 dtrs.ddi_fini_array,
	                                 dtrs.ddi_fini_array_sz,
	                                 "DT_FINI_ARRAY");
}

PRIVATE NONNULL((1)) void
NOTHROW(FCALL run_driver_finiall)(struct driver *__restrict self) {
	struct driver_destructor_info dtrs;
	NESTED_EXCEPTION;
	TRY {
		driver_getdtors(self, &dtrs);
	} EXCEPT {
		dump_driver_initfini_exception(self, "DT_FINI[_ARRAY]");
		return;
	}
	if (dtrs.ddi_fini != NULL)
		run_driver_initfini_cb_nx(self, dtrs.ddi_fini, "DT_FINI");
	run_driver_initfini_vector_cb_nx(self,
	                                 dtrs.ddi_fini_array,
	                                 dtrs.ddi_fini_array_sz,
	                                 "DT_FINI_ARRAY");
}



/* Initialize the given driver by transitioning its state  to
 * `DRIVER_STATE_LOADED'. If this fails, or if the driver was
 * already finalized, its state is set to `DRIVER_STATE_KILL'
 * instead. */
PUBLIC NONNULL((1)) void FCALL
driver_initialize(struct driver *__restrict self)
		THROWS(E_WOULDBLOCK, E_FSERROR, E_NOT_EXECUTABLE) {
	uintptr_t state;
again:
	state = ATOMIC_READ(self->d_state);
	switch (state) {

	case DRIVER_STATE_INIT: {
		struct driver_constructor_info ctrs;
		/* Start initialization. */
		if (!ATOMIC_CMPXCH_WEAK(self->d_state,
		                        DRIVER_STATE_INIT,
		                        DRIVER_STATE_INIT_DEPS))
			goto again;
		ATOMIC_WRITE(self->_d_initthread, THIS_TASK);
		sig_broadcast(&driver_state_changed);
		/* At this point, we have exclusive authority to initialize
		 * the given driver however we  want, as well as the  right
		 * to finalize it if something happens that we don't  like! */
		TRY {
			/* Step #1: Like already indicate by `DRIVER_STATE_INIT_DEPS',
			 *          we  have to load+initialize  other drivers that we
			 *          have dependencies upon.
			 *          Note that in the  case of cyclic dependencies,  the
			 *          init-loop is broken by the init_thread == THIS_TASK
			 *          check when  `driver_initialize()'  is  called  with
			 *          `d_state == DRIVER_STATE_INIT_DEPS'! */
			if (self->d_depcnt != 0) {
				driver_runstate_init_deps(self);
			}

			/* Step #2: service relocations. */
			ATOMIC_WRITE(self->d_state, DRIVER_STATE_INIT_RELO);
			sig_broadcast(&driver_state_changed);
			if (!driver_runstate_init_relo(self)) {
				/* This can happen if a IFUNC/IDATA element called `driver_finalize()' */
				assert(self->d_state == DRIVER_STATE_FINI_RDPS);
				goto abort_initialization_fini_deps;
			}

			/* Inform any attached debugger of the new driver.
			 * NOTE: This is done after relocations, but before initializers, so that
			 *       a debugger is able to safely set breakpoints without  overriding
			 *       memory locations possibly affected by relocations. */
#ifdef CONFIG_VBOXGDB
			vboxgdb_trap(VBOXGDB_TRAP_LIBRARY);
#endif /* CONFIG_VBOXGDB */
			if (kernel_debugtrap_enabled()) {
				struct debugtrap_reason r;
				r.dtr_signo  = SIGTRAP;
				r.dtr_reason = DEBUGTRAP_REASON_LIBRARY;
				kernel_debugtrap(&r);
			}

			/* Step #3: Load driver constructors. */
			ATOMIC_WRITE(self->d_state, DRIVER_STATE_INIT_CTRS);
			sig_broadcast(&driver_state_changed);
			driver_getctors(self, &ctrs);
		} EXCEPT {
			ATOMIC_WRITE(self->_d_initthread, NULL);
			ATOMIC_WRITE(self->d_state, DRIVER_STATE_FINI_DEPS);
			sig_broadcast(&driver_state_changed);
			driver_runstate_fini_deps(self);
			RETHROW();
		}
		/* Run driver constructors. */
		TRY {
			if (!run_driver_init_vector_cb(self,
			                               ctrs.dci_preinit_array,
			                               ctrs.dci_preinit_array_sz,
			                               "DT_PREINIT_ARRAY"))
				goto abort_initialization;
			if (!run_driver_init_vector_cb(self,
			                               ctrs.dci_init_array,
			                               ctrs.dci_init_array_sz,
			                               "DT_INIT_ARRAY"))
				goto abort_initialization;
		} EXCEPT {
			ATOMIC_WRITE(self->d_state, DRIVER_STATE_FINI_DTRS);
			sig_broadcast(&driver_state_changed);
			/* Run vector finalizers (but don't run the primary DT_FINI-function). */
			run_driver_fini_vector(self);
			ATOMIC_WRITE(self->_d_initthread, NULL);
			ATOMIC_WRITE(self->d_state, DRIVER_STATE_FINI_DEPS);
			sig_broadcast(&driver_state_changed);
			driver_runstate_fini_deps(self);
			RETHROW();
		}
		/* The primary DT_INIT-callback is handled specifically, because
		 * if  it throws an exception, then we must also invoke DT_INIT,
		 * which we mustn't do if  (e.g.) a DT_INIT_ARRAY callback  does
		 * the same! */
		if (ctrs.dci_init != NULL) {
			TRY {
				run_driver_initfini_cb(self, ctrs.dci_init, "DT_INIT");
			} EXCEPT {
				ATOMIC_WRITE(self->d_state, DRIVER_STATE_FINI_DTRS);
				sig_broadcast(&driver_state_changed);
				/* Run vector finalizers (including the DT_FINI-function). */
				run_driver_finiall(self);
				ATOMIC_WRITE(self->_d_initthread, NULL);
				ATOMIC_WRITE(self->d_state, DRIVER_STATE_FINI_DEPS);
				sig_broadcast(&driver_state_changed);
				driver_runstate_fini_deps(self);
				RETHROW();
			}
			if (ATOMIC_READ(self->d_state) != DRIVER_STATE_INIT_CTRS)
				goto abort_initialization_full;
		}

		/* Step #4: Indicate that the driver has been initialized! */
		ATOMIC_WRITE(self->_d_initthread, NULL);
		ATOMIC_WRITE(self->d_state, DRIVER_STATE_LOADED);
		sig_broadcast(&driver_state_changed);
		break;

		/* These can happen if a driver initializer called `driver_finalize()' */
abort_initialization_full:
		assert(self->d_state == DRIVER_STATE_FINI_DTRS);
		run_driver_finiall(self);
		goto abort_initialization_fini_deps;
abort_initialization:
		assert(self->d_state == DRIVER_STATE_FINI_DTRS);
		run_driver_fini_vector(self);
abort_initialization_fini_deps:
		ATOMIC_WRITE(self->_d_initthread, NULL);
		ATOMIC_WRITE(self->d_state, DRIVER_STATE_FINI_DEPS);
		sig_broadcast(&driver_state_changed);
		driver_runstate_fini_deps(self);
	}	break;

	case DRIVER_STATE_INIT_DEPS:
	case DRIVER_STATE_INIT_RELO:
	case DRIVER_STATE_INIT_CTRS: {
		/* Wait for another thread to finish the initialization.
		 * However, make sure to deal  with the case where  it's
		 * the calling thread that's doing the init! */
		struct task *init_thread;
		init_thread = ATOMIC_READ(self->_d_initthread);
		if unlikely(state != ATOMIC_READ(self->d_state))
			goto again;
		if unlikely(init_thread == THIS_TASK)
			return; /* Don't cause a deadlock by waiting for ourselves to finish init! */
		/* Wait for the driver's state to become `>= DRIVER_STATE_LOADED' */
		task_connect(&driver_state_changed);
		if unlikely(ATOMIC_READ(self->d_state) >= DRIVER_STATE_LOADED) {
			task_disconnectall();
			break;
		}
		task_waitfor();
		goto again;
	}	break;

	default:
		/* All other states indicate already-initialized,  or
		 * already-finalized (with no way of re-initializing)
		 *
		 * iow: We don't have to do anything in these cases! */
		break;
	}
}

/* Finalize the given driver. Note that driver finalizers are
 * allowed to block and  throw exceptions (meaning that  this
 * function  may  also block  and throw).  Additionally, this
 * function  may wait  for a  driver that  is currently being
 * initialized or finalized  by another to  finish doing  so,
 * however if it's the caller thread that does the init/fini,
 * then this function will  return immediately, so-as to  not
 * cause a deadlock! */
PUBLIC NONNULL((1)) void FCALL
driver_finalize(struct driver *__restrict self) {
	uintptr_t state;
again:
	state = ATOMIC_READ(self->d_state);
	switch (state) {

	case DRIVER_STATE_INIT:
		/* Directly switch over to the FINI_TEXT state. */
		if (!ATOMIC_CMPXCH_WEAK(self->d_state,
		                        DRIVER_STATE_INIT,
		                        DRIVER_STATE_FINI_TEXT))
			goto again;
		sig_broadcast(&driver_state_changed);
		driver_runstate_fini_text(self);
		break;

	case DRIVER_STATE_INIT_DEPS:
	case DRIVER_STATE_INIT_RELO:
	case DRIVER_STATE_INIT_CTRS: {
		/* The driver  is being  initialized _right_  _now_
		 * In  this case, we must differentiate between the
		 * case where it's the calling thread that does the
		 * init, and some other thread.
		 *
		 * In the former case, we  must mark the driver  for
		 * finalization, and return to our caller, such that
		 * the call to `driver_initialize()' from further up
		 * the stack can initiate the actual finalization.
		 *
		 * In the later case, we must wait for the other thread
		 * to finish initialization,  so we  can instigate  the
		 * driver's finalization immediately afterwards. */
		struct task *init_thread;
		init_thread = ATOMIC_READ(self->_d_initthread);
		if unlikely(state != ATOMIC_READ(self->d_state))
			goto again;
		if unlikely(init_thread == THIS_TASK) {
			unsigned int new_state;
			new_state = DRIVER_STATE_FINI_RDPS;
			if (state == DRIVER_STATE_INIT_CTRS)
				state = DRIVER_STATE_FINI_DTRS;
			/* Mark the driver for lazy finalization once
			 * we return back up the stack to our caller! */
			if (!ATOMIC_CMPXCH_WEAK(self->d_state, state, new_state))
				goto again;
			sig_broadcast(&driver_state_changed);
			return;
		}
		/* Wait for another thread to finish initialization. */
		task_connect(&driver_state_changed);
		if unlikely(state != ATOMIC_READ(self->d_state)) {
			task_disconnectall();
			goto again;
		}
		task_waitfor();
		goto again;
	}	break;

	case DRIVER_STATE_LOADED: {
		/* The actual heart-piece of `driver_finalize()':
		 * Finalize a driver  that is fully  initialized. */
		if (!ATOMIC_CMPXCH_WEAK(self->d_state,
		                        DRIVER_STATE_LOADED,
		                        DRIVER_STATE_FINI_DTRS))
			goto again;
		ATOMIC_WRITE(self->_d_initthread, THIS_TASK);
		sig_broadcast(&driver_state_changed);

		/* Invoke driver finalizers. */
		run_driver_finiall(self);

		/* Finalizer driver dependencies, and proceed to
		 * finalizing driver text, etc... */
		ATOMIC_WRITE(self->_d_initthread, NULL);
		ATOMIC_WRITE(self->d_state, DRIVER_STATE_FINI_DEPS);
		sig_broadcast(&driver_state_changed);
		driver_runstate_fini_deps(self);
	}	break;

	default:
		/* All other states indicate already-finalized, or currently finalizing.
		 * iow: We don't have to do anything in these cases! */
		break;
	}
}



/************************************************************************/
/* The primary driver operators table.                                  */
/************************************************************************/
PUBLIC NOBLOCK NONNULL((1)) void
NOTHROW(FCALL driver_free_)(struct driver *__restrict self) {
	kfree(self);
}

PRIVATE NOBLOCK NONNULL((1)) void
NOTHROW(FCALL driver_destroy)(struct driver *__restrict self) {
	assert(self->d_module.md_mman == &mman_kernel);

	/* Change the driver's state from KILL to DEAD */
	assert(self->d_state == DRIVER_STATE_KILL);
	ATOMIC_WRITE(self->d_state, DRIVER_STATE_DEAD);
	sig_broadcast(&driver_state_changed);

	/* Clear out members that are no longer valid for a destroyed driver. */
	xdecref(self->d_module.md_file);
	xdecref(self->d_module.md_fspath);
	xdecref(self->d_module.md_fsname);
	kfree(self->d_cmdline);
	kfree(self->d_argv);
	kfree(self->d_depvec);
	if (self->d_eh_frame_cache != NULL)
		dfc_freetree(self->d_eh_frame_cache);
	kfree(self->d_shdr);
	kfree(self->d_shstrtab);

	/* Invalidate fields that may no longer be accessed. */
	DBG_memset(&self->d_module.md_file, 0xcc, sizeof(self->d_module.md_file));
	DBG_memset(&self->d_module.md_fspath, 0xcc, sizeof(self->d_module.md_fspath));
	DBG_memset(&self->d_module.md_fsname, 0xcc, sizeof(self->d_module.md_fsname));
	DBG_memset(&self->d_name, 0xcc, sizeof(self->d_name));
	DBG_memset(&self->d_cmdline, 0xcc, sizeof(self->d_cmdline));
	DBG_memset(&self->d_argc, 0xcc, sizeof(self->d_argc));
	DBG_memset(&self->d_argv, 0xcc, sizeof(self->d_argv));
	DBG_memset(&self->d_depcnt, 0xcc, sizeof(self->d_depcnt));
	DBG_memset(&self->d_depvec, 0xcc, sizeof(self->d_depvec));
	DBG_memset(&self->d_eh_frame_start, 0xcc, sizeof(self->d_eh_frame_start));
	DBG_memset(&self->d_eh_frame_end, 0xcc, sizeof(self->d_eh_frame_end));
	DBG_memset(&self->d_eh_frame_cache, 0xcc, sizeof(self->d_eh_frame_cache));
	DBG_memset(&self->d_eh_frame_cache_lock, 0xcc, sizeof(self->d_eh_frame_cache_lock));
	DBG_memset(&self->d_dyncnt, 0xcc, sizeof(self->d_dyncnt));
	DBG_memset(&self->d_dynhdr, 0xcc, sizeof(self->d_dynhdr));
	DBG_memset(&self->d_dynsym_tab, 0xcc, sizeof(self->d_dynsym_tab));
	DBG_memset(&self->d_dynsym_cnt, 0xcc, sizeof(self->d_dynsym_cnt));
	DBG_memset(&self->d_gnuhashtab, 0xcc, sizeof(self->d_gnuhashtab));
	DBG_memset(&self->d_hashtab, 0xcc, sizeof(self->d_hashtab));
	DBG_memset(&self->d_dynstr, 0xcc, sizeof(self->d_dynstr));
	DBG_memset(&self->d_dynstr_end, 0xcc, sizeof(self->d_dynstr_end));
	DBG_memset(&self->d_shstrndx, 0xcc, sizeof(self->d_shstrndx));
	DBG_memset(&self->d_shdr, 0xcc, sizeof(self->d_shdr));
	DBG_memset(&self->d_shstrtab, 0xcc, sizeof(self->d_shstrtab));
	DBG_memset(&self->d_shstrsiz, 0xcc, sizeof(self->d_shstrsiz));
	DBG_memset(&self->_d_mm_lop, 0xcc, sizeof(self->_d_mm_lop));
	DBG_memset(&self->_d_initthread, 0xcc, sizeof(self->_d_initthread));
	DBG_memset(&self->d_phdr, 0xcc, self->d_phnum * sizeof(ElfW(Phdr)));
	DBG_memset(&self->d_phnum, 0xcc, sizeof(self->d_phnum));

	/* TODO: Try to remove `self' from the current `drivers'. And if
	 *       that's not possible, set `loaded_drivers_contains_dead' */

	/* TODO: Using lock operations, remove all still-loaded
	 *       driver sections from the module section cache.
	 * NOTE: */
	//TODO: kfree(self->d_sections);
	//TODO: DBG_memset(&self->d_shnum, 0xcc, sizeof(self->d_shnum));
	//TODO: DBG_memset(&self->d_sections, 0xcc, sizeof(self->d_sections));

	/* TODO: Using  lock operations, remove all of the mnode
	 *       self-pointers back towards this driver from the
	 *       kernel mman. */

	weakdecref(self);
}

PRIVATE NOBLOCK NONNULL((1)) void
NOTHROW(FCALL driver_nonodes)(struct driver *__restrict self) {
	weakdecref(self);
}

PRIVATE WUNUSED NONNULL((1)) REF struct module_section *FCALL
driver_locksection_index(struct driver *__restrict self,
                         unsigned int section_index) {
	/* TODO */
	return NULL;
}

PRIVATE WUNUSED NONNULL((1)) REF struct module_section *FCALL
driver_locksection(struct driver *__restrict self,
                   USER CHECKED char const *section_name) {
	/* TODO */
	return NULL;
}


PRIVATE WUNUSED NONNULL((1, 3)) bool FCALL
driver_sectinfo(struct driver *__restrict self,
                uintptr_t module_relative_addr,
                struct module_sectinfo *__restrict info) {
	unsigned int i;
	bool allow_section_end_pointers;
	allow_section_end_pointers = false;
again:
	for (i = 0; i < self->d_shnum; ++i) {
		if (module_relative_addr < self->d_shdr[i].sh_addr)
			continue;
		if (module_relative_addr >= self->d_shdr[i].sh_addr + self->d_shdr[i].sh_size) {
			if (!allow_section_end_pointers)
				continue;
			if (module_relative_addr > self->d_shdr[i].sh_addr + self->d_shdr[i].sh_size)
				continue;
		}
		/* Found it! (fill in section information for our caller) */
		info->msi_name    = (char *)&kernel_shstrtab_data + self->d_shdr[i].sh_name;
		info->msi_addr    = self->d_shdr[i].sh_addr;
		info->msi_size    = self->d_shdr[i].sh_size;
		info->msi_entsize = self->d_shdr[i].sh_entsize;
		info->msi_type    = self->d_shdr[i].sh_type;
		info->msi_flags   = self->d_shdr[i].sh_flags;
		info->msi_link    = self->d_shdr[i].sh_link;
		info->msi_info    = self->d_shdr[i].sh_info;
		info->msi_index   = i;
		return true;
	}
	if (!allow_section_end_pointers) {
		allow_section_end_pointers = true;
		goto again;
	}
	return false;
}


PRIVATE struct module_ops const driver_module_ops = {
	.mo_free              = (void(FCALL *)(struct module *__restrict))&driver_free_,
	.mo_destroy           = (void(FCALL *)(struct module *__restrict))&driver_destroy,
	.mo_nonodes           = (void(FCALL *)(struct module *__restrict))&driver_nonodes,
	.mo_locksection       = (REF struct module_section *(FCALL *)(struct module *__restrict, USER CHECKED char const *))&driver_locksection,
	.mo_locksection_index = (REF struct module_section *(FCALL *)(struct module *__restrict, unsigned int))&driver_locksection_index,
	.mo_sectinfo          = (bool (FCALL *)(struct module *__restrict, uintptr_t, struct module_sectinfo *__restrict))&driver_sectinfo,
};



PRIVATE NONNULL((1)) void KCALL
driver_verify_ElfEhdr(ElfW(Ehdr) const *__restrict ehdr)
		THROWS(E_NOT_EXECUTABLE) {
	unsigned int reason;
	if unlikely(ehdr->e_ident[EI_MAG0] != ELFMAG0 ||
	            ehdr->e_ident[EI_MAG1] != ELFMAG1 ||
	            ehdr->e_ident[EI_MAG2] != ELFMAG2 ||
	            ehdr->e_ident[EI_MAG3] != ELFMAG3)
		THROW(E_NOT_EXECUTABLE_NOT_A_BINARY);
	reason = E_NOT_EXECUTABLE_FAULTY_REASON_ELF_BADCLASS;
	if unlikely(ehdr->e_ident[EI_CLASS] != KERNEL_DRIVER_REQUIRED_CLASS)
		goto err_elf_reason;
	reason = E_NOT_EXECUTABLE_FAULTY_REASON_ELF_BADORDER;
	if unlikely(ehdr->e_ident[EI_DATA] != KERNEL_DRIVER_REQUIRED_DATA)
		goto err_elf_reason;
	reason = E_NOT_EXECUTABLE_FAULTY_REASON_ELF_BADVERSION;
	if unlikely(ehdr->e_ident[EI_VERSION] != EV_CURRENT)
		goto err_elf_reason;
	reason = E_NOT_EXECUTABLE_FAULTY_REASON_ELF_BADVERSION2;
	if unlikely(ehdr->e_version != EV_CURRENT)
		goto err_elf_reason;
	reason = E_NOT_EXECUTABLE_FAULTY_REASON_ELF_BADTYPE;
	if unlikely(ehdr->e_type != ET_DYN)
		goto err_elf_reason;
	reason = E_NOT_EXECUTABLE_FAULTY_REASON_ELF_BADMACH;
	if unlikely(ehdr->e_machine != KERNEL_DRIVER_REQUIRED_MACHINE)
		goto err_elf_reason;
	reason = E_NOT_EXECUTABLE_FAULTY_REASON_ELF_BADHEADER;
	if unlikely(ehdr->e_ehsize < offsetafter(ElfW(Ehdr), e_phnum))
		goto err_elf_reason;
	reason = E_NOT_EXECUTABLE_FAULTY_REASON_ELF_BADSEGMENTS;
	if unlikely(ehdr->e_phentsize != sizeof(ElfW(Phdr)))
		goto err_elf_reason;
	return;
err_elf_reason:
	THROW(E_NOT_EXECUTABLE_FAULTY,
	      E_NOT_EXECUTABLE_FAULTY_FORMAT_ELF,
	      reason);
}


PRIVATE NONNULL((1)) void KCALL
driver_init_cmdline(struct driver *__restrict self,
                    USER CHECKED char const *cmdline)
		THROWS(E_BADALLOC, E_SEGFAULT, E_WOULDBLOCK) {
	size_t cmdline_size;
	char *cmdline_copy;
	cmdline_size = strlen(cmdline);
	cmdline_copy = (char *)kmalloc((cmdline_size + 2) * sizeof(char),
	                               GFP_LOCKED | GFP_PREFLT);
	TRY {
		memcpy(cmdline_copy, cmdline, cmdline_size, sizeof(char));
		/* Add a double-NUL terminator to allow the commandline
		 * to  always be interpreted into a NUL-NUL string-list */
		cmdline_copy[cmdline_size + 0] = '\0';
		cmdline_copy[cmdline_size + 1] = '\0';
		/* Split the commandline through use of libcmdline */
		self->d_argv = cmdline_decode_argv(cmdline_copy, &self->d_argc,
		                                   GFP_LOCKED | GFP_PREFLT);
	} EXCEPT {
		kfree(cmdline_copy);
		RETHROW();
	}
	self->d_cmdline = cmdline_copy;
}


/* Destroy a partially initialized driver `self' */
PRIVATE NOBLOCK NONNULL((1)) void
NOTHROW(FCALL destroy_partially_initialized_driver)(struct driver *__restrict self) {
	kfree(self->d_depvec);
	kfree(self->d_sections);
	kfree(self->d_shstrtab);
	kfree(self->d_shdr);
	kfree(self->d_argv);
	kfree(self->d_cmdline);
	kfree(self);
}

PRIVATE NOBLOCK NONNULL((1, 2)) bool
NOTHROW(FCALL image_validate)(struct driver *__restrict self,
                              void const *src, size_t num_bytes) {
	return (byte_t const *)src >= self->d_module.md_loadmin &&
	       (byte_t const *)src + num_bytes <= (self->d_module.md_loadmax + 1);
}

PRIVATE NOBLOCK NONNULL((1, 2)) bool
NOTHROW(FCALL image_read_nopf)(struct driver *__restrict self,
                               void *dst, void const *src,
                               size_t num_bytes) {
	if unlikely(!image_validate(self, src, num_bytes))
		return false;
	return memcpy_nopf(dst, src, num_bytes) == 0;
}

/* Using the driver's currently loaded image,
 * initialize the following fields of `self':
 *
 * >> char const               *d_name;
 * >> byte_t const             *d_eh_frame_start;
 * >> byte_t const             *d_eh_frame_end;
 * >> size_t                    d_dyncnt;
 * >> ElfW(Dyn) const          *d_dynhdr;
 * >> ElfW(Sym) const          *d_dynsym_tab;
 * >> size_t                    d_dynsym_cnt;
 * >> ElfW(GnuHashTable) const *d_gnuhashtab;
 * >> ElfW(HashTable) const    *d_hashtab;
 * >> char const               *d_dynstr;
 * >> char const               *d_dynstr_end;
 *
 * @return: 0 : Success
 * @return: * : Corrupted image (`return' is one of `E_NOT_EXECUTABLE_FAULTY_REASON_ELF_*') */
PRIVATE NOBLOCK NONNULL((1)) unsigned int
NOTHROW(FCALL driver_load_image_pointers)(struct driver *__restrict self,
                                          uintptr_t reasons[2]) {
	char lastch;
	size_t i;
	ElfW(Half) pt_dynamic_idx;
	uintptr_t soname_offset;
	size_t dynstr_size;

	/* Locate the `.dynamic' section */
	for (pt_dynamic_idx = 0;; ++pt_dynamic_idx) {
		if (pt_dynamic_idx >= self->d_phnum)
			return E_NOT_EXECUTABLE_FAULTY_REASON_ELF_NO_DYNAMIC;
		if (ATOMIC_READ(self->d_phdr[pt_dynamic_idx].p_type) == PT_DYNAMIC)
			break;
	}

	/* Write-back information about the `.dynamic' section. */
	self->d_dynhdr = (ElfW(Dyn) *)(self->d_module.md_loadaddr + self->d_phdr[pt_dynamic_idx].p_vaddr);
	self->d_dyncnt = self->d_phdr[pt_dynamic_idx].p_memsz / sizeof(ElfW(Dyn));
	if (!image_validate(self, self->d_dynhdr, self->d_dyncnt * sizeof(ElfW(Dyn))))
		return E_NOT_EXECUTABLE_FAULTY_REASON_ELF_BAD_DYNAMIC;
	self->d_dynsym_tab = NULL;
	self->d_gnuhashtab = NULL;
	self->d_hashtab    = NULL;
	self->d_dynstr     = NULL;
	soname_offset      = (uintptr_t)-1;
	dynstr_size        = (size_t)-1;
	for (i = 0; i < self->d_dyncnt; ++i) {
		ElfW(Dyn) tag;
		if (memcpy_nopf(&tag, &self->d_dynhdr[i], sizeof(tag)) != 0)
			return E_NOT_EXECUTABLE_FAULTY_REASON_ELF_BAD_DYNAMIC;
		switch (tag.d_tag) {

		case DT_NULL:
			self->d_dyncnt = i;
			goto done_dynhdr;

		case DT_SONAME:
			soname_offset = tag.d_un.d_ptr;
			break;

		case DT_STRTAB:
			self->d_dynstr = (char *)(self->d_module.md_loadaddr + tag.d_un.d_ptr);
			break;

		case DT_STRSZ:
			dynstr_size = tag.d_un.d_val;
			break;

		case DT_HASH:
			self->d_hashtab = (ElfW(HashTable) *)(self->d_module.md_loadaddr + tag.d_un.d_ptr);
			break;

		case DT_GNU_HASH:
			self->d_gnuhashtab = (ElfW(GnuHashTable) const *)(self->d_module.md_loadaddr + tag.d_un.d_ptr);
			break;

		case DT_SYMTAB:
			self->d_dynsym_tab = (ElfW(Sym) *)(self->d_module.md_loadaddr + tag.d_un.d_ptr);
			break;

		case DT_SYMENT:
			if unlikely(tag.d_un.d_val != sizeof(ElfW(Sym)))
				THROW_FAULTY_ELF_ERROR(E_NOT_EXECUTABLE_FAULTY_REASON_ELF_BAD_SYMENT, tag.d_un.d_ptr);
			break;

		case DT_RELAENT:
			if unlikely(tag.d_un.d_val != sizeof(ElfW(Rela)))
				THROW_FAULTY_ELF_ERROR(E_NOT_EXECUTABLE_FAULTY_REASON_ELF_BAD_RELAENT, tag.d_un.d_ptr);
			break;

		case DT_RELENT:
			if unlikely(tag.d_un.d_val != sizeof(ElfW(Rel)))
				THROW_FAULTY_ELF_ERROR(E_NOT_EXECUTABLE_FAULTY_REASON_ELF_BAD_RELENT, tag.d_un.d_ptr);
			break;

		default:
			break;
		}
	}
done_dynhdr:
	if unlikely(!self->d_dynstr)
		return E_NOT_EXECUTABLE_FAULTY_REASON_ELF_NO_SONAME;
	if (dynstr_size == (size_t)-1) {
		/* Determine the .dynstr segment size by searching for the associated section header. */
		for (i = 0; i < self->d_shnum; ++i) {
			byte_t const *sect_start, *sect_end;
			if (!(self->d_shdr[i].sh_flags & SHF_ALLOC))
				continue; /* Unallocated section... */
			sect_start = (byte_t const *)(self->d_module.md_loadaddr +
			                              self->d_shdr[i].sh_addr);
			if unlikely(!image_validate(self, sect_start, self->d_shdr[i].sh_size))
				continue;
			sect_end = sect_start + self->d_shdr[i].sh_size;
			if ((byte_t const *)self->d_dynstr >= (byte_t const *)sect_start &&
			    (byte_t const *)self->d_dynstr < (byte_t const *)sect_end) {
				/* Found the section! */
				dynstr_size = (size_t)(sect_end - (byte_t const *)self->d_dynstr);
				goto got_dynstr_size;
			}
		}
		/* Check which program header contains the .dynstr segment. */
		for (i = 0; i < self->d_phnum; ++i) {
			byte_t const *phdr_start, *phdr_end;
			if (self->d_phdr[i].p_type != PT_LOAD)
				continue;
			phdr_start = (byte_t const *)(self->d_module.md_loadaddr +
			                              self->d_phdr[i].p_vaddr);
			phdr_end = phdr_start + self->d_phdr[i].p_memsz;
			if ((byte_t const *)self->d_dynstr >= (byte_t const *)phdr_start &&
			    (byte_t const *)self->d_dynstr < (byte_t const *)phdr_end) {
				/* Found the program header! */
				dynstr_size = (size_t)(phdr_end - (byte_t const *)self->d_dynstr);
				goto got_dynstr_size;
			}
		}
		/* Error: Nothing appears to be mapping the .dynstr segment... */
		reasons[0] = (uintptr_t)self->d_dynstr - self->d_module.md_loadaddr;
		reasons[1] = 1;
		return E_NOT_EXECUTABLE_FAULTY_REASON_ELF_BAD_DYNSTR;
	}
got_dynstr_size:
	if unlikely(!image_validate(self, self->d_dynstr, dynstr_size)) {
		reasons[0] = (uintptr_t)self->d_dynstr - self->d_module.md_loadaddr;
		reasons[1] = dynstr_size;
		return E_NOT_EXECUTABLE_FAULTY_REASON_ELF_BAD_DYNSTR;
	}
	if unlikely(soname_offset >= dynstr_size)
		return E_NOT_EXECUTABLE_FAULTY_REASON_ELF_BAD_SONAME;
	/* Try to include as many trailing NUL characters with .dynstr as possible. */
	while ((byte_t const *)(self->d_dynstr + dynstr_size) <= self->d_module.md_loadmax &&
	       memcpy_nopf(&lastch, &self->d_dynstr[dynstr_size], sizeof(lastch)) == 0 &&
	       lastch == '\0')
		++dynstr_size;
	self->d_dynstr_end = self->d_dynstr + dynstr_size;
	self->d_name       = self->d_dynstr + soname_offset;

	/* Make sure that `d_name' isn't an empty string! */
	if (memcpy_nopf(&lastch, self->d_name, sizeof(lastch)) == 0 || lastch == '\0')
		return E_NOT_EXECUTABLE_FAULTY_REASON_ELF_BAD_SONAME;

	/* Make sure that the DT_SONAME string is NUL-terminated _before_ the end of the .dynstr table. */
	{
		size_t soname_len = 1;
		for (;;) {
			if (memcpy_nopf(&lastch, &self->d_name[soname_len], sizeof(lastch)) != 0)
				return E_NOT_EXECUTABLE_FAULTY_REASON_ELF_BAD_SONAME;
			if (lastch == '\0')
				break;
			++soname_len;
		}
	}

	/* Verify .dynsym & friends. */
	if (self->d_dynsym_tab != NULL) {
		self->d_dynsym_cnt = 0;
		if (self->d_hashtab) {
			struct {
				ElfW(Word) ht_nbuckts; /* # of buckets. */
				ElfW(Word) ht_nchains; /* # of symbols. */
			} hashtab_hdr;
			size_t hashsize, temp;
			if (!image_read_nopf(self, &hashtab_hdr, self->d_hashtab, sizeof(hashtab_hdr))) {
bad_elf_symhash:
				reasons[0] = (uintptr_t)self->d_hashtab - self->d_module.md_loadaddr;
				return E_NOT_EXECUTABLE_FAULTY_REASON_ELF_BAD_SYMHASH;
			}
			/* Verify that the ELF `.hash' section is in-bounds of the driver image. */
			if (OVERFLOW_UMUL(hashtab_hdr.ht_nbuckts, sizeof(ElfW(Word)), &temp))
				goto bad_elf_symhash;
			if (OVERFLOW_UADD(offsetof(ElfW(HashTable), ht_table), temp, &hashsize))
				goto bad_elf_symhash;
			if (OVERFLOW_UMUL(hashtab_hdr.ht_nbuckts, sizeof(ElfW(Word)), &temp))
				goto bad_elf_symhash;
			if (OVERFLOW_UADD(hashsize, temp, &hashsize))
				goto bad_elf_symhash;
			if (!image_validate(self, self->d_hashtab, hashsize))
				goto bad_elf_symhash;
			if (self->d_dynsym_cnt == 0)
				self->d_dynsym_cnt = (size_t)hashtab_hdr.ht_nchains;
		}
		if (self->d_gnuhashtab) {
			size_t count;
			struct {
				ElfW(Word) gh_nbuckets;      /* # of buckets. */
				ElfW(Word) gh_symoffset;     /* Offset of symbol indices. */
				ElfW(Word) gh_bloom_size;    /* # of elements in the bloom table. */
				ElfW(Word) gh_bloom_shift;   /* Shift for bloom masks */
			} hashtab_hdr;
			ElfW(Word) const *buckets, *chains;
			if (!image_read_nopf(self, &hashtab_hdr, self->d_gnuhashtab, sizeof(hashtab_hdr))) {
bad_gnu_symhash:
				reasons[0] = (uintptr_t)self->d_hashtab - self->d_module.md_loadaddr;
				return E_NOT_EXECUTABLE_FAULTY_REASON_ELF_BAD_GNUSYMHASH;
			}
			/* GNU hash tables are a bit more complicated, since we need  to
			 * find the symbol with the greatest index, then add +1 to that. */
			buckets = (ElfW(Word) const *)(self->d_gnuhashtab->gh_bloom + hashtab_hdr.gh_bloom_size);
			chains  = (ElfW(Word) const *)(buckets + hashtab_hdr.gh_nbuckets);
			if unlikely(!image_validate(self, self->d_gnuhashtab,
			                            (size_t)((byte_t const *)chains - (byte_t const *)self->d_gnuhashtab)))
				goto bad_gnu_symhash;
			if (memcpy_nopf(&count, &chains[-1], sizeof(count)) != 0)
				goto bad_gnu_symhash;
			for (;;) {
				ElfW(Word) word;
				if (!image_read_nopf(self, &word, &chains[count - hashtab_hdr.gh_symoffset], sizeof(word)))
					goto bad_gnu_symhash;
				if ((word & 1) != 0)
					break;
				++count;
			}
			if (self->d_dynsym_cnt == 0)
				self->d_dynsym_cnt = count;
		}
		if (self->d_dynsym_cnt == 0) {
			/* Calculate # of symbols based on section info. */
			for (i = 0; i < self->d_shnum; ++i) {
				byte_t const *sect_start, *sect_end;
				if (!(self->d_shdr[i].sh_flags & SHF_ALLOC))
					continue; /* Unallocated section... */
				sect_start = (byte_t const *)(self->d_module.md_loadaddr +
				                              self->d_shdr[i].sh_addr);
				if unlikely(!image_validate(self, sect_start, self->d_shdr[i].sh_size))
					continue;
				sect_end = sect_start + self->d_shdr[i].sh_size;
				if ((byte_t const *)self->d_dynsym_tab >= (byte_t const *)sect_start &&
				    (byte_t const *)self->d_dynsym_tab < (byte_t const *)sect_end) {
					/* Found the section! */
					self->d_dynsym_cnt = (size_t)((ElfW(Sym) const *)sect_end - self->d_dynsym_tab);
					goto got_dynsym_size;
				}
			}
			/* Calculate # of symbols based on program headers. */
			for (i = 0; i < self->d_phnum; ++i) {
				byte_t const *phdr_start, *phdr_end;
				if (self->d_phdr[i].p_type != PT_LOAD)
					continue;
				phdr_start = (byte_t const *)(self->d_module.md_loadaddr +
				                              self->d_phdr[i].p_vaddr);
				phdr_end = phdr_start + self->d_phdr[i].p_memsz;
				if ((byte_t const *)self->d_dynstr >= (byte_t const *)phdr_start &&
				    (byte_t const *)self->d_dynstr < (byte_t const *)phdr_end) {
					/* Found the program header! */
					self->d_dynsym_cnt = (size_t)((ElfW(Sym) const *)phdr_end - self->d_dynsym_tab);
					goto got_dynsym_size;
				}
			}
err_bad_dynsym:
			reasons[0] = (uintptr_t)self->d_dynsym_tab - self->d_module.md_loadaddr;
			return E_NOT_EXECUTABLE_FAULTY_REASON_ELF_BAD_DYNSYM;
		}
		/* Verify that the .dynsym section is in-bounds. */
		{
			size_t dynsym_size;
			if unlikely(OVERFLOW_UMUL(self->d_dynsym_cnt, sizeof(ElfW(Sym)), &dynsym_size))
				goto err_bad_dynsym;
			if unlikely(!image_validate(self, self->d_dynsym_tab, dynsym_size))
				goto err_bad_dynsym;
		}

	} else {
		/* Without a symbol table, there can be no hash table(s) */
		self->d_dynsym_cnt = 0;
		self->d_hashtab    = NULL;
		self->d_gnuhashtab = NULL;
	}
got_dynsym_size:

	/* Locate this driver's .eh_frame section. */
	self->d_eh_frame_start = NULL;
	self->d_eh_frame_end   = NULL;
	for (i = 0; i < self->d_shnum; ++i) {
		char namebuf[sizeof(".eh_frame")];
		if unlikely(self->d_shdr[i].sh_name >= self->d_shstrsiz)
			continue;
		if (memcpy_nopf(namebuf, self->d_shstrtab + self->d_shdr[i].sh_name, sizeof(namebuf)) != 0)
			continue;
		if (memcmp(namebuf, ".eh_frame", sizeof(".eh_frame")) != 0)
			continue;
		/* Found the `.eh_frame' section! */

		/* Make sure that `.eh_frame' has the SHF_ALLOC flag set! */
		if unlikely(!(self->d_shdr[i].sh_flags & SHF_ALLOC)) {
			reasons[0] = i;
			return E_NOT_EXECUTABLE_FAULTY_REASON_ELF_NALLOC_EH_FRAME;
		}

		/* Set `.eh_frame' section pointers. */
		self->d_eh_frame_start = (byte_t const *)(self->d_module.md_loadaddr + self->d_shdr[i].sh_addr);
		self->d_eh_frame_end   = self->d_eh_frame_start + self->d_shdr[i].sh_size;
		if unlikely(self->d_eh_frame_start > self->d_eh_frame_end ||
		            self->d_eh_frame_start < self->d_module.md_loadmin ||
		            self->d_eh_frame_end > (self->d_module.md_loadmax + 1)) {
			reasons[0] = self->d_shdr[i].sh_addr;
			reasons[1] = self->d_shdr[i].sh_size;
			return E_NOT_EXECUTABLE_FAULTY_REASON_ELF_BAD_EH_FRAME;
		}
		break;
	}
	return 0;
}

PRIVATE WUNUSED NONNULL((1)) struct mnode *FCALL
create_mnode_for_phdr(ElfW(Phdr) const *__restrict phdr,
                      USER CHECKED byte_t const *base, size_t num_bytes,
                      struct path *drv_fspath, struct directory_entry *drv_fsname) {
	/* TODO */
}


/* Create  a driver from the given parameters (or return an already-
 * loaded driver that matches the `DT_SONAME' attribute found within
 * the given data-blob) */
PRIVATE ATTR_RETNONNULL WUNUSED NONNULL((1)) REF struct driver *KCALL
driver_create(USER CHECKED byte_t const *base, size_t num_bytes,
              struct mfile *drv_file, struct path *drv_fspath,
              struct directory_entry *drv_fsname,
              USER CHECKED char const *drv_cmdline,
              bool *pnew_driver_loaded)
		THROWS(E_SEGFAULT, E_NOT_EXECUTABLE, E_BADALLOC, E_IOERROR) {
	REF struct driver *result;
	ElfW(Dyn) const *pt_dynamic, *pt_dynamic_end;
	ElfW(Ehdr) const *ehdr;
	ElfW(Phdr) const *phdrv;
	ElfW(Half) phidx, phnum;
	size_t dt_needed_count;
	if unlikely(num_bytes < sizeof(ElfW(Ehdr)))
		THROW(E_NOT_EXECUTABLE_TOOSMALL);

	/* First of: Verify that we're actually dealing with an ELF File. */
	ehdr = (ElfW(Ehdr) const *)base;
	driver_verify_ElfEhdr(ehdr);

	/* Validate the program header pointers. */
	phdrv = (ElfW(Phdr) const *)(base + ATOMIC_READ(ehdr->e_phoff));
	phnum = ATOMIC_READ(ehdr->e_phnum);
	if unlikely(phnum > KERNEL_DRIVER_MAXPROGRAMHEADERCOUNT)
		THROW_FAULTY_ELF_ERROR(E_NOT_EXECUTABLE_FAULTY_REASON_ELF_TOOMANYSEGMENTS);
	if unlikely(!phnum)
		THROW_FAULTY_ELF_ERROR(E_NOT_EXECUTABLE_FAULTY_REASON_ELF_NOSEGMENTS);
	if unlikely((byte_t const *)(phdrv) < base ||
	            (byte_t const *)(phdrv + phnum) > base + num_bytes)
		THROW_FAULTY_ELF_ERROR(E_NOT_EXECUTABLE_FAULTY_REASON_ELF_BAD_HEADERS);

	/* Search for the .dynamic section.
	 * We  need find this section first, because it contains the DT_SONAME
	 * tag which we need to determine the driver's name, which is used for
	 * matching the driver against other, already-loaded drivers in  order
	 * to prevent  unnecessary work  when it  comes to  ensuring that  any
	 * driver is only loaded once! */
	for (phidx = 0;; ++phidx) {
		if (phidx >= phnum)
			THROW_FAULTY_ELF_ERROR(E_NOT_EXECUTABLE_FAULTY_REASON_ELF_NO_DYNAMIC);
		if (ATOMIC_READ(phdrv[phidx].p_type) == PT_DYNAMIC)
			break;
	}

	/* Load the in-file range of the .dynamic program header */
	pt_dynamic     = (ElfW(Dyn) const *)(base + ATOMIC_READ(phdrv[phidx].p_offset));
	pt_dynamic_end = (ElfW(Dyn) const *)((byte_t const *)pt_dynamic + phdrv[phidx].p_filesz);

	/* Ensure that the .dynamic program header bounds are valid. */
	if unlikely((byte_t const *)pt_dynamic > (byte_t const *)pt_dynamic_end ||
	            (byte_t const *)pt_dynamic < base ||
	            (byte_t const *)pt_dynamic_end > base + num_bytes)
		THROW_FAULTY_ELF_ERROR(E_NOT_EXECUTABLE_FAULTY_REASON_ELF_BAD_DYNAMIC);

	/* Search for an existing driver, based on this one's DT_SONAME */
	{
		uintptr_t dynstr_vla    = (uintptr_t)-1;
		uintptr_t soname_offset = (uintptr_t)-1;
		ElfW(Dyn) const *iter;
		dt_needed_count = 0;
		for (iter = pt_dynamic;
		     iter < pt_dynamic_end; ++iter) {
			switch (ATOMIC_READ(iter->d_tag)) {

			case DT_NULL:
				pt_dynamic_end = iter;
				goto done_dynhdr_for_soname;

			case DT_NEEDED:
				++dt_needed_count;
				break;

			case DT_SONAME:
				soname_offset = ATOMIC_READ(iter->d_un.d_ptr);
				break;

			case DT_STRTAB:
				dynstr_vla = ATOMIC_READ(iter->d_un.d_ptr);
				break;

			default:
				break;
			}
		}
done_dynhdr_for_soname:
		/* Make sure that we found both a .dynstr section, as well as a DT_SONAME tag. */
		if unlikely(dynstr_vla == (uintptr_t)-1 || soname_offset == (uintptr_t)-1)
			THROW_FAULTY_ELF_ERROR(E_NOT_EXECUTABLE_FAULTY_REASON_ELF_NO_SONAME);

		/* Figure out which PT_LOAD-phdr contains `dynstr_vla' */
		for (phidx = 0;; ++phidx) {
			char *dynstr_base;
			size_t dynstr_offset;
			size_t dynstr_size;
			char *so_name_start;
			char *so_name_end;
			ElfW(Addr) p_vaddr;
			size_t p_filesz;
			if (phidx >= phnum)
				THROW_FAULTY_ELF_ERROR(E_NOT_EXECUTABLE_FAULTY_REASON_ELF_BAD_SONAME);
			if (ATOMIC_READ(phdrv[phidx].p_type) != PT_LOAD)
				continue;
			p_vaddr = ATOMIC_READ(phdrv[phidx].p_vaddr);
			if (p_vaddr > dynstr_vla)
				continue;
			p_filesz = ATOMIC_READ(phdrv[phidx].p_filesz);
			/* Technically p_memsz, but any pointer above would
			 * always  be an empty string, which isn't allowed! */
			if (p_vaddr + p_filesz <= dynstr_vla)
				continue;
			/* Found the segment. */
			dynstr_offset = dynstr_vla - p_vaddr; /* Offset of `.dynstr' in segment */
			dynstr_base   = (char *)(base + ATOMIC_READ(phdrv[phidx].p_offset) + dynstr_offset);
			dynstr_size   = p_filesz - dynstr_offset;
			/* Make sure that .dynstr is in-bounds of the driver image. */
			if unlikely((byte_t const *)dynstr_base < base || soname_offset > dynstr_size ||
			            (byte_t const *)(dynstr_base + dynstr_size) > base + num_bytes)
				THROW_FAULTY_ELF_ERROR(E_NOT_EXECUTABLE_FAULTY_REASON_ELF_BAD_SONAME);
			so_name_start = dynstr_base + soname_offset;
			so_name_end   = (char *)memend(so_name_start, '\0', dynstr_size - soname_offset);
			/* All right! we've got the DT_SONAME string!
			 * -> Search for an existing driver with this name. */
			result = driver_fromname_with_len(so_name_start, (size_t)(so_name_end - so_name_start));
			if (result) {
				if (pnew_driver_loaded)
					*pnew_driver_loaded = false;
				return result; /* This driver has already been loaded. */
			}
		}
	}

	/* Load + initialize a new driver. */
	result = (REF struct driver *)kmalloc(offsetof(struct driver, d_phdr) +
	                                      phnum * sizeof(ElfW(Phdr)),
	                                      GFP_LOCKED | GFP_CALLOC | GFP_PREFLT);
	TRY {
		uintptr_t shoff;
		struct mnode_slist nodes;

		/* Copy program headers. */
		memcpy(result->d_phdr, phdrv, phnum, sizeof(ElfW(Phdr)));
		/* Initialize the driver commandline. */
		if (drv_cmdline) {
			driver_init_cmdline(result, drv_cmdline);
		} else {
			/* Always allocate an argument vector, even if that vector is empty. */
			result->d_argv = (char **)kmalloc(1 * sizeof(char *),
			                                  GFP_CALLOC | GFP_LOCKED | GFP_PREFLT);
		}
		shoff              = (uintptr_t)ATOMIC_READ(ehdr->e_shoff);
		result->d_shstrndx = ATOMIC_READ(ehdr->e_shstrndx);
		result->d_shnum    = ATOMIC_READ(ehdr->e_shnum);
		COMPILER_READ_BARRIER();
		if unlikely(ATOMIC_READ(ehdr->e_shentsize) != sizeof(ElfW(Shdr)))
			THROW_FAULTY_ELF_ERROR(E_NOT_EXECUTABLE_FAULTY_REASON_ELF_BAD_SHENT, ehdr->e_shentsize);
		if unlikely(result->d_shnum > ELF_ARCH_MAXSHCOUNT)
			THROW_FAULTY_ELF_ERROR(E_NOT_EXECUTABLE_FAULTY_REASON_ELF_TOOMANYSECTIONS, result->d_shnum);
		/* Validate offsets for section headers */
		if unlikely(result->d_shstrndx >= result->d_shnum) {
			THROW_FAULTY_ELF_ERROR(E_NOT_EXECUTABLE_FAULTY_REASON_ELF_BAD_SHSTRNDX,
			                       result->d_shstrndx, result->d_shnum);
		}
		if unlikely(shoff >= num_bytes || shoff + (result->d_shnum * sizeof(ElfW(Shdr))) > num_bytes) {
			THROW_FAULTY_ELF_ERROR(E_NOT_EXECUTABLE_FAULTY_REASON_ELF_BAD_SHOFF,
			                       shoff, result->d_shnum, num_bytes);
		}

		/* Load the sections header into memory. */
		result->d_shdr = (ElfW(Shdr) *)kmalloc(result->d_shnum * sizeof(ElfW(Shdr)), GFP_PREFLT);
		memcpy((ElfW(Shdr) *)result->d_shdr, base + shoff,
		       result->d_shnum, sizeof(ElfW(Shdr)));

		/* Allocate the driver section reference vector. */
		result->d_sections = (struct driver_section_awref *)kmalloc(result->d_shnum *
		                                                            sizeof(struct driver_section_awref),
		                                                            GFP_PREFLT | GFP_CALLOC);

		/* Load the section header string table. */
		{
			ElfW(Shdr) const *shstrtab;
			size_t shstrtab_size;
			char const *shstrtab_base, *shstrtab_end;
			shstrtab = &result->d_shdr[result->d_shstrndx];
			if unlikely(shstrtab->sh_type == SHT_NOBITS) {
				THROW_FAULTY_ELF_ERROR(E_NOT_EXECUTABLE_FAULTY_REASON_ELF_NOBITS_SHSTRTAB,
				                       result->d_shstrndx);
			}
			shstrtab_base = (char *)(base + shstrtab->sh_offset);
			shstrtab_size = shstrtab->sh_size;
			shstrtab_end  = shstrtab_base + shstrtab_size;
			if unlikely((byte_t const *)shstrtab_end < (byte_t const *)shstrtab_base ||
			            (byte_t const *)shstrtab_base < base ||
			            (byte_t const *)shstrtab_end > base + num_bytes) {
				THROW_FAULTY_ELF_ERROR(E_NOT_EXECUTABLE_FAULTY_REASON_ELF_BAD_SHSTRTAB,
				                       (size_t)((byte_t const *)shstrtab_base - base),
				                       shstrtab_size, num_bytes);
			}
			while (shstrtab_size && !shstrtab_base[shstrtab_size - 1])
				--shstrtab_size;
			/* Load the .shstrtab section into memory. */
			result->d_shstrtab = (char *)kmalloc((shstrtab_size + 1) * sizeof(char), GFP_PREFLT);
			memcpy((char *)result->d_shstrtab, shstrtab_base, shstrtab_size, sizeof(char));
			((char *)result->d_shstrtab)[shstrtab_size] = '\0';
			result->d_shstrsiz = shstrtab_size;
		}

		/* Allocate the vector of dependencies. */
		result->d_depcnt = dt_needed_count;
		result->d_depvec = (struct driver_axref *)kmalloc(dt_needed_count * sizeof(struct driver_axref),
		                                                  GFP_CALLOC | GFP_PREFLT);

		/* Create fully faulted+locked mnode/mpart pairs
		 * for all  of  the  driver's  program  headers. */
		SLIST_INIT(&nodes);
		TRY {
			uintptr_t cache_version;
			cache_version = 0;

			result->d_module.md_loadmin = (byte_t *)-1;
			for (phidx = 0; phidx < phnum; ++phidx) {
				struct mnode *node;
				if (result->d_phdr[phidx].p_type != PT_LOAD)
					continue;

				/* Create a new node for this program header. */
				node = create_mnode_for_phdr(&result->d_phdr[phidx],
				                             base, num_bytes,
				                             drv_fspath,
				                             drv_fsname);
				SLIST_INSERT(&nodes, node, _mn_dead);

				/* Update the new driver's min/max bounds according to the new node. */
				if (result->d_module.md_loadmin > node->mn_minaddr)
					result->d_module.md_loadmin = node->mn_minaddr;
				if (result->d_module.md_loadmax < node->mn_maxaddr)
					result->d_module.md_loadmax = node->mn_maxaddr;
			}

			/* The following _really_ shouldn't happen... */
			if unlikely(SLIST_EMPTY(&nodes))
				THROW_FAULTY_ELF_ERROR(E_NOT_EXECUTABLE_FAULTY_REASON_ELF_NOSEGMENTS);
			assert(result->d_module.md_loadmin <= result->d_module.md_loadmax);

			/* Fill in simple fields. */
			result->d_module.md_refcnt     = 2; /* +1:<return-value>, +1:Not setting the `DRIVER_STATE_INIT' flag */
			result->d_module.md_weakrefcnt = 3; /* +1:md_refcnt != 0, +1:md_nodecount != 0, +1:new_ll->dll_drivers[new_ll_insert_index] */
/*			result->d_module.md_nodecount  = 0; */ /* Already initialized by `GFP_CALLOC' */
			result->d_module.md_ops        = &driver_module_ops;
			result->d_module.md_mman       = &mman_kernel; /* !!! NOT A REFERENCE */
			result->d_module.md_file       = xincref(drv_file);
			result->d_module.md_fspath     = xincref(drv_fspath);
			result->d_module.md_fsname     = xincref(drv_fsname);
#ifdef _MODULE_HAVE_SIZEOF_POINTER
			result->d_module.md_sizeof_pointer = sizeof(void *);
#endif /* _MODULE_HAVE_SIZEOF_POINTER */
#if DRIVER_STATE_INIT != 0
			result->d_state = DRIVER_STATE_INIT;
#endif /* DRIVER_STATE_INIT != 0 */
			result->d_phnum = phnum;
/*			result->d_eh_frame_cache = NULL;*/ /* Already initialized by `GFP_CALLOC' */
			atomic_rwlock_cinit(&result->d_eh_frame_cache_lock);

			TRY {
				REF struct driver_loadlist *old_ll;
				REF struct driver_loadlist *new_ll;
				/* Allocate a new `struct driver_loadlist' descriptor
				 * which can then be used to replace the current one. */
again_get_driver_loadlist:
				old_ll = arref_get(&drivers);
				FINALLY_DECREF_UNLIKELY(old_ll);
				new_ll = (REF struct driver_loadlist *)kmalloc(offsetof(struct driver_loadlist, dll_drivers) +
				                                               (old_ll->dll_count + 1) *
				                                               sizeof(WEAK REF struct driver *),
				                                               GFP_LOCKED | GFP_PREFLT);
				TRY {
					size_t i, new_ll_insert_index;
					uintptr_t loadaddr;
					/* Acquire a lock to the kernel mman, so we can insert the new nodes. */
again_acquire_mman_lock:
					mman_lock_acquire(&mman_kernel);

					/************************************************************************/
					/* !!!NOEXCEPT FROM HERE!!!                                             */
					/************************************************************************/

					/* Figure out where we want to load the driver. */
					{
						void *loadbase;
						size_t loadsize;
						loadsize = (size_t)((result->d_module.md_loadmax + 1) -
						                    (result->d_module.md_loadmin));
						loadbase = mman_findunmapped(&mman_kernel, HINT_GETADDR(KERNEL_VMHINT_DRIVER),
						                             loadsize, HINT_GETMODE(KERNEL_VMHINT_DRIVER));
						if unlikely(loadbase == MAP_FAILED) {
							mman_lock_release(&mman_kernel);
							if (system_clearcaches_s(&cache_version))
								goto again_acquire_mman_lock;
							/*driver_deadnodes_freelist(&nodes);*/
							/*destroy_partially_initialized_driver(result);*/
							THROW(E_BADALLOC_INSUFFICIENT_VIRTUAL_MEMORY, loadsize);
						}
						loadaddr = (uintptr_t)((byte_t *)loadbase - result->d_module.md_loadmin);
					}
					result->d_module.md_loadaddr = loadaddr;
					result->d_module.md_loadmin += loadaddr;
					result->d_module.md_loadmax += loadaddr;

					/* Insert all of the driver's mem-nodes into the kernel mman. */
					while (!SLIST_EMPTY(&nodes)) {
						struct mnode *node;
						node = SLIST_FIRST(&nodes);
						SLIST_REMOVE_HEAD(&nodes, _mn_dead);
						/* Relocate the node relative to our chosen `loadaddr'.
						 * NOTE: Because we're using anonymous,  pre-initialized,
						 *       locked mem-parts to  back these  nodes, we  know
						 *       that the  associated mem-parts  won't have  been
						 *       truncated since we've created them above, simply
						 *       because we're the only ones how even know  about
						 *       their existence! */
						node->mn_minaddr += loadaddr;
						node->mn_maxaddr += loadaddr;

						/* Keep track of the # of nodes that map our driver. */
						node->mn_module = &result->d_module;
						++result->d_module.md_nodecount;

						/* Insert this node into the kernel mman. */
						if unlikely(!mman_mappings_tryinsert(&mman_kernel, node)) {
							/* This could happen if the different program headers
							 * overlap with each other (which would be indicative
							 * of a faulty driver file)
							 *
							 * As such, undo all already-inserted nodes, followed
							 * by throwing an exception. */
							SLIST_INSERT(&nodes, node, _mn_dead);
							mman_remove_driver_nodes(result, &nodes);
							mman_lock_release(&mman_kernel);
							/*driver_deadnodes_freelist(&nodes);*/
							/*destroy_partially_initialized_driver(result);*/
							THROW_FAULTY_ELF_ERROR(E_NOT_EXECUTABLE_FAULTY_REASON_ELF_SEGMENTOVERLAP);
						}

						/* Prepare the kernel page directory for the driver. */
#ifdef ARCH_PAGEDIR_NEED_PERPARE_FOR_KERNELSPACE
						if unlikely(!mnode_pagedir_kernelprepare(node)) {
							SLIST_INSERT(&nodes, node, _mn_dead);
							mman_remove_driver_nodes(result, &nodes);
							mman_lock_release(&mman_kernel);
							/*driver_deadnodes_freelist(&nodes);*/
							/*destroy_partially_initialized_driver(result);*/
							THROW(E_BADALLOC_INSUFFICIENT_PHYSICAL_MEMORY, 1);
						}
#endif /* ARCH_PAGEDIR_NEED_PERPARE_FOR_KERNELSPACE */

						/* Force-map the node's part into kernelspace. */
						mpart_mmap_force(node->mn_part, mnode_getaddr(node),
						                 mnode_getsize(node), node->mn_partoff,
						                 mnode_getperm_force_nouser(node));
					}
					assert(result->d_module.md_nodecount != 0);

					/* Sync the fact that we've just created bunch of new memory mappings. */
					mman_supersyncall();

					/* At this point, we still have to initialize all of the following,
					 * all of which depend on where we end up loading the driver to, or
					 * are simply easier to calculate once we've mapped the driver:
					 * >> char const               *d_name;
					 * >> byte_t const             *d_eh_frame_start;
					 * >> byte_t const             *d_eh_frame_end;
					 * >> size_t                    d_dyncnt;
					 * >> ElfW(Dyn) const          *d_dynhdr;
					 * >> ElfW(Sym) const          *d_dynsym_tab;
					 * >> size_t                    d_dynsym_cnt;
					 * >> ElfW(GnuHashTable) const *d_gnuhashtab;
					 * >> ElfW(HashTable) const    *d_hashtab;
					 * >> char const               *d_dynstr;
					 * >> char const               *d_dynstr_end;
					 *
					 * NOTE: At this point, we can  also already access the  driver's
					 *       memory mappings (everything was prefaulted at the start,
					 *       meaning  that  everything should  be mapped  right now).
					 *       However, still make use of `memcpy_nopf()', since faulty
					 *       driver files can still cause programs at this point. */
					{
						unsigned int status;
						uintptr_t reasons[2] = { 0, 0 };
						status = driver_load_image_pointers(result, reasons);
						if unlikely(status != 0) {
							mman_remove_driver_nodes(result, &nodes);
							mman_lock_release(&mman_kernel);
							THROW_FAULTY_ELF_ERROR(status, reasons[0], reasons[1]);
						}
					}

					/* Check if the current driver load list already contains
					 * a driver with  a name  identical to  `result->d_name'. */
					new_ll_insert_index = old_ll->dll_count;
					for (i = 0; i < old_ll->dll_count; ++i) {
						REF struct driver *other_driver;
						other_driver = old_ll->dll_drivers[i];
						if (result->d_module.md_loadmin < other_driver->d_module.md_loadmin)
							new_ll_insert_index = i;
						if (!tryincref(other_driver))
							continue;
						if unlikely(strcmp(other_driver->d_name, result->d_name) == 0) {
							/* Driver already loaded... (undo everything...) */
							mman_remove_driver_nodes(result, &nodes);
							mman_lock_release_f(&mman_kernel);
							kfree(new_ll);
							weakdecref_nokill(&mman_kernel);
							xdecref_nokill(drv_file);
							xdecref_nokill(drv_fspath);
							xdecref_nokill(drv_fsname);
							driver_deadnodes_freelist(&nodes);
							destroy_partially_initialized_driver(result);
							mman_lockops_reap(&mman_kernel);
							if (pnew_driver_loaded)
								*pnew_driver_loaded = false;
							return other_driver;
						}
						decref_unlikely(other_driver);
					}

					/* Initialize the new driver load-list */
					new_ll->dll_refcnt = 1;
					new_ll->dll_count  = old_ll->dll_count + 1;
					memcpy(&new_ll->dll_drivers[0], &old_ll->dll_drivers[0],
					       new_ll_insert_index, sizeof(WEAK REF struct driver *));
					new_ll->dll_drivers[new_ll_insert_index] = result;
					memcpy(&new_ll->dll_drivers[new_ll_insert_index + 1],
					       &old_ll->dll_drivers[new_ll_insert_index],
					       old_ll->dll_count - new_ll_insert_index,
					       sizeof(WEAK REF struct driver *));
					for (i = 0; i < old_ll->dll_count; ++i)
						weakincref(old_ll->dll_drivers[i]);

					/* Remove drivers that have since been destroyed from the driver load list. */
					for (i = 0; i < new_ll->dll_count;) {
						if likely(!wasdestroyed(new_ll->dll_drivers[i])) {
							++i;
							continue;
						}
						weakdecref_nokill(new_ll->dll_drivers[i]);
						--new_ll->dll_count;
						memmovedown(&new_ll->dll_drivers[i],
						            &new_ll->dll_drivers[i + 1],
						            new_ll->dll_count - i,
						            sizeof(WEAK REF struct driver *));
					}

					/* Write-back the new driver load-list, and check that the  driver
					 * load-list hasn't changed  since then. (If  it did change,  then
					 * there's a chance that the `result'-driver has since been loaded
					 * by some other thread...) */
					if unlikely(!arref_cmpxch_inherit_new(&drivers, old_ll, new_ll)) {
						/* The set of loaded drivers has changed.
						 * Undo everything we did since we've read out `old_ll' */
						struct mnode *node;
						mman_remove_driver_nodes(result, &nodes);
						mman_lock_release_f(&mman_kernel);
						destroy(new_ll);
						mman_lockops_reap(&mman_kernel);

						/* Undo relocations already performed based on `loadaddr' */
						/*result->d_module.md_loadaddr = 0;*/ /* Not needed... */
						result->d_module.md_loadmin -= loadaddr;
						result->d_module.md_loadmax -= loadaddr;
						SLIST_FOREACH (node, &nodes, _mn_dead) {
							node->mn_minaddr -= loadaddr;
							node->mn_maxaddr -= loadaddr;
						}
						goto again_get_driver_loadlist;
					}

					/* With everything initialized and double-check for consistency, we
					 * can  finally release our lock from the kernel mman, knowing that
					 * the new driver has been safely loaded into the kernel. */
					mman_lock_release(&mman_kernel);
				} EXCEPT {
					kfree(new_ll);
					RETHROW();
				}
			} EXCEPT {
				weakdecref_nokill(&mman_kernel);
				xdecref_nokill(drv_file);
				xdecref_nokill(drv_fspath);
				xdecref_nokill(drv_fsname);
				RETHROW();
			}
		} EXCEPT {
			driver_deadnodes_freelist(&nodes);
			RETHROW();
		}
	} EXCEPT {
		destroy_partially_initialized_driver(result);
		RETHROW();
	}

	printk(KERN_NOTICE "[mod][+] Insmod: %q\n", result->d_name);
	if (pnew_driver_loaded)
		*pnew_driver_loaded = true;
	return result;
}


/* Load & return  a driver from  a given `driver_file'.  The
 * returned driver may  not have been  initialized, yet.  As
 * such, the  caller  should  use  `driver_initialize()'  in
 * order to ensure that the returned driver was initialized.
 * @param: pnew_driver_loaded: When non-NULL, set to true/false in
 *                             order to indicate  if the  returned
 *                             driver was newly loaded, or already
 *                             present before. */
PUBLIC ATTR_RETNONNULL WUNUSED NONNULL((1)) REF struct driver *KCALL
driver_loadmod_file(struct mfile *__restrict driver_file,
                    struct path *driver_path,
                    struct directory_entry *driver_dentry,
                    USER CHECKED char const *driver_cmdline,
                    bool *pnew_driver_loaded)
		THROWS(E_SEGFAULT, E_NOT_EXECUTABLE, E_BADALLOC, E_IOERROR) {
	REF struct driver *result;
	/* Check if we've already got a driver loaded under `driver_file' */
	result = driver_fromfile(driver_file);
	if (result) {
		if (pnew_driver_loaded)
			*pnew_driver_loaded = false;
	} else {
		/* Fallback: Try to create a new driver. */
		void *unmap_cookie, *tempmap;
		pos_t tempmap_size;
#ifdef CONFIG_USE_NEW_FS
		tempmap_size = (pos_t)atomic64_read(&driver_file->mf_filesize);
#else /* CONFIG_USE_NEW_FS */
		if unlikely(!vm_datablock_isinode(driver_file))
			THROW(E_NOT_EXECUTABLE);
		inode_loadattr((struct inode *)driver_file);
		tempmap_size = ((struct inode *)driver_file)->i_filesize;
#endif /* !CONFIG_USE_NEW_FS */
		/* Impose a limit on how large driver files may be. */
		if (tempmap_size >= (pos_t)UINT32_C(0x10000000))
			THROW(E_NOT_EXECUTABLE_TOOLARGE);
		/* Allocate  a cookie which we can use to NOBLOCK-unmap the temporary
		 * mapping of the given `driver_file' once we're done, or upon error. */
		unmap_cookie = kmalloc(sizeof(struct mman_unmap_kram_job), GFP_LOCKED | GFP_PREFLT);
		/* Create a temporary memory mapping of the given file. */
		TRY {
			tempmap = mman_map(/* self:          */ &mman_kernel,
			                   /* hint:          */ HINT_GETADDR(KERNEL_VMHINT_TEMPORARY),
			                   /* num_bytes:     */ (size_t)tempmap_size,
			                   /* prot:          */ PROT_READ | PROT_WRITE,
			                   /* flags:         */ HINT_GETMODE(KERNEL_VMHINT_TEMPORARY),
			                   /* file:          */ driver_file,
			                   /* file_fspath:   */ driver_path,
			                   /* file_fsname:   */ driver_dentry,
			                   /* file_pos:      */ (pos_t)0);
		} EXCEPT {
			kfree(unmap_cookie);
			RETHROW();
		}
		TRY {
			/* Create a driver from the file mapping. */
			result = driver_create((byte_t const *)tempmap,
			                       (size_t)tempmap_size,
			                       driver_file, driver_path,
			                       driver_dentry, driver_cmdline,
			                       pnew_driver_loaded);
		} EXCEPT {
			mman_unmap_kram_and_kfree(tempmap, (size_t)tempmap_size,
			                          unmap_cookie);
			RETHROW();
		}
		/* Delete the temporary mapping created above. */
		mman_unmap_kram_and_kfree(tempmap, (size_t)tempmap_size,
		                          unmap_cookie);
	}
	return result;
}

PUBLIC ATTR_RETNONNULL WUNUSED NONNULL((1)) REF struct driver *KCALL
driver_loadmod_blob(USER CHECKED void const *base, size_t num_bytes,
                    USER CHECKED char const *driver_cmdline,
                    bool *pnew_driver_loaded)
		THROWS(E_SEGFAULT, E_NOT_EXECUTABLE, E_BADALLOC, E_IOERROR) {
	REF struct driver *result;
	/* Try to create a driver from the given blob. */
	result = driver_create((USER CHECKED byte_t const *)base,
	                       num_bytes, NULL, NULL, NULL,
	                       driver_cmdline, pnew_driver_loaded);
	return result;
}

/* Load a driver, given its filename. */
PRIVATE ATTR_RETNONNULL WUNUSED REF struct driver *KCALL
driver_loadmod_filename(USER CHECKED char const *driver_filename,
                        USER CHECKED char const *driver_cmdline,
                        bool *pnew_driver_loaded)
		THROWS(E_SEGFAULT, E_NOT_EXECUTABLE, E_BADALLOC, E_IOERROR, E_FSERROR) {
	REF struct driver *result;
	USER CHECKED char const *driver_name;
	REF struct mfile *driver_file;
	REF struct path *driver_path;
	REF struct directory_node *driver_directory;
	REF struct directory_entry *driver_dentry;
	u16 driver_namelen;
	/* Open the specified file. */
	driver_path = path_traverse(THIS_FS, driver_filename,
	                            &driver_name, &driver_namelen,
	                            FS_MODE_FNORMAL, NULL);
	FINALLY_DECREF(driver_path);
	sync_read(driver_path);
	driver_directory = (REF struct directory_node *)incref(driver_path->p_inode);
	sync_endread(driver_path);
	{
		FINALLY_DECREF_UNLIKELY(driver_directory);
		driver_file = directory_getnode(driver_directory, driver_name, driver_namelen,
		                                directory_entry_hash(driver_name, driver_namelen),
		                                &driver_dentry);
		if unlikely(!driver_file)
			THROW(E_FSERROR_FILE_NOT_FOUND);
	}
	FINALLY_DECREF(driver_dentry);
	FINALLY_DECREF(driver_file);

	/* Verify that we're dealing with a regular file. */
#ifdef CONFIG_USE_NEW_FS
	if (!mfile_isreg(driver_file))
#else /* CONFIG_USE_NEW_FS */
	if (!INODE_ISREG((struct inode *)driver_file))
#endif /* !CONFIG_USE_NEW_FS */
	{
		THROW(E_NOT_EXECUTABLE_NOT_REGULAR);
	}
	/* Try to create a driver from the given file. */
	result = driver_loadmod_file(driver_file, driver_path, driver_dentry,
	                             driver_cmdline, pnew_driver_loaded);
	return result;
}


struct path_segment {
	USER CHECKED char const *ps_base; /* Segment base */
	size_t                   ps_size; /* Segment length */
};

/* Extract the last segment from `path', and update
 * `*p_pathlen'  to point at the preceding segment. */
PRIVATE WUNUSED NONNULL((2, 3)) bool KCALL
path_get_last_segment_raw(USER CHECKED char const *path,
                          size_t *__restrict p_pathlen,
                          struct path_segment *__restrict result) {
	char const *seg_start;
	size_t pathlen = *p_pathlen;
	/* Skip trialing slashes. */
	while (pathlen && path[pathlen - 1] == '/')
		--pathlen;
	if (!pathlen)
		return false;
	/* Find the start of the current segment. */
	seg_start = (char const *)memrend(path, '/', pathlen) + 1;
	/* Write back results. */
	result->ps_base = seg_start;
	result->ps_size = (size_t)((path + pathlen) - seg_start);
	*p_pathlen      = (size_t)(seg_start - path);
	return true;
}

PRIVATE WUNUSED NONNULL((2, 3)) bool KCALL
path_get_last_segment(USER CHECKED char const *path,
                      size_t *__restrict p_pathlen,
                      struct path_segment *__restrict result) {
	size_t indirection;
	indirection = 0;
again:
	if (path_get_last_segment_raw(path, p_pathlen, result)) {
		/* Check for special directories '.' and '..' */
		if (result->ps_size == 1 && result->ps_base[0] == '.')
			goto again;
		if (result->ps_size == 2 && result->ps_base[0] == '.' && result->ps_base[1] == '.') {
			++indirection;
			goto again;
		}
		if (indirection) {
			--indirection;
			goto again;
		}
		return true;
	}
	return false;
}

/* Check if the given `pth' equals `pth_str...pth_len' */
PRIVATE WUNUSED NONNULL((1)) bool KCALL
path_equals_string(struct path *__restrict pth,
                   USER CHECKED char const *pth_str,
                   size_t pth_len) {
	bool path_is_root = false;
	struct path_segment lastseg;
	while (pth_len && pth_str[0] == '/') {
		path_is_root = true;
		++pth_str;
		--pth_len;
	}
	while (path_get_last_segment(pth_str, &pth_len, &lastseg)) {
		if (lastseg.ps_size != pth->p_dirent->de_namelen)
			goto nope;
		if (memcmp(pth->p_dirent->de_name, lastseg.ps_base,
		           lastseg.ps_size * sizeof(char)) != 0)
			goto nope;
		if unlikely((pth = pth->p_parent) == NULL)
			goto nope;
	}
	if (path_is_root)
		return pth == &vfs_kernel;
	return pth == ATOMIC_READ(THIS_FS->f_cwd);
nope:
	return false;
}


PRIVATE WUNUSED REF struct driver *KCALL
driver_fromfilename(USER CHECKED char const *driver_filename)
		THROWS(E_SEGFAULT) {
	REF struct driver *result;
	REF struct driver_loadlist *ll;
	char const *name;
	size_t pathlen, i;
	name = strrchr(driver_filename, '/');
	if (name) {
		pathlen = (size_t)(name - driver_filename);
		++name;
	} else {
		pathlen = 0;
		name    = driver_filename;
	}
	ll = get_driver_loadlist();
	FINALLY_DECREF_UNLIKELY(ll);
	for (i = 0; i < ll->dll_count; ++i) {
		result = ll->dll_drivers[i];
		if (!tryincref(result))
			continue;
		if (result->d_module.md_fspath && result->d_module.md_fsname) {
			TRY {
				if (strcmp(result->d_module.md_fsname->de_name, name) == 0 &&
				    path_equals_string(result->d_module.md_fspath,
				                       driver_filename, pathlen))
					return result; /* This is the one! */
			} EXCEPT {
				decref_unlikely(result);
				RETHROW();
			}
		}
		decref_unlikely(result);
	}
	return NULL;
}


PRIVATE WUNUSED REF struct driver *KCALL
driver_loadmod_file_in_path(char const *__restrict path, size_t pathlen,
                            USER CHECKED char const *driver_name, size_t driver_namelen,
                            USER CHECKED char const *driver_cmdline,
                            bool *pnew_driver_loaded,
                            bool create_new_drivers)
		THROWS(E_SEGFAULT, E_NOT_EXECUTABLE, E_BADALLOC, E_IOERROR, E_FSERROR) {
	REF struct driver *result;
	char *buf;
	/* Strip leading/trailing slashes from strings. */
	while (pathlen && path[pathlen - 1] == '/')
		--pathlen;
	while (driver_namelen && driver_name[0] == '/') {
		++driver_name;
		--driver_namelen;
	}
	buf = (char *)malloca((pathlen + driver_namelen + 2) * sizeof(char));
	TRY {
		char *p;
		/* Construct the filename  */
		p = (char *)mempcpy(buf, path, pathlen, sizeof(char));
		*p++ = '/';
		p = (char *)mempcpy(buf, driver_name, driver_namelen, sizeof(char));
		*p++ = '\0';
		if (create_new_drivers) {
			/* Second-phase (actually load new drivers) */
			result = driver_loadmod_filename(buf, driver_cmdline,
			                                 pnew_driver_loaded);
		} else {
			result = driver_fromfilename(buf);
		}
	} EXCEPT {
		freea(buf);
		RETHROW();
	}
	freea(buf);
	return result;
}

PRIVATE WUNUSED REF struct driver *KCALL
driver_loadmod_file_with_path(struct driver_libpath_struct *__restrict lp,
                              USER CHECKED char const *driver_name,
                              USER CHECKED char const *driver_cmdline,
                              bool *pnew_driver_loaded,
                              bool create_new_drivers)
		THROWS(E_SEGFAULT, E_NOT_EXECUTABLE, E_BADALLOC, E_IOERROR, E_FSERROR) {
	REF struct driver *result;
	char const *sep;
	char const *path_start = lp->dlp_path;
	size_t driver_namelen  = strlen(driver_name);
	for (;;) {
		sep = strchrnul(path_start, ':');
		TRY {
			result = driver_loadmod_file_in_path(path_start, (size_t)(sep - path_start),
			                                     driver_name, driver_namelen,
			                                     driver_cmdline, pnew_driver_loaded,
			                                     create_new_drivers);
		} EXCEPT {
			error_code_t code = error_code();
			if (ERROR_CLASS(code) != E_FSERROR || !E_FSERROR_IS_FILE_NOT_FOUND(code))
				RETHROW(); /* Something other than file-not-found */
			if (create_new_drivers && !*sep)
				RETHROW(); /* This is the last possible path to search in the second phase. */
			result = NULL;
		}
		if (result)
			break;
		if (!*sep)
			break;
		path_start = sep + 1;
	}
	return result;
}

/* Load a driver, given its name. */
PUBLIC ATTR_RETNONNULL WUNUSED REF struct driver *KCALL
driver_loadmod(USER CHECKED char const *driver_name,
               USER CHECKED char const *driver_cmdline,
               bool *pnew_driver_loaded)
		THROWS(E_SEGFAULT, E_NOT_EXECUTABLE, E_BADALLOC, E_IOERROR, E_FSERROR) {
	REF struct driver *result;
	/* Check if the given `driver_name' is actually a file-path.
	 * We can identify paths  by them containing slashes  (`/'),
	 * and if  this is  the case,  rather than  searching for  a
	 * driver  with a specific name, we must search for one with
	 * a specific filename. */
	if (strchr(driver_name, '/')) {
		/* Check if we've already got a driver with this filename. */
		result = driver_fromfilename(driver_name);
		if (result) {
			if (pnew_driver_loaded)
				*pnew_driver_loaded = true;
		} else {
			result = driver_loadmod_filename(driver_name,
			                                 driver_cmdline,
			                                 pnew_driver_loaded);
		}
	} else if ((result = driver_fromname(driver_name)) != NULL) {
		if (pnew_driver_loaded)
			*pnew_driver_loaded = true;
	} else {
		REF struct driver_libpath_struct *lp;
		/* Search for a  file matching `driver_name'  in the  driver
		 * library path list (which usually contains `/os/drivers'). */
		lp = arref_get(&driver_libpath);
		FINALLY_DECREF_UNLIKELY(lp);
		/* Don't create new drivers during the first pass. */
		result = driver_loadmod_file_with_path(lp, driver_name,
		                                       driver_cmdline,
		                                       pnew_driver_loaded,
		                                       false);
		if (!result) {
			/* If no already-loaded driver matches the given name,
			 * then do a second pass where we actually open files,
			 * map them, and call `driver_create()' */
			result = driver_loadmod_file_with_path(lp, driver_name,
			                                       driver_cmdline,
			                                       pnew_driver_loaded,
			                                       true);
		}
	}
	return result;
}


/* Helper-wrappers for the functions above that will automatically
 * ensure that the driver is initialized successfully, and if said
 * initialization were to cause an exception, finalize the  driver
 * to have it be unloaded once again. */
PUBLIC ATTR_RETNONNULL WUNUSED NONNULL((1)) REF struct driver *KCALL
driver_insmod_file(struct mfile *__restrict driver_file,
                   struct path *driver_path,
                   struct directory_entry *driver_dentry,
                   USER CHECKED char const *driver_cmdline,
                   bool *pnew_driver_loaded)
		THROWS(E_SEGFAULT, E_NOT_EXECUTABLE, E_BADALLOC, E_IOERROR) {
	REF struct driver *result;
	result = driver_loadmod_file(driver_file, driver_path, driver_dentry,
	                             driver_cmdline, pnew_driver_loaded);
	TRY {
		driver_initialize(result);
	} EXCEPT {
		decref(result);
		RETHROW();
	}
	return result;
}

PUBLIC ATTR_RETNONNULL WUNUSED NONNULL((1)) REF struct driver *KCALL
driver_insmod_blob(USER CHECKED void const *base, size_t num_bytes,
                   USER CHECKED char const *driver_cmdline,
                   bool *pnew_driver_loaded)
		THROWS(E_SEGFAULT, E_NOT_EXECUTABLE, E_BADALLOC, E_IOERROR) {
	REF struct driver *result;
	result = driver_loadmod_blob(base, num_bytes,
	                             driver_cmdline,
	                             pnew_driver_loaded);
	TRY {
		driver_initialize(result);
	} EXCEPT {
		decref(result);
		RETHROW();
	}
	return result;
}

PUBLIC ATTR_RETNONNULL WUNUSED REF struct driver *KCALL
driver_insmod(USER CHECKED char const *driver_name,
              USER CHECKED char const *driver_cmdline,
              bool *pnew_driver_loaded)
		THROWS(E_SEGFAULT, E_NOT_EXECUTABLE, E_BADALLOC, E_IOERROR, E_FSERROR) {
	REF struct driver *result;
	result = driver_loadmod(driver_name, driver_cmdline,
	                        pnew_driver_loaded);
	TRY {
		driver_initialize(result);
	} EXCEPT {
		decref(result);
		RETHROW();
	}
	return result;
}



/* Try to unload a driver module.
 * This function will:
 *   - Call `driver_finalize()'
 *   - if (!DRIVER_DELMOD_F_NODEPEND):
 *         Search for other modules  make use of `self'  through
 *         dependencies and finalize all of them such that their
 *         dependency vectors  can  be  cleared,  including  the
 *         contained references to `self'
 *   - weakincref(self);
 *   - if (DRIVER_DELMOD_F_FORCE):
 *         force the refcnt of `self' to 0
 *     else:
 *         Drop a reference from `self'
 *   - if (!DRIVER_DELMOD_F_NONBLOCK):
 *         Wait for `self' to reach its `DRIVER_STATE_DEAD'-state
 *   - weakdecref(self);
 * @param: self:                      The driver to try to unload.
 * @param: flags:                     Set of `DRIVER_DELMOD_F_*'
 * @return: DRIVER_DELMOD_ST_SUCCESS: Successfully unloaded the driver and inherited the reference to `self'
 * @return: DRIVER_DELMOD_ST_INUSE:   Only when  `DRIVER_DELMOD_F_NONBLOCK' is  given: There  are
 *                                    still other references to `self' that aren't accounted for.
 * @return: DRIVER_DELMOD_ST_INUSE:   The given driver `self' is `kernel_driver' */
PUBLIC NONNULL((1)) unsigned int FCALL
driver_try_decref_and_delmod(/*inherit(always)*/ REF struct driver *__restrict self,
                             unsigned int flags)
		THROWS(E_WOULDBLOCK);



/* Unload a driver from the kernel core.
 * @param: driver_name: The name of the driver, or its
 *                      filename (when the first character is `/')
 * @param: flags: Set of `DRIVER_DELMOD_F_*'
 * @return: * :   One of `DRIVER_DELMOD_ST_*' */
PUBLIC NONNULL((1)) unsigned int FCALL
driver_delmod(USER CHECKED char const *driver_name,
              unsigned int flags)
		THROWS(E_WOULDBLOCK, E_BADALLOC, ...);
PUBLIC NONNULL((1)) unsigned int FCALL
driver_delmod_file(struct mfile *__restrict driver_file,
                   unsigned int flags)
		THROWS(E_WOULDBLOCK, E_BADALLOC, ...);




/* [1..1] The current driver library path.
 * This path  is  a  ':'-separated list  of  UNIX-style  pathnames
 * that are used to resolve dependencies of kernel driver modules.
 * By default, this  string is  KERNEL_DRIVER_DEFAULT_LIBRARY_PATH */
PUBLIC struct driver_libpath_arref driver_libpath;



DECL_END
#endif /* CONFIG_USE_NEW_DRIVER */

#endif /* !GUARD_KERNEL_SRC_MEMORY_DRIVER_C */
