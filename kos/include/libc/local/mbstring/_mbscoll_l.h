/* HASH CRC-32:0xc2ca16a */
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
#ifndef __local__mbscoll_l_defined
#define __local__mbscoll_l_defined
#include <__crt.h>
#if defined(__CRT_HAVE__mbscmp_l) || defined(__CRT_HAVE__ismbblead_l) || defined(__CRT_HAVE__mbctype) || defined(__CRT_HAVE___p__mbctype)
__NAMESPACE_LOCAL_BEGIN
#ifndef __local___localdep__mbscmp_l_defined
#define __local___localdep__mbscmp_l_defined
#ifdef __CRT_HAVE__mbscmp_l
__CREDIRECT(__ATTR_PURE __ATTR_WUNUSED __ATTR_ACCESS_RO(1) __ATTR_ACCESS_RO(2),int,__NOTHROW_NCX,__localdep__mbscmp_l,(unsigned char const *__lhs, unsigned char const *__rhs, __locale_t __locale),_mbscmp_l,(__lhs,__rhs,__locale))
#elif defined(__CRT_HAVE__ismbblead_l) || defined(__CRT_HAVE__mbctype) || defined(__CRT_HAVE___p__mbctype)
__NAMESPACE_LOCAL_END
#include <libc/local/mbstring/_mbscmp_l.h>
__NAMESPACE_LOCAL_BEGIN
#define __localdep__mbscmp_l __LIBC_LOCAL_NAME(_mbscmp_l)
#else /* ... */
#undef __local___localdep__mbscmp_l_defined
#endif /* !... */
#endif /* !__local___localdep__mbscmp_l_defined */
__LOCAL_LIBC(_mbscoll_l) __ATTR_PURE __ATTR_WUNUSED __ATTR_ACCESS_RO(1) __ATTR_ACCESS_RO(2) int
__NOTHROW_NCX(__LIBCCALL __LIBC_LOCAL_NAME(_mbscoll_l))(unsigned char const *__lhs, unsigned char const *__rhs, __locale_t __locale) {
	/* XXX: Implement properly? */
	return (__NAMESPACE_LOCAL_SYM __localdep__mbscmp_l)(__lhs, __rhs, __locale);
}
__NAMESPACE_LOCAL_END
#ifndef __local___localdep__mbscoll_l_defined
#define __local___localdep__mbscoll_l_defined
#define __localdep__mbscoll_l __LIBC_LOCAL_NAME(_mbscoll_l)
#endif /* !__local___localdep__mbscoll_l_defined */
#else /* __CRT_HAVE__mbscmp_l || __CRT_HAVE__ismbblead_l || __CRT_HAVE__mbctype || __CRT_HAVE___p__mbctype */
#undef __local__mbscoll_l_defined
#endif /* !__CRT_HAVE__mbscmp_l && !__CRT_HAVE__ismbblead_l && !__CRT_HAVE__mbctype && !__CRT_HAVE___p__mbctype */
#endif /* !__local__mbscoll_l_defined */
