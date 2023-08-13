/* HASH CRC-32:0x9884714b */
/* Copyright (c) 2019-2023 Griefer@Work                                       *
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
 *    Portions Copyright (c) 2019-2023 Griefer@Work                           *
 * 2. Altered source versions must be plainly marked as such, and must not be *
 *    misrepresented as being the original software.                          *
 * 3. This notice may not be removed or altered from any source distribution. *
 */
#ifndef __local_ttyname_defined
#define __local_ttyname_defined
#include <__crt.h>
#include <paths.h>
#include <asm/os/fcntl.h>
#include <bits/os/dirent.h>
#include <features.h>
#include <asm/os/oflags.h>
#include <bits/os/stat.h>
#if defined(__CRT_HAVE_ttyname_r) || defined(__CRT_HAVE_frealpath4) || ((defined(__CRT_HAVE_opendir) || defined(__CRT_HAVE___libc_opendir) || (defined(__AT_FDCWD) && (defined(__CRT_HAVE_opendirat) || defined(__CRT_HAVE_fopendirat) || (defined(__CRT_HAVE_fdopendir) && (defined(__CRT_HAVE_openat64) || defined(__CRT_HAVE_openat))))) || (defined(__CRT_HAVE_fdopendir) && (defined(__CRT_HAVE_open64) || defined(__CRT_HAVE___open64) || defined(__CRT_HAVE_open) || defined(__CRT_HAVE__open) || defined(__CRT_HAVE___open) || defined(__CRT_HAVE___libc_open) || (defined(__AT_FDCWD) && (defined(__CRT_HAVE_openat64) || defined(__CRT_HAVE_openat))))) || defined(__USE_DOS_DIRENT)) && ((defined(__CRT_HAVE_readdirk) && defined(__CRT_KOS) && defined(_DIRENT_MATCHES_DIRENT64)) || (defined(__CRT_HAVE_readdirk64) && defined(__CRT_KOS)) || (defined(__CRT_HAVE_readdir) && !defined(__CRT_KOS) && defined(_DIRENT_MATCHES_DIRENT64)) || (defined(__CRT_HAVE___libc_readdir) && !defined(__CRT_KOS) && defined(_DIRENT_MATCHES_DIRENT64)) || (defined(__CRT_HAVE_readdir64) && !defined(__CRT_KOS)) || (defined(__USE_DOS_DIRENT) && defined(_DIRENT_MATCHES_DIRENT64))) && ((defined(__CRT_HAVE_kfstat) && defined(__CRT_KOS_PRIMARY)) || (defined(__CRT_HAVE_kfstat64) && defined(__CRT_KOS_PRIMARY)) || (defined(__CRT_HAVE__fstat64) && defined(__CRT_DOS_PRIMARY) && defined(__USE_TIME_BITS64)) || (defined(__CRT_HAVE__fstat64i32) && defined(__CRT_DOS_PRIMARY) && defined(__USE_TIME_BITS64)) || (defined(__CRT_HAVE__fstati64) && defined(__CRT_DOS_PRIMARY) && !defined(__USE_TIME_BITS64)) || (defined(__CRT_HAVE__fstat32i64) && defined(__CRT_DOS_PRIMARY) && !defined(__USE_TIME_BITS64)) || (defined(__CRT_HAVE_fstat) && defined(__STAT32_MATCHES_STAT64)) || defined(__CRT_HAVE_fstat64)) && ((defined(__CRT_HAVE_klstat) && defined(__CRT_KOS_PRIMARY)) || (defined(__CRT_HAVE_klstat64) && defined(__CRT_KOS_PRIMARY)) || (defined(__CRT_HAVE__stat64) && defined(__CRT_DOS_PRIMARY) && defined(__USE_TIME_BITS64)) || (defined(__CRT_HAVE__stat64i32) && defined(__CRT_DOS_PRIMARY) && defined(__USE_TIME_BITS64)) || (defined(__CRT_HAVE__stati64) && defined(__CRT_DOS_PRIMARY) && !defined(__USE_TIME_BITS64)) || (defined(__CRT_HAVE__stat32i64) && defined(__CRT_DOS_PRIMARY) && !defined(__USE_TIME_BITS64)) || (defined(__CRT_HAVE_lstat) && defined(__STAT32_MATCHES_STAT64)) || defined(__CRT_HAVE_lstat64)) && defined(_PATH_DEV))
#include <bits/types.h>
__NAMESPACE_LOCAL_BEGIN
#ifndef __local___localdep_free_defined
#define __local___localdep_free_defined
#if __has_builtin(__builtin_free) && defined(__LIBC_BIND_CRTBUILTINS) && defined(__CRT_HAVE_free)
__CEIREDIRECT(,void,__NOTHROW_NCX,__localdep_free,(void *__mallptr),free,{ __builtin_free(__mallptr); })
#elif defined(__CRT_HAVE_free)
__CREDIRECT_VOID(,__NOTHROW_NCX,__localdep_free,(void *__mallptr),free,(__mallptr))
#elif defined(__CRT_HAVE_cfree)
__CREDIRECT_VOID(,__NOTHROW_NCX,__localdep_free,(void *__mallptr),cfree,(__mallptr))
#elif defined(__CRT_HAVE___libc_free)
__CREDIRECT_VOID(,__NOTHROW_NCX,__localdep_free,(void *__mallptr),__libc_free,(__mallptr))
#else /* ... */
#undef __local___localdep_free_defined
#endif /* !... */
#endif /* !__local___localdep_free_defined */
#ifndef __local___localdep_malloc_usable_size_defined
#define __local___localdep_malloc_usable_size_defined
#ifdef __CRT_HAVE_malloc_usable_size
__NAMESPACE_LOCAL_END
#include <hybrid/typecore.h>
__NAMESPACE_LOCAL_BEGIN
__CREDIRECT(__ATTR_PURE __ATTR_WUNUSED,__SIZE_TYPE__,__NOTHROW_NCX,__localdep_malloc_usable_size,(void *__restrict __mallptr),malloc_usable_size,(__mallptr))
#elif defined(__CRT_HAVE__msize)
__NAMESPACE_LOCAL_END
#include <hybrid/typecore.h>
__NAMESPACE_LOCAL_BEGIN
__CREDIRECT(__ATTR_PURE __ATTR_WUNUSED,__SIZE_TYPE__,__NOTHROW_NCX,__localdep_malloc_usable_size,(void *__restrict __mallptr),_msize,(__mallptr))
#else /* ... */
#undef __local___localdep_malloc_usable_size_defined
#endif /* !... */
#endif /* !__local___localdep_malloc_usable_size_defined */
#ifndef __local___localdep_realloc_defined
#define __local___localdep_realloc_defined
#if __has_builtin(__builtin_realloc) && defined(__LIBC_BIND_CRTBUILTINS) && defined(__CRT_HAVE_realloc)
__NAMESPACE_LOCAL_END
#include <hybrid/typecore.h>
__NAMESPACE_LOCAL_BEGIN
__CEIREDIRECT(__ATTR_MALL_DEFAULT_ALIGNED __ATTR_WUNUSED __ATTR_ALLOC_SIZE((2)),void *,__NOTHROW_NCX,__localdep_realloc,(void *__mallptr, __SIZE_TYPE__ __num_bytes),realloc,{ return __builtin_realloc(__mallptr, __num_bytes); })
#elif defined(__CRT_HAVE_realloc)
__NAMESPACE_LOCAL_END
#include <hybrid/typecore.h>
__NAMESPACE_LOCAL_BEGIN
__CREDIRECT(__ATTR_MALL_DEFAULT_ALIGNED __ATTR_WUNUSED __ATTR_ALLOC_SIZE((2)),void *,__NOTHROW_NCX,__localdep_realloc,(void *__mallptr, __SIZE_TYPE__ __num_bytes),realloc,(__mallptr,__num_bytes))
#elif defined(__CRT_HAVE___libc_realloc)
__NAMESPACE_LOCAL_END
#include <hybrid/typecore.h>
__NAMESPACE_LOCAL_BEGIN
__CREDIRECT(__ATTR_MALL_DEFAULT_ALIGNED __ATTR_WUNUSED __ATTR_ALLOC_SIZE((2)),void *,__NOTHROW_NCX,__localdep_realloc,(void *__mallptr, __SIZE_TYPE__ __num_bytes),__libc_realloc,(__mallptr,__num_bytes))
#else /* ... */
#undef __local___localdep_realloc_defined
#endif /* !... */
#endif /* !__local___localdep_realloc_defined */
#ifndef __local___localdep_strlen_defined
#define __local___localdep_strlen_defined
#ifdef __CRT_HAVE_strlen
__NAMESPACE_LOCAL_END
#include <hybrid/typecore.h>
__NAMESPACE_LOCAL_BEGIN
__CREDIRECT(__ATTR_PURE __ATTR_WUNUSED __ATTR_IN(1),__SIZE_TYPE__,__NOTHROW_NCX,__localdep_strlen,(char const *__restrict __str),strlen,(__str))
#else /* __CRT_HAVE_strlen */
__NAMESPACE_LOCAL_END
#include <libc/local/string/strlen.h>
__NAMESPACE_LOCAL_BEGIN
#define __localdep_strlen __LIBC_LOCAL_NAME(strlen)
#endif /* !__CRT_HAVE_strlen */
#endif /* !__local___localdep_strlen_defined */
#ifndef __local___localdep_ttyname_r_defined
#define __local___localdep_ttyname_r_defined
#ifdef __CRT_HAVE_ttyname_r
__CREDIRECT(__ATTR_FDARG(1) __ATTR_OUTS(2, 3),__errno_t,__NOTHROW_RPC,__localdep_ttyname_r,(__fd_t __fd, char *__buf, __SIZE_TYPE__ __buflen),ttyname_r,(__fd,__buf,__buflen))
#elif defined(__CRT_HAVE_frealpath4) || ((defined(__CRT_HAVE_opendir) || defined(__CRT_HAVE___libc_opendir) || (defined(__AT_FDCWD) && (defined(__CRT_HAVE_opendirat) || defined(__CRT_HAVE_fopendirat) || (defined(__CRT_HAVE_fdopendir) && (defined(__CRT_HAVE_openat64) || defined(__CRT_HAVE_openat))))) || (defined(__CRT_HAVE_fdopendir) && (defined(__CRT_HAVE_open64) || defined(__CRT_HAVE___open64) || defined(__CRT_HAVE_open) || defined(__CRT_HAVE__open) || defined(__CRT_HAVE___open) || defined(__CRT_HAVE___libc_open) || (defined(__AT_FDCWD) && (defined(__CRT_HAVE_openat64) || defined(__CRT_HAVE_openat))))) || defined(__USE_DOS_DIRENT)) && ((defined(__CRT_HAVE_readdirk) && defined(__CRT_KOS) && defined(_DIRENT_MATCHES_DIRENT64)) || (defined(__CRT_HAVE_readdirk64) && defined(__CRT_KOS)) || (defined(__CRT_HAVE_readdir) && !defined(__CRT_KOS) && defined(_DIRENT_MATCHES_DIRENT64)) || (defined(__CRT_HAVE___libc_readdir) && !defined(__CRT_KOS) && defined(_DIRENT_MATCHES_DIRENT64)) || (defined(__CRT_HAVE_readdir64) && !defined(__CRT_KOS)) || (defined(__USE_DOS_DIRENT) && defined(_DIRENT_MATCHES_DIRENT64))) && ((defined(__CRT_HAVE_kfstat) && defined(__CRT_KOS_PRIMARY)) || (defined(__CRT_HAVE_kfstat64) && defined(__CRT_KOS_PRIMARY)) || (defined(__CRT_HAVE__fstat64) && defined(__CRT_DOS_PRIMARY) && defined(__USE_TIME_BITS64)) || (defined(__CRT_HAVE__fstat64i32) && defined(__CRT_DOS_PRIMARY) && defined(__USE_TIME_BITS64)) || (defined(__CRT_HAVE__fstati64) && defined(__CRT_DOS_PRIMARY) && !defined(__USE_TIME_BITS64)) || (defined(__CRT_HAVE__fstat32i64) && defined(__CRT_DOS_PRIMARY) && !defined(__USE_TIME_BITS64)) || (defined(__CRT_HAVE_fstat) && defined(__STAT32_MATCHES_STAT64)) || defined(__CRT_HAVE_fstat64)) && ((defined(__CRT_HAVE_klstat) && defined(__CRT_KOS_PRIMARY)) || (defined(__CRT_HAVE_klstat64) && defined(__CRT_KOS_PRIMARY)) || (defined(__CRT_HAVE__stat64) && defined(__CRT_DOS_PRIMARY) && defined(__USE_TIME_BITS64)) || (defined(__CRT_HAVE__stat64i32) && defined(__CRT_DOS_PRIMARY) && defined(__USE_TIME_BITS64)) || (defined(__CRT_HAVE__stati64) && defined(__CRT_DOS_PRIMARY) && !defined(__USE_TIME_BITS64)) || (defined(__CRT_HAVE__stat32i64) && defined(__CRT_DOS_PRIMARY) && !defined(__USE_TIME_BITS64)) || (defined(__CRT_HAVE_lstat) && defined(__STAT32_MATCHES_STAT64)) || defined(__CRT_HAVE_lstat64)) && defined(_PATH_DEV))
__NAMESPACE_LOCAL_END
#include <libc/local/unistd/ttyname_r.h>
__NAMESPACE_LOCAL_BEGIN
#define __localdep_ttyname_r __LIBC_LOCAL_NAME(ttyname_r)
#else /* ... */
#undef __local___localdep_ttyname_r_defined
#endif /* !... */
#endif /* !__local___localdep_ttyname_r_defined */
__NAMESPACE_LOCAL_END
#include <asm/os/errno.h>
__NAMESPACE_LOCAL_BEGIN
__LOCAL_LIBC(ttyname) __ATTR_WUNUSED __ATTR_FDARG(1) char *
__NOTHROW_RPC(__LIBCCALL __LIBC_LOCAL_NAME(ttyname))(__fd_t __fd) {
#if defined(__CRT_HAVE_realloc) || defined(__CRT_HAVE___libc_realloc)
	/* Buffer is typed as `void *' so it can be re-used for `wttyname(3)' */
	static void *__ttyname_buf;
	__errno_t __error;
#if defined(__CRT_HAVE_malloc_usable_size) || defined(__CRT_HAVE__msize)
	__SIZE_TYPE__ __bufsize = (__NAMESPACE_LOCAL_SYM __localdep_malloc_usable_size)(__ttyname_buf) / sizeof(char);
#else /* __CRT_HAVE_malloc_usable_size || __CRT_HAVE__msize */
	__SIZE_TYPE__ __bufsize = __ttyname_buf ? 32 : 0;
#endif /* !__CRT_HAVE_malloc_usable_size && !__CRT_HAVE__msize */
	if (__bufsize < 32) {
		void *__newbuf;
		__bufsize = 32;
		__newbuf  = (__NAMESPACE_LOCAL_SYM __localdep_realloc)(__ttyname_buf, __bufsize * sizeof(char));
		if __unlikely(!__newbuf)
			goto __err;
		__ttyname_buf = __newbuf;
	}
#ifdef __ERANGE
__again:
#endif /* __ERANGE */
	__error = (__NAMESPACE_LOCAL_SYM __localdep_ttyname_r)(__fd, (char *)__ttyname_buf, __bufsize);
	if __likely(__error == 0) {
		/* Trim unused memory (if a certain threshold is exceeded) */
		__SIZE_TYPE__ __retlen = (__NAMESPACE_LOCAL_SYM __localdep_strlen)((char *)__ttyname_buf) + 1;
		if (__retlen < 32)
			__retlen = 32; /* Retain minimal buffer size */
		if __likely((__retlen + 32) < __bufsize) {
			void *__retbuf = (__NAMESPACE_LOCAL_SYM __localdep_realloc)(__ttyname_buf, __retlen * sizeof(char));
			if __likely(__retbuf)
				__ttyname_buf = __retbuf;
		}
		return (char *)__ttyname_buf;
	}
#ifdef __ERANGE
	if (__error == __ERANGE && __bufsize < 1024) {
		void *__newbuf;
		__bufsize *= 2;
		__newbuf = (__NAMESPACE_LOCAL_SYM __localdep_realloc)(__ttyname_buf, __bufsize * sizeof(char));
		if __unlikely(!__newbuf)
			goto __err;
		__ttyname_buf = __newbuf;
		goto __again;
	}
#endif /* __ERANGE */
#if defined(__CRT_HAVE_free) || defined(__CRT_HAVE_cfree) || defined(__CRT_HAVE___libc_free)
	(__NAMESPACE_LOCAL_SYM __localdep_free)(__ttyname_buf);
	__ttyname_buf = __NULLPTR;
#endif /* __CRT_HAVE_free || __CRT_HAVE_cfree || __CRT_HAVE___libc_free */
__err:
	return __NULLPTR;
#else /* __CRT_HAVE_realloc || __CRT_HAVE___libc_realloc */
	static char __ttyname_buf[32];
	if __likely((__NAMESPACE_LOCAL_SYM __localdep_ttyname_r)(__fd, __ttyname_buf, __COMPILER_LENOF(__ttyname_buf)) == 0)
		return __ttyname_buf;
	return __NULLPTR;
#endif /* !__CRT_HAVE_realloc && !__CRT_HAVE___libc_realloc */
}
__NAMESPACE_LOCAL_END
#ifndef __local___localdep_ttyname_defined
#define __local___localdep_ttyname_defined
#define __localdep_ttyname __LIBC_LOCAL_NAME(ttyname)
#endif /* !__local___localdep_ttyname_defined */
#else /* __CRT_HAVE_ttyname_r || __CRT_HAVE_frealpath4 || ((__CRT_HAVE_opendir || __CRT_HAVE___libc_opendir || (__AT_FDCWD && (__CRT_HAVE_opendirat || __CRT_HAVE_fopendirat || (__CRT_HAVE_fdopendir && (__CRT_HAVE_openat64 || __CRT_HAVE_openat)))) || (__CRT_HAVE_fdopendir && (__CRT_HAVE_open64 || __CRT_HAVE___open64 || __CRT_HAVE_open || __CRT_HAVE__open || __CRT_HAVE___open || __CRT_HAVE___libc_open || (__AT_FDCWD && (__CRT_HAVE_openat64 || __CRT_HAVE_openat)))) || __USE_DOS_DIRENT) && ((__CRT_HAVE_readdirk && __CRT_KOS && _DIRENT_MATCHES_DIRENT64) || (__CRT_HAVE_readdirk64 && __CRT_KOS) || (__CRT_HAVE_readdir && !__CRT_KOS && _DIRENT_MATCHES_DIRENT64) || (__CRT_HAVE___libc_readdir && !__CRT_KOS && _DIRENT_MATCHES_DIRENT64) || (__CRT_HAVE_readdir64 && !__CRT_KOS) || (__USE_DOS_DIRENT && _DIRENT_MATCHES_DIRENT64)) && ((__CRT_HAVE_kfstat && __CRT_KOS_PRIMARY) || (__CRT_HAVE_kfstat64 && __CRT_KOS_PRIMARY) || (__CRT_HAVE__fstat64 && __CRT_DOS_PRIMARY && __USE_TIME_BITS64) || (__CRT_HAVE__fstat64i32 && __CRT_DOS_PRIMARY && __USE_TIME_BITS64) || (__CRT_HAVE__fstati64 && __CRT_DOS_PRIMARY && !__USE_TIME_BITS64) || (__CRT_HAVE__fstat32i64 && __CRT_DOS_PRIMARY && !__USE_TIME_BITS64) || (__CRT_HAVE_fstat && __STAT32_MATCHES_STAT64) || __CRT_HAVE_fstat64) && ((__CRT_HAVE_klstat && __CRT_KOS_PRIMARY) || (__CRT_HAVE_klstat64 && __CRT_KOS_PRIMARY) || (__CRT_HAVE__stat64 && __CRT_DOS_PRIMARY && __USE_TIME_BITS64) || (__CRT_HAVE__stat64i32 && __CRT_DOS_PRIMARY && __USE_TIME_BITS64) || (__CRT_HAVE__stati64 && __CRT_DOS_PRIMARY && !__USE_TIME_BITS64) || (__CRT_HAVE__stat32i64 && __CRT_DOS_PRIMARY && !__USE_TIME_BITS64) || (__CRT_HAVE_lstat && __STAT32_MATCHES_STAT64) || __CRT_HAVE_lstat64) && _PATH_DEV) */
#undef __local_ttyname_defined
#endif /* !__CRT_HAVE_ttyname_r && !__CRT_HAVE_frealpath4 && ((!__CRT_HAVE_opendir && !__CRT_HAVE___libc_opendir && (!__AT_FDCWD || (!__CRT_HAVE_opendirat && !__CRT_HAVE_fopendirat && (!__CRT_HAVE_fdopendir || (!__CRT_HAVE_openat64 && !__CRT_HAVE_openat)))) && (!__CRT_HAVE_fdopendir || (!__CRT_HAVE_open64 && !__CRT_HAVE___open64 && !__CRT_HAVE_open && !__CRT_HAVE__open && !__CRT_HAVE___open && !__CRT_HAVE___libc_open && (!__AT_FDCWD || (!__CRT_HAVE_openat64 && !__CRT_HAVE_openat)))) && !__USE_DOS_DIRENT) || ((!__CRT_HAVE_readdirk || !__CRT_KOS || !_DIRENT_MATCHES_DIRENT64) && (!__CRT_HAVE_readdirk64 || !__CRT_KOS) && (!__CRT_HAVE_readdir || __CRT_KOS || !_DIRENT_MATCHES_DIRENT64) && (!__CRT_HAVE___libc_readdir || __CRT_KOS || !_DIRENT_MATCHES_DIRENT64) && (!__CRT_HAVE_readdir64 || __CRT_KOS) && (!__USE_DOS_DIRENT || !_DIRENT_MATCHES_DIRENT64)) || ((!__CRT_HAVE_kfstat || !__CRT_KOS_PRIMARY) && (!__CRT_HAVE_kfstat64 || !__CRT_KOS_PRIMARY) && (!__CRT_HAVE__fstat64 || !__CRT_DOS_PRIMARY || !__USE_TIME_BITS64) && (!__CRT_HAVE__fstat64i32 || !__CRT_DOS_PRIMARY || !__USE_TIME_BITS64) && (!__CRT_HAVE__fstati64 || !__CRT_DOS_PRIMARY || __USE_TIME_BITS64) && (!__CRT_HAVE__fstat32i64 || !__CRT_DOS_PRIMARY || __USE_TIME_BITS64) && (!__CRT_HAVE_fstat || !__STAT32_MATCHES_STAT64) && !__CRT_HAVE_fstat64) || ((!__CRT_HAVE_klstat || !__CRT_KOS_PRIMARY) && (!__CRT_HAVE_klstat64 || !__CRT_KOS_PRIMARY) && (!__CRT_HAVE__stat64 || !__CRT_DOS_PRIMARY || !__USE_TIME_BITS64) && (!__CRT_HAVE__stat64i32 || !__CRT_DOS_PRIMARY || !__USE_TIME_BITS64) && (!__CRT_HAVE__stati64 || !__CRT_DOS_PRIMARY || __USE_TIME_BITS64) && (!__CRT_HAVE__stat32i64 || !__CRT_DOS_PRIMARY || __USE_TIME_BITS64) && (!__CRT_HAVE_lstat || !__STAT32_MATCHES_STAT64) && !__CRT_HAVE_lstat64) || !_PATH_DEV) */
#endif /* !__local_ttyname_defined */
