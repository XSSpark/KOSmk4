/* HASH CRC-32:0x20476cac */
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
#ifndef __local__c16scanf_l_defined
#define __local__c16scanf_l_defined
#include <__crt.h>
#if (defined(__CRT_HAVE__vwscanf_l) && __SIZEOF_WCHAR_T__ == 2 && defined(__LIBCCALL_IS_LIBDCALL)) || defined(__CRT_HAVE_DOS$_vwscanf_l) || ((defined(__CRT_HAVE_vwscanf) || (defined(__CRT_HAVE_vfwscanf) && !defined(__NO_STDSTREAMS))) && __SIZEOF_WCHAR_T__ == 2) || (defined(__CRT_HAVE_vwscanf) && __SIZEOF_WCHAR_T__ == 2 && defined(__LIBCCALL_IS_LIBDCALL)) || defined(__CRT_HAVE_DOS$vwscanf) || (defined(__CRT_HAVE_vfwscanf) && !defined(__NO_STDSTREAMS) && __SIZEOF_WCHAR_T__ == 2) || (((defined(__CRT_HAVE_vfwscanf) && __SIZEOF_WCHAR_T__ == 2 && defined(__LIBCCALL_IS_LIBDCALL)) || defined(__CRT_HAVE_DOS$vfwscanf)) && !defined(__NO_STDSTREAMS))
#include <features.h>
__NAMESPACE_LOCAL_BEGIN
#ifndef __local___localdep__vc16scanf_l_defined
#define __local___localdep__vc16scanf_l_defined
#if defined(__CRT_HAVE__vwscanf_l) && __SIZEOF_WCHAR_T__ == 2 && defined(__LIBCCALL_IS_LIBDCALL)
__CREDIRECT(__ATTR_WUNUSED __ATTR_NONNULL((1)),__STDC_INT_AS_SSIZE_T,__NOTHROW_NCX,__localdep__vc16scanf_l,(__CHAR16_TYPE__ const *__format, __locale_t __locale, __builtin_va_list __args),_vwscanf_l,(__format,__locale,__args))
#elif defined(__CRT_HAVE_DOS$_vwscanf_l)
__CREDIRECT_DOS(__ATTR_WUNUSED __ATTR_NONNULL((1)),__STDC_INT_AS_SSIZE_T,__NOTHROW_NCX,__localdep__vc16scanf_l,(__CHAR16_TYPE__ const *__format, __locale_t __locale, __builtin_va_list __args),_vwscanf_l,(__format,__locale,__args))
#elif (defined(__CRT_HAVE_vwscanf) || (defined(__CRT_HAVE_vfwscanf) && !defined(__NO_STDSTREAMS))) && __SIZEOF_WCHAR_T__ == 2
__NAMESPACE_LOCAL_END
#include <libc/local/corecrt_wstdio/_vwscanf_l.h>
__NAMESPACE_LOCAL_BEGIN
#define __localdep__vc16scanf_l __NAMESPACE_LOCAL_TYPEHAX(__STDC_INT_AS_SSIZE_T(__LIBDCALL*)(__CHAR16_TYPE__ const *,__locale_t,__builtin_va_list),__STDC_INT_AS_SSIZE_T(__LIBDCALL&)(__CHAR16_TYPE__ const *,__locale_t,__builtin_va_list),_vwscanf_l)
#elif (defined(__CRT_HAVE_vwscanf) && __SIZEOF_WCHAR_T__ == 2 && defined(__LIBCCALL_IS_LIBDCALL)) || defined(__CRT_HAVE_DOS$vwscanf) || (defined(__CRT_HAVE_vfwscanf) && !defined(__NO_STDSTREAMS) && __SIZEOF_WCHAR_T__ == 2) || (((defined(__CRT_HAVE_vfwscanf) && __SIZEOF_WCHAR_T__ == 2 && defined(__LIBCCALL_IS_LIBDCALL)) || defined(__CRT_HAVE_DOS$vfwscanf)) && !defined(__NO_STDSTREAMS))
__NAMESPACE_LOCAL_END
#include <libc/local/parts.uchar.stdio/_vc16scanf_l.h>
__NAMESPACE_LOCAL_BEGIN
#define __localdep__vc16scanf_l __LIBC_LOCAL_NAME(_vc16scanf_l)
#else /* ... */
#undef __local___localdep__vc16scanf_l_defined
#endif /* !... */
#endif /* !__local___localdep__vc16scanf_l_defined */
__LOCAL_LIBC(_c16scanf_l) __ATTR_WUNUSED __ATTR_NONNULL((1)) __STDC_INT_AS_SSIZE_T
__NOTHROW_NCX(__VLIBDCALL __LIBC_LOCAL_NAME(_c16scanf_l))(__CHAR16_TYPE__ const *__format, __locale_t __locale, ...) {
	__STDC_INT_AS_SSIZE_T __result;
	__builtin_va_list __args;
	__builtin_va_start(__args, __locale);
	__result = (__NAMESPACE_LOCAL_SYM __localdep__vc16scanf_l)(__format, __locale, __args);
	__builtin_va_end(__args);
	return __result;
}
__NAMESPACE_LOCAL_END
#ifndef __local___localdep__c16scanf_l_defined
#define __local___localdep__c16scanf_l_defined
#define __localdep__c16scanf_l __LIBC_LOCAL_NAME(_c16scanf_l)
#endif /* !__local___localdep__c16scanf_l_defined */
#else /* (__CRT_HAVE__vwscanf_l && __SIZEOF_WCHAR_T__ == 2 && __LIBCCALL_IS_LIBDCALL) || __CRT_HAVE_DOS$_vwscanf_l || ((__CRT_HAVE_vwscanf || (__CRT_HAVE_vfwscanf && !__NO_STDSTREAMS)) && __SIZEOF_WCHAR_T__ == 2) || (__CRT_HAVE_vwscanf && __SIZEOF_WCHAR_T__ == 2 && __LIBCCALL_IS_LIBDCALL) || __CRT_HAVE_DOS$vwscanf || (__CRT_HAVE_vfwscanf && !__NO_STDSTREAMS && __SIZEOF_WCHAR_T__ == 2) || (((__CRT_HAVE_vfwscanf && __SIZEOF_WCHAR_T__ == 2 && __LIBCCALL_IS_LIBDCALL) || __CRT_HAVE_DOS$vfwscanf) && !__NO_STDSTREAMS) */
#undef __local__c16scanf_l_defined
#endif /* (!__CRT_HAVE__vwscanf_l || __SIZEOF_WCHAR_T__ != 2 || !__LIBCCALL_IS_LIBDCALL) && !__CRT_HAVE_DOS$_vwscanf_l && ((!__CRT_HAVE_vwscanf && (!__CRT_HAVE_vfwscanf || __NO_STDSTREAMS)) || __SIZEOF_WCHAR_T__ != 2) && (!__CRT_HAVE_vwscanf || __SIZEOF_WCHAR_T__ != 2 || !__LIBCCALL_IS_LIBDCALL) && !__CRT_HAVE_DOS$vwscanf && (!__CRT_HAVE_vfwscanf || __NO_STDSTREAMS || __SIZEOF_WCHAR_T__ != 2) && (((!__CRT_HAVE_vfwscanf || __SIZEOF_WCHAR_T__ != 2 || !__LIBCCALL_IS_LIBDCALL) && !__CRT_HAVE_DOS$vfwscanf) || __NO_STDSTREAMS) */
#endif /* !__local__c16scanf_l_defined */
