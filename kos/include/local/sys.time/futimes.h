/* HASH CRC-32:0x223253f3 */
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
#ifndef __local_futimes_defined
#if defined(__CRT_HAVE_futimes) || defined(__CRT_HAVE_futimes64)
#define __local_futimes_defined 1
/* Dependency: "futimes32" from "sys.time" */
#ifndef ____localdep_futimes32_defined
#define ____localdep_futimes32_defined 1
#if defined(__CRT_HAVE_futimes)
/* Same as `utimes', but does not follow symbolic links */
__CREDIRECT(,int,__NOTHROW_NCX,__localdep_futimes32,(__fd_t __fd, struct __timeval32 const __tvp[2]),futimes,(__fd,__tvp))
#else /* LIBC: futimes */
#undef ____localdep_futimes32_defined
#endif /* futimes32... */
#endif /* !____localdep_futimes32_defined */

/* Dependency: "futimes64" from "sys.time" */
#ifndef ____localdep_futimes64_defined
#define ____localdep_futimes64_defined 1
#if defined(__CRT_HAVE_futimes64)
/* Same as `utimes', but does not follow symbolic links */
__CREDIRECT(,int,__NOTHROW_NCX,__localdep_futimes64,(__fd_t __fd, struct __timeval64 const __tvp[2]),futimes64,(__fd,__tvp))
#elif defined(__CRT_HAVE_lutimes) && (__SIZEOF_TIME32_T__ == __SIZEOF_TIME64_T__)
/* Same as `utimes', but does not follow symbolic links */
__CREDIRECT(,int,__NOTHROW_NCX,__localdep_futimes64,(__fd_t __fd, struct __timeval64 const __tvp[2]),lutimes,(__fd,__tvp))
#elif defined(__CRT_HAVE_futimes)
#include <local/sys.time/futimes64.h>
/* Same as `utimes', but does not follow symbolic links */
#define __localdep_futimes64 (__NAMESPACE_LOCAL_SYM __LIBC_LOCAL_NAME(futimes64))
#else /* CUSTOM: futimes64 */
#undef ____localdep_futimes64_defined
#endif /* futimes64... */
#endif /* !____localdep_futimes64_defined */

__NAMESPACE_LOCAL_BEGIN
/* Same as `utimes', but takes an open file descriptor instead of a name */
__LOCAL_LIBC(futimes) int
__NOTHROW_NCX(__LIBCCALL __LIBC_LOCAL_NAME(futimes))(__fd_t __fd,
                                                     struct __TM_TYPE(timeval) const __tvp[2]) {
#line 403 "kos/src/libc/magic/sys.time.c"
#ifdef __CRT_HAVE_utimes
	struct __timeval32 __tv32[2];
	if (!__tvp)
		return __localdep_futimes32(__fd, __NULLPTR);
	__tv32[0].tv_sec  = (__time32_t)__tvp[0].tv_sec;
	__tv32[0].tv_usec = __tvp[0].tv_usec;
	__tv32[1].tv_sec  = (__time32_t)__tvp[1].tv_sec;
	__tv32[1].tv_usec = __tvp[1].tv_usec;
	return __localdep_futimes32(__fd, __tv32);
#else /* __CRT_HAVE_utimes */
	struct __timeval64 __tv64[2];
	if (!__tvp)
		return __localdep_futimes64(__fd, __NULLPTR);
	__tv64[0].tv_sec  = (__time64_t)__tvp[0].tv_sec;
	__tv64[0].tv_usec = __tvp[0].tv_usec;
	__tv64[1].tv_sec  = (__time64_t)__tvp[1].tv_sec;
	__tv64[1].tv_usec = __tvp[1].tv_usec;
	return __localdep_futimes64(__fd, __tv64);
#endif /* !__CRT_HAVE_utimes */
}
__NAMESPACE_LOCAL_END
#endif /* defined(__CRT_HAVE_futimes) || defined(__CRT_HAVE_futimes64) */
#endif /* !__local_futimes_defined */
