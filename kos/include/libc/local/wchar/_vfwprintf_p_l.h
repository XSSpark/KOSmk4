/* HASH CRC-32:0x1763724 */
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
#ifndef __local__vfwprintf_p_l_defined
#define __local__vfwprintf_p_l_defined
#include <__crt.h>
#include <kos/anno.h>
#include <features.h>
#include <hybrid/typecore.h>
__NAMESPACE_LOCAL_BEGIN
#ifndef __local___localdep__vfwprintf_p_defined
#define __local___localdep__vfwprintf_p_defined
#ifdef __CRT_HAVE__vfwprintf_p
__CREDIRECT(__ATTR_NONNULL((1, 2)),__STDC_INT_AS_SSIZE_T,__THROWING,__localdep__vfwprintf_p,(__FILE *__stream, __WCHAR_TYPE__ const *__format, __builtin_va_list __args),_vfwprintf_p,(__stream,__format,__args))
#else /* __CRT_HAVE__vfwprintf_p */
__NAMESPACE_LOCAL_END
#include <libc/local/wchar/_vfwprintf_p.h>
__NAMESPACE_LOCAL_BEGIN
#define __localdep__vfwprintf_p __LIBC_LOCAL_NAME(_vfwprintf_p)
#endif /* !__CRT_HAVE__vfwprintf_p */
#endif /* !__local___localdep__vfwprintf_p_defined */
__LOCAL_LIBC(_vfwprintf_p_l) __ATTR_NONNULL((1, 2)) __STDC_INT_AS_SSIZE_T
(__LIBCCALL __LIBC_LOCAL_NAME(_vfwprintf_p_l))(__FILE *__stream, __WCHAR_TYPE__ const *__format, __locale_t __locale, __builtin_va_list __args) __THROWS(...) {
	(void)__locale;
	return (__NAMESPACE_LOCAL_SYM __localdep__vfwprintf_p)(__stream, __format, __args);
}
__NAMESPACE_LOCAL_END
#ifndef __local___localdep__vfwprintf_p_l_defined
#define __local___localdep__vfwprintf_p_l_defined
#define __localdep__vfwprintf_p_l __LIBC_LOCAL_NAME(_vfwprintf_p_l)
#endif /* !__local___localdep__vfwprintf_p_l_defined */
#endif /* !__local__vfwprintf_p_l_defined */
