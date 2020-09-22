/* HASH CRC-32:0xb3a16804 */
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
#ifndef __local_clock_gettime_defined
#define __local_clock_gettime_defined 1
#include <__crt.h>
#if defined(__CRT_HAVE_clock_gettime64) || defined(__CRT_HAVE_clock_gettime) || defined(__CRT_HAVE___clock_gettime)
#include <bits/os/timespec.h>
__NAMESPACE_LOCAL_BEGIN
/* Dependency: clock_gettime32 from time */
#ifndef __local___localdep_clock_gettime32_defined
#define __local___localdep_clock_gettime32_defined 1
#ifdef __CRT_HAVE_clock_gettime
/* Get current value of clock CLOCK_ID and store it in TP */
__CREDIRECT(__ATTR_NONNULL((2)),int,__NOTHROW_NCX,__localdep_clock_gettime32,(__clockid_t __clock_id, struct __timespec32 *__tp),clock_gettime,(__clock_id,__tp))
#elif defined(__CRT_HAVE___clock_gettime)
/* Get current value of clock CLOCK_ID and store it in TP */
__CREDIRECT(__ATTR_NONNULL((2)),int,__NOTHROW_NCX,__localdep_clock_gettime32,(__clockid_t __clock_id, struct __timespec32 *__tp),__clock_gettime,(__clock_id,__tp))
#else /* ... */
#undef __local___localdep_clock_gettime32_defined
#endif /* !... */
#endif /* !__local___localdep_clock_gettime32_defined */
/* Dependency: clock_gettime64 from time */
#ifndef __local___localdep_clock_gettime64_defined
#define __local___localdep_clock_gettime64_defined 1
#ifdef __CRT_HAVE_clock_gettime64
__NAMESPACE_LOCAL_END
#include <bits/types.h>
__NAMESPACE_LOCAL_BEGIN
/* Get current value of clock CLOCK_ID and store it in TP */
__CREDIRECT(__ATTR_NONNULL((2)),int,__NOTHROW_NCX,__localdep_clock_gettime64,(__clockid_t __clock_id, struct __timespec64 *__tp),clock_gettime64,(__clock_id,__tp))
#elif defined(__CRT_HAVE_clock_gettime) && __SIZEOF_TIME32_T__ == __SIZEOF_TIME64_T__
__NAMESPACE_LOCAL_END
#include <bits/types.h>
__NAMESPACE_LOCAL_BEGIN
/* Get current value of clock CLOCK_ID and store it in TP */
__CREDIRECT(__ATTR_NONNULL((2)),int,__NOTHROW_NCX,__localdep_clock_gettime64,(__clockid_t __clock_id, struct __timespec64 *__tp),clock_gettime,(__clock_id,__tp))
#elif defined(__CRT_HAVE_clock_gettime) || defined(__CRT_HAVE___clock_gettime)
__NAMESPACE_LOCAL_END
#include <libc/local/time/clock_gettime64.h>
__NAMESPACE_LOCAL_BEGIN
/* Get current value of clock CLOCK_ID and store it in TP */
#define __localdep_clock_gettime64 __LIBC_LOCAL_NAME(clock_gettime64)
#else /* ... */
#undef __local___localdep_clock_gettime64_defined
#endif /* !... */
#endif /* !__local___localdep_clock_gettime64_defined */
/* Get current value of clock CLOCK_ID and store it in TP */
__LOCAL_LIBC(clock_gettime) __ATTR_NONNULL((2)) int
__NOTHROW_NCX(__LIBCCALL __LIBC_LOCAL_NAME(clock_gettime))(__clockid_t __clock_id, struct timespec *__tp) {
#if defined(__CRT_HAVE_clock_gettime) || defined(__CRT_HAVE___clock_gettime)
	int __result;
	struct __timespec32 __res32;
	__result = __localdep_clock_gettime32(__clock_id, &__res32);
	if (!__result) {
		__tp->tv_sec  = (__time64_t)__res32.tv_sec;
		__tp->tv_nsec = __res32.tv_nsec;
	}
	return __result;
#else /* __CRT_HAVE_clock_gettime || __CRT_HAVE___clock_gettime */
	int __result;
	struct __timespec64 __res64;
	__result = __localdep_clock_gettime64(__clock_id, &__res64);
	if (!__result) {
		__tp->tv_sec  = (__time32_t)__res64.tv_sec;
		__tp->tv_nsec = __res64.tv_nsec;
	}
	return __result;
#endif /* !__CRT_HAVE_clock_gettime && !__CRT_HAVE___clock_gettime */
}
__NAMESPACE_LOCAL_END
#ifndef __local___localdep_clock_gettime_defined
#define __local___localdep_clock_gettime_defined 1
#define __localdep_clock_gettime __LIBC_LOCAL_NAME(clock_gettime)
#endif /* !__local___localdep_clock_gettime_defined */
#else /* __CRT_HAVE_clock_gettime64 || __CRT_HAVE_clock_gettime || __CRT_HAVE___clock_gettime */
#undef __local_clock_gettime_defined
#endif /* !__CRT_HAVE_clock_gettime64 && !__CRT_HAVE_clock_gettime && !__CRT_HAVE___clock_gettime */
#endif /* !__local_clock_gettime_defined */
