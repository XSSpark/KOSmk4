/* Copyright (c) 2019-2020 Griefer@Work                                       *
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
 *    Portions Copyright (c) 2019-2020 Griefer@Work                           *
 * 2. Altered source versions must be plainly marked as such, and must not be *
 *    misrepresented as being the original software.                          *
 * 3. This notice may not be removed or altered from any source distribution. *
 */
#ifndef GUARD_KERNEL_INCLUDE_KERNEL_EXECABI_H
#define GUARD_KERNEL_INCLUDE_KERNEL_EXECABI_H 1

#include <kernel/compiler.h>

#include <kernel/arch/execabi.h>
#include <kernel/types.h>
#include <misc/atomic-ref.h>

#include <compat/config.h>

/* Executable format ABI definition hook (for drivers
 * that wish to provide extended support for exec formats) */

/* Max # of ~magic~ bytes that may be provided by exec ABIs.
 * Also specifies the # of bytes that get pre-read by the exec-ABI
 * invocation machinery before the ABI's exec callback is invoked. */
#ifndef CONFIG_EXECABI_MAXHEADER
#include <elf.h>
#include <hybrid/typecore.h>
#if defined(__ARCH_HAVE_COMPAT) && __ARCH_COMPAT_SIZEOF_POINTER != __SIZEOF_POINTER__
#define CONFIG_EXECABI_MAXHEADER __SIZEOF_ELF64_EHDR
#elif __SIZEOF_POINTER__ == 8
#define CONFIG_EXECABI_MAXHEADER __SIZEOF_ELF64_EHDR
#else /* ... */
#define CONFIG_EXECABI_MAXHEADER __SIZEOF_ELF32_EHDR
#endif /* !... */
#endif /* !CONFIG_EXECABI_MAXHEADER */

/* Define feature test macros for ABIs built into the kernel core */
#undef CONFIG_EXECABI_HAVE_BUILTIN_ELF
#define CONFIG_EXECABI_HAVE_BUILTIN_ELF 1


#ifdef __CC__
DECL_BEGIN

/* Base address and size symbols for the system RTLD */
#ifndef CONFIG_EXECABI_ARCH_HEADER_DEFINES_EXECABI_SYSTEM_RTLD
DATDEF byte_t execabi_system_rtld[];

#undef execabi_system_rtld_size
#ifdef __INTELLISENSE__
DATDEF size_t const execabi_system_rtld_size;
#else /* __INTELLISENSE__ */
DATDEF byte_t execabi_system_rtld_size[];
#define execabi_system_rtld_size ((size_t)execabi_system_rtld_size)
#endif /* !__INTELLISENSE__ */
#endif /* !CONFIG_EXECABI_ARCH_HEADER_DEFINES_EXECABI_SYSTEM_RTLD */

/* A static VM file blob for the built-in RTLD user-space program.
 * This is a raw ELF binary blob that is hard-linked into the kernel
 * core, and is mapped via copy-on-write into any user-space process
 * that requests the use of a dynamic linker
 * NOTE: The associated source code can be found in `/kos/src/libdl/...', and
 *       the (unstripped) library file can be found on-disk as `/lib/libdl.so' */
#ifndef CONFIG_EXECABI_ARCH_HEADER_DEFINES_EXECABI_SYSTEM_RTLD_FILE
DATDEF struct vm_ramfile execabi_system_rtld_file;
#endif /* !CONFIG_EXECABI_ARCH_HEADER_DEFINES_EXECABI_SYSTEM_RTLD_FILE */

#ifdef __ARCH_HAVE_COMPAT
/* Base address and size symbols for the system RTLD */
#ifndef CONFIG_EXECABI_ARCH_HEADER_DEFINES_COMPAT_ELFEXEC_SYSTEM_RTLD
DATDEF byte_t compat_execabi_system_rtld[];

#undef compat_execabi_system_rtld_size
#ifdef __INTELLISENSE__
DATDEF size_t const compat_execabi_system_rtld_size;
#else /* __INTELLISENSE__ */
DATDEF byte_t compat_execabi_system_rtld_size[];
#define compat_execabi_system_rtld_size  ((size_t)compat_execabi_system_rtld_size)
#endif /* !__INTELLISENSE__ */
#endif /* !CONFIG_EXECABI_ARCH_HEADER_DEFINES_COMPAT_ELFEXEC_SYSTEM_RTLD */

/* A static VM file blob for the building RTLD user-space program.
 * This is a raw ELF binary blob that is hard-linked into the kernel
 * core, and is mapped via copy-on-write into any user-space process
 * that requests the use of a dynamic linker
 * NOTE: The associated source code can be found in `/kos/src/libdl/...' */
#ifndef CONFIG_EXECABI_ARCH_HEADER_DEFINES_COMPAT_ELFEXEC_SYSTEM_RTLD_FILE
DATDEF struct vm_ramfile compat_execabi_system_rtld_file;
#endif /* !CONFIG_EXECABI_ARCH_HEADER_DEFINES_COMPAT_ELFEXEC_SYSTEM_RTLD_FILE */
#endif /* __ARCH_HAVE_COMPAT */



#ifdef __ARCH_HAVE_COMPAT
typedef USER CHECKED void const *execabi_strings_t;
#define EXECABI_PARAM_argv_is_compat  bool argv_is_compat
#define EXECABI_PARAM__argv_is_compat , bool argv_is_compat
#define EXECABI_PARAM_argv_is_compat_ bool argv_is_compat,
#define EXECABI_ARG_argv_is_compat    argv_is_compat
#define EXECABI_ARG__argv_is_compat   , argv_is_compat
#define EXECABI_ARG_argv_is_compat_   argv_is_compat,
#else /* __ARCH_HAVE_COMPAT */
typedef USER UNCHECKED char const *USER CHECKED const *execabi_strings_t;
#define EXECABI_PARAM_argv_is_compat  /* nothing */
#define EXECABI_PARAM__argv_is_compat /* nothing */
#define EXECABI_PARAM_argv_is_compat_ /* nothing */
#define EXECABI_ARG_argv_is_compat    /* nothing */
#define EXECABI_ARG__argv_is_compat   /* nothing */
#define EXECABI_ARG_argv_is_compat_   /* nothing */
#endif /* !__ARCH_HAVE_COMPAT */

struct driver;
struct execabi {
	/* NOTE: All fields of this structure are [const] */
	WEAK REF struct driver *ea_driver; /* [1..1] Weak reference to the implementing driver.
	                                    * Before any of the callbacks below are invoked, a strong
	                                    * reference is acquired to this driver (using `tryincref()'),
	                                    * and in case this fails, an attempt is made to discard the
	                                    * associated ABI (and the kernel will behave as though the
	                                    * ABI hadn't actually been defined) */
	u8                      ea_magsiz; /* # of provided magic bytes (s.a. `ea_magic')
	                                    * When non-zero, this ABI will only be used for files that
	                                    * start with a byte sequence identical to `ea_magic[:ea_magsiz]'
	                                    * When set to 0, all files are considered suitable candidates
	                                    * for this ABI. */
	u8                      ea_magic[CONFIG_EXECABI_MAXHEADER]; /* ABI ~magic~ */

	/* [1..1] ABI execution callback.
	 * @param: effective_vm: The VM into which to map the executable.
	 *                       This must not be the kernel VM, which causes an assertion failure.
	 *                       NOTE: When `change_vm_to_effective_vm' is `true', prior to a successful
	 *                             return of this function, it will also do a `task_setvm(effective_vm)',
	 *                             meaning that the caller will become apart of the given VM.
	 * @param: user_state:   The user-space CPU state to update upon success in a manner that
	 *                       proper execution of the loaded binary is possible.
	 *                       Note however that in the case of a dynamic binary, a dynamic linker
	 *                       may be injected to perform dynamic linking whilst already in user-space.
	 * @param: exec_path:    Filesystem path for the directory inside of which `exec_node' is located.
	 * @param: exec_dentry:  Directory entry containing the filename of `exec_node'
	 * @param: exec_node:    The filesystem node which should be loaded as an executable binary.
	 * @param: exec_header:  The first `CONFIG_EXECABI_MAXHEADER' bytes of `exec_node'. Of this buffer,
	 *                       the leading `ea_magsiz' bytes are known to be equal to `ea_magic'. If the
	 *                       executable file is smaller than `CONFIG_EXECABI_MAXHEADER', trailing bytes
	 *                       are simply zero-initialized.
	 * @param: argc_inject:  The number of arguments from `argv_inject' to inject at the beginning of the user-space argc/argv vector.
	 * @param: argv_inject:  Vector of arguments to inject at the beginning of the user-space argc/argv vector.
	 * @param: argv:         NULL-terminated vector of arguments to-be passed to program being loaded.
	 * @param: envp:         NULL-terminated vector of environment variables to-be passed to program being loaded.
	 * @return: * :          A pointer to the user-space register state to-be loaded in order to start
	 *                       execution of the newly loaded binary (usually equal to `user_state')
	 * @return: NULL:        File doesn't belong this ABI. (But its magic bytes were correct none-the-less)
	 * @throw: E_NOT_EXECUTABLE_NOT_A_BINARY: File cannot be executed using this ABI. (maybe try another ABI?)
	 *                                        But note that the caller must ensure that `ea_magic' is matched. */
	ATTR_RETNONNULL WUNUSED struct icpustate *
	(NONNULL((1, 2, 3, 4, 5, 6)) KCALL *ea_exec)(struct vm *__restrict effective_vm,
	                                             struct icpustate *__restrict user_state,
	                                             struct path *__restrict exec_path,
	                                             struct directory_entry *__restrict exec_dentry,
	                                             struct regular_node *__restrict exec_node,
	                                             void const *exec_header,
	                                             bool change_vm_to_effective_vm,
	                                             size_t argc_inject, KERNEL char const *const *argv_inject,
	                                             execabi_strings_t argv, execabi_strings_t envp
	                                             EXECABI_PARAM__argv_is_compat)
			THROWS(E_WOULDBLOCK, E_BADALLOC, E_SEGFAULT, E_NOT_EXECUTABLE, E_IOERROR);
};

struct execabis_struct {
	WEAK refcnt_t                           eas_refcnt; /* Reference counter. */
	size_t                                  eas_count;  /* [const] # of ABIs defined. */
	COMPILER_FLEXIBLE_ARRAY(struct execabi, eas_abis);  /* [eas_count] Defined ABIs. */
};

/* Destroy a given exec-ABIs listing. */
FUNDEF NOBLOCK NONNULL((1)) void NOTHROW(FCALL execabis_destroy)(struct execabis_struct *__restrict self);
DEFINE_REFCOUNT_FUNCTIONS(struct execabis_struct, eas_refcnt, execabis_destroy)

/* [1..1] Currently defined exec-ABIs */
DATDEF ATOMIC_REF(struct execabis_struct) execabis;

/* Register a given exec-ABI. Note that the only way to unregister
 * an ABI is to unload the associated driver (in which case the ABI
 * is automatically unregistered)
 * @return: true:  Successfully registered the given `abi'.
 * @return: false: The given `abi' had already been registered. */
FUNDEF NONNULL((1)) bool FCALL
execabis_register(struct execabi const *__restrict abi)
		THROWS(E_BADALLOC, E_WOULDBLOCK);

#ifdef CONFIG_BUILDING_KERNEL_CORE
INTDEF NONNULL((1)) bool FCALL
driver_clear_execabis(struct driver *__restrict self)
		THROWS(E_BADALLOC, E_WOULDBLOCK);
#endif /* CONFIG_BUILDING_KERNEL_CORE */


#endif /* __CC__ */



DECL_END

#endif /* !GUARD_KERNEL_INCLUDE_KERNEL_EXECABI_H */
