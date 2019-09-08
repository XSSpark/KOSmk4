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
#ifndef GUARD_LIBDLMALLOC_CONFIG_H
#define GUARD_LIBDLMALLOC_CONFIG_H 1
#define _GNU_SOURCE 1
#define _KOS_SOURCE 1

/* Configure dlmalloc.c */
#undef WIN32
#undef _WIN32
#undef _WIN32_WCE
#define DLMALLOC_EXPORT INTDEF
#define MALLOC_ALIGNMENT __LIBC_MALLOC_ALIGNMENT
#undef MSPACES
#undef ONLY_MSPACES
#define USE_LOCKS 1
#define USE_SPIN_LOCKS 1
#undef USE_RECURSIVE_LOCKS
#undef LOCK_AT_FORK
#undef FOOTERS
#ifdef NDEBUG
#define INSECURE 1
#else
#undef INSECURE
#endif
#define USE_DL_PREFIX 1
#undef MALLOC_INSPECT_ALL
#undef ABORT
#define MALLOC_FAILURE_ACTION (void)0; /* nothing... */
#define HAVE_MORECORE 0
#undef MORECORE
#undef MORECORE_CONTIGUOUS
#undef MORECORE_CANNOT_TRIM
#undef NO_SEGMENT_TRAVERSAL
#define HAVE_MMAP 1
#define HAVE_MREMAP 0 /* TODO */
#define MMAP_CLEARS 1
#define USE_BUILTIN_FFS 1
#define malloc_getpagesize PAGESIZE
#define USE_DEV_RANDOM 0
#define NO_MALLINFO 0
#define MALLINFO_FIELD_TYPE size_t
#define NO_MALLOC_STATS 1 /* Disable to prevent relocation against `stderr' */
#define ABORT  assert(0)
#undef REALLOC_ZERO_BYTES_FREES
#undef DEFAULT_GRANULARITY
#undef DEFAULT_TRIM_THRESHOLD
#undef DEFAULT_MMAP_THRESHOLD
#undef MAX_RELEASE_CHECK_RATE

/* We've got all of these! */
#undef LACKS_UNISTD_H
#undef LACKS_FCNTL_H
#undef LACKS_SYS_PARAM_H
#undef LACKS_SYS_MMAN_H
#undef LACKS_STRINGS_H
#undef LACKS_STRING_H
#undef LACKS_SYS_TYPES_H
#undef LACKS_ERRNO_H
#undef LACKS_STDLIB_H
#undef LACKS_SCHED_H
#undef LACKS_TIME_H

#define FORCEINLINE   FORCELOCAL
#define NOINLINE      ATTR_NOINLINE



/* Configure for a free-standing environment in
 * which system calls are directly invoked. */
#ifdef CONFIG_DLMALLOC_FREESTANDING_SYSTEM_CALLS
#undef __CRT_FREESTANDING
#undef __WANT_INLINE_SYSCALLS
#define __CRT_FREESTANDING     1
#define __WANT_INLINE_SYSCALLS 1
#include <kos/syscalls.h>
#if __CRT_HAVE_SC(time)
#define time(p)    sys_time(p)
#else /* time */
#define time(p)  ((p)?(*(p)=42):42)
#endif /* !time */
#if __CRT_HAVE_SC(sched_yield)
#define sched_yield()  sys_sched_yield()
#else /* sched_yield */
#define sched_yield() (void)0
#endif /* !sched_yield */
#define mmap(hint,size,prot,flags,fd,offset)  sys_mmap(hint,size,prot,flags,fd,offset)
#define munmap(addr,size)                     sys_munmap(addr,size)
#if HAVE_MREMAP
#if __CRT_HAVE_SC(mremap)
#define mremap(addr,osz,nsz,mv)  sys_mremap(addr,osz,nsz,mv)
#else /* mremap */
#undef HAVE_MREMAP
#define HAVE_MREMAP 0
#endif /* !mremap */
#endif /* HAVE_MREMAP */
#endif /* CONFIG_DLMALLOC_FREESTANDING_SYSTEM_CALLS */

#include <hybrid/compiler.h>
#include <hybrid/typecore.h>
#include <hybrid/limits.h>
#include <dlfcn.h>
#include <__crt.h>
#include <string.h>
#include <assert.h>


#if MALLOC_ALIGNMENT < 8
#undef MALLOC_ALIGNMENT
#define MALLOC_ALIGNMENT 8
#endif


DECL_BEGIN

/* Declare exported functions within individual sections. */

DLMALLOC_EXPORT ATTR_SECTION(".text.crt.dlmalloc") void *dlmalloc(size_t);
DLMALLOC_EXPORT ATTR_SECTION(".text.crt.dlfree") void dlfree(void *);
DLMALLOC_EXPORT ATTR_SECTION(".text.crt.dlcalloc") void *dlcalloc(size_t, size_t);
DLMALLOC_EXPORT ATTR_SECTION(".text.crt.dlrealloc") void *dlrealloc(void *, size_t);
DLMALLOC_EXPORT ATTR_SECTION(".text.crt.dlrealloc_in_place") void *dlrealloc_in_place(void *, size_t);
DLMALLOC_EXPORT ATTR_SECTION(".text.crt.dlmemalign") void *dlmemalign(size_t, size_t);
DLMALLOC_EXPORT ATTR_SECTION(".text.crt.dlposix_memalign") int dlposix_memalign(void **, size_t, size_t);
DLMALLOC_EXPORT ATTR_SECTION(".text.crt.dlvalloc") void *dlvalloc(size_t);
DLMALLOC_EXPORT ATTR_SECTION(".text.crt.dlmallopt") int dlmallopt(int, int);
DLMALLOC_EXPORT ATTR_SECTION(".text.crt.dlmalloc_footprint") size_t dlmalloc_footprint(void);
DLMALLOC_EXPORT ATTR_SECTION(".text.crt.dlmalloc_max_footprint") size_t dlmalloc_max_footprint(void);
DLMALLOC_EXPORT ATTR_SECTION(".text.crt.dlmalloc_footprint_limit") size_t dlmalloc_footprint_limit(void);
DLMALLOC_EXPORT ATTR_SECTION(".text.crt.dlmalloc_set_footprint_limit") size_t dlmalloc_set_footprint_limit(size_t bytes);
DLMALLOC_EXPORT ATTR_SECTION(".text.crt.dlmallinfo") struct mallinfo dlmallinfo(void);
DLMALLOC_EXPORT ATTR_SECTION(".text.crt.dlindependent_calloc") void **dlindependent_calloc(size_t, size_t, void **);
DLMALLOC_EXPORT ATTR_SECTION(".text.crt.dlindependent_comalloc") void **dlindependent_comalloc(size_t, size_t *, void **);
DLMALLOC_EXPORT ATTR_SECTION(".text.crt.dlbulk_free") size_t dlbulk_free(void **, size_t n_elements);
DLMALLOC_EXPORT ATTR_SECTION(".text.crt.dlpvalloc") void *dlpvalloc(size_t);
DLMALLOC_EXPORT ATTR_SECTION(".text.crt.dlmalloc_trim") int dlmalloc_trim(size_t);
DLMALLOC_EXPORT ATTR_SECTION(".text.crt.dlmalloc_usable_size") size_t dlmalloc_usable_size(void *);

#ifndef __NO_DL_REGISTER_CACHE
DL_REGISTER_CACHE(dl_clear_caches) {
	return dlmalloc_trim(0);
}
#endif /* !__NO_DL_REGISTER_CACHE */

/* Create public exports (without weak linkage, thus overruling `libc's) */
#ifndef DEFINE_DL_EXPORT_ALIAS
#ifdef CONFIG_DLMALLOC_EXPORT_AS_INTERN
#define DEFINE_DL_EXPORT_ALIAS  DEFINE_INTERN_ALIAS
#else
#define DEFINE_DL_EXPORT_ALIAS  DEFINE_PUBLIC_ALIAS
#endif
#endif /* !DEFINE_DL_EXPORT_ALIAS */

DEFINE_DL_EXPORT_ALIAS(malloc, dlmalloc);
DEFINE_DL_EXPORT_ALIAS(free, dlfree);
DEFINE_DL_EXPORT_ALIAS(cfree, dlfree);
DEFINE_DL_EXPORT_ALIAS(calloc, dlcalloc);
DEFINE_DL_EXPORT_ALIAS(realloc, dlrealloc);
DEFINE_DL_EXPORT_ALIAS(realloc_in_place, dlrealloc_in_place);
DEFINE_DL_EXPORT_ALIAS(memalign, dlmemalign);
DEFINE_DL_EXPORT_ALIAS(aligned_alloc, dlmemalign);
DEFINE_DL_EXPORT_ALIAS(posix_memalign, dlposix_memalign);
DEFINE_DL_EXPORT_ALIAS(valloc, dlvalloc);
DEFINE_DL_EXPORT_ALIAS(mallopt, dlmallopt);
DEFINE_DL_EXPORT_ALIAS(malloc_footprint, dlmalloc_footprint);
DEFINE_DL_EXPORT_ALIAS(malloc_max_footprint, dlmalloc_max_footprint);
DEFINE_DL_EXPORT_ALIAS(malloc_footprint_limit, dlmalloc_footprint_limit);
DEFINE_DL_EXPORT_ALIAS(malloc_set_footprint_limit, dlmalloc_set_footprint_limit);
DEFINE_DL_EXPORT_ALIAS(mallinfo, dlmallinfo);
DEFINE_DL_EXPORT_ALIAS(independent_calloc, dlindependent_calloc);
DEFINE_DL_EXPORT_ALIAS(independent_comalloc, dlindependent_comalloc);
DEFINE_DL_EXPORT_ALIAS(bulk_free, dlbulk_free);
DEFINE_DL_EXPORT_ALIAS(pvalloc, dlpvalloc);
DEFINE_DL_EXPORT_ALIAS(malloc_trim, dlmalloc_trim);
DEFINE_DL_EXPORT_ALIAS(malloc_usable_size, dlmalloc_usable_size);
DEFINE_DL_EXPORT_ALIAS(_msize, dlmalloc_usable_size);
#undef DEFINE_DL_EXPORT_ALIAS

/* Also create libc-overrides exports
 * -> This allows libc internals to use our malloc functions
 *    if libc happens to be combined with our library within
 *    a static link (such as is the case when building the
 *    dynamic linker binary blobs) */
DEFINE_INTERN_ALIAS(libc_malloc, dlmalloc);
DEFINE_INTERN_ALIAS(libc_free, dlfree);
DEFINE_INTERN_ALIAS(libc_cfree, dlfree);
DEFINE_INTERN_ALIAS(libc_calloc, dlcalloc);
DEFINE_INTERN_ALIAS(libc_realloc, dlrealloc);
DEFINE_INTERN_ALIAS(libc_realloc_in_place, dlrealloc_in_place);
DEFINE_INTERN_ALIAS(libc_memalign, dlmemalign);
DEFINE_INTERN_ALIAS(libc_posix_memalign, dlposix_memalign);
DEFINE_INTERN_ALIAS(libc_valloc, dlvalloc);
DEFINE_INTERN_ALIAS(libc_mallopt, dlmallopt);
DEFINE_INTERN_ALIAS(libc_malloc_footprint, dlmalloc_footprint);
DEFINE_INTERN_ALIAS(libc_malloc_max_footprint, dlmalloc_max_footprint);
DEFINE_INTERN_ALIAS(libc_malloc_footprint_limit, dlmalloc_footprint_limit);
DEFINE_INTERN_ALIAS(libc_malloc_set_footprint_limit, dlmalloc_set_footprint_limit);
DEFINE_INTERN_ALIAS(libc_mallinfo, dlmallinfo);
DEFINE_INTERN_ALIAS(libc_independent_calloc, dlindependent_calloc);
DEFINE_INTERN_ALIAS(libc_independent_comalloc, dlindependent_comalloc);
DEFINE_INTERN_ALIAS(libc_bulk_free, dlbulk_free);
DEFINE_INTERN_ALIAS(libc_pvalloc, dlpvalloc);
DEFINE_INTERN_ALIAS(libc_malloc_trim, dlmalloc_trim);
DEFINE_INTERN_ALIAS(libc_malloc_usable_size, dlmalloc_usable_size);

DECL_END


#endif /* !GUARD_LIBDLMALLOC_CONFIG_H */
