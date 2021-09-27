/* HASH CRC-32:0x564df71d */
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
#ifndef __local_fuzzy_wcsncasecmp_defined
#define __local_fuzzy_wcsncasecmp_defined
#include <__crt.h>
#include <parts/malloca.h>
#if defined(__CRT_HAVE_fuzzy_wmemcasecmp) || !defined(__NO_MALLOCA)
#include <hybrid/typecore.h>
__NAMESPACE_LOCAL_BEGIN
#ifndef __local___localdep_fuzzy_wmemcasecmp_defined
#define __local___localdep_fuzzy_wmemcasecmp_defined
#ifdef __CRT_HAVE_fuzzy_wmemcasecmp
__CREDIRECT(__ATTR_PURE __ATTR_WUNUSED __ATTR_NONNULL((1, 3)),__SIZE_TYPE__,__NOTHROW_NCX,__localdep_fuzzy_wmemcasecmp,(__WCHAR_TYPE__ const *__s1, __SIZE_TYPE__ __s1_bytes, __WCHAR_TYPE__ const *__s2, __SIZE_TYPE__ __s2_bytes),fuzzy_wmemcasecmp,(__s1,__s1_bytes,__s2,__s2_bytes))
#elif !defined(__NO_MALLOCA)
__NAMESPACE_LOCAL_END
#include <libc/local/wchar/fuzzy_wmemcasecmp.h>
__NAMESPACE_LOCAL_BEGIN
#define __localdep_fuzzy_wmemcasecmp __LIBC_LOCAL_NAME(fuzzy_wmemcasecmp)
#else /* ... */
#undef __local___localdep_fuzzy_wmemcasecmp_defined
#endif /* !... */
#endif /* !__local___localdep_fuzzy_wmemcasecmp_defined */
#ifndef __local___localdep_wcsnlen_defined
#define __local___localdep_wcsnlen_defined
#ifdef __CRT_HAVE_wcsnlen
__CREDIRECT(__ATTR_PURE __ATTR_WUNUSED __ATTR_NONNULL((1)),__SIZE_TYPE__,__NOTHROW_NCX,__localdep_wcsnlen,(__WCHAR_TYPE__ const *__restrict __str, __SIZE_TYPE__ __maxlen),wcsnlen,(__str,__maxlen))
#else /* __CRT_HAVE_wcsnlen */
__NAMESPACE_LOCAL_END
#include <libc/local/wchar/wcsnlen.h>
__NAMESPACE_LOCAL_BEGIN
#define __localdep_wcsnlen __LIBC_LOCAL_NAME(wcsnlen)
#endif /* !__CRT_HAVE_wcsnlen */
#endif /* !__local___localdep_wcsnlen_defined */
__LOCAL_LIBC(fuzzy_wcsncasecmp) __ATTR_PURE __ATTR_WUNUSED __ATTR_NONNULL((1, 3)) __SIZE_TYPE__
__NOTHROW_NCX(__LIBCCALL __LIBC_LOCAL_NAME(fuzzy_wcsncasecmp))(__WCHAR_TYPE__ const *__s1, __SIZE_TYPE__ __s1_maxlen, __WCHAR_TYPE__ const *__s2, __SIZE_TYPE__ __s2_maxlen) {
	return (__NAMESPACE_LOCAL_SYM __localdep_fuzzy_wmemcasecmp)(__s1, (__NAMESPACE_LOCAL_SYM __localdep_wcsnlen)(__s1, __s1_maxlen), __s2, (__NAMESPACE_LOCAL_SYM __localdep_wcsnlen)(__s2, __s2_maxlen));
}
__NAMESPACE_LOCAL_END
#ifndef __local___localdep_fuzzy_wcsncasecmp_defined
#define __local___localdep_fuzzy_wcsncasecmp_defined
#define __localdep_fuzzy_wcsncasecmp __LIBC_LOCAL_NAME(fuzzy_wcsncasecmp)
#endif /* !__local___localdep_fuzzy_wcsncasecmp_defined */
#else /* __CRT_HAVE_fuzzy_wmemcasecmp || !__NO_MALLOCA */
#undef __local_fuzzy_wcsncasecmp_defined
#endif /* !__CRT_HAVE_fuzzy_wmemcasecmp && __NO_MALLOCA */
#endif /* !__local_fuzzy_wcsncasecmp_defined */
