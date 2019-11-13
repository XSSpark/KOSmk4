/* HASH CRC-32:0xc90592e5 */
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
#ifndef __local__vwprintf_p_defined
#if !defined(__NO_STDSTREAMS)
#define __local__vwprintf_p_defined 1
#include <kos/anno.h>
#include <local/stdstreams.h>
/* Dependency: "_vfwprintf_p" from "wchar" */
#ifndef ____localdep__vfwprintf_p_defined
#define ____localdep__vfwprintf_p_defined 1
#ifdef __CRT_HAVE__vfwprintf_p
__CREDIRECT(__ATTR_NONNULL((1, 2)),__STDC_INT_AS_SSIZE_T,,__localdep__vfwprintf_p,(__FILE *__stream, __WCHAR_TYPE__ const *__format, __builtin_va_list __args),_vfwprintf_p,(__stream,__format,__args)) __THROWS(...)
#else /* LIBC: _vfwprintf_p */
#include <local/wchar/_vfwprintf_p.h>
#define __localdep__vfwprintf_p (__NAMESPACE_LOCAL_SYM __LIBC_LOCAL_NAME(_vfwprintf_p))
#endif /* _vfwprintf_p... */
#endif /* !____localdep__vfwprintf_p_defined */

__NAMESPACE_LOCAL_BEGIN
__LOCAL_LIBC(_vwprintf_p) __ATTR_NONNULL((1)) __STDC_INT_AS_SSIZE_T
(__LIBCCALL __LIBC_LOCAL_NAME(_vwprintf_p))(__WCHAR_TYPE__ const *__format,
                                            __builtin_va_list __args) __THROWS(...) {
#line 1894 "kos/src/libc/magic/wchar.c"
	return __localdep__vfwprintf_p(__LOCAL_stdout, __format, __args);
}
__NAMESPACE_LOCAL_END
#endif /* !defined(__NO_STDSTREAMS) */
#endif /* !__local__vwprintf_p_defined */
