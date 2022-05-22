/* HASH CRC-32:0x498a6643 */
/* Copyright (c) 2019-2022 Griefer@Work                                       *
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
 *    Portions Copyright (c) 2019-2022 Griefer@Work                           *
 * 2. Altered source versions must be plainly marked as such, and must not be *
 *    misrepresented as being the original software.                          *
 * 3. This notice may not be removed or altered from any source distribution. *
 */
#ifndef __local__mbsnbcat_s_l_defined
#define __local__mbsnbcat_s_l_defined
#include <__crt.h>
#if defined(__CRT_HAVE__mbsnbcat_l) || defined(__CRT_HAVE__ismbblead_l) || defined(__CRT_HAVE__mbctype) || defined(__CRT_HAVE___p__mbctype)
#include <hybrid/typecore.h>
#include <bits/types.h>
__NAMESPACE_LOCAL_BEGIN
#ifndef __local___localdep__mbsnbcat_l_defined
#define __local___localdep__mbsnbcat_l_defined
#ifdef __CRT_HAVE__mbsnbcat_l
__CREDIRECT(__ATTR_ACCESS_ROS(2, 3) __ATTR_ACCESS_RW(1),unsigned char *,__NOTHROW_NCX,__localdep__mbsnbcat_l,(unsigned char *__buf, unsigned char const *__src, __SIZE_TYPE__ __max_bytes, __locale_t __locale),_mbsnbcat_l,(__buf,__src,__max_bytes,__locale))
#elif defined(__CRT_HAVE__ismbblead_l) || defined(__CRT_HAVE__mbctype) || defined(__CRT_HAVE___p__mbctype)
__NAMESPACE_LOCAL_END
#include <libc/local/mbstring/_mbsnbcat_l.h>
__NAMESPACE_LOCAL_BEGIN
#define __localdep__mbsnbcat_l __LIBC_LOCAL_NAME(_mbsnbcat_l)
#else /* ... */
#undef __local___localdep__mbsnbcat_l_defined
#endif /* !... */
#endif /* !__local___localdep__mbsnbcat_l_defined */
#ifndef __local___localdep_strnlen_defined
#define __local___localdep_strnlen_defined
#if __has_builtin(__builtin_strnlen) && defined(__LIBC_BIND_CRTBUILTINS) && defined(__CRT_HAVE_strnlen)
__CEIREDIRECT(__ATTR_PURE __ATTR_WUNUSED __ATTR_ACCESS_ROS(1, 2) __ATTR_NONNULL((1)),__SIZE_TYPE__,__NOTHROW_NCX,__localdep_strnlen,(char const *__restrict __str, __SIZE_TYPE__ __maxlen),strnlen,{ return __builtin_strnlen(__str, __maxlen); })
#elif defined(__CRT_HAVE_strnlen)
__CREDIRECT(__ATTR_PURE __ATTR_WUNUSED __ATTR_ACCESS_ROS(1, 2) __ATTR_NONNULL((1)),__SIZE_TYPE__,__NOTHROW_NCX,__localdep_strnlen,(char const *__restrict __str, __SIZE_TYPE__ __maxlen),strnlen,(__str,__maxlen))
#elif defined(__CRT_HAVE___strncnt)
__CREDIRECT(__ATTR_PURE __ATTR_WUNUSED __ATTR_ACCESS_ROS(1, 2) __ATTR_NONNULL((1)),__SIZE_TYPE__,__NOTHROW_NCX,__localdep_strnlen,(char const *__restrict __str, __SIZE_TYPE__ __maxlen),__strncnt,(__str,__maxlen))
#else /* ... */
__NAMESPACE_LOCAL_END
#include <libc/local/string/strnlen.h>
__NAMESPACE_LOCAL_BEGIN
#define __localdep_strnlen __LIBC_LOCAL_NAME(strnlen)
#endif /* !... */
#endif /* !__local___localdep_strnlen_defined */
__LOCAL_LIBC(_mbsnbcat_s_l) __ATTR_ACCESS_ROS(3, 4) __ATTR_ACCESS_RWS(1, 2) __errno_t
__NOTHROW_NCX(__LIBCCALL __LIBC_LOCAL_NAME(_mbsnbcat_s_l))(unsigned char *__buf, __SIZE_TYPE__ __true_bufsize, unsigned char const *__src, __SIZE_TYPE__ __max_bytes, __locale_t __locale) {
	if (!__buf || (!__src && __max_bytes) ||
	    __true_bufsize <= (__NAMESPACE_LOCAL_SYM __localdep_strnlen)((char const *)__src, __max_bytes))
		return 22;
	(__NAMESPACE_LOCAL_SYM __localdep__mbsnbcat_l)(__buf, __src, __max_bytes, __locale);
	return 0;
}
__NAMESPACE_LOCAL_END
#ifndef __local___localdep__mbsnbcat_s_l_defined
#define __local___localdep__mbsnbcat_s_l_defined
#define __localdep__mbsnbcat_s_l __LIBC_LOCAL_NAME(_mbsnbcat_s_l)
#endif /* !__local___localdep__mbsnbcat_s_l_defined */
#else /* __CRT_HAVE__mbsnbcat_l || __CRT_HAVE__ismbblead_l || __CRT_HAVE__mbctype || __CRT_HAVE___p__mbctype */
#undef __local__mbsnbcat_s_l_defined
#endif /* !__CRT_HAVE__mbsnbcat_l && !__CRT_HAVE__ismbblead_l && !__CRT_HAVE__mbctype && !__CRT_HAVE___p__mbctype */
#endif /* !__local__mbsnbcat_s_l_defined */
