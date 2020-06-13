/* HASH CRC-32:0x63a399ec */
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
#ifndef __local_wcsxfrm_l_defined
#define __local_wcsxfrm_l_defined 1
#include <__crt.h>
__NAMESPACE_LOCAL_BEGIN
/* Dependency: wcsxfrm from wchar */
#ifndef __local___localdep_wcsxfrm_defined
#define __local___localdep_wcsxfrm_defined 1
#ifdef __CRT_HAVE_wcsxfrm
__CREDIRECT(__ATTR_NONNULL((1, 2)),__SIZE_TYPE__,__NOTHROW_NCX,__localdep_wcsxfrm,(__WCHAR_TYPE__ *__dst, __WCHAR_TYPE__ const *__restrict __src, __SIZE_TYPE__ __maxlen),wcsxfrm,(__dst,__src,__maxlen))
#elif defined(__CRT_HAVE_DOS$wcsxfrm) && __SIZEOF_WCHAR_T__ == 4
__CREDIRECT_KOS(__ATTR_NONNULL((1, 2)),__SIZE_TYPE__,__NOTHROW_NCX,__localdep_wcsxfrm,(__WCHAR_TYPE__ *__dst, __WCHAR_TYPE__ const *__restrict __src, __SIZE_TYPE__ __maxlen),wcsxfrm,(__dst,__src,__maxlen))
#elif defined(__CRT_HAVE_DOS$wcsxfrm) && __SIZEOF_WCHAR_T__ == 2
__CREDIRECT_DOS(__ATTR_NONNULL((1, 2)),__SIZE_TYPE__,__NOTHROW_NCX,__localdep_wcsxfrm,(__WCHAR_TYPE__ *__dst, __WCHAR_TYPE__ const *__restrict __src, __SIZE_TYPE__ __maxlen),wcsxfrm,(__dst,__src,__maxlen))
#else /* ... */
__NAMESPACE_LOCAL_END
#include <local/wchar/wcsxfrm.h>
__NAMESPACE_LOCAL_BEGIN
#define __localdep_wcsxfrm __LIBC_LOCAL_NAME(wcsxfrm)
#endif /* !... */
#endif /* !__local___localdep_wcsxfrm_defined */
__LOCAL_LIBC(wcsxfrm_l) __ATTR_NONNULL((2)) __SIZE_TYPE__
__NOTHROW_NCX(__LIBCCALL __LIBC_LOCAL_NAME(wcsxfrm_l))(__WCHAR_TYPE__ *__dst, __WCHAR_TYPE__ const *__restrict __src, __SIZE_TYPE__ __maxlen, __locale_t __locale) {
	(void)__locale;
	return __localdep_wcsxfrm(__dst, __src, __maxlen);
}
__NAMESPACE_LOCAL_END
#ifndef __local___localdep_wcsxfrm_l_defined
#define __local___localdep_wcsxfrm_l_defined 1
#define __localdep_wcsxfrm_l __LIBC_LOCAL_NAME(wcsxfrm_l)
#endif /* !__local___localdep_wcsxfrm_l_defined */
#endif /* !__local_wcsxfrm_l_defined */
