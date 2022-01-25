/* HASH CRC-32:0x85c50c08 */
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
#ifndef __local___stdio_common_vfc16printf_s_defined
#define __local___stdio_common_vfc16printf_s_defined
#include <__crt.h>
#include <features.h>
#if (defined(__CRT_HAVE___stdio_common_vfwprintf) && __SIZEOF_WCHAR_T__ == 2 && defined(__LIBCCALL_IS_LIBDCALL)) || defined(__CRT_HAVE_DOS$__stdio_common_vfwprintf) || ((defined(__CRT_HAVE_vfwprintf) || defined(__CRT_HAVE_file_wprinter) || defined(__CRT_HAVE_file_wprinter_unlocked) || defined(__CRT_HAVE_fputwc) || defined(__CRT_HAVE_putwc)) && __SIZEOF_WCHAR_T__ == 2) || (defined(__CRT_HAVE_vfwprintf) && __SIZEOF_WCHAR_T__ == 2 && defined(__LIBCCALL_IS_LIBDCALL)) || defined(__CRT_HAVE_DOS$vfwprintf) || ((defined(__CRT_HAVE_file_wprinter) || defined(__CRT_HAVE_file_wprinter_unlocked) || defined(__CRT_HAVE_fputwc) || defined(__CRT_HAVE_putwc)) && __SIZEOF_WCHAR_T__ == 2) || (defined(__CRT_HAVE_file_wprinter) && __SIZEOF_WCHAR_T__ == 2 && defined(__LIBCCALL_IS_LIBDCALL)) || defined(__CRT_HAVE_DOS$file_wprinter) || (defined(__CRT_HAVE_file_wprinter_unlocked) && __SIZEOF_WCHAR_T__ == 2 && defined(__LIBCCALL_IS_LIBDCALL)) || defined(__CRT_HAVE_DOS$file_wprinter_unlocked) || ((defined(__CRT_HAVE_fputwc) || defined(__CRT_HAVE_putwc)) && __SIZEOF_WCHAR_T__ == 2) || (defined(__CRT_HAVE_fputwc) && __SIZEOF_WCHAR_T__ == 2 && defined(__LIBCCALL_IS_LIBDCALL)) || defined(__CRT_HAVE_DOS$fputwc) || (defined(__CRT_HAVE_putwc) && __SIZEOF_WCHAR_T__ == 2 && defined(__LIBCCALL_IS_LIBDCALL)) || defined(__CRT_HAVE_DOS$putwc)
#include <hybrid/typecore.h>
__NAMESPACE_LOCAL_BEGIN
#ifndef __local___localdep___stdio_common_vfc16printf_defined
#define __local___localdep___stdio_common_vfc16printf_defined
#if defined(__CRT_HAVE___stdio_common_vfwprintf) && __SIZEOF_WCHAR_T__ == 2 && defined(__LIBCCALL_IS_LIBDCALL)
__CREDIRECT(__ATTR_NONNULL((2, 3)),__STDC_INT_AS_SSIZE_T,__NOTHROW_NCX,__localdep___stdio_common_vfc16printf,(__UINT64_TYPE__ __options, __FILE *__stream, __CHAR16_TYPE__ const *__format, __locale_t __locale, __builtin_va_list __args),__stdio_common_vfwprintf,(__options,__stream,__format,__locale,__args))
#elif defined(__CRT_HAVE_DOS$__stdio_common_vfwprintf)
__CREDIRECT_DOS(__ATTR_NONNULL((2, 3)),__STDC_INT_AS_SSIZE_T,__NOTHROW_NCX,__localdep___stdio_common_vfc16printf,(__UINT64_TYPE__ __options, __FILE *__stream, __CHAR16_TYPE__ const *__format, __locale_t __locale, __builtin_va_list __args),__stdio_common_vfwprintf,(__options,__stream,__format,__locale,__args))
#elif (defined(__CRT_HAVE_vfwprintf) || defined(__CRT_HAVE_file_wprinter) || defined(__CRT_HAVE_file_wprinter_unlocked) || defined(__CRT_HAVE_fputwc) || defined(__CRT_HAVE_putwc)) && __SIZEOF_WCHAR_T__ == 2
__NAMESPACE_LOCAL_END
#include <libc/local/corecrt_wstdio/__stdio_common_vfwprintf.h>
__NAMESPACE_LOCAL_BEGIN
#define __localdep___stdio_common_vfc16printf __NAMESPACE_LOCAL_TYPEHAX(__STDC_INT_AS_SSIZE_T(__LIBDCALL*)(__UINT64_TYPE__,__FILE *,__CHAR16_TYPE__ const *,__locale_t,__builtin_va_list),__STDC_INT_AS_SSIZE_T(__LIBDCALL&)(__UINT64_TYPE__,__FILE *,__CHAR16_TYPE__ const *,__locale_t,__builtin_va_list),__stdio_common_vfwprintf)
#elif (defined(__CRT_HAVE_vfwprintf) && __SIZEOF_WCHAR_T__ == 2 && defined(__LIBCCALL_IS_LIBDCALL)) || defined(__CRT_HAVE_DOS$vfwprintf) || ((defined(__CRT_HAVE_file_wprinter) || defined(__CRT_HAVE_file_wprinter_unlocked) || defined(__CRT_HAVE_fputwc) || defined(__CRT_HAVE_putwc)) && __SIZEOF_WCHAR_T__ == 2) || (defined(__CRT_HAVE_file_wprinter) && __SIZEOF_WCHAR_T__ == 2 && defined(__LIBCCALL_IS_LIBDCALL)) || defined(__CRT_HAVE_DOS$file_wprinter) || (defined(__CRT_HAVE_file_wprinter_unlocked) && __SIZEOF_WCHAR_T__ == 2 && defined(__LIBCCALL_IS_LIBDCALL)) || defined(__CRT_HAVE_DOS$file_wprinter_unlocked) || ((defined(__CRT_HAVE_fputwc) || defined(__CRT_HAVE_putwc)) && __SIZEOF_WCHAR_T__ == 2) || (defined(__CRT_HAVE_fputwc) && __SIZEOF_WCHAR_T__ == 2 && defined(__LIBCCALL_IS_LIBDCALL)) || defined(__CRT_HAVE_DOS$fputwc) || (defined(__CRT_HAVE_putwc) && __SIZEOF_WCHAR_T__ == 2 && defined(__LIBCCALL_IS_LIBDCALL)) || defined(__CRT_HAVE_DOS$putwc)
__NAMESPACE_LOCAL_END
#include <libc/local/parts.uchar.stdio/__stdio_common_vfc16printf.h>
__NAMESPACE_LOCAL_BEGIN
#define __localdep___stdio_common_vfc16printf __LIBC_LOCAL_NAME(__stdio_common_vfc16printf)
#else /* ... */
#undef __local___localdep___stdio_common_vfc16printf_defined
#endif /* !... */
#endif /* !__local___localdep___stdio_common_vfc16printf_defined */
__LOCAL_LIBC(__stdio_common_vfc16printf_s) __ATTR_NONNULL((2, 3)) __STDC_INT_AS_SSIZE_T
__NOTHROW_NCX(__LIBDCALL __LIBC_LOCAL_NAME(__stdio_common_vfc16printf_s))(__UINT64_TYPE__ __options, __FILE *__stream, __CHAR16_TYPE__ const *__format, __locale_t __locale, __builtin_va_list __args) {
	return (__NAMESPACE_LOCAL_SYM __localdep___stdio_common_vfc16printf)(__options, __stream, __format, __locale, __args);
}
__NAMESPACE_LOCAL_END
#ifndef __local___localdep___stdio_common_vfc16printf_s_defined
#define __local___localdep___stdio_common_vfc16printf_s_defined
#define __localdep___stdio_common_vfc16printf_s __LIBC_LOCAL_NAME(__stdio_common_vfc16printf_s)
#endif /* !__local___localdep___stdio_common_vfc16printf_s_defined */
#else /* (__CRT_HAVE___stdio_common_vfwprintf && __SIZEOF_WCHAR_T__ == 2 && __LIBCCALL_IS_LIBDCALL) || __CRT_HAVE_DOS$__stdio_common_vfwprintf || ((__CRT_HAVE_vfwprintf || __CRT_HAVE_file_wprinter || __CRT_HAVE_file_wprinter_unlocked || __CRT_HAVE_fputwc || __CRT_HAVE_putwc) && __SIZEOF_WCHAR_T__ == 2) || (__CRT_HAVE_vfwprintf && __SIZEOF_WCHAR_T__ == 2 && __LIBCCALL_IS_LIBDCALL) || __CRT_HAVE_DOS$vfwprintf || ((__CRT_HAVE_file_wprinter || __CRT_HAVE_file_wprinter_unlocked || __CRT_HAVE_fputwc || __CRT_HAVE_putwc) && __SIZEOF_WCHAR_T__ == 2) || (__CRT_HAVE_file_wprinter && __SIZEOF_WCHAR_T__ == 2 && __LIBCCALL_IS_LIBDCALL) || __CRT_HAVE_DOS$file_wprinter || (__CRT_HAVE_file_wprinter_unlocked && __SIZEOF_WCHAR_T__ == 2 && __LIBCCALL_IS_LIBDCALL) || __CRT_HAVE_DOS$file_wprinter_unlocked || ((__CRT_HAVE_fputwc || __CRT_HAVE_putwc) && __SIZEOF_WCHAR_T__ == 2) || (__CRT_HAVE_fputwc && __SIZEOF_WCHAR_T__ == 2 && __LIBCCALL_IS_LIBDCALL) || __CRT_HAVE_DOS$fputwc || (__CRT_HAVE_putwc && __SIZEOF_WCHAR_T__ == 2 && __LIBCCALL_IS_LIBDCALL) || __CRT_HAVE_DOS$putwc */
#undef __local___stdio_common_vfc16printf_s_defined
#endif /* (!__CRT_HAVE___stdio_common_vfwprintf || __SIZEOF_WCHAR_T__ != 2 || !__LIBCCALL_IS_LIBDCALL) && !__CRT_HAVE_DOS$__stdio_common_vfwprintf && ((!__CRT_HAVE_vfwprintf && !__CRT_HAVE_file_wprinter && !__CRT_HAVE_file_wprinter_unlocked && !__CRT_HAVE_fputwc && !__CRT_HAVE_putwc) || __SIZEOF_WCHAR_T__ != 2) && (!__CRT_HAVE_vfwprintf || __SIZEOF_WCHAR_T__ != 2 || !__LIBCCALL_IS_LIBDCALL) && !__CRT_HAVE_DOS$vfwprintf && ((!__CRT_HAVE_file_wprinter && !__CRT_HAVE_file_wprinter_unlocked && !__CRT_HAVE_fputwc && !__CRT_HAVE_putwc) || __SIZEOF_WCHAR_T__ != 2) && (!__CRT_HAVE_file_wprinter || __SIZEOF_WCHAR_T__ != 2 || !__LIBCCALL_IS_LIBDCALL) && !__CRT_HAVE_DOS$file_wprinter && (!__CRT_HAVE_file_wprinter_unlocked || __SIZEOF_WCHAR_T__ != 2 || !__LIBCCALL_IS_LIBDCALL) && !__CRT_HAVE_DOS$file_wprinter_unlocked && ((!__CRT_HAVE_fputwc && !__CRT_HAVE_putwc) || __SIZEOF_WCHAR_T__ != 2) && (!__CRT_HAVE_fputwc || __SIZEOF_WCHAR_T__ != 2 || !__LIBCCALL_IS_LIBDCALL) && !__CRT_HAVE_DOS$fputwc && (!__CRT_HAVE_putwc || __SIZEOF_WCHAR_T__ != 2 || !__LIBCCALL_IS_LIBDCALL) && !__CRT_HAVE_DOS$putwc */
#endif /* !__local___stdio_common_vfc16printf_s_defined */
