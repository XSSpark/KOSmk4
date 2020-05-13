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
#ifndef GUARD_KERNEL_INCLUDE_KERNEL_PAGING_H
#define GUARD_KERNEL_INCLUDE_KERNEL_PAGING_H 1

#include <kernel/compiler.h>

#include <kernel/arch/paging.h>
#include <kernel/types.h>

#include <asm/pagesize.h>
#include <kos/kernel/paging.h>

#ifndef PAGESIZE
#define PAGESIZE __ARCH_PAGESIZE
#elif PAGESIZE != __ARCH_PAGESIZE
#error "Miss-configured: `PAGESIZE != __ARCH_PAGESIZE'"
#endif

#ifndef PAGESHIFT
#define PAGESHIFT __ARCH_PAGESHIFT
#elif PAGESHIFT != __ARCH_PAGESHIFT
#error "Miss-configured: `PAGESHIFT != __ARCH_PAGESHIFT'"
#endif

#ifndef PAGEMASK
#define PAGEMASK __ARCH_PAGEMASK
#elif PAGEMASK != __ARCH_PAGEMASK
#error "Miss-configured: `PAGEMASK != __ARCH_PAGEMASK'"
#endif


/* Check if a given `addr' or [startaddr, endaddr)
 * is consider apart of user- or kernel-space.
 * The *_PARTIAL functions check if the range has any overlapping
 * parts with the named address space, while the non-_PARTIAL functions
 * check that the entire range is apart of the named address space.
 * Note that the range-checking functions are allowed to assume that
 * `endaddr >= startaddr'. In the case where `endaddr == startaddr',
 * the range-checking functions behave identical to `ADDR_IS(KERN|USER)',
 * or in other words: will use `endaddr = startaddr+1' */
#ifdef KERNELSPACE_HIGHMEM
#define ADDR_ISKERN(addr)                            (__CCAST(__UINTPTR_TYPE__)(addr) >= KERNELSPACE_BASE)
#define ADDR_ISUSER(addr)                            (__CCAST(__UINTPTR_TYPE__)(addr) < KERNELSPACE_BASE)
#define ADDRRANGE_ISKERN(startaddr, endaddr)         (__CCAST(__UINTPTR_TYPE__)(startaddr) >= KERNELSPACE_BASE)
#define ADDRRANGE_ISUSER(startaddr, endaddr)         (__CCAST(__UINTPTR_TYPE__)(endaddr) <= KERNELSPACE_BASE)
#define ADDRRANGE_ISKERN_PARTIAL(startaddr, endaddr) (__CCAST(__UINTPTR_TYPE__)(endaddr) > KERNELSPACE_BASE)
#define ADDRRANGE_ISUSER_PARTIAL(startaddr, endaddr) (__CCAST(__UINTPTR_TYPE__)(startaddr) < KERNELSPACE_BASE)
#elif defined(KERNELSPACE_LOWMEM)
#define ADDR_ISKERN(addr)                            (__CCAST(__UINTPTR_TYPE__)(addr) < KERNELSPACE_END)
#define ADDR_ISUSER(addr)                            (__CCAST(__UINTPTR_TYPE__)(addr) >= KERNELSPACE_END)
#define ADDRRANGE_ISKERN(startaddr, endaddr)         (__CCAST(__UINTPTR_TYPE__)(endaddr) <= KERNELSPACE_END)
#define ADDRRANGE_ISUSER(startaddr, endaddr)         (__CCAST(__UINTPTR_TYPE__)(startaddr) >= KERNELSPACE_END)
#define ADDRRANGE_ISKERN_PARTIAL(startaddr, endaddr) (__CCAST(__UINTPTR_TYPE__)(startaddr) < KERNELSPACE_END)
#define ADDRRANGE_ISUSER_PARTIAL(startaddr, endaddr) (__CCAST(__UINTPTR_TYPE__)(endaddr) > KERNELSPACE_END)
#else
#define ADDR_ISKERN(addr)                            (__CCAST(__UINTPTR_TYPE__)(addr) >= KERNELSPACE_BASE && __CCAST(__UINTPTR_TYPE__)(addr) < KERNELSPACE_END)
#define ADDR_ISUSER(addr)                            (__CCAST(__UINTPTR_TYPE__)(addr) < KERNELSPACE_BASE && __CCAST(__UINTPTR_TYPE__)(addr) >= KERNELSPACE_END)
#define ADDRRANGE_ISKERN(startaddr, endaddr)         (__CCAST(__UINTPTR_TYPE__)(startaddr) >= KERNELSPACE_BASE && __CCAST(__UINTPTR_TYPE__)(endaddr) <= KERNELSPACE_END)
#define ADDRRANGE_ISUSER(startaddr, endaddr)         (__CCAST(__UINTPTR_TYPE__)(startaddr) >= KERNELSPACE_END || __CCAST(__UINTPTR_TYPE__)(endaddr) <= KERNELSPACE_BASE)
#define ADDRRANGE_ISKERN_PARTIAL(startaddr, endaddr) (__CCAST(__UINTPTR_TYPE__)(startaddr) < KERNELSPACE_END || __CCAST(__UINTPTR_TYPE__)(endaddr) > KERNELSPACE_BASE)
#define ADDRRANGE_ISUSER_PARTIAL(startaddr, endaddr) (__CCAST(__UINTPTR_TYPE__)(startaddr) < KERNELSPACE_BASE && __CCAST(__UINTPTR_TYPE__)(endaddr) > KERNELSPACE_END)
#endif



DECL_BEGIN

#ifndef PAGEDIR_ALIGN
#define PAGEDIR_ALIGN PAGESIZE
#endif /* !PAGEDIR_ALIGN */
#ifndef PAGEDIR_SIZE
#define PAGEDIR_SIZE PAGESIZE
#endif /* !PAGEDIR_SIZE */

#ifndef __pagedir_pushval_t_defined
#define __pagedir_pushval_t_defined 1
#define PAGEDIR_PUSHVAL_INVALID (__CCAST(pagedir_pushval_t)-1)
#define SIZEOF_PAGEDIR_PUSHVAL  __SIZEOF_POINTER__
#ifdef __CC__
typedef uintptr_t pagedir_pushval_t;
#endif /* __CC__ */
#endif /* !__pagedir_pushval_t_defined */


#ifdef __CC__
/* The physical and virtual address bindings of the kernel's page directory.
 * This is the initial page directory active when KOS boots, as well as the
 * the directory later used for thread running in kernel-space only. */
#ifndef ARCH_PAGEDIR_ARCHHEADER_DEFINES_PAGEDIR_KERNEL
DATDEF VIRT pagedir_t pagedir_kernel;
DATDEF PHYS pagedir_t pagedir_kernel_phys;
#endif /* !ARCH_PAGEDIR_ARCHHEADER_DEFINES_PAGEDIR_KERNEL */

/* Initialize the given page directory.
 * The caller is required to allocate the page directory controller itself, which
 * must be aligned and sized according to `PAGEDIR_ALIGN' and `PAGEDIR_SIZE'. */
#ifndef ARCH_PAGEDIR_ARCHHEADER_DEFINES_PAGEDIR_INIT
#ifdef ARCH_PAGEDIR_INIT_NEED_PHYS_SELF
#ifdef ARCH_PAGEDIR_INIT_IS_NOEXCEPT
FUNDEF NOBLOCK NONNULL((1)) void
NOTHROW(FCALL pagedir_init)(VIRT pagedir_t *__restrict self,
                            PHYS vm_phys_t phys_self);
#else /* ARCH_PAGEDIR_INIT_IS_NOEXCEPT */
FUNDEF NOBLOCK NONNULL((1)) void FCALL
pagedir_init(VIRT pagedir_t *__restrict self,
             PHYS vm_phys_t phys_self) THROWS(E_BADALLOC);
#endif /* !ARCH_PAGEDIR_INIT_IS_NOEXCEPT */
#define pagedir_init1(self)            pagedir_init(self, pagedir_translate(self))
#define pagedir_init2(self, phys_self) pagedir_init(self, phys_self)
#else /* ARCH_PAGEDIR_INIT_NEED_PHYS_SELF */
#ifdef ARCH_PAGEDIR_INIT_IS_NOEXCEPT
FUNDEF NOBLOCK NONNULL((1)) void
NOTHROW(FCALL pagedir_init)(VIRT pagedir_t *__restrict self);
#else /* ARCH_PAGEDIR_INIT_IS_NOEXCEPT */
FUNDEF NOBLOCK NONNULL((1)) void FCALL
pagedir_init(VIRT pagedir_t *__restrict self) THROWS(E_BADALLOC);
#endif /* !ARCH_PAGEDIR_INIT_IS_NOEXCEPT */
#define pagedir_init1(self)            pagedir_init(self)
#define pagedir_init2(self, phys_self) pagedir_init(self)
#endif /* !ARCH_PAGEDIR_INIT_NEED_PHYS_SELF */
#endif /* !ARCH_PAGEDIR_ARCHHEADER_DEFINES_PAGEDIR_INIT */

/* Finalize a given page directory. */
#ifndef ARCH_PAGEDIR_ARCHHEADER_DEFINES_PAGEDIR_FINI
#ifdef ARCH_PAGEDIR_FINI_NEED_PHYS_SELF
FUNDEF NOBLOCK NONNULL((1)) void
NOTHROW(FCALL pagedir_fini)(VIRT pagedir_t *__restrict self,
                            PHYS vm_phys_t phys_self);
#define pagedir_fini1(self)            pagedir_fini(self, pagedir_translate(self))
#define pagedir_fini2(self, phys_self) pagedir_fini(self, phys_self)
#else /* ARCH_PAGEDIR_FINI_NEED_PHYS_SELF */
FUNDEF NOBLOCK NONNULL((1)) void
NOTHROW(FCALL pagedir_fini)(VIRT pagedir_t *__restrict self);
#define pagedir_fini1(self)            pagedir_fini(self)
#define pagedir_fini2(self, phys_self) pagedir_fini(self)
#endif /* !ARCH_PAGEDIR_FINI_NEED_PHYS_SELF */
#endif /* !ARCH_PAGEDIR_ARCHHEADER_DEFINES_PAGEDIR_FINI */
#endif /* __CC__ */


/* WARNING: If the host does not support some combination of permission
 *          bits, more permissions than specified may be granted.
 *          For example: On X86, write-permissions require read-permissions
 *                       to be granted, and exec-permissions require read. */
#ifndef PAGEDIR_MAP_FMASK
#define PAGEDIR_MAP_FEXEC      0x0001 /* Permission bit: Allow execution within the mapping. */
#define PAGEDIR_MAP_FWRITE     0x0002 /* Permission bit: Permit write-access to memory within mapping. */
#define PAGEDIR_MAP_FREAD      0x0004 /* Permission bit: Permit read-access to memory within mapping. */
#define PAGEDIR_MAP_FUSER      0x0008 /* Permission bit: Permit the user access to the mapping. */
#define PAGEDIR_MAP_FMASK      0x000f /* Mask of valid permission bits. */
#endif /* !PAGEDIR_MAP_FMASK */






#ifdef __CC__

#ifndef PAGEDIR_API_P_SELFARG
#ifdef ARCH_PAGEDIR_GETSET_USES_POINTER
#define PAGEDIR_P_SELFTYPE        PHYS pagedir_t *
#define PAGEDIR_P_SELFOFVM(vm)    (vm)->v_pdir_phys_ptr
#define PAGEDIR_P_ISKERNEL(x)     ((x) == &pagedir_kernel_phys)
#define PAGEDIR_P_ISFROMVM(x, vm) ((x) == (vm)->v_pdir_phys_ptr)
#else /* ARCH_PAGEDIR_GETSET_USES_POINTER */
#define PAGEDIR_P_SELFTYPE        PHYS vm_phys_t
#define PAGEDIR_P_SELFOFVM(vm)    (vm)->v_pdir_phys
#define PAGEDIR_P_ISKERNEL(x)     ((x) == vm_kernel.v_pdir_phys)
#define PAGEDIR_P_ISFROMVM(x, vm) ((x) == (vm)->v_pdir_phys)
#endif /* !ARCH_PAGEDIR_GETSET_USES_POINTER */
#endif /* !PAGEDIR_API_P_SELFARG */

#if defined(__INTELLISENSE__)
typedef struct { PAGEDIR_P_SELFTYPE _m_self; } __pagedir_p_selftype;
__pagedir_p_selftype __pagedir_p_selfofvm(struct vm *__restrict v);
#undef PAGEDIR_P_SELFTYPE
#undef PAGEDIR_P_SELFOFVM
#define PAGEDIR_P_SELFTYPE __pagedir_p_selftype
#define PAGEDIR_P_SELFOFVM __pagedir_p_selfofvm
#ifdef ARCH_PAGEDIR_GETSET_USES_POINTER
#define PAGEDIR_P_ISKERNEL(x)     ((x)._m_self == &pagedir_kernel_phys)
#define PAGEDIR_P_ISFROMVM(x, vm) ((x)._m_self == (vm)->v_pdir_phys_ptr)
#else /* ARCH_PAGEDIR_GETSET_USES_POINTER */
#define PAGEDIR_P_ISKERNEL(x)     ((x)._m_self == vm_kernel.v_pdir_phys)
#define PAGEDIR_P_ISFROMVM(x, vm) ((x)._m_self == (vm)->v_pdir_phys)
#endif /* !ARCH_PAGEDIR_GETSET_USES_POINTER */
#endif /* __INTELLISENSE__ */





#ifndef ARCH_PAGEDIR_ARCHHEADER_DEFINES_PAGEDIR_SYNCALL
/* Synchronize the entirety of the current page directory.
 * Note that this function may only sync all user-space mappings. If the
 * intent is to also sync all of kernel-space, `pagedir_syncall()'
 * must be used instead. */
FUNDEF NOBLOCK void NOTHROW(FCALL pagedir_syncall_user)(void);
/* Same as `pagedir_syncall()', but also ensures that
 * all of kernel-space is synced. */
FUNDEF NOBLOCK void NOTHROW(FCALL pagedir_syncall)(void);
#endif /* !ARCH_PAGEDIR_ARCHHEADER_DEFINES_PAGEDIR_SYNCALL */

#ifndef ARCH_PAGEDIR_ARCHHEADER_DEFINES_PAGEDIR_SYNCONE
/* Synchronize mappings within the given address range. */
FUNDEF NOBLOCK void NOTHROW(FCALL pagedir_syncone)(VIRT void *addr);
#endif /* !ARCH_PAGEDIR_ARCHHEADER_DEFINES_PAGEDIR_SYNCONE */

/* Translate a virtual address into its physical counterpart. */
FUNDEF NOBLOCK ATTR_PURE WUNUSED PHYS vm_phys_t NOTHROW(FCALL pagedir_translate)(VIRT void *addr);
FUNDEF NOBLOCK ATTR_PURE WUNUSED PHYS vm_phys_t NOTHROW(KCALL pagedir_translate_p)(PAGEDIR_P_SELFTYPE self, VIRT void *addr);

/* Low-level Get/Set the physical address of the currently active page directory. */
#ifndef ARCH_PAGEDIR_ARCHHEADER_DEFINES_PAGEDIR_GETSET
#ifdef ARCH_PAGEDIR_GETSET_USES_POINTER
FUNDEF NOBLOCK ATTR_PURE WUNUSED PHYS pagedir_t *NOTHROW(KCALL pagedir_get)(void);
FUNDEF NOBLOCK void NOTHROW(KCALL pagedir_set)(PHYS pagedir_t *__restrict value);
#else /* ARCH_PAGEDIR_GETSET_USES_POINTER */
FUNDEF NOBLOCK ATTR_PURE WUNUSED PHYS vm_phys_t NOTHROW(KCALL pagedir_get)(void);
FUNDEF NOBLOCK void NOTHROW(KCALL pagedir_set)(PHYS vm_phys_t value);
#endif /* !ARCH_PAGEDIR_GETSET_USES_POINTER */
#endif /* !ARCH_PAGEDIR_ARCHHEADER_DEFINES_PAGEDIR_GETSET */


struct vm;
/* Unmap the entirety of user-space.
 * NOTE: Unlike all other unmap() functions, this one guaranties that it
 *       can perform the task without needing to allocate more memory! */
#ifndef ARCH_PAGEDIR_ARCHHEADER_DEFINES_PAGEDIR_UNMAP_USERSPACE
FUNDEF NOBLOCK void NOTHROW(FCALL pagedir_unmap_userspace)(struct vm *__restrict sync_vm);
FUNDEF NOBLOCK void NOTHROW(FCALL pagedir_unmap_userspace_nosync)(void);
#endif /* !ARCH_PAGEDIR_ARCHHEADER_DEFINES_PAGEDIR_UNMAP_USERSPACE */

#ifndef ARCH_PAGEDIR_ARCHHEADER_DEFINES_PAGEDIR_UNMAP_USERSPACE_P
FUNDEF NOBLOCK void NOTHROW(KCALL pagedir_unmap_userspace_p)(PAGEDIR_P_SELFTYPE self, struct vm *__restrict sync_vm);
FUNDEF NOBLOCK void NOTHROW(KCALL pagedir_unmap_userspace_nosync_p)(PAGEDIR_P_SELFTYPE self);
#endif /* !ARCH_PAGEDIR_ARCHHEADER_DEFINES_PAGEDIR_UNMAP_USERSPACE_P */

/* Temporarily switch to a different page directory `self'
 * @param: self: The page directory to switch to (of type `PAGEDIR_P_SELFTYPE') */
#ifdef __INTELLISENSE__
#define PAGEDIR_P_BEGINUSE(self) do
#define PAGEDIR_P_ENDUSE(self)   __WHILE0
#else /* __INTELLISENSE__ */
#define PAGEDIR_P_BEGINUSE(self)                              \
	do {                                                      \
		PAGEDIR_P_SELFTYPE _old_pdir;                         \
		pflag_t _p_was = PREEMPTION_PUSHOFF();                \
		assert(IS_ALIGNED((uintptr_t)(self), PAGEDIR_ALIGN)); \
		_old_pdir = pagedir_get();                            \
		if (_old_pdir != (self))                              \
			pagedir_set(self);                                \
		do
#define PAGEDIR_P_ENDUSE(self)      \
		__WHILE0;                   \
		if (_old_pdir != (self))    \
			pagedir_set(_old_pdir); \
		PREEMPTION_POP(_p_was);     \
	} __WHILE0
#endif /* !__INTELLISENSE__ */




#ifndef PAGEDIR_PAGEALIGNED
#define PAGEDIR_PAGEALIGNED /* Annotation for variables that need to be aligned on page boundaries. */
#endif /* !PAGEDIR_PAGEALIGNED */

/* Prepare the page directory for a future map() operation.
 * The full cycle of a single mapping then looks like this:
 * >> pagedir_prepare_map(...);
 * >> pagedir_map(...);
 * >> pagedir_unmap(...);
 * >> pagedir_unprepare_map(...);
 * NOTE: A prepared mapping is not only valid for the exact range.
 *       Wishing to map something at a sub-range of it is also allowed:
 *       >> pagedir_prepare_map(5, 6);   // Prepage 6 pages at page-index 5 for modification
 *       >> pagedir_map(5, 3);           // OK
 *       >> pagedir_map(8, 3);           // OK
 *       >> pagedir_unprepare_map(5, 6); // Indicate that the 6 pages must not longer be modified
 * NOTE: `pagedir_prepare_map_keep()' is the same as `pagedir_prepare_map()', but
 *        will not undo already successfully made preparations after a later one fails.
 *        This will include the undoing of redundant preparations of the given range that
 *        were made in prior calls.
 * WARNING: prepare() + unprepare() _DONT_ work recursively, and are also not thread-safe
 * WARNING: unprepare() should always be called with the same range as prepare()
 * @return: true:  Successfully allocated structures required for creating mappings.
 * @return: false: Insufficient physical memory to change mappings. */
#ifndef ARCH_PAGEDIR_ARCHHEADER_DEFINES_PAGEDIR_PREPARE
FUNDEF NOBLOCK WUNUSED __BOOL
NOTHROW(FCALL pagedir_prepare_mapone)(PAGEDIR_PAGEALIGNED VIRT void *addr);
FUNDEF NOBLOCK void
NOTHROW(FCALL pagedir_unprepare_mapone)(PAGEDIR_PAGEALIGNED VIRT void *addr);
#ifndef __OMIT_PAGING_CONSTANT_P_WRAPPERS
FUNDEF NOBLOCK WUNUSED __BOOL
NOTHROW(FCALL __os_pagedir_prepare_map)(PAGEDIR_PAGEALIGNED VIRT void *addr,
                                        PAGEDIR_PAGEALIGNED size_t num_bytes)
		ASMNAME("pagedir_prepare_map");
FUNDEF NOBLOCK WUNUSED __BOOL
NOTHROW(FCALL __os_pagedir_prepare_map_keep)(PAGEDIR_PAGEALIGNED VIRT void *addr,
                                             PAGEDIR_PAGEALIGNED size_t num_bytes)
		ASMNAME("pagedir_prepare_map_keep");
FUNDEF NOBLOCK void
NOTHROW(FCALL __os_pagedir_unprepare_map)(PAGEDIR_PAGEALIGNED VIRT void *addr,
                                          PAGEDIR_PAGEALIGNED size_t num_bytes)
		ASMNAME("pagedir_unprepare_map");
FORCELOCAL NOBLOCK WUNUSED __BOOL
NOTHROW(FCALL pagedir_prepare_map)(PAGEDIR_PAGEALIGNED VIRT void *addr,
                                   PAGEDIR_PAGEALIGNED size_t num_bytes) {
	if (__builtin_constant_p(num_bytes)) {
		if (num_bytes == 0)
			return true;
		if (num_bytes <= PAGESIZE)
			return pagedir_prepare_mapone(addr);
	}
	return __os_pagedir_prepare_map(addr, num_bytes);
}

FORCELOCAL NOBLOCK WUNUSED __BOOL
NOTHROW(FCALL pagedir_prepare_map_keep)(PAGEDIR_PAGEALIGNED VIRT void *addr,
                                        PAGEDIR_PAGEALIGNED size_t num_bytes) {
	if (__builtin_constant_p(num_bytes)) {
		if (num_bytes == 0)
			return true;
		if (num_bytes <= PAGESIZE)
			return pagedir_prepare_mapone(addr);
	}
	return __os_pagedir_prepare_map_keep(addr, num_bytes);
}

FORCELOCAL NOBLOCK void
NOTHROW(FCALL pagedir_unprepare_map)(PAGEDIR_PAGEALIGNED VIRT void *addr,
                                     PAGEDIR_PAGEALIGNED size_t num_bytes) {
	if (__builtin_constant_p(num_bytes)) {
		if (num_bytes == 0)
			return;
		if (num_bytes <= PAGESIZE) {
			pagedir_unprepare_mapone(addr);
			return;
		}
	}
	__os_pagedir_unprepare_map(addr, num_bytes);
}
#else /* !__OMIT_PAGING_CONSTANT_P_WRAPPERS */
FUNDEF NOBLOCK WUNUSED __BOOL
NOTHROW(FCALL pagedir_prepare_map)(PAGEDIR_PAGEALIGNED VIRT void *addr,
                                   PAGEDIR_PAGEALIGNED size_t num_bytes);
FUNDEF NOBLOCK WUNUSED __BOOL
NOTHROW(FCALL pagedir_prepare_map_keep)(PAGEDIR_PAGEALIGNED VIRT void *addr,
                                        PAGEDIR_PAGEALIGNED size_t num_bytes);
FUNDEF NOBLOCK void
NOTHROW(FCALL pagedir_unprepare_map)(PAGEDIR_PAGEALIGNED VIRT void *addr,
                                     PAGEDIR_PAGEALIGNED size_t num_bytes);
#endif /* __OMIT_PAGING_CONSTANT_P_WRAPPERS */
#endif /* !ARCH_PAGEDIR_ARCHHEADER_DEFINES_PAGEDIR_PREPARE */



/* Set a mapping hint for pages apart of the given virtual memory range.
 * Mapping hints are overwritten once a page has been mapped, and when
 * not specified, will default to `NULL'.
 * Their main purpose is to be accessible through atomic means, allowing
 * them to be used by the PAGE_FAULT handler, while still ensuring that
 * access remains non-blocking.
 * Note that because of platform-specific restrictions, only pointers
 * which are aligned by at least `PAGEDIR_MAPHINT_ALIGNMENT' bytes can
 * be specified as hints.
 *   - Use of this function requires a prior call to `pagedir_prepare_map()'!
 *   - Memory mappings defined in `vm_kernel' to be non-blocking have their
 *     mapping hint set to the address of the associated `struct vm_node' */
#ifndef ARCH_PAGEDIR_ARCHHEADER_DEFINES_PAGEDIR_MAPHINT
FUNDEF NOBLOCK void
NOTHROW(FCALL pagedir_maphintone)(PAGEDIR_PAGEALIGNED VIRT void *addr,
                                  VIRT /*ALIGNED(PAGEDIR_MAPHINT_ALIGNMENT)*/ void *hint);
#ifndef __OMIT_PAGING_CONSTANT_P_WRAPPERS
FUNDEF NOBLOCK void
NOTHROW(FCALL __os_pagedir_maphint)(PAGEDIR_PAGEALIGNED VIRT void *addr,
                                    PAGEDIR_PAGEALIGNED size_t num_bytes,
                                    VIRT /*ALIGNED(PAGEDIR_MAPHINT_ALIGNMENT)*/ void *hint)
		ASMNAME("pagedir_maphint");
FORCELOCAL NOBLOCK void
NOTHROW(FCALL pagedir_maphint)(PAGEDIR_PAGEALIGNED VIRT void *addr,
                               PAGEDIR_PAGEALIGNED size_t num_bytes,
                               VIRT /*ALIGNED(PAGEDIR_MAPHINT_ALIGNMENT)*/ void *hint) {
	if (__builtin_constant_p(num_bytes)) {
		if (num_bytes == 0)
			return;
		if (num_bytes <= PAGESIZE) {
			pagedir_maphintone(addr, hint);
			return;
		}
	}
	__os_pagedir_maphint(addr, num_bytes, hint);
}
#else /* !__OMIT_PAGING_CONSTANT_P_WRAPPERS */
FUNDEF NOBLOCK void
NOTHROW(FCALL pagedir_maphint)(PAGEDIR_PAGEALIGNED VIRT void *addr,
                               PAGEDIR_PAGEALIGNED size_t num_bytes,
                               VIRT /*ALIGNED(PAGEDIR_MAPHINT_ALIGNMENT)*/ void *hint);
#endif /* __OMIT_PAGING_CONSTANT_P_WRAPPERS */
#endif /* !ARCH_PAGEDIR_ARCHHEADER_DEFINES_PAGEDIR_MAPHINT */

/* Return the given of the given page, or NULL if no hint has been mapped. */
#ifndef ARCH_PAGEDIR_ARCHHEADER_DEFINES_PAGEDIR_GETHINT
FUNDEF NOBLOCK WUNUSED void *NOTHROW(FCALL pagedir_gethint)(VIRT void *addr);
#endif /* !ARCH_PAGEDIR_ARCHHEADER_DEFINES_PAGEDIR_GETHINT */

/* Create/delete a page-directory mapping.
 * @param: perm: A set of `PAGEDIR_MAP_F*' detailing how memory should be mapped.
 * `pagedir_sync()' must be called while specifying a virtual address range containing
 * `addr...+=num_bytes' in order to ensure that changes will become visible.
 * NOTE: This function can be called regardless of which page directory is active. */
#ifndef ARCH_PAGEDIR_ARCHHEADER_DEFINES_PAGEDIR_MAP
FUNDEF NOBLOCK void NOTHROW(FCALL pagedir_mapone)(PAGEDIR_PAGEALIGNED VIRT void *addr,
                                                  PAGEDIR_PAGEALIGNED PHYS vm_phys_t phys,
                                                  u16 perm);
#ifndef __OMIT_PAGING_CONSTANT_P_WRAPPERS
FUNDEF NOBLOCK void NOTHROW(FCALL __os_pagedir_map)(PAGEDIR_PAGEALIGNED VIRT void *addr,
                                                    PAGEDIR_PAGEALIGNED size_t num_bytes,
                                                    PAGEDIR_PAGEALIGNED PHYS vm_phys_t phys,
                                                    u16 perm)
		ASMNAME("pagedir_map");
FORCELOCAL NOBLOCK void
NOTHROW(FCALL pagedir_map)(PAGEDIR_PAGEALIGNED VIRT void *addr,
                           PAGEDIR_PAGEALIGNED size_t num_bytes,
                           PAGEDIR_PAGEALIGNED PHYS vm_phys_t phys,
                           u16 perm) {
	if (__builtin_constant_p(num_bytes)) {
		if (num_bytes == 0)
			return;
		if (num_bytes <= PAGESIZE) {
			pagedir_mapone(addr, phys, perm);
			return;
		}
	}
	__os_pagedir_map(addr, num_bytes, phys, perm);
}
#else /* !__OMIT_PAGING_CONSTANT_P_WRAPPERS */
FUNDEF NOBLOCK void
NOTHROW(FCALL pagedir_map)(PAGEDIR_PAGEALIGNED VIRT void *addr,
                           PAGEDIR_PAGEALIGNED size_t num_bytes,
                           PAGEDIR_PAGEALIGNED PHYS vm_phys_t phys,
                           u16 perm);
#endif /* __OMIT_PAGING_CONSTANT_P_WRAPPERS */
#endif /* !ARCH_PAGEDIR_ARCHHEADER_DEFINES_PAGEDIR_MAP */

/* Special variants of `pagedir_mapone()' that should be used to
 * temporary override the mapping of a single, prepared page.
 * These functions are mainly intended for use with `this_trampoline_page', allowing
 * each thread to push/pop its trampoline page, with doing so actually being an atomic
 * operation in the sense that the data is entirely thread-private, while modifications
 * do not require any kind of lock.
 * NOTE: If the page had been mapped, `pagedir_pop_mapone()' will automatically sync the page. */
#ifndef ARCH_PAGEDIR_ARCHHEADER_DEFINES_PAGEDIR_PUSH_MAPONE
FUNDEF NOBLOCK WUNUSED pagedir_pushval_t
NOTHROW(FCALL pagedir_push_mapone)(PAGEDIR_PAGEALIGNED VIRT void *addr,
                                   PAGEDIR_PAGEALIGNED PHYS vm_phys_t phys,
                                   u16 perm);
FUNDEF NOBLOCK void
NOTHROW(FCALL pagedir_pop_mapone)(PAGEDIR_PAGEALIGNED VIRT void *addr,
                                  pagedir_pushval_t backup);
#endif /* !ARCH_PAGEDIR_ARCHHEADER_DEFINES_PAGEDIR_PUSH_MAPONE */

/* Unmap pages from the given address range. (requires that the given area be prepared) */
#ifndef ARCH_PAGEDIR_ARCHHEADER_DEFINES_PAGEDIR_UNMAP
FUNDEF NOBLOCK void
NOTHROW(FCALL pagedir_unmapone)(PAGEDIR_PAGEALIGNED VIRT void *addr);
#ifndef __OMIT_PAGING_CONSTANT_P_WRAPPERS
FUNDEF NOBLOCK void
NOTHROW(FCALL __os_pagedir_unmap)(PAGEDIR_PAGEALIGNED VIRT void *addr,
                                  PAGEDIR_PAGEALIGNED size_t num_bytes)
		ASMNAME("pagedir_unmap");
FORCELOCAL NOBLOCK void
NOTHROW(FCALL pagedir_unmap)(PAGEDIR_PAGEALIGNED VIRT void *addr,
                             PAGEDIR_PAGEALIGNED size_t num_bytes) {
	if (__builtin_constant_p(num_bytes)) {
		if (num_bytes == 0)
			return;
		if (num_bytes <= PAGESIZE) {
			pagedir_unmapone(addr);
			return;
		}
	}
	__os_pagedir_unmap(addr, num_bytes);
}
#else /* !__OMIT_PAGING_CONSTANT_P_WRAPPERS */
FUNDEF NOBLOCK void
NOTHROW(FCALL pagedir_unmap)(PAGEDIR_PAGEALIGNED VIRT void *addr,
                             PAGEDIR_PAGEALIGNED size_t num_bytes);
#endif /* __OMIT_PAGING_CONSTANT_P_WRAPPERS */
#endif /* !ARCH_PAGEDIR_ARCHHEADER_DEFINES_PAGEDIR_UNMAP */


/* Remove write-permissions from the given address range. (requires that the given area be prepared) */
#ifdef ARCH_PAGEDIR_HAVE_UNWRITE
#ifndef ARCH_PAGEDIR_ARCHHEADER_DEFINES_PAGEDIR_UNWRITE
FUNDEF NOBLOCK void
NOTHROW(FCALL pagedir_unwriteone)(PAGEDIR_PAGEALIGNED VIRT void *addr);
#ifndef __OMIT_PAGING_CONSTANT_P_WRAPPERS
FUNDEF NOBLOCK void
NOTHROW(FCALL __os_pagedir_unwrite)(PAGEDIR_PAGEALIGNED VIRT void *addr,
                                    PAGEDIR_PAGEALIGNED size_t num_bytes)
		ASMNAME("pagedir_unwrite");
FORCELOCAL NOBLOCK void
NOTHROW(FCALL pagedir_unwrite)(PAGEDIR_PAGEALIGNED VIRT void *addr,
                               PAGEDIR_PAGEALIGNED size_t num_bytes) {
	if (__builtin_constant_p(num_bytes)) {
		if (num_bytes == 0)
			return;
		if (num_bytes <= PAGESIZE) {
			pagedir_unwriteone(addr);
			return;
		}
	}
	__os_pagedir_unwrite(addr, num_bytes);
}
#else /* !__OMIT_PAGING_CONSTANT_P_WRAPPERS */
FUNDEF NOBLOCK void
NOTHROW(FCALL pagedir_unwrite)(PAGEDIR_PAGEALIGNED VIRT void *addr,
                               PAGEDIR_PAGEALIGNED size_t num_bytes);
#endif /* __OMIT_PAGING_CONSTANT_P_WRAPPERS */
#endif /* !ARCH_PAGEDIR_ARCHHEADER_DEFINES_PAGEDIR_UNWRITE */
#endif /* ARCH_PAGEDIR_HAVE_UNWRITE */


#ifndef ARCH_PAGEDIR_ARCHHEADER_DEFINES_PAGEDIR_SYNC
#ifndef __OMIT_PAGING_CONSTANT_P_WRAPPERS
FUNDEF NOBLOCK void
NOTHROW(FCALL __os_pagedir_sync)(PAGEDIR_PAGEALIGNED VIRT void *addr,
                                 PAGEDIR_PAGEALIGNED size_t num_bytes)
		ASMNAME("pagedir_sync");
FORCELOCAL NOBLOCK void
NOTHROW(FCALL pagedir_sync)(PAGEDIR_PAGEALIGNED VIRT void *addr,
                            PAGEDIR_PAGEALIGNED size_t num_bytes) {
	if (__builtin_constant_p(num_bytes)) {
		if (num_bytes == 0)
			return;
		if (num_bytes <= PAGESIZE) {
			pagedir_syncone(addr);
			return;
		}
	}
	__os_pagedir_sync(addr, num_bytes);
}
#else
FUNDEF NOBLOCK void
NOTHROW(FCALL pagedir_sync)(PAGEDIR_PAGEALIGNED VIRT void *addr,
                            PAGEDIR_PAGEALIGNED size_t num_bytes);
#endif
#endif /* !ARCH_PAGEDIR_ARCHHEADER_DEFINES_PAGEDIR_SYNC */

/* Check if the given page is mapped.
 * NOTE: The given `addr' */
#ifndef ARCH_PAGEDIR_ARCHHEADER_DEFINES_PAGEDIR_ISMAPPED
FUNDEF NOBLOCK ATTR_PURE WUNUSED __BOOL NOTHROW(FCALL pagedir_ismapped)(VIRT void const *addr);
FUNDEF NOBLOCK ATTR_PURE WUNUSED __BOOL NOTHROW(FCALL pagedir_iswritable)(VIRT void *addr);
FUNDEF NOBLOCK ATTR_PURE WUNUSED __BOOL NOTHROW(FCALL pagedir_isuseraccessible)(VIRT void const *addr);
FUNDEF NOBLOCK ATTR_PURE WUNUSED __BOOL NOTHROW(FCALL pagedir_isuserwritable)(VIRT void *addr);
#endif /* !ARCH_PAGEDIR_ARCHHEADER_DEFINES_PAGEDIR_ISMAPPED */

#ifdef ARCH_PAGEDIR_HAVE_CHANGED
#ifndef ARCH_PAGEDIR_ARCHHEADER_DEFINES_PAGEDIR_HASCHANGED
/* TODO: Figure out a better design for these functions
 *       The current system is written under the assumption that 4MiB pages don't exist... */
FUNDEF NOBLOCK ATTR_PURE WUNUSED __BOOL NOTHROW(FCALL pagedir_haschanged)(VIRT void *addr);
FUNDEF NOBLOCK void NOTHROW(FCALL pagedir_unsetchanged)(VIRT void *addr);
#endif /* !ARCH_PAGEDIR_ARCHHEADER_DEFINES_PAGEDIR_HASCHANGED */
#endif /* ARCH_PAGEDIR_HAVE_CHANGED */


/* Same as the functions above, but operate on the specified
 * page directory, rather than the one currently being used.
 * NOTE: The caller is responsible for synchronization. */
FUNDEF NOBLOCK WUNUSED __BOOL NOTHROW(KCALL pagedir_prepare_mapone_p)(PAGEDIR_P_SELFTYPE self, PAGEDIR_PAGEALIGNED VIRT void *addr);
FUNDEF NOBLOCK void NOTHROW(KCALL pagedir_unprepare_mapone_p)(PAGEDIR_P_SELFTYPE self, PAGEDIR_PAGEALIGNED VIRT void *addr);
FUNDEF NOBLOCK void NOTHROW(KCALL pagedir_maphintone_p)(PAGEDIR_P_SELFTYPE self, PAGEDIR_PAGEALIGNED VIRT void *addr, VIRT /*ALIGNED(PAGEDIR_MAPHINT_ALIGNMENT)*/ void *hint);
FUNDEF NOBLOCK void NOTHROW(KCALL pagedir_mapone_p)(PAGEDIR_P_SELFTYPE self, PAGEDIR_PAGEALIGNED VIRT void *addr, PAGEDIR_PAGEALIGNED PHYS vm_phys_t phys, u16 perm);
FUNDEF NOBLOCK void NOTHROW(KCALL pagedir_unmapone_p)(PAGEDIR_P_SELFTYPE self, PAGEDIR_PAGEALIGNED VIRT void *addr);
#ifdef ARCH_PAGEDIR_HAVE_UNWRITE
FUNDEF NOBLOCK void NOTHROW(KCALL pagedir_unwriteone_p)(PAGEDIR_P_SELFTYPE self, PAGEDIR_PAGEALIGNED VIRT void *addr);
#endif /* ARCH_PAGEDIR_HAVE_UNWRITE */
FUNDEF NOBLOCK ATTR_PURE WUNUSED __BOOL NOTHROW(KCALL pagedir_ismapped_p)(PAGEDIR_P_SELFTYPE self, VIRT void const *addr);
FUNDEF NOBLOCK ATTR_PURE WUNUSED __BOOL NOTHROW(KCALL pagedir_iswritable_p)(PAGEDIR_P_SELFTYPE self, VIRT void *addr);
FUNDEF NOBLOCK ATTR_PURE WUNUSED __BOOL NOTHROW(KCALL pagedir_isuseraccessible_p)(PAGEDIR_P_SELFTYPE self, VIRT void const *addr);
FUNDEF NOBLOCK ATTR_PURE WUNUSED __BOOL NOTHROW(KCALL pagedir_isuserwritable_p)(PAGEDIR_P_SELFTYPE self, VIRT void *addr);
#ifdef ARCH_PAGEDIR_HAVE_CHANGED
FUNDEF NOBLOCK ATTR_PURE WUNUSED __BOOL NOTHROW(KCALL pagedir_haschanged_p)(PAGEDIR_P_SELFTYPE self, VIRT void *addr);
FUNDEF NOBLOCK void NOTHROW(KCALL pagedir_unsetchanged_p)(PAGEDIR_P_SELFTYPE self, VIRT void *addr);
#endif /* ARCH_PAGEDIR_HAVE_CHANGED */
#ifndef __OMIT_PAGING_CONSTANT_P_WRAPPERS
FUNDEF NOBLOCK WUNUSED __BOOL
NOTHROW(KCALL __os_pagedir_prepare_map_p)(PAGEDIR_P_SELFTYPE self,
                                          PAGEDIR_PAGEALIGNED VIRT void *addr,
                                          PAGEDIR_PAGEALIGNED size_t num_bytes)
		ASMNAME("pagedir_prepare_map_p");
FUNDEF NOBLOCK WUNUSED __BOOL
NOTHROW(KCALL __os_pagedir_prepare_map_keep_p)(PAGEDIR_P_SELFTYPE self,
                                               PAGEDIR_PAGEALIGNED VIRT void *addr,
                                               PAGEDIR_PAGEALIGNED size_t num_bytes)
		ASMNAME("pagedir_prepare_map_keep_p");
FUNDEF NOBLOCK void
NOTHROW(KCALL __os_pagedir_unprepare_map_p)(PAGEDIR_P_SELFTYPE self,
                                            PAGEDIR_PAGEALIGNED VIRT void *addr,
                                            PAGEDIR_PAGEALIGNED size_t num_bytes)
		ASMNAME("pagedir_unprepare_map_p");
FUNDEF NOBLOCK void
NOTHROW(KCALL __os_pagedir_maphint_p)(PAGEDIR_P_SELFTYPE self,
                                      PAGEDIR_PAGEALIGNED VIRT void *addr,
                                      PAGEDIR_PAGEALIGNED size_t num_bytes,
                                      VIRT /*ALIGNED(PAGEDIR_MAPHINT_ALIGNMENT)*/ void *hint)
		ASMNAME("pagedir_maphint_p");
FUNDEF NOBLOCK void
NOTHROW(KCALL __os_pagedir_map_p)(PAGEDIR_P_SELFTYPE self,
                                  PAGEDIR_PAGEALIGNED VIRT void *addr,
                                  PAGEDIR_PAGEALIGNED size_t num_bytes,
                                  PAGEDIR_PAGEALIGNED PHYS vm_phys_t phys,
                                  u16 perm)
		ASMNAME("pagedir_map_p");
FUNDEF NOBLOCK void
NOTHROW(KCALL __os_pagedir_unmap_p)(PAGEDIR_P_SELFTYPE self,
                                    PAGEDIR_PAGEALIGNED VIRT void *addr,
                                    PAGEDIR_PAGEALIGNED size_t num_bytes)
		ASMNAME("pagedir_unmap_p");
#ifdef ARCH_PAGEDIR_HAVE_UNWRITE
FUNDEF NOBLOCK void
NOTHROW(KCALL __os_pagedir_unwrite_p)(PAGEDIR_P_SELFTYPE self,
                                      PAGEDIR_PAGEALIGNED VIRT void *addr,
                                      PAGEDIR_PAGEALIGNED size_t num_bytes)
		ASMNAME("pagedir_unwrite_p");
#endif /* ARCH_PAGEDIR_HAVE_UNWRITE */
FORCELOCAL NOBLOCK WUNUSED __BOOL
NOTHROW(KCALL pagedir_prepare_map_p)(PAGEDIR_P_SELFTYPE self,
                                     PAGEDIR_PAGEALIGNED VIRT void *addr,
                                     PAGEDIR_PAGEALIGNED size_t num_bytes) {
	if (__builtin_constant_p(num_bytes)) {
		if (num_bytes == 0)
			return true;
		if (num_bytes <= PAGESIZE)
			return pagedir_prepare_mapone_p(self, addr);
	}
	return __os_pagedir_prepare_map_p(self, addr, num_bytes);
}
FORCELOCAL NOBLOCK WUNUSED __BOOL
NOTHROW(KCALL pagedir_prepare_map_keep_p)(PAGEDIR_P_SELFTYPE self,
                                          PAGEDIR_PAGEALIGNED VIRT void *addr,
                                          PAGEDIR_PAGEALIGNED size_t num_bytes) {
	if (__builtin_constant_p(num_bytes)) {
		if (num_bytes == 0)
			return true;
		if (num_bytes <= PAGESIZE)
			return pagedir_prepare_mapone_p(self, addr);
	}
	return __os_pagedir_prepare_map_keep_p(self, addr, num_bytes);
}
FORCELOCAL NOBLOCK void
NOTHROW(KCALL pagedir_unprepare_map_p)(PAGEDIR_P_SELFTYPE self,
                                       PAGEDIR_PAGEALIGNED VIRT void *addr,
                                       PAGEDIR_PAGEALIGNED size_t num_bytes) {
	if (__builtin_constant_p(num_bytes)) {
		if (num_bytes == 0)
			return;
		if (num_bytes <= PAGESIZE) {
			pagedir_unprepare_mapone_p(self, addr);
			return;
		}
	}
	__os_pagedir_unprepare_map_p(self, addr, num_bytes);
}
FORCELOCAL NOBLOCK void
NOTHROW(KCALL pagedir_maphint_p)(PAGEDIR_P_SELFTYPE self,
                                 PAGEDIR_PAGEALIGNED VIRT void *addr,
                                 PAGEDIR_PAGEALIGNED size_t num_bytes,
                                 VIRT /*ALIGNED(PAGEDIR_MAPHINT_ALIGNMENT)*/ void *hint) {
	if (__builtin_constant_p(num_bytes)) {
		if (num_bytes == 0)
			return;
		if (num_bytes <= PAGESIZE) {
			pagedir_maphintone_p(self, addr, hint);
			return;
		}
	}
	__os_pagedir_maphint_p(self, addr, num_bytes, hint);
}
FORCELOCAL NOBLOCK void
NOTHROW(KCALL pagedir_map_p)(PAGEDIR_P_SELFTYPE self,
                             PAGEDIR_PAGEALIGNED VIRT void *addr,
                             PAGEDIR_PAGEALIGNED size_t num_bytes,
                             PAGEDIR_PAGEALIGNED PHYS vm_phys_t phys,
                             u16 perm) {
	if (__builtin_constant_p(num_bytes)) {
		if (num_bytes == 0)
			return;
		if (num_bytes <= PAGESIZE) {
			pagedir_mapone_p(self, addr, phys, perm);
			return;
		}
	}
	__os_pagedir_map_p(self, addr, num_bytes, phys, perm);
}
FORCELOCAL NOBLOCK void
NOTHROW(KCALL pagedir_unmap_p)(PAGEDIR_P_SELFTYPE self,
                               PAGEDIR_PAGEALIGNED VIRT void *addr,
                               PAGEDIR_PAGEALIGNED size_t num_bytes) {
	if (__builtin_constant_p(num_bytes)) {
		if (num_bytes == 0)
			return;
		if (num_bytes <= PAGESIZE) {
			pagedir_unmapone_p(self, addr);
			return;
		}
	}
	__os_pagedir_unmap_p(self, addr, num_bytes);
}
#ifdef ARCH_PAGEDIR_HAVE_UNWRITE
FORCELOCAL NOBLOCK void
NOTHROW(KCALL pagedir_unwrite_p)(PAGEDIR_P_SELFTYPE self,
                                 PAGEDIR_PAGEALIGNED VIRT void *addr,
                                 PAGEDIR_PAGEALIGNED size_t num_bytes) {
	if (__builtin_constant_p(num_bytes)) {
		if (num_bytes == 0)
			return;
		if (num_bytes <= PAGESIZE) {
			pagedir_unwriteone_p(self, addr);
			return;
		}
	}
	__os_pagedir_unwrite_p(self, addr, num_bytes);
}
#endif /* ARCH_PAGEDIR_HAVE_UNWRITE */
#else /* !__OMIT_PAGING_CONSTANT_P_WRAPPERS */
FUNDEF NOBLOCK WUNUSED __BOOL
NOTHROW(KCALL pagedir_prepare_map_p)(PAGEDIR_P_SELFTYPE self,
                                     PAGEDIR_PAGEALIGNED VIRT void *addr,
                                     PAGEDIR_PAGEALIGNED size_t num_bytes);
FUNDEF NOBLOCK WUNUSED __BOOL
NOTHROW(KCALL pagedir_prepare_map_keep_p)(PAGEDIR_P_SELFTYPE self,
                                          PAGEDIR_PAGEALIGNED VIRT void *addr,
                                          PAGEDIR_PAGEALIGNED size_t num_bytes);
FUNDEF NOBLOCK void
NOTHROW(KCALL pagedir_unprepare_map_p)(PAGEDIR_P_SELFTYPE self,
                                       PAGEDIR_PAGEALIGNED VIRT void *addr,
                                       PAGEDIR_PAGEALIGNED size_t num_bytes);
FUNDEF NOBLOCK void
NOTHROW(KCALL pagedir_maphint_p)(PAGEDIR_P_SELFTYPE self,
                                 PAGEDIR_PAGEALIGNED VIRT void *addr,
                                 PAGEDIR_PAGEALIGNED size_t num_bytes,
                                 VIRT /*ALIGNED(PAGEDIR_MAPHINT_ALIGNMENT)*/ void *hint);
FUNDEF NOBLOCK void
NOTHROW(KCALL pagedir_map_p)(PAGEDIR_P_SELFTYPE self,
                             PAGEDIR_PAGEALIGNED VIRT void *addr,
                             PAGEDIR_PAGEALIGNED size_t num_bytes,
                             PAGEDIR_PAGEALIGNED PHYS vm_phys_t phys,
                             u16 perm);
FUNDEF NOBLOCK void
NOTHROW(KCALL pagedir_unmap_p)(PAGEDIR_P_SELFTYPE self,
                               PAGEDIR_PAGEALIGNED VIRT void *addr,
                               PAGEDIR_PAGEALIGNED size_t num_bytes);
#ifdef ARCH_PAGEDIR_HAVE_UNWRITE
FUNDEF NOBLOCK void
NOTHROW(KCALL pagedir_unwrite_p)(PAGEDIR_P_SELFTYPE self,
                                 PAGEDIR_PAGEALIGNED VIRT void *addr,
                                 PAGEDIR_PAGEALIGNED size_t num_bytes);
#endif /* ARCH_PAGEDIR_HAVE_UNWRITE */
#endif /* __OMIT_PAGING_CONSTANT_P_WRAPPERS */

#endif /* __CC__ */


DECL_END

#endif /* !GUARD_KERNEL_INCLUDE_I386_KOS_KERNEL_PAGING_H */
