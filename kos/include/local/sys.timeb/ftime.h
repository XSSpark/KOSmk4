/* HASH CRC-32:0xbfa781ee */
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
#ifndef __local_ftime_defined
#if defined(__CRT_HAVE_ftime) || defined(__CRT_HAVE_ftime64) || defined(__CRT_HAVE__ftime32) || defined(__CRT_HAVE__ftime64) || defined(__CRT_HAVE__ftime32_s) || defined(__CRT_HAVE__ftime64_s)
#define __local_ftime_defined 1
#include <parts/errno.h>
/* Dependency: "__crt_ftime32_s" from "sys.timeb" */
#ifndef ____localdep___crt_ftime32_s_defined
#define ____localdep___crt_ftime32_s_defined 1
#if defined(__CRT_HAVE__ftime32_s)
/* Fill in TIMEBUF with information about the current time */
__CREDIRECT(__ATTR_NONNULL((1)),__errno_t,__NOTHROW_NCX,__localdep___crt_ftime32_s,(struct __timeb32 *__timebuf),_ftime32_s,(__timebuf))
#else /* LIBC: _ftime32_s */
#undef ____localdep___crt_ftime32_s_defined
#endif /* __crt_ftime32_s... */
#endif /* !____localdep___crt_ftime32_s_defined */

/* Dependency: "__crt_ftime64_s" from "sys.timeb" */
#ifndef ____localdep___crt_ftime64_s_defined
#define ____localdep___crt_ftime64_s_defined 1
#if defined(__CRT_HAVE__ftime64_s)
/* Fill in TIMEBUF with information about the current time */
__CREDIRECT(__ATTR_NONNULL((1)),__errno_t,__NOTHROW_NCX,__localdep___crt_ftime64_s,(struct __timeb64 *__timebuf),_ftime64_s,(__timebuf))
#else /* LIBC: _ftime64_s */
#undef ____localdep___crt_ftime64_s_defined
#endif /* __crt_ftime64_s... */
#endif /* !____localdep___crt_ftime64_s_defined */

/* Dependency: "__crt_dos_ftime32" from "sys.timeb" */
#ifndef ____localdep___crt_dos_ftime32_defined
#define ____localdep___crt_dos_ftime32_defined 1
#if defined(__CRT_HAVE__ftime32)
/* Fill in TIMEBUF with information about the current time */
__CREDIRECT_VOID(__ATTR_NONNULL((1)),__NOTHROW_NCX,__localdep___crt_dos_ftime32,(struct __timeb32 *__timebuf),_ftime32,(__timebuf))
#else /* LIBC: _ftime32 */
#undef ____localdep___crt_dos_ftime32_defined
#endif /* __crt_dos_ftime32... */
#endif /* !____localdep___crt_dos_ftime32_defined */

/* Dependency: "__crt_dos_ftime64" from "sys.timeb" */
#ifndef ____localdep___crt_dos_ftime64_defined
#define ____localdep___crt_dos_ftime64_defined 1
#if defined(__CRT_HAVE__ftime64)
/* Fill in TIMEBUF with information about the current time */
__CREDIRECT_VOID(__ATTR_NONNULL((1)),__NOTHROW_NCX,__localdep___crt_dos_ftime64,(struct __timeb64 *__timebuf),_ftime64,(__timebuf))
#else /* LIBC: _ftime64 */
#undef ____localdep___crt_dos_ftime64_defined
#endif /* __crt_dos_ftime64... */
#endif /* !____localdep___crt_dos_ftime64_defined */

/* Dependency: "__crt_ftime32" from "sys.timeb" */
#ifndef ____localdep___crt_ftime32_defined
#define ____localdep___crt_ftime32_defined 1
#if defined(__CRT_HAVE_ftime)
/* Fill in TIMEBUF with information about the current time */
__CREDIRECT(__ATTR_NONNULL((1)),int,__NOTHROW_NCX,__localdep___crt_ftime32,(struct __timeb32 *__timebuf),ftime,(__timebuf))
#else /* LIBC: ftime */
#undef ____localdep___crt_ftime32_defined
#endif /* __crt_ftime32... */
#endif /* !____localdep___crt_ftime32_defined */

/* Dependency: "__crt_ftime64" from "sys.timeb" */
#ifndef ____localdep___crt_ftime64_defined
#define ____localdep___crt_ftime64_defined 1
#if defined(__CRT_HAVE_ftime64)
/* Fill in TIMEBUF with information about the current time */
__CREDIRECT(__ATTR_NONNULL((1)),int,__NOTHROW_NCX,__localdep___crt_ftime64,(struct __timeb64 *__timebuf),ftime64,(__timebuf))
#else /* LIBC: ftime64 */
#undef ____localdep___crt_ftime64_defined
#endif /* __crt_ftime64... */
#endif /* !____localdep___crt_ftime64_defined */

__NAMESPACE_LOCAL_BEGIN
/* Fill in TIMEBUF with information about the current time */
__LOCAL_LIBC(ftime) __ATTR_NONNULL((1)) int
__NOTHROW_NCX(__LIBCCALL __LIBC_LOCAL_NAME(ftime))(struct timeb *__timebuf) {
#line 258 "kos/src/libc/magic/sys.timeb.c"
#if defined(__CRT_HAVE__ftime32_s) && !defined(__USE_TIME_BITS64)
	__errno_t __error = __localdep___crt_ftime32_s(__timebuf);
	if __unlikely(__error) {
		__libc_seterrno(__error);
		__error = -1;
	}
	return (int)__error;
#elif defined(__CRT_HAVE__ftime64_s) && defined(__USE_TIME_BITS64)
	__errno_t __error = __localdep___crt_ftime64_s(__timebuf);
	if __unlikely(__error) {
		__libc_seterrno(__error);
		__error = -1;
	}
	return (int)__error;
#elif defined(__CRT_HAVE__ftime32) && !defined(__USE_TIME_BITS64)
	__localdep___crt_dos_ftime32(&__temp);
	return 0;
#elif defined(__CRT_HAVE__ftime64) && defined(__USE_TIME_BITS64)
	__localdep___crt_dos_ftime64(&__temp);
	return 0;
#elif defined(__CRT_HAVE_ftime)
	struct __timeb32 __temp;
	int __error = __localdep___crt_ftime32(&__temp);
	if __likely(!__error) {
		__timebuf->time     = (__time64_t)__temp.time;
		__timebuf->millitm  = __temp.millitm;
		__timebuf->timezone = __temp.timezone;
		__timebuf->dstflag  = __temp.dstflag;
	}
	return __error;
#elif defined(__CRT_HAVE_ftime64)
	struct __timeb64 __temp;
	int __error = __localdep___crt_ftime64(&__temp);
	if __likely(!__error) {
		__timebuf->time     = (__time32_t)__temp.time;
		__timebuf->millitm  = __temp.millitm;
		__timebuf->timezone = __temp.timezone;
		__timebuf->dstflag  = __temp.dstflag;
	}
	return __error;
#elif defined(__CRT_HAVE__ftime32_s)
	struct __timeb32 __temp;
	__errno_t __error = __localdep___crt_ftime32_s(&__temp);
	if __unlikely(__error) {
		__libc_seterrno(__error);
		__error = -1;
	} else {
		__timebuf->time     = (__time64_t)__temp.time;
		__timebuf->millitm  = __temp.millitm;
		__timebuf->timezone = __temp.timezone;
		__timebuf->dstflag  = __temp.dstflag;
	}
	return (int)__error;
#elif defined(__CRT_HAVE__ftime64_s)
	struct __timeb64 __temp;
	__errno_t __error = __localdep___crt_ftime64_s(&__temp);
	if __unlikely(__error) {
		__libc_seterrno(__error);
		__error = -1;
	} else {
		__timebuf->time     = (__time32_t)__temp.time;
		__timebuf->millitm  = __temp.millitm;
		__timebuf->timezone = __temp.timezone;
		__timebuf->dstflag  = __temp.dstflag;
	}
	return (int)__error;
#elif defined(__CRT_HAVE__ftime32)
	struct __timeb32 __temp;
	__localdep___crt_dos_ftime32(&__temp);
	__timebuf->time     = (__time64_t)__temp.time;
	__timebuf->millitm  = __temp.millitm;
	__timebuf->timezone = __temp.timezone;
	__timebuf->dstflag  = __temp.dstflag;
	return 0;
#else
	struct __timeb64 __temp;
	__localdep___crt_dos_ftime64(&__temp);
	__timebuf->time     = (__time32_t)__temp.time;
	__timebuf->millitm  = __temp.millitm;
	__timebuf->timezone = __temp.timezone;
	__timebuf->dstflag  = __temp.dstflag;
	return 0;
#endif
}
__NAMESPACE_LOCAL_END
#endif /* defined(__CRT_HAVE_ftime) || defined(__CRT_HAVE_ftime64) || defined(__CRT_HAVE__ftime32) || defined(__CRT_HAVE__ftime64) || defined(__CRT_HAVE__ftime32_s) || defined(__CRT_HAVE__ftime64_s) */
#endif /* !__local_ftime_defined */
