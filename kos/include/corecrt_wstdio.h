/* HASH CRC-32:0x97890195 */
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
/* (#) Portability: Windows Kits (/ucrt/corecrt_wstdio.h) */
#ifndef _CORECRT_WSTDIO_H
#define _CORECRT_WSTDIO_H 1

#include "__stdinc.h"
#include "__crt.h"

#ifdef __COMPILER_HAVE_PRAGMA_GCC_SYSTEM_HEADER
#pragma GCC system_header
#endif /* __COMPILER_HAVE_PRAGMA_GCC_SYSTEM_HEADER */

#include <features.h>
#include <hybrid/typecore.h>
#include <bits/types.h>
#include <asm/crt/stdio.h>
#include <corecrt_stdio_config.h>

#ifdef __CC__
__SYSDECL_BEGIN

#ifndef __size_t_defined
#define __size_t_defined
typedef __SIZE_TYPE__ size_t;
#endif /* !__size_t_defined */

#ifndef __ernno_t_defined
#define __ernno_t_defined
typedef __errno_t errno_t;
#endif /* !__ernno_t_defined */

#ifndef __wchar_t_defined
#define __wchar_t_defined
typedef __WCHAR_TYPE__ wchar_t;
#endif /* !__wchar_t_defined */

#ifndef __rsize_t_defined
#define __rsize_t_defined
typedef __SIZE_TYPE__ rsize_t;
#endif /* !__rsize_t_defined */

#ifndef WEOF
#define WEOF __WEOF
#endif /* !WEOF */

#ifndef __std_FILE_defined
#define __std_FILE_defined
__NAMESPACE_STD_BEGIN
typedef __FILE FILE;
__NAMESPACE_STD_END
#endif /* !__std_FILE_defined */
#ifndef __FILE_defined
#define __FILE_defined
__NAMESPACE_STD_USING(FILE)
#endif /* !__FILE_defined */

#ifndef _WSTDIO_DEFINED
#define _WSTDIO_DEFINED 1
/* Standard functions (already defined in <wchar.h>) */
#ifndef __fgetwc_defined
#define __fgetwc_defined
#ifdef __std_fgetwc_defined
/* >> fgetwc(3) */
__NAMESPACE_STD_USING(fgetwc)
#elif defined(__CRT_HAVE_fgetwc)
/* >> fgetwc(3) */
__CDECLARE(__ATTR_NONNULL((1)),wint_t,__THROWING,fgetwc,(FILE *__restrict __stream),(__stream))
#elif defined(__CRT_HAVE_getwc)
/* >> fgetwc(3) */
__CREDIRECT(__ATTR_NONNULL((1)),wint_t,__THROWING,fgetwc,(FILE *__restrict __stream),getwc,(__stream))
#else /* ... */
#undef __fgetwc_defined
#endif /* !... */
#endif /* !__fgetwc_defined */
#ifndef __fputwc_defined
#define __fputwc_defined
#ifdef __std_fputwc_defined
/* >> fputwc(3) */
__NAMESPACE_STD_USING(fputwc)
#elif defined(__CRT_HAVE_fputwc)
/* >> fputwc(3) */
__CDECLARE(__ATTR_NONNULL((2)),wint_t,__THROWING,fputwc,(wchar_t __wc, FILE *__stream),(__wc,__stream))
#elif defined(__CRT_HAVE_putwc)
/* >> fputwc(3) */
__CREDIRECT(__ATTR_NONNULL((2)),wint_t,__THROWING,fputwc,(wchar_t __wc, FILE *__stream),putwc,(__wc,__stream))
#else /* ... */
#undef __fputwc_defined
#endif /* !... */
#endif /* !__fputwc_defined */
#ifndef __getwc_defined
#define __getwc_defined
#ifdef __CRT_HAVE_fgetwc
/* >> fgetwc(3) */
__CREDIRECT(__ATTR_NONNULL((1)),wint_t,__THROWING,getwc,(FILE *__restrict __stream),fgetwc,(__stream))
#elif defined(__CRT_HAVE_getwc)
/* >> fgetwc(3) */
__CDECLARE(__ATTR_NONNULL((1)),wint_t,__THROWING,getwc,(FILE *__restrict __stream),(__stream))
#else /* ... */
#undef __getwc_defined
#endif /* !... */
#endif /* !__getwc_defined */
#ifndef __getwchar_defined
#define __getwchar_defined
#ifdef __std_getwchar_defined
/* >> getwchar(3) */
__NAMESPACE_STD_USING(getwchar)
#elif defined(__CRT_HAVE_getwchar)
/* >> getwchar(3) */
__CDECLARE(,wint_t,__THROWING,getwchar,(void),())
#elif defined(__CRT_HAVE__fgetwchar)
/* >> getwchar(3) */
__CREDIRECT(,wint_t,__THROWING,getwchar,(void),_fgetwchar,())
#else /* ... */
#include <__crt.h>
#if !defined(__NO_STDSTREAMS) && (defined(__CRT_HAVE_fgetwc) || defined(__CRT_HAVE_getwc))
#include <libc/local/wchar/getwchar.h>
/* >> getwchar(3) */
__NAMESPACE_LOCAL_USING_OR_IMPL(getwchar, __FORCELOCAL __ATTR_ARTIFICIAL wint_t (__LIBCCALL getwchar)(void) __THROWS(...) { return (__NAMESPACE_LOCAL_SYM __LIBC_LOCAL_NAME(getwchar))(); })
#else /* !__NO_STDSTREAMS && (__CRT_HAVE_fgetwc || __CRT_HAVE_getwc) */
#undef __getwchar_defined
#endif /* __NO_STDSTREAMS || (!__CRT_HAVE_fgetwc && !__CRT_HAVE_getwc) */
#endif /* !... */
#endif /* !__getwchar_defined */
#ifndef __putwc_defined
#define __putwc_defined
#ifdef __CRT_HAVE_fputwc
/* >> fputwc(3) */
__CREDIRECT(__ATTR_NONNULL((2)),wint_t,__THROWING,putwc,(wchar_t __wc, FILE *__stream),fputwc,(__wc,__stream))
#elif defined(__CRT_HAVE_putwc)
/* >> fputwc(3) */
__CDECLARE(__ATTR_NONNULL((2)),wint_t,__THROWING,putwc,(wchar_t __wc, FILE *__stream),(__wc,__stream))
#else /* ... */
#undef __putwc_defined
#endif /* !... */
#endif /* !__putwc_defined */
#ifndef __putwchar_defined
#define __putwchar_defined
#ifdef __std_putwchar_defined
/* >> putwchar(3) */
__NAMESPACE_STD_USING(putwchar)
#elif defined(__CRT_HAVE_putwchar)
/* >> putwchar(3) */
__CDECLARE(,wint_t,__THROWING,putwchar,(wchar_t __wc),(__wc))
#elif defined(__CRT_HAVE__fputwchar)
/* >> putwchar(3) */
__CREDIRECT(,wint_t,__THROWING,putwchar,(wchar_t __wc),_fputwchar,(__wc))
#else /* ... */
#include <__crt.h>
#if !defined(__NO_STDSTREAMS) && (defined(__CRT_HAVE_fputwc) || defined(__CRT_HAVE_putwc))
#include <libc/local/wchar/putwchar.h>
/* >> putwchar(3) */
__NAMESPACE_LOCAL_USING_OR_IMPL(putwchar, __FORCELOCAL __ATTR_ARTIFICIAL wint_t (__LIBCCALL putwchar)(wchar_t __wc) __THROWS(...) { return (__NAMESPACE_LOCAL_SYM __LIBC_LOCAL_NAME(putwchar))(__wc); })
#else /* !__NO_STDSTREAMS && (__CRT_HAVE_fputwc || __CRT_HAVE_putwc) */
#undef __putwchar_defined
#endif /* __NO_STDSTREAMS || (!__CRT_HAVE_fputwc && !__CRT_HAVE_putwc) */
#endif /* !... */
#endif /* !__putwchar_defined */
#ifndef __ungetwc_defined
#define __ungetwc_defined
#ifdef __std_ungetwc_defined
/* >> ungetwc(3) */
__NAMESPACE_STD_USING(ungetwc)
#elif defined(__CRT_HAVE_ungetwc_unlocked) && defined(__USE_STDIO_UNLOCKED)
/* >> ungetwc(3) */
__CREDIRECT(__ATTR_NONNULL((2)),wint_t,__NOTHROW_NCX,ungetwc,(wint_t __wc, FILE *__stream),ungetwc_unlocked,(__wc,__stream))
#elif defined(__CRT_HAVE_ungetwc)
/* >> ungetwc(3) */
__CDECLARE(__ATTR_NONNULL((2)),wint_t,__NOTHROW_NCX,ungetwc,(wint_t __wc, FILE *__stream),(__wc,__stream))
#elif defined(__CRT_HAVE_ungetwc_unlocked)
/* >> ungetwc(3) */
__CREDIRECT(__ATTR_NONNULL((2)),wint_t,__NOTHROW_NCX,ungetwc,(wint_t __wc, FILE *__stream),ungetwc_unlocked,(__wc,__stream))
#else /* ... */
#undef __ungetwc_defined
#endif /* !... */
#endif /* !__ungetwc_defined */
#ifndef __fgetws_defined
#define __fgetws_defined
#ifdef __std_fgetws_defined
/* >> fgetws(3) */
__NAMESPACE_STD_USING(fgetws)
#elif defined(__CRT_HAVE_fgetws_unlocked) && defined(__USE_STDIO_UNLOCKED)
/* >> fgetws(3) */
__CREDIRECT(__ATTR_WUNUSED __ATTR_NONNULL((1, 3)),wchar_t *,__THROWING,fgetws,(wchar_t *__restrict __buf, __STDC_INT_AS_SIZE_T __bufsize, FILE *__restrict __stream),fgetws_unlocked,(__buf,__bufsize,__stream))
#elif defined(__CRT_HAVE__fgetws_nolock) && defined(__USE_STDIO_UNLOCKED)
/* >> fgetws(3) */
__CREDIRECT(__ATTR_WUNUSED __ATTR_NONNULL((1, 3)),wchar_t *,__THROWING,fgetws,(wchar_t *__restrict __buf, __STDC_INT_AS_SIZE_T __bufsize, FILE *__restrict __stream),_fgetws_nolock,(__buf,__bufsize,__stream))
#elif defined(__CRT_HAVE_fgetws)
/* >> fgetws(3) */
__CDECLARE(__ATTR_WUNUSED __ATTR_NONNULL((1, 3)),wchar_t *,__THROWING,fgetws,(wchar_t *__restrict __buf, __STDC_INT_AS_SIZE_T __bufsize, FILE *__restrict __stream),(__buf,__bufsize,__stream))
#elif defined(__CRT_HAVE_fgetws_unlocked)
/* >> fgetws(3) */
__CREDIRECT(__ATTR_WUNUSED __ATTR_NONNULL((1, 3)),wchar_t *,__THROWING,fgetws,(wchar_t *__restrict __buf, __STDC_INT_AS_SIZE_T __bufsize, FILE *__restrict __stream),fgetws_unlocked,(__buf,__bufsize,__stream))
#elif defined(__CRT_HAVE__fgetws_nolock)
/* >> fgetws(3) */
__CREDIRECT(__ATTR_WUNUSED __ATTR_NONNULL((1, 3)),wchar_t *,__THROWING,fgetws,(wchar_t *__restrict __buf, __STDC_INT_AS_SIZE_T __bufsize, FILE *__restrict __stream),_fgetws_nolock,(__buf,__bufsize,__stream))
#elif (defined(__CRT_HAVE_fgetwc) || defined(__CRT_HAVE_getwc)) && (defined(__CRT_HAVE_ungetwc) || defined(__CRT_HAVE_ungetwc_unlocked)) && (defined(__CRT_HAVE_ferror) || defined(__CRT_HAVE__IO_ferror) || defined(__CRT_HAVE_ferror_unlocked))
#include <libc/local/wchar/fgetws.h>
/* >> fgetws(3) */
__NAMESPACE_LOCAL_USING_OR_IMPL(fgetws, __FORCELOCAL __ATTR_ARTIFICIAL __ATTR_WUNUSED __ATTR_NONNULL((1, 3)) wchar_t *(__LIBCCALL fgetws)(wchar_t *__restrict __buf, __STDC_INT_AS_SIZE_T __bufsize, FILE *__restrict __stream) __THROWS(...) { return (__NAMESPACE_LOCAL_SYM __LIBC_LOCAL_NAME(fgetws))(__buf, __bufsize, __stream); })
#else /* ... */
#undef __fgetws_defined
#endif /* !... */
#endif /* !__fgetws_defined */
#ifndef __fputws_defined
#define __fputws_defined
#ifdef __std_fputws_defined
/* >> fputws(3) */
__NAMESPACE_STD_USING(fputws)
#elif defined(__CRT_HAVE_fputws_unlocked) && defined(__USE_STDIO_UNLOCKED)
/* >> fputws(3) */
__CREDIRECT(__ATTR_NONNULL((1, 2)),__STDC_INT_AS_SIZE_T,__THROWING,fputws,(wchar_t const *__restrict __str, FILE *__restrict __stream),fputws_unlocked,(__str,__stream))
#elif defined(__CRT_HAVE__fputws_nolock) && defined(__USE_STDIO_UNLOCKED)
/* >> fputws(3) */
__CREDIRECT(__ATTR_NONNULL((1, 2)),__STDC_INT_AS_SIZE_T,__THROWING,fputws,(wchar_t const *__restrict __str, FILE *__restrict __stream),_fputws_nolock,(__str,__stream))
#elif defined(__CRT_HAVE_fputws)
/* >> fputws(3) */
__CDECLARE(__ATTR_NONNULL((1, 2)),__STDC_INT_AS_SIZE_T,__THROWING,fputws,(wchar_t const *__restrict __str, FILE *__restrict __stream),(__str,__stream))
#elif defined(__CRT_HAVE_fputws_unlocked)
/* >> fputws(3) */
__CREDIRECT(__ATTR_NONNULL((1, 2)),__STDC_INT_AS_SIZE_T,__THROWING,fputws,(wchar_t const *__restrict __str, FILE *__restrict __stream),fputws_unlocked,(__str,__stream))
#elif defined(__CRT_HAVE__fputws_nolock)
/* >> fputws(3) */
__CREDIRECT(__ATTR_NONNULL((1, 2)),__STDC_INT_AS_SIZE_T,__THROWING,fputws,(wchar_t const *__restrict __str, FILE *__restrict __stream),_fputws_nolock,(__str,__stream))
#elif defined(__CRT_HAVE_file_wprinter) || defined(__CRT_HAVE_file_wprinter_unlocked) || defined(__CRT_HAVE_fputwc) || defined(__CRT_HAVE_putwc)
#include <libc/local/wchar/fputws.h>
/* >> fputws(3) */
__NAMESPACE_LOCAL_USING_OR_IMPL(fputws, __FORCELOCAL __ATTR_ARTIFICIAL __ATTR_NONNULL((1, 2)) __STDC_INT_AS_SIZE_T (__LIBCCALL fputws)(wchar_t const *__restrict __str, FILE *__restrict __stream) __THROWS(...) { return (__NAMESPACE_LOCAL_SYM __LIBC_LOCAL_NAME(fputws))(__str, __stream); })
#else /* ... */
#undef __fputws_defined
#endif /* !... */
#endif /* !__fputws_defined */
#ifndef __vfwprintf_defined
#define __vfwprintf_defined
#ifdef __std_vfwprintf_defined
/* >> vfwprintf(3) */
__NAMESPACE_STD_USING(vfwprintf)
#elif defined(__CRT_HAVE_vfwprintf)
/* >> vfwprintf(3) */
__CDECLARE(__ATTR_LIBC_WPRINTF(2, 0) __ATTR_NONNULL((1, 2)),__STDC_INT_AS_SIZE_T,__THROWING,vfwprintf,(FILE *__restrict __stream, wchar_t const *__restrict __format, __builtin_va_list __args),(__stream,__format,__args))
#elif defined(__CRT_HAVE_file_wprinter) || defined(__CRT_HAVE_file_wprinter_unlocked) || defined(__CRT_HAVE_fputwc) || defined(__CRT_HAVE_putwc)
#include <libc/local/wchar/vfwprintf.h>
/* >> vfwprintf(3) */
__NAMESPACE_LOCAL_USING_OR_IMPL(vfwprintf, __FORCELOCAL __ATTR_ARTIFICIAL __ATTR_LIBC_WPRINTF(2, 0) __ATTR_NONNULL((1, 2)) __STDC_INT_AS_SIZE_T (__LIBCCALL vfwprintf)(FILE *__restrict __stream, wchar_t const *__restrict __format, __builtin_va_list __args) __THROWS(...) { return (__NAMESPACE_LOCAL_SYM __LIBC_LOCAL_NAME(vfwprintf))(__stream, __format, __args); })
#else /* ... */
#undef __vfwprintf_defined
#endif /* !... */
#endif /* !__vfwprintf_defined */
#ifndef __fwprintf_defined
#define __fwprintf_defined
#ifdef __std_fwprintf_defined
/* >> fwprintf(3) */
__NAMESPACE_STD_USING(fwprintf)
#elif defined(__CRT_HAVE_fwprintf)
/* >> fwprintf(3) */
__LIBC __ATTR_LIBC_WPRINTF(2, 3) __ATTR_NONNULL((1, 2)) __STDC_INT_AS_SIZE_T (__VLIBCCALL fwprintf)(FILE *__restrict __stream, wchar_t const *__restrict __format, ...) __THROWS(...) __CASMNAME_SAME("fwprintf");
#elif defined(__CRT_HAVE_vfwprintf) || defined(__CRT_HAVE_file_wprinter) || defined(__CRT_HAVE_file_wprinter_unlocked) || defined(__CRT_HAVE_fputwc) || defined(__CRT_HAVE_putwc)
#include <libc/local/wchar/fwprintf.h>
/* >> fwprintf(3) */
#ifdef __cplusplus
__NAMESPACE_LOCAL_USING_OR_IMPL(fwprintf, __FORCELOCAL __ATTR_ARTIFICIAL __ATTR_LIBC_WPRINTF(2, 3) __ATTR_NONNULL((1, 2)) __STDC_INT_AS_SIZE_T (__VLIBCCALL fwprintf)(FILE *__restrict __stream, wchar_t const *__restrict __format, ...) __THROWS(...) { return (__NAMESPACE_LOCAL_SYM __LIBC_LOCAL_NAME(fwprintf))(__stream, __format, __builtin_va_arg_pack()); })
#else /* __cplusplus */
#define fwprintf (__NAMESPACE_LOCAL_SYM __LIBC_LOCAL_NAME(fwprintf))
#endif /* !__cplusplus */
#else /* ... */
#undef __fwprintf_defined
#endif /* !... */
#endif /* !__fwprintf_defined */
#ifndef __vfwscanf_defined
#define __vfwscanf_defined
#ifdef __std_vfwscanf_defined
/* >> vfwscanf(3) */
__NAMESPACE_STD_USING(vfwscanf)
#elif defined(__CRT_HAVE_vfwscanf)
/* >> vfwscanf(3) */
__CDECLARE(__ATTR_WUNUSED __ATTR_LIBC_WSCANF(2, 0) __ATTR_NONNULL((1, 2)),__STDC_INT_AS_SIZE_T,__THROWING,vfwscanf,(FILE *__restrict __stream, wchar_t const *__restrict __format, __builtin_va_list __args),(__stream,__format,__args))
#else /* ... */
#undef __vfwscanf_defined
#endif /* !... */
#endif /* !__vfwscanf_defined */
#ifndef __fwscanf_defined
#define __fwscanf_defined
#ifdef __std_fwscanf_defined
/* >> fwscanf(3) */
__NAMESPACE_STD_USING(fwscanf)
#elif defined(__CRT_HAVE_fwscanf)
/* >> fwscanf(3) */
__LIBC __ATTR_LIBC_WSCANF(2, 3) __ATTR_NONNULL((1, 2)) __STDC_INT_AS_SIZE_T (__VLIBCCALL fwscanf)(FILE *__restrict __stream, wchar_t const *__restrict __format, ...) __THROWS(...) __CASMNAME_SAME("fwscanf");
#elif defined(__CRT_HAVE_vfwscanf)
#include <libc/local/wchar/fwscanf.h>
/* >> fwscanf(3) */
#ifdef __cplusplus
__NAMESPACE_LOCAL_USING_OR_IMPL(fwscanf, __FORCELOCAL __ATTR_ARTIFICIAL __ATTR_LIBC_WSCANF(2, 3) __ATTR_NONNULL((1, 2)) __STDC_INT_AS_SIZE_T (__VLIBCCALL fwscanf)(FILE *__restrict __stream, wchar_t const *__restrict __format, ...) __THROWS(...) { return (__NAMESPACE_LOCAL_SYM __LIBC_LOCAL_NAME(fwscanf))(__stream, __format, __builtin_va_arg_pack()); })
#else /* __cplusplus */
#define fwscanf (__NAMESPACE_LOCAL_SYM __LIBC_LOCAL_NAME(fwscanf))
#endif /* !__cplusplus */
#else /* ... */
#undef __fwscanf_defined
#endif /* !... */
#endif /* !__fwscanf_defined */
#ifndef __vwprintf_defined
#define __vwprintf_defined
#ifdef __std_vwprintf_defined
/* >> vwprintf(3) */
__NAMESPACE_STD_USING(vwprintf)
#elif defined(__CRT_HAVE_vwprintf)
/* >> vwprintf(3) */
__CDECLARE(__ATTR_LIBC_WPRINTF(1, 0) __ATTR_NONNULL((1)),__STDC_INT_AS_SIZE_T,__THROWING,vwprintf,(wchar_t const *__restrict __format, __builtin_va_list __args),(__format,__args))
#else /* ... */
#include <__crt.h>
#if (defined(__CRT_HAVE_vfwprintf) || defined(__CRT_HAVE_file_wprinter) || defined(__CRT_HAVE_file_wprinter_unlocked) || defined(__CRT_HAVE_fputwc) || defined(__CRT_HAVE_putwc)) && !defined(__NO_STDSTREAMS)
#include <libc/local/wchar/vwprintf.h>
/* >> vwprintf(3) */
__NAMESPACE_LOCAL_USING_OR_IMPL(vwprintf, __FORCELOCAL __ATTR_ARTIFICIAL __ATTR_LIBC_WPRINTF(1, 0) __ATTR_NONNULL((1)) __STDC_INT_AS_SIZE_T (__LIBCCALL vwprintf)(wchar_t const *__restrict __format, __builtin_va_list __args) __THROWS(...) { return (__NAMESPACE_LOCAL_SYM __LIBC_LOCAL_NAME(vwprintf))(__format, __args); })
#else /* (__CRT_HAVE_vfwprintf || __CRT_HAVE_file_wprinter || __CRT_HAVE_file_wprinter_unlocked || __CRT_HAVE_fputwc || __CRT_HAVE_putwc) && !__NO_STDSTREAMS */
#undef __vwprintf_defined
#endif /* (!__CRT_HAVE_vfwprintf && !__CRT_HAVE_file_wprinter && !__CRT_HAVE_file_wprinter_unlocked && !__CRT_HAVE_fputwc && !__CRT_HAVE_putwc) || __NO_STDSTREAMS */
#endif /* !... */
#endif /* !__vwprintf_defined */
#ifndef __wprintf_defined
#define __wprintf_defined
#ifdef __std_wprintf_defined
/* >> wprintf(3) */
__NAMESPACE_STD_USING(wprintf)
#elif defined(__CRT_HAVE_wprintf)
/* >> wprintf(3) */
__LIBC __ATTR_LIBC_WPRINTF(1, 2) __ATTR_NONNULL((1)) __STDC_INT_AS_SIZE_T (__VLIBCCALL wprintf)(wchar_t const *__restrict __format, ...) __THROWS(...) __CASMNAME_SAME("wprintf");
#else /* ... */
#include <__crt.h>
#if defined(__CRT_HAVE_vwprintf) || ((defined(__CRT_HAVE_vfwprintf) || defined(__CRT_HAVE_file_wprinter) || defined(__CRT_HAVE_file_wprinter_unlocked) || defined(__CRT_HAVE_fputwc) || defined(__CRT_HAVE_putwc)) && !defined(__NO_STDSTREAMS))
#include <libc/local/wchar/wprintf.h>
/* >> wprintf(3) */
#ifdef __cplusplus
__NAMESPACE_LOCAL_USING_OR_IMPL(wprintf, __FORCELOCAL __ATTR_ARTIFICIAL __ATTR_LIBC_WPRINTF(1, 2) __ATTR_NONNULL((1)) __STDC_INT_AS_SIZE_T (__VLIBCCALL wprintf)(wchar_t const *__restrict __format, ...) __THROWS(...) { return (__NAMESPACE_LOCAL_SYM __LIBC_LOCAL_NAME(wprintf))(__format, __builtin_va_arg_pack()); })
#else /* __cplusplus */
#define wprintf (__NAMESPACE_LOCAL_SYM __LIBC_LOCAL_NAME(wprintf))
#endif /* !__cplusplus */
#else /* __CRT_HAVE_vwprintf || ((__CRT_HAVE_vfwprintf || __CRT_HAVE_file_wprinter || __CRT_HAVE_file_wprinter_unlocked || __CRT_HAVE_fputwc || __CRT_HAVE_putwc) && !__NO_STDSTREAMS) */
#undef __wprintf_defined
#endif /* !__CRT_HAVE_vwprintf && ((!__CRT_HAVE_vfwprintf && !__CRT_HAVE_file_wprinter && !__CRT_HAVE_file_wprinter_unlocked && !__CRT_HAVE_fputwc && !__CRT_HAVE_putwc) || __NO_STDSTREAMS) */
#endif /* !... */
#endif /* !__wprintf_defined */
#ifndef __vwscanf_defined
#define __vwscanf_defined
#ifdef __std_vwscanf_defined
/* >> vwscanf(3) */
__NAMESPACE_STD_USING(vwscanf)
#elif defined(__CRT_HAVE_vwscanf)
/* >> vwscanf(3) */
__CDECLARE(__ATTR_WUNUSED __ATTR_LIBC_WSCANF(1, 0) __ATTR_NONNULL((1)),__STDC_INT_AS_SIZE_T,__THROWING,vwscanf,(wchar_t const *__restrict __format, __builtin_va_list __args),(__format,__args))
#else /* ... */
#include <__crt.h>
#if defined(__CRT_HAVE_vfwscanf) && !defined(__NO_STDSTREAMS)
#include <libc/local/wchar/vwscanf.h>
/* >> vwscanf(3) */
__NAMESPACE_LOCAL_USING_OR_IMPL(vwscanf, __FORCELOCAL __ATTR_ARTIFICIAL __ATTR_WUNUSED __ATTR_LIBC_WSCANF(1, 0) __ATTR_NONNULL((1)) __STDC_INT_AS_SIZE_T (__LIBCCALL vwscanf)(wchar_t const *__restrict __format, __builtin_va_list __args) __THROWS(...) { return (__NAMESPACE_LOCAL_SYM __LIBC_LOCAL_NAME(vwscanf))(__format, __args); })
#else /* __CRT_HAVE_vfwscanf && !__NO_STDSTREAMS */
#undef __vwscanf_defined
#endif /* !__CRT_HAVE_vfwscanf || __NO_STDSTREAMS */
#endif /* !... */
#endif /* !__vwscanf_defined */
#ifndef __wscanf_defined
#define __wscanf_defined
#ifdef __std_wscanf_defined
/* >> wscanf(3) */
__NAMESPACE_STD_USING(wscanf)
#elif defined(__CRT_HAVE_wscanf)
/* >> wscanf(3) */
__LIBC __ATTR_LIBC_WSCANF(1, 2) __ATTR_NONNULL((1)) __STDC_INT_AS_SIZE_T (__VLIBCCALL wscanf)(wchar_t const *__restrict __format, ...) __THROWS(...) __CASMNAME_SAME("wscanf");
#else /* ... */
#include <__crt.h>
#if defined(__CRT_HAVE_vwscanf) || (defined(__CRT_HAVE_vfwscanf) && !defined(__NO_STDSTREAMS))
#include <libc/local/wchar/wscanf.h>
/* >> wscanf(3) */
#ifdef __cplusplus
__NAMESPACE_LOCAL_USING_OR_IMPL(wscanf, __FORCELOCAL __ATTR_ARTIFICIAL __ATTR_LIBC_WSCANF(1, 2) __ATTR_NONNULL((1)) __STDC_INT_AS_SIZE_T (__VLIBCCALL wscanf)(wchar_t const *__restrict __format, ...) __THROWS(...) { return (__NAMESPACE_LOCAL_SYM __LIBC_LOCAL_NAME(wscanf))(__format, __builtin_va_arg_pack()); })
#else /* __cplusplus */
#define wscanf (__NAMESPACE_LOCAL_SYM __LIBC_LOCAL_NAME(wscanf))
#endif /* !__cplusplus */
#else /* __CRT_HAVE_vwscanf || (__CRT_HAVE_vfwscanf && !__NO_STDSTREAMS) */
#undef __wscanf_defined
#endif /* !__CRT_HAVE_vwscanf && (!__CRT_HAVE_vfwscanf || __NO_STDSTREAMS) */
#endif /* !... */
#endif /* !__wscanf_defined */
#ifndef __vswscanf_defined
#define __vswscanf_defined
#ifdef __std_vswscanf_defined
/* >> vswscanf(3) */
__NAMESPACE_STD_USING(vswscanf)
#elif defined(__CRT_HAVE_vswscanf)
/* >> vswscanf(3) */
__CDECLARE(__ATTR_WUNUSED __ATTR_LIBC_WSCANF(2, 0) __ATTR_NONNULL((1, 2)),__STDC_INT_AS_SIZE_T,__NOTHROW_NCX,vswscanf,(wchar_t const *__restrict __src, wchar_t const *__restrict __format, __builtin_va_list __args),(__src,__format,__args))
#else /* ... */
#include <libc/local/wchar/vswscanf.h>
/* >> vswscanf(3) */
__NAMESPACE_LOCAL_USING_OR_IMPL(vswscanf, __FORCELOCAL __ATTR_ARTIFICIAL __ATTR_WUNUSED __ATTR_LIBC_WSCANF(2, 0) __ATTR_NONNULL((1, 2)) __STDC_INT_AS_SIZE_T __NOTHROW_NCX(__LIBCCALL vswscanf)(wchar_t const *__restrict __src, wchar_t const *__restrict __format, __builtin_va_list __args) { return (__NAMESPACE_LOCAL_SYM __LIBC_LOCAL_NAME(vswscanf))(__src, __format, __args); })
#endif /* !... */
#endif /* !__vswscanf_defined */
#ifndef __swscanf_defined
#define __swscanf_defined
#ifdef __std_swscanf_defined
/* >> swscanf(3) */
__NAMESPACE_STD_USING(swscanf)
#elif defined(__CRT_HAVE_swscanf)
/* >> swscanf(3) */
__LIBC __ATTR_LIBC_WSCANF(2, 3) __ATTR_NONNULL((1, 2)) __STDC_INT_AS_SIZE_T __NOTHROW_NCX(__VLIBCCALL swscanf)(wchar_t const *__restrict __src, wchar_t const *__restrict __format, ...) __CASMNAME_SAME("swscanf");
#else /* ... */
#include <libc/local/wchar/swscanf.h>
/* >> swscanf(3) */
#ifdef __cplusplus
__NAMESPACE_LOCAL_USING_OR_IMPL(swscanf, __FORCELOCAL __ATTR_ARTIFICIAL __ATTR_LIBC_WSCANF(2, 3) __ATTR_NONNULL((1, 2)) __STDC_INT_AS_SIZE_T __NOTHROW_NCX(__VLIBCCALL swscanf)(wchar_t const *__restrict __src, wchar_t const *__restrict __format, ...) { return (__NAMESPACE_LOCAL_SYM __LIBC_LOCAL_NAME(swscanf))(__src, __format, __builtin_va_arg_pack()); })
#else /* __cplusplus */
#define swscanf (__NAMESPACE_LOCAL_SYM __LIBC_LOCAL_NAME(swscanf))
#endif /* !__cplusplus */
#endif /* !... */
#endif /* !__swscanf_defined */



/* DOS-specific aliases for differently named functions */
#ifdef __CRT_HAVE_wfopen
__CREDIRECT(__ATTR_WUNUSED __ATTR_NONNULL((1, 2)),__FILE *,__NOTHROW_NCX,_wfopen,(wchar_t const *__filename, wchar_t const *__mode),wfopen,(__filename,__mode))
#elif defined(__CRT_HAVE__wfopen)
__CDECLARE(__ATTR_WUNUSED __ATTR_NONNULL((1, 2)),__FILE *,__NOTHROW_NCX,_wfopen,(wchar_t const *__filename, wchar_t const *__mode),(__filename,__mode))
#else /* ... */
#include <asm/os/oflags.h>
#if ((defined(__CRT_HAVE_fopen) && (!defined(__USE_FILE_OFFSET64) || !defined(__O_LARGEFILE) || !__O_LARGEFILE)) || (defined(__CRT_HAVE__IO_fopen) && (!defined(__USE_FILE_OFFSET64) || !defined(__O_LARGEFILE) || !__O_LARGEFILE)) || defined(__CRT_HAVE_fopen64)) && (defined(__CRT_HAVE_convert_wcstombs) || defined(__CRT_HAVE_convert_wcstombsn) || defined(__CRT_HAVE_format_aprintf_printer) || defined(__CRT_HAVE_format_aprintf_alloc) || defined(__CRT_HAVE_realloc) || defined(__CRT_HAVE___libc_realloc))
#include <libc/local/parts.wchar.stdio/wfopen.h>
__FORCELOCAL __ATTR_ARTIFICIAL __ATTR_WUNUSED __ATTR_NONNULL((1, 2)) __FILE *__NOTHROW_NCX(__LIBCCALL _wfopen)(wchar_t const *__filename, wchar_t const *__mode) { return (__NAMESPACE_LOCAL_SYM __LIBC_LOCAL_NAME(wfopen))(__filename, __mode); }
#endif /* ((__CRT_HAVE_fopen && (!__USE_FILE_OFFSET64 || !__O_LARGEFILE || !__O_LARGEFILE)) || (__CRT_HAVE__IO_fopen && (!__USE_FILE_OFFSET64 || !__O_LARGEFILE || !__O_LARGEFILE)) || __CRT_HAVE_fopen64) && (__CRT_HAVE_convert_wcstombs || __CRT_HAVE_convert_wcstombsn || __CRT_HAVE_format_aprintf_printer || __CRT_HAVE_format_aprintf_alloc || __CRT_HAVE_realloc || __CRT_HAVE___libc_realloc) */
#endif /* !... */
#ifdef __CRT_HAVE_wfreopen
__CREDIRECT(__ATTR_WUNUSED __ATTR_NONNULL((1, 2)),__FILE *,__NOTHROW_NCX,_wfreopen,(wchar_t const *__filename, wchar_t const *__mode, __FILE *__stream),wfreopen,(__filename,__mode,__stream))
#elif defined(__CRT_HAVE__wfreopen)
__CDECLARE(__ATTR_WUNUSED __ATTR_NONNULL((1, 2)),__FILE *,__NOTHROW_NCX,_wfreopen,(wchar_t const *__filename, wchar_t const *__mode, __FILE *__stream),(__filename,__mode,__stream))
#else /* ... */
#include <asm/os/oflags.h>
#if ((defined(__CRT_HAVE_freopen) && (!defined(__USE_FILE_OFFSET64) || !defined(__O_LARGEFILE) || !__O_LARGEFILE)) || defined(__CRT_HAVE_freopen64) || (defined(__CRT_HAVE_freopen_unlocked) && (!defined(__USE_FILE_OFFSET64) || !defined(__O_LARGEFILE) || !__O_LARGEFILE)) || defined(__CRT_HAVE_freopen64_unlocked)) && (defined(__CRT_HAVE_convert_wcstombs) || defined(__CRT_HAVE_convert_wcstombsn) || defined(__CRT_HAVE_format_aprintf_printer) || defined(__CRT_HAVE_format_aprintf_alloc) || defined(__CRT_HAVE_realloc) || defined(__CRT_HAVE___libc_realloc))
#include <libc/local/parts.wchar.stdio/wfreopen.h>
__FORCELOCAL __ATTR_ARTIFICIAL __ATTR_WUNUSED __ATTR_NONNULL((1, 2)) __FILE *__NOTHROW_NCX(__LIBCCALL _wfreopen)(wchar_t const *__filename, wchar_t const *__mode, __FILE *__stream) { return (__NAMESPACE_LOCAL_SYM __LIBC_LOCAL_NAME(wfreopen))(__filename, __mode, __stream); }
#endif /* ((__CRT_HAVE_freopen && (!__USE_FILE_OFFSET64 || !__O_LARGEFILE || !__O_LARGEFILE)) || __CRT_HAVE_freopen64 || (__CRT_HAVE_freopen_unlocked && (!__USE_FILE_OFFSET64 || !__O_LARGEFILE || !__O_LARGEFILE)) || __CRT_HAVE_freopen64_unlocked) && (__CRT_HAVE_convert_wcstombs || __CRT_HAVE_convert_wcstombsn || __CRT_HAVE_format_aprintf_printer || __CRT_HAVE_format_aprintf_alloc || __CRT_HAVE_realloc || __CRT_HAVE___libc_realloc) */
#endif /* !... */
#ifdef __CRT_HAVE_getwchar
/* >> getwchar(3) */
__CREDIRECT(,wint_t,__THROWING,_fgetwchar,(void),getwchar,())
#elif defined(__CRT_HAVE__fgetwchar)
/* >> getwchar(3) */
__CDECLARE(,wint_t,__THROWING,_fgetwchar,(void),())
#else /* ... */
#include <__crt.h>
#if !defined(__NO_STDSTREAMS) && (defined(__CRT_HAVE_fgetwc) || defined(__CRT_HAVE_getwc))
#include <libc/local/wchar/getwchar.h>
/* >> getwchar(3) */
__FORCELOCAL __ATTR_ARTIFICIAL wint_t (__LIBCCALL _fgetwchar)(void) __THROWS(...) { return (__NAMESPACE_LOCAL_SYM __LIBC_LOCAL_NAME(getwchar))(); }
#endif /* !__NO_STDSTREAMS && (__CRT_HAVE_fgetwc || __CRT_HAVE_getwc) */
#endif /* !... */
#ifdef __CRT_HAVE_putwchar
/* >> putwchar(3) */
__CREDIRECT(,wint_t,__THROWING,_fputwchar,(wchar_t __wc),putwchar,(__wc))
#elif defined(__CRT_HAVE__fputwchar)
/* >> putwchar(3) */
__CDECLARE(,wint_t,__THROWING,_fputwchar,(wchar_t __wc),(__wc))
#else /* ... */
#include <__crt.h>
#if !defined(__NO_STDSTREAMS) && (defined(__CRT_HAVE_fputwc) || defined(__CRT_HAVE_putwc))
#include <libc/local/wchar/putwchar.h>
/* >> putwchar(3) */
__FORCELOCAL __ATTR_ARTIFICIAL wint_t (__LIBCCALL _fputwchar)(wchar_t __wc) __THROWS(...) { return (__NAMESPACE_LOCAL_SYM __LIBC_LOCAL_NAME(putwchar))(__wc); }
#endif /* !__NO_STDSTREAMS && (__CRT_HAVE_fputwc || __CRT_HAVE_putwc) */
#endif /* !... */
#ifdef __CRT_HAVE_wpopen
__CREDIRECT(__ATTR_WUNUSED __ATTR_NONNULL((1, 2)),__FILE *,__NOTHROW_NCX,_wpopen,(wchar_t const *__command, wchar_t const *__mode),wpopen,(__command,__mode))
#elif defined(__CRT_HAVE__wpopen)
__CDECLARE(__ATTR_WUNUSED __ATTR_NONNULL((1, 2)),__FILE *,__NOTHROW_NCX,_wpopen,(wchar_t const *__command, wchar_t const *__mode),(__command,__mode))
#elif (defined(__CRT_HAVE_popen) || defined(__CRT_HAVE__popen) || defined(__CRT_HAVE__IO_popen)) && (defined(__CRT_HAVE_convert_wcstombs) || defined(__CRT_HAVE_convert_wcstombsn) || defined(__CRT_HAVE_format_aprintf_printer) || defined(__CRT_HAVE_format_aprintf_alloc) || defined(__CRT_HAVE_realloc) || defined(__CRT_HAVE___libc_realloc))
#include <libc/local/parts.wchar.stdio/wpopen.h>
__FORCELOCAL __ATTR_ARTIFICIAL __ATTR_WUNUSED __ATTR_NONNULL((1, 2)) __FILE *__NOTHROW_NCX(__LIBCCALL _wpopen)(wchar_t const *__command, wchar_t const *__mode) { return (__NAMESPACE_LOCAL_SYM __LIBC_LOCAL_NAME(wpopen))(__command, __mode); }
#endif /* ... */
#ifdef __CRT_HAVE_wremove
__CREDIRECT(__ATTR_NONNULL((1)),int,__NOTHROW_RPC,_wremove,(wchar_t const *__filename),wremove,(__filename))
#elif defined(__CRT_HAVE__wremove)
__CDECLARE(__ATTR_NONNULL((1)),int,__NOTHROW_RPC,_wremove,(wchar_t const *__filename),(__filename))
#else /* ... */
#include <asm/os/fcntl.h>
#include <asm/os/errno.h>
#if (defined(__AT_FDCWD) && (defined(__CRT_HAVE_wremoveat) || ((defined(__CRT_HAVE_removeat) || (defined(__AT_REMOVEDIR) && defined(__CRT_HAVE_unlinkat))) && (defined(__CRT_HAVE_convert_wcstombs) || defined(__CRT_HAVE_convert_wcstombsn) || defined(__CRT_HAVE_format_aprintf_printer) || defined(__CRT_HAVE_format_aprintf_alloc) || defined(__CRT_HAVE_realloc) || defined(__CRT_HAVE___libc_realloc))))) || ((defined(__CRT_HAVE_remove) || (defined(__AT_FDCWD) && (defined(__CRT_HAVE_removeat) || (defined(__AT_REMOVEDIR) && defined(__CRT_HAVE_unlinkat)))) || (defined(__EISDIR) && defined(__ENOTDIR) && (defined(__CRT_HAVE_unlink) || defined(__CRT_HAVE__unlink) || defined(__CRT_HAVE___unlink) || defined(__CRT_HAVE___libc_unlink) || (defined(__AT_FDCWD) && defined(__CRT_HAVE_unlinkat))) && (defined(__CRT_HAVE_rmdir) || defined(__CRT_HAVE__rmdir) || defined(__CRT_HAVE___rmdir) || defined(__CRT_HAVE___libc_rmdir) || (defined(__AT_FDCWD) && defined(__AT_REMOVEDIR) && defined(__CRT_HAVE_unlinkat))))) && (defined(__CRT_HAVE_convert_wcstombs) || defined(__CRT_HAVE_convert_wcstombsn) || defined(__CRT_HAVE_format_aprintf_printer) || defined(__CRT_HAVE_format_aprintf_alloc) || defined(__CRT_HAVE_realloc) || defined(__CRT_HAVE___libc_realloc)))
#include <libc/local/parts.wchar.stdio/wremove.h>
__FORCELOCAL __ATTR_ARTIFICIAL __ATTR_NONNULL((1)) int __NOTHROW_RPC(__LIBCCALL _wremove)(wchar_t const *__filename) { return (__NAMESPACE_LOCAL_SYM __LIBC_LOCAL_NAME(wremove))(__filename); }
#endif /* (__AT_FDCWD && (__CRT_HAVE_wremoveat || ((__CRT_HAVE_removeat || (__AT_REMOVEDIR && __CRT_HAVE_unlinkat)) && (__CRT_HAVE_convert_wcstombs || __CRT_HAVE_convert_wcstombsn || __CRT_HAVE_format_aprintf_printer || __CRT_HAVE_format_aprintf_alloc || __CRT_HAVE_realloc || __CRT_HAVE___libc_realloc)))) || ((__CRT_HAVE_remove || (__AT_FDCWD && (__CRT_HAVE_removeat || (__AT_REMOVEDIR && __CRT_HAVE_unlinkat))) || (__EISDIR && __ENOTDIR && (__CRT_HAVE_unlink || __CRT_HAVE__unlink || __CRT_HAVE___unlink || __CRT_HAVE___libc_unlink || (__AT_FDCWD && __CRT_HAVE_unlinkat)) && (__CRT_HAVE_rmdir || __CRT_HAVE__rmdir || __CRT_HAVE___rmdir || __CRT_HAVE___libc_rmdir || (__AT_FDCWD && __AT_REMOVEDIR && __CRT_HAVE_unlinkat)))) && (__CRT_HAVE_convert_wcstombs || __CRT_HAVE_convert_wcstombsn || __CRT_HAVE_format_aprintf_printer || __CRT_HAVE_format_aprintf_alloc || __CRT_HAVE_realloc || __CRT_HAVE___libc_realloc)) */
#endif /* !... */
#ifdef __CRT_HAVE_fgetwc_unlocked
/* >> getwc_unlocked(3), fgetwc_unlocked(3) */
__CREDIRECT(__ATTR_NONNULL((1)),__WINT_TYPE__,__THROWING,_fgetwc_nolock,(__FILE *__restrict __stream),fgetwc_unlocked,(__stream))
#elif defined(__CRT_HAVE__fgetwc_nolock)
/* >> getwc_unlocked(3), fgetwc_unlocked(3) */
__CDECLARE(__ATTR_NONNULL((1)),__WINT_TYPE__,__THROWING,_fgetwc_nolock,(__FILE *__restrict __stream),(__stream))
#endif /* ... */
#ifdef __CRT_HAVE_fputwc_unlocked
/* >> putwc_unlocked(3), fputwc_unlocked(3) */
__CREDIRECT(__ATTR_NONNULL((2)),__WINT_TYPE__,__THROWING,_fputwc_nolock,(wchar_t __wc, __FILE *__restrict __stream),fputwc_unlocked,(__wc,__stream))
#elif defined(__CRT_HAVE__fputwc_nolock)
/* >> putwc_unlocked(3), fputwc_unlocked(3) */
__CDECLARE(__ATTR_NONNULL((2)),__WINT_TYPE__,__THROWING,_fputwc_nolock,(wchar_t __wc, __FILE *__restrict __stream),(__wc,__stream))
#endif /* ... */
#ifdef __CRT_HAVE_ungetwc_unlocked
/* >> ungetwc_unlocked(3) */
__CREDIRECT(__ATTR_NONNULL((2)),__WINT_TYPE__,__NOTHROW_NCX,_ungetwc_nolock,(__WINT_TYPE__ __ch, __FILE *__restrict __stream),ungetwc_unlocked,(__ch,__stream))
#elif defined(__CRT_HAVE__ungetwc_nolock)
/* >> ungetwc_unlocked(3) */
__CDECLARE(__ATTR_NONNULL((2)),__WINT_TYPE__,__NOTHROW_NCX,_ungetwc_nolock,(__WINT_TYPE__ __ch, __FILE *__restrict __stream),(__ch,__stream))
#elif defined(__CRT_HAVE_ungetwc)
/* >> ungetwc_unlocked(3) */
__CREDIRECT(__ATTR_NONNULL((2)),__WINT_TYPE__,__NOTHROW_NCX,_ungetwc_nolock,(__WINT_TYPE__ __ch, __FILE *__restrict __stream),ungetwc,(__ch,__stream))
#endif /* ... */
#ifdef __CRT_HAVE_fgetwc_unlocked
/* >> getwc_unlocked(3), fgetwc_unlocked(3) */
__CREDIRECT(__ATTR_NONNULL((1)),__WINT_TYPE__,__THROWING,_getwc_nolock,(__FILE *__restrict __stream),fgetwc_unlocked,(__stream))
#elif defined(__CRT_HAVE__fgetwc_nolock)
/* >> getwc_unlocked(3), fgetwc_unlocked(3) */
__CREDIRECT(__ATTR_NONNULL((1)),__WINT_TYPE__,__THROWING,_getwc_nolock,(__FILE *__restrict __stream),_fgetwc_nolock,(__stream))
#endif /* ... */
#ifdef __CRT_HAVE_fputwc_unlocked
/* >> putwc_unlocked(3), fputwc_unlocked(3) */
__CREDIRECT(__ATTR_NONNULL((2)),__WINT_TYPE__,__THROWING,_putwc_nolock,(wchar_t __wc, __FILE *__restrict __stream),fputwc_unlocked,(__wc,__stream))
#elif defined(__CRT_HAVE__fputwc_nolock)
/* >> putwc_unlocked(3), fputwc_unlocked(3) */
__CREDIRECT(__ATTR_NONNULL((2)),__WINT_TYPE__,__THROWING,_putwc_nolock,(wchar_t __wc, __FILE *__restrict __stream),_fputwc_nolock,(__wc,__stream))
#endif /* ... */
#ifndef _CRT_WPERROR_DEFINED
#define _CRT_WPERROR_DEFINED
#ifdef __CRT_HAVE__wperror
__CDECLARE_VOID(__ATTR_COLD,__THROWING,_wperror,(wchar_t const *__restrict __message),(__message))
#else /* __CRT_HAVE__wperror */
#include <__crt.h>
#include <libc/errno.h>
#if !defined(__NO_STDSTREAMS) && defined(__libc_geterrno) && (defined(__CRT_HAVE_fprintf) || defined(__CRT_HAVE__IO_fprintf) || defined(__CRT_HAVE_fprintf_s) || defined(__CRT_HAVE_fprintf_unlocked) || defined(__CRT_HAVE_vfprintf) || defined(__CRT_HAVE_vfprintf_s) || defined(__CRT_HAVE__IO_vfprintf) || defined(__CRT_HAVE_vfprintf_unlocked) || defined(__CRT_HAVE_file_printer) || defined(__CRT_HAVE_file_printer_unlocked) || defined(__CRT_HAVE_fputc) || defined(__CRT_HAVE_putc) || defined(__CRT_HAVE__IO_putc) || defined(__CRT_HAVE_fputc_unlocked) || defined(__CRT_HAVE_putc_unlocked) || (defined(__CRT_DOS) && defined(__CRT_HAVE__flsbuf)) || defined(__CRT_HAVE_fwrite) || defined(__CRT_HAVE__IO_fwrite) || defined(__CRT_HAVE_fwrite_s) || defined(__CRT_HAVE_fwrite_unlocked) || defined(__CRT_HAVE__fwrite_nolock))
#include <libc/local/corecrt_wstdio/_wperror.h>
__NAMESPACE_LOCAL_USING_OR_IMPL(_wperror, __FORCELOCAL __ATTR_ARTIFICIAL __ATTR_COLD void (__LIBCCALL _wperror)(wchar_t const *__restrict __message) __THROWS(...) { (__NAMESPACE_LOCAL_SYM __LIBC_LOCAL_NAME(_wperror))(__message); })
#else /* !__NO_STDSTREAMS && __libc_geterrno && (__CRT_HAVE_fprintf || __CRT_HAVE__IO_fprintf || __CRT_HAVE_fprintf_s || __CRT_HAVE_fprintf_unlocked || __CRT_HAVE_vfprintf || __CRT_HAVE_vfprintf_s || __CRT_HAVE__IO_vfprintf || __CRT_HAVE_vfprintf_unlocked || __CRT_HAVE_file_printer || __CRT_HAVE_file_printer_unlocked || __CRT_HAVE_fputc || __CRT_HAVE_putc || __CRT_HAVE__IO_putc || __CRT_HAVE_fputc_unlocked || __CRT_HAVE_putc_unlocked || (__CRT_DOS && __CRT_HAVE__flsbuf) || __CRT_HAVE_fwrite || __CRT_HAVE__IO_fwrite || __CRT_HAVE_fwrite_s || __CRT_HAVE_fwrite_unlocked || __CRT_HAVE__fwrite_nolock) */
#undef _CRT_WPERROR_DEFINED
#endif /* __NO_STDSTREAMS || !__libc_geterrno || (!__CRT_HAVE_fprintf && !__CRT_HAVE__IO_fprintf && !__CRT_HAVE_fprintf_s && !__CRT_HAVE_fprintf_unlocked && !__CRT_HAVE_vfprintf && !__CRT_HAVE_vfprintf_s && !__CRT_HAVE__IO_vfprintf && !__CRT_HAVE_vfprintf_unlocked && !__CRT_HAVE_file_printer && !__CRT_HAVE_file_printer_unlocked && !__CRT_HAVE_fputc && !__CRT_HAVE_putc && !__CRT_HAVE__IO_putc && !__CRT_HAVE_fputc_unlocked && !__CRT_HAVE_putc_unlocked && (!__CRT_DOS || !__CRT_HAVE__flsbuf) && !__CRT_HAVE_fwrite && !__CRT_HAVE__IO_fwrite && !__CRT_HAVE_fwrite_s && !__CRT_HAVE_fwrite_unlocked && !__CRT_HAVE__fwrite_nolock) */
#endif /* !__CRT_HAVE__wperror */
#endif /* !_CRT_WPERROR_DEFINED */



/* DOS-specific functions */
#ifdef __CRT_HAVE__wfsopen
__CDECLARE(__ATTR_WUNUSED __ATTR_NONNULL((1, 2)),FILE *,__NOTHROW_NCX,_wfsopen,(wchar_t const *__filename, wchar_t const *__mode, __STDC_INT_AS_UINT_T __sh_flag),(__filename,__mode,__sh_flag))
#else /* __CRT_HAVE__wfsopen */
#include <asm/os/oflags.h>
#if (defined(__CRT_HAVE__fsopen) || (defined(__CRT_HAVE_fopen) && (!defined(__USE_FILE_OFFSET64) || !defined(__O_LARGEFILE) || !__O_LARGEFILE)) || (defined(__CRT_HAVE__IO_fopen) && (!defined(__USE_FILE_OFFSET64) || !defined(__O_LARGEFILE) || !__O_LARGEFILE)) || defined(__CRT_HAVE_fopen64)) && (defined(__CRT_HAVE_convert_wcstombs) || defined(__CRT_HAVE_convert_wcstombsn) || defined(__CRT_HAVE_format_aprintf_printer) || defined(__CRT_HAVE_format_aprintf_alloc) || defined(__CRT_HAVE_realloc) || defined(__CRT_HAVE___libc_realloc))
#include <libc/local/corecrt_wstdio/_wfsopen.h>
__NAMESPACE_LOCAL_USING_OR_IMPL(_wfsopen, __FORCELOCAL __ATTR_ARTIFICIAL __ATTR_WUNUSED __ATTR_NONNULL((1, 2)) FILE *__NOTHROW_NCX(__LIBCCALL _wfsopen)(wchar_t const *__filename, wchar_t const *__mode, __STDC_INT_AS_UINT_T __sh_flag) { return (__NAMESPACE_LOCAL_SYM __LIBC_LOCAL_NAME(_wfsopen))(__filename, __mode, __sh_flag); })
#endif /* (__CRT_HAVE__fsopen || (__CRT_HAVE_fopen && (!__USE_FILE_OFFSET64 || !__O_LARGEFILE || !__O_LARGEFILE)) || (__CRT_HAVE__IO_fopen && (!__USE_FILE_OFFSET64 || !__O_LARGEFILE || !__O_LARGEFILE)) || __CRT_HAVE_fopen64) && (__CRT_HAVE_convert_wcstombs || __CRT_HAVE_convert_wcstombsn || __CRT_HAVE_format_aprintf_printer || __CRT_HAVE_format_aprintf_alloc || __CRT_HAVE_realloc || __CRT_HAVE___libc_realloc) */
#endif /* !__CRT_HAVE__wfsopen */
#ifdef __CRT_HAVE__wfdopen
__CDECLARE(__ATTR_WUNUSED __ATTR_NONNULL((2)),FILE *,__NOTHROW_NCX,_wfdopen,(__fd_t __fd, wchar_t const *__mode),(__fd,__mode))
#elif (defined(__CRT_HAVE_fdopen) || defined(__CRT_HAVE__fdopen) || defined(__CRT_HAVE__IO_fdopen)) && (defined(__CRT_HAVE_convert_wcstombs) || defined(__CRT_HAVE_convert_wcstombsn) || defined(__CRT_HAVE_format_aprintf_printer) || defined(__CRT_HAVE_format_aprintf_alloc) || defined(__CRT_HAVE_realloc) || defined(__CRT_HAVE___libc_realloc))
#include <libc/local/corecrt_wstdio/_wfdopen.h>
__NAMESPACE_LOCAL_USING_OR_IMPL(_wfdopen, __FORCELOCAL __ATTR_ARTIFICIAL __ATTR_WUNUSED __ATTR_NONNULL((2)) FILE *__NOTHROW_NCX(__LIBCCALL _wfdopen)(__fd_t __fd, wchar_t const *__mode) { return (__NAMESPACE_LOCAL_SYM __LIBC_LOCAL_NAME(_wfdopen))(__fd, __mode); })
#endif /* ... */
#ifdef __CRT_HAVE__wfopen_s
__CDECLARE(__ATTR_NONNULL((1, 2, 3)),__errno_t,__NOTHROW_NCX,_wfopen_s,(FILE **__pstream, wchar_t const *__filename, wchar_t const *__mode),(__pstream,__filename,__mode))
#else /* __CRT_HAVE__wfopen_s */
#include <asm/os/oflags.h>
#if (defined(__CRT_HAVE_fopen_s) || (defined(__CRT_HAVE_fopen) && (!defined(__O_LARGEFILE) || !__O_LARGEFILE)) || defined(__CRT_HAVE_fopen64)) && (defined(__CRT_HAVE_convert_wcstombs) || defined(__CRT_HAVE_convert_wcstombsn) || defined(__CRT_HAVE_format_aprintf_printer) || defined(__CRT_HAVE_format_aprintf_alloc) || defined(__CRT_HAVE_realloc) || defined(__CRT_HAVE___libc_realloc))
#include <libc/local/corecrt_wstdio/_wfopen_s.h>
__NAMESPACE_LOCAL_USING_OR_IMPL(_wfopen_s, __FORCELOCAL __ATTR_ARTIFICIAL __ATTR_NONNULL((1, 2, 3)) __errno_t __NOTHROW_NCX(__LIBCCALL _wfopen_s)(FILE **__pstream, wchar_t const *__filename, wchar_t const *__mode) { return (__NAMESPACE_LOCAL_SYM __LIBC_LOCAL_NAME(_wfopen_s))(__pstream, __filename, __mode); })
#endif /* (__CRT_HAVE_fopen_s || (__CRT_HAVE_fopen && (!__O_LARGEFILE || !__O_LARGEFILE)) || __CRT_HAVE_fopen64) && (__CRT_HAVE_convert_wcstombs || __CRT_HAVE_convert_wcstombsn || __CRT_HAVE_format_aprintf_printer || __CRT_HAVE_format_aprintf_alloc || __CRT_HAVE_realloc || __CRT_HAVE___libc_realloc) */
#endif /* !__CRT_HAVE__wfopen_s */
#ifdef __CRT_HAVE__wfreopen_s
__CDECLARE(__ATTR_NONNULL((1, 2, 3)),__errno_t,__NOTHROW_NCX,_wfreopen_s,(FILE **__pstream, wchar_t const *__filename, wchar_t const *__mode, FILE *__stream),(__pstream,__filename,__mode,__stream))
#else /* __CRT_HAVE__wfreopen_s */
#include <asm/os/oflags.h>
#if (defined(__CRT_HAVE_freopen_s) || (defined(__CRT_HAVE_freopen) && (!defined(__USE_FILE_OFFSET64) || !defined(__O_LARGEFILE) || !__O_LARGEFILE)) || defined(__CRT_HAVE_freopen64) || (defined(__CRT_HAVE_freopen_unlocked) && (!defined(__USE_FILE_OFFSET64) || !defined(__O_LARGEFILE) || !__O_LARGEFILE)) || defined(__CRT_HAVE_freopen64_unlocked)) && (defined(__CRT_HAVE_convert_wcstombs) || defined(__CRT_HAVE_convert_wcstombsn) || defined(__CRT_HAVE_format_aprintf_printer) || defined(__CRT_HAVE_format_aprintf_alloc) || defined(__CRT_HAVE_realloc) || defined(__CRT_HAVE___libc_realloc))
#include <libc/local/corecrt_wstdio/_wfreopen_s.h>
__NAMESPACE_LOCAL_USING_OR_IMPL(_wfreopen_s, __FORCELOCAL __ATTR_ARTIFICIAL __ATTR_NONNULL((1, 2, 3)) __errno_t __NOTHROW_NCX(__LIBCCALL _wfreopen_s)(FILE **__pstream, wchar_t const *__filename, wchar_t const *__mode, FILE *__stream) { return (__NAMESPACE_LOCAL_SYM __LIBC_LOCAL_NAME(_wfreopen_s))(__pstream, __filename, __mode, __stream); })
#endif /* (__CRT_HAVE_freopen_s || (__CRT_HAVE_freopen && (!__USE_FILE_OFFSET64 || !__O_LARGEFILE || !__O_LARGEFILE)) || __CRT_HAVE_freopen64 || (__CRT_HAVE_freopen_unlocked && (!__USE_FILE_OFFSET64 || !__O_LARGEFILE || !__O_LARGEFILE)) || __CRT_HAVE_freopen64_unlocked) && (__CRT_HAVE_convert_wcstombs || __CRT_HAVE_convert_wcstombsn || __CRT_HAVE_format_aprintf_printer || __CRT_HAVE_format_aprintf_alloc || __CRT_HAVE_realloc || __CRT_HAVE___libc_realloc) */
#endif /* !__CRT_HAVE__wfreopen_s */
#ifdef __CRT_HAVE__getws_s
__CDECLARE(,wchar_t *,__NOTHROW_NCX,_getws_s,(wchar_t *__buf, size_t __buflen),(__buf,__buflen))
#else /* __CRT_HAVE__getws_s */
#include <__crt.h>
#if (defined(__CRT_HAVE_fgetws) || defined(__CRT_HAVE_fgetws_unlocked) || defined(__CRT_HAVE__fgetws_nolock) || ((defined(__CRT_HAVE_fgetwc) || defined(__CRT_HAVE_getwc)) && (defined(__CRT_HAVE_ungetwc) || defined(__CRT_HAVE_ungetwc_unlocked)) && (defined(__CRT_HAVE_ferror) || defined(__CRT_HAVE__IO_ferror) || defined(__CRT_HAVE_ferror_unlocked)))) && !defined(__NO_STDSTREAMS)
#include <libc/local/corecrt_wstdio/_getws_s.h>
__NAMESPACE_LOCAL_USING_OR_IMPL(_getws_s, __FORCELOCAL __ATTR_ARTIFICIAL wchar_t *__NOTHROW_NCX(__LIBCCALL _getws_s)(wchar_t *__buf, size_t __buflen) { return (__NAMESPACE_LOCAL_SYM __LIBC_LOCAL_NAME(_getws_s))(__buf, __buflen); })
#endif /* (__CRT_HAVE_fgetws || __CRT_HAVE_fgetws_unlocked || __CRT_HAVE__fgetws_nolock || ((__CRT_HAVE_fgetwc || __CRT_HAVE_getwc) && (__CRT_HAVE_ungetwc || __CRT_HAVE_ungetwc_unlocked) && (__CRT_HAVE_ferror || __CRT_HAVE__IO_ferror || __CRT_HAVE_ferror_unlocked))) && !__NO_STDSTREAMS */
#endif /* !__CRT_HAVE__getws_s */
#ifdef __CRT_HAVE__putws
__CDECLARE(__ATTR_NONNULL((1)),__STDC_INT_AS_SIZE_T,__NOTHROW_NCX,_putws,(wchar_t const *__str),(__str))
#else /* __CRT_HAVE__putws */
#include <__crt.h>
#if (defined(__CRT_HAVE_fputws) || defined(__CRT_HAVE_fputws_unlocked) || defined(__CRT_HAVE__fputws_nolock) || defined(__CRT_HAVE_file_wprinter) || defined(__CRT_HAVE_file_wprinter_unlocked) || defined(__CRT_HAVE_fputwc) || defined(__CRT_HAVE_putwc)) && !defined(__NO_STDSTREAMS)
#include <libc/local/corecrt_wstdio/_putws.h>
__NAMESPACE_LOCAL_USING_OR_IMPL(_putws, __FORCELOCAL __ATTR_ARTIFICIAL __ATTR_NONNULL((1)) __STDC_INT_AS_SIZE_T __NOTHROW_NCX(__LIBCCALL _putws)(wchar_t const *__str) { return (__NAMESPACE_LOCAL_SYM __LIBC_LOCAL_NAME(_putws))(__str); })
#endif /* (__CRT_HAVE_fputws || __CRT_HAVE_fputws_unlocked || __CRT_HAVE__fputws_nolock || __CRT_HAVE_file_wprinter || __CRT_HAVE_file_wprinter_unlocked || __CRT_HAVE_fputwc || __CRT_HAVE_putwc) && !__NO_STDSTREAMS */
#endif /* !__CRT_HAVE__putws */
__CDECLARE_OPT(__ATTR_WUNUSED,wchar_t *,__NOTHROW_NCX,_wtempnam,(wchar_t const *__directory, wchar_t const *__file_prefix),(__directory,__file_prefix))
__CDECLARE_OPT(__ATTR_NONNULL((1)),__errno_t,__NOTHROW_NCX,_wtmpnam_s,(wchar_t *__dst, size_t __wchar_count),(__dst,__wchar_count))
__CDECLARE_OPT(,wchar_t *,__NOTHROW_NCX,_wtmpnam,(wchar_t *__buf),(__buf))
#ifdef __CRT_HAVE___stdio_common_vfwprintf
__CDECLARE(__ATTR_NONNULL((2, 3)),__STDC_INT_AS_SSIZE_T,__NOTHROW_NCX,__stdio_common_vfwprintf,(__UINT64_TYPE__ __options, FILE *__stream, wchar_t const *__format, __locale_t __locale, __builtin_va_list __args),(__options,__stream,__format,__locale,__args))
#elif defined(__CRT_HAVE_vfwprintf) || defined(__CRT_HAVE_file_wprinter) || defined(__CRT_HAVE_file_wprinter_unlocked) || defined(__CRT_HAVE_fputwc) || defined(__CRT_HAVE_putwc)
#include <libc/local/corecrt_wstdio/__stdio_common_vfwprintf.h>
__NAMESPACE_LOCAL_USING_OR_IMPL(__stdio_common_vfwprintf, __FORCELOCAL __ATTR_ARTIFICIAL __ATTR_NONNULL((2, 3)) __STDC_INT_AS_SSIZE_T __NOTHROW_NCX(__LIBCCALL __stdio_common_vfwprintf)(__UINT64_TYPE__ __options, FILE *__stream, wchar_t const *__format, __locale_t __locale, __builtin_va_list __args) { return (__NAMESPACE_LOCAL_SYM __LIBC_LOCAL_NAME(__stdio_common_vfwprintf))(__options, __stream, __format, __locale, __args); })
#endif /* ... */
#ifdef __CRT_HAVE___stdio_common_vfwprintf_s
__CDECLARE(__ATTR_NONNULL((2, 3)),__STDC_INT_AS_SSIZE_T,__NOTHROW_NCX,__stdio_common_vfwprintf_s,(__UINT64_TYPE__ __options, FILE *__stream, wchar_t const *__format, __locale_t __locale, __builtin_va_list __args),(__options,__stream,__format,__locale,__args))
#elif defined(__CRT_HAVE___stdio_common_vfwprintf) || defined(__CRT_HAVE_vfwprintf) || defined(__CRT_HAVE_file_wprinter) || defined(__CRT_HAVE_file_wprinter_unlocked) || defined(__CRT_HAVE_fputwc) || defined(__CRT_HAVE_putwc)
#include <libc/local/corecrt_wstdio/__stdio_common_vfwprintf_s.h>
__NAMESPACE_LOCAL_USING_OR_IMPL(__stdio_common_vfwprintf_s, __FORCELOCAL __ATTR_ARTIFICIAL __ATTR_NONNULL((2, 3)) __STDC_INT_AS_SSIZE_T __NOTHROW_NCX(__LIBCCALL __stdio_common_vfwprintf_s)(__UINT64_TYPE__ __options, FILE *__stream, wchar_t const *__format, __locale_t __locale, __builtin_va_list __args) { return (__NAMESPACE_LOCAL_SYM __LIBC_LOCAL_NAME(__stdio_common_vfwprintf_s))(__options, __stream, __format, __locale, __args); })
#endif /* ... */
#ifdef __CRT_HAVE___stdio_common_vfwprintf_p
__CDECLARE(__ATTR_NONNULL((2, 3)),__STDC_INT_AS_SSIZE_T,__NOTHROW_NCX,__stdio_common_vfwprintf_p,(__UINT64_TYPE__ __options, FILE *__stream, wchar_t const *__format, __locale_t __locale, __builtin_va_list __args),(__options,__stream,__format,__locale,__args))
#elif defined(__CRT_HAVE_vfwprintf) || defined(__CRT_HAVE_file_wprinter) || defined(__CRT_HAVE_file_wprinter_unlocked) || defined(__CRT_HAVE_fputwc) || defined(__CRT_HAVE_putwc)
#include <libc/local/corecrt_wstdio/__stdio_common_vfwprintf_p.h>
__NAMESPACE_LOCAL_USING_OR_IMPL(__stdio_common_vfwprintf_p, __FORCELOCAL __ATTR_ARTIFICIAL __ATTR_NONNULL((2, 3)) __STDC_INT_AS_SSIZE_T __NOTHROW_NCX(__LIBCCALL __stdio_common_vfwprintf_p)(__UINT64_TYPE__ __options, FILE *__stream, wchar_t const *__format, __locale_t __locale, __builtin_va_list __args) { return (__NAMESPACE_LOCAL_SYM __LIBC_LOCAL_NAME(__stdio_common_vfwprintf_p))(__options, __stream, __format, __locale, __args); })
#endif /* ... */
#ifdef __CRT_HAVE___stdio_common_vswprintf
__CDECLARE(__ATTR_NONNULL((4)),__STDC_INT_AS_SSIZE_T,__NOTHROW_NCX,__stdio_common_vswprintf,(__UINT64_TYPE__ __options, wchar_t *__buf, size_t __bufsize, wchar_t const *__format, __locale_t __locale, __builtin_va_list __args),(__options,__buf,__bufsize,__format,__locale,__args))
#else /* __CRT_HAVE___stdio_common_vswprintf */
#include <libc/local/corecrt_wstdio/__stdio_common_vswprintf.h>
__NAMESPACE_LOCAL_USING_OR_IMPL(__stdio_common_vswprintf, __FORCELOCAL __ATTR_ARTIFICIAL __ATTR_NONNULL((4)) __STDC_INT_AS_SSIZE_T __NOTHROW_NCX(__LIBCCALL __stdio_common_vswprintf)(__UINT64_TYPE__ __options, wchar_t *__buf, size_t __bufsize, wchar_t const *__format, __locale_t __locale, __builtin_va_list __args) { return (__NAMESPACE_LOCAL_SYM __LIBC_LOCAL_NAME(__stdio_common_vswprintf))(__options, __buf, __bufsize, __format, __locale, __args); })
#endif /* !__CRT_HAVE___stdio_common_vswprintf */
#ifdef __CRT_HAVE___stdio_common_vswprintf_s
__CDECLARE(__ATTR_NONNULL((4)),__STDC_INT_AS_SSIZE_T,__NOTHROW_NCX,__stdio_common_vswprintf_s,(__UINT64_TYPE__ __options, wchar_t *__buf, size_t __bufsize, wchar_t const *__format, __locale_t __locale, __builtin_va_list __args),(__options,__buf,__bufsize,__format,__locale,__args))
#else /* __CRT_HAVE___stdio_common_vswprintf_s */
#include <libc/local/corecrt_wstdio/__stdio_common_vswprintf_s.h>
__NAMESPACE_LOCAL_USING_OR_IMPL(__stdio_common_vswprintf_s, __FORCELOCAL __ATTR_ARTIFICIAL __ATTR_NONNULL((4)) __STDC_INT_AS_SSIZE_T __NOTHROW_NCX(__LIBCCALL __stdio_common_vswprintf_s)(__UINT64_TYPE__ __options, wchar_t *__buf, size_t __bufsize, wchar_t const *__format, __locale_t __locale, __builtin_va_list __args) { return (__NAMESPACE_LOCAL_SYM __LIBC_LOCAL_NAME(__stdio_common_vswprintf_s))(__options, __buf, __bufsize, __format, __locale, __args); })
#endif /* !__CRT_HAVE___stdio_common_vswprintf_s */
#ifdef __CRT_HAVE___stdio_common_vsnwprintf_s
__CDECLARE(__ATTR_NONNULL((5)),__STDC_INT_AS_SSIZE_T,__NOTHROW_NCX,__stdio_common_vsnwprintf_s,(__UINT64_TYPE__ __options, wchar_t *__buf, size_t __bufsize, size_t __maxsize, wchar_t const *__format, __locale_t __locale, __builtin_va_list __args),(__options,__buf,__bufsize,__maxsize,__format,__locale,__args))
#else /* __CRT_HAVE___stdio_common_vsnwprintf_s */
#include <libc/local/corecrt_wstdio/__stdio_common_vsnwprintf_s.h>
__NAMESPACE_LOCAL_USING_OR_IMPL(__stdio_common_vsnwprintf_s, __FORCELOCAL __ATTR_ARTIFICIAL __ATTR_NONNULL((5)) __STDC_INT_AS_SSIZE_T __NOTHROW_NCX(__LIBCCALL __stdio_common_vsnwprintf_s)(__UINT64_TYPE__ __options, wchar_t *__buf, size_t __bufsize, size_t __maxsize, wchar_t const *__format, __locale_t __locale, __builtin_va_list __args) { return (__NAMESPACE_LOCAL_SYM __LIBC_LOCAL_NAME(__stdio_common_vsnwprintf_s))(__options, __buf, __bufsize, __maxsize, __format, __locale, __args); })
#endif /* !__CRT_HAVE___stdio_common_vsnwprintf_s */
#ifdef __CRT_HAVE___stdio_common_vswprintf_p
__CDECLARE(__ATTR_NONNULL((4)),__STDC_INT_AS_SSIZE_T,__NOTHROW_NCX,__stdio_common_vswprintf_p,(__UINT64_TYPE__ __options, wchar_t *__buf, size_t __bufsize, wchar_t const *__format, __locale_t __locale, __builtin_va_list __args),(__options,__buf,__bufsize,__format,__locale,__args))
#else /* __CRT_HAVE___stdio_common_vswprintf_p */
#include <libc/local/corecrt_wstdio/__stdio_common_vswprintf_p.h>
__NAMESPACE_LOCAL_USING_OR_IMPL(__stdio_common_vswprintf_p, __FORCELOCAL __ATTR_ARTIFICIAL __ATTR_NONNULL((4)) __STDC_INT_AS_SSIZE_T __NOTHROW_NCX(__LIBCCALL __stdio_common_vswprintf_p)(__UINT64_TYPE__ __options, wchar_t *__buf, size_t __bufsize, wchar_t const *__format, __locale_t __locale, __builtin_va_list __args) { return (__NAMESPACE_LOCAL_SYM __LIBC_LOCAL_NAME(__stdio_common_vswprintf_p))(__options, __buf, __bufsize, __format, __locale, __args); })
#endif /* !__CRT_HAVE___stdio_common_vswprintf_p */
#ifdef __CRT_HAVE___stdio_common_vfwscanf
__CDECLARE(__ATTR_NONNULL((2, 3)),__STDC_INT_AS_SSIZE_T,__NOTHROW_NCX,__stdio_common_vfwscanf,(__UINT64_TYPE__ __options, FILE *__stream, wchar_t const *__format, __locale_t __locale, __builtin_va_list __args),(__options,__stream,__format,__locale,__args))
#elif defined(__CRT_HAVE_vfwscanf)
#include <libc/local/corecrt_wstdio/__stdio_common_vfwscanf.h>
__NAMESPACE_LOCAL_USING_OR_IMPL(__stdio_common_vfwscanf, __FORCELOCAL __ATTR_ARTIFICIAL __ATTR_NONNULL((2, 3)) __STDC_INT_AS_SSIZE_T __NOTHROW_NCX(__LIBCCALL __stdio_common_vfwscanf)(__UINT64_TYPE__ __options, FILE *__stream, wchar_t const *__format, __locale_t __locale, __builtin_va_list __args) { return (__NAMESPACE_LOCAL_SYM __LIBC_LOCAL_NAME(__stdio_common_vfwscanf))(__options, __stream, __format, __locale, __args); })
#endif /* ... */
#ifdef __CRT_HAVE___stdio_common_vswscanf
__CDECLARE(__ATTR_NONNULL((4)),__STDC_INT_AS_SSIZE_T,__NOTHROW_NCX,__stdio_common_vswscanf,(__UINT64_TYPE__ __options, wchar_t const *__buf, size_t __bufsize, wchar_t const *__format, __locale_t __locale, __builtin_va_list __args),(__options,__buf,__bufsize,__format,__locale,__args))
#else /* __CRT_HAVE___stdio_common_vswscanf */
#include <libc/local/corecrt_wstdio/__stdio_common_vswscanf.h>
__NAMESPACE_LOCAL_USING_OR_IMPL(__stdio_common_vswscanf, __FORCELOCAL __ATTR_ARTIFICIAL __ATTR_NONNULL((4)) __STDC_INT_AS_SSIZE_T __NOTHROW_NCX(__LIBCCALL __stdio_common_vswscanf)(__UINT64_TYPE__ __options, wchar_t const *__buf, size_t __bufsize, wchar_t const *__format, __locale_t __locale, __builtin_va_list __args) { return (__NAMESPACE_LOCAL_SYM __LIBC_LOCAL_NAME(__stdio_common_vswscanf))(__options, __buf, __bufsize, __format, __locale, __args); })
#endif /* !__CRT_HAVE___stdio_common_vswscanf */
#ifdef __CRT_HAVE__vsnwprintf_l
__CDECLARE(__ATTR_NONNULL((3)),__STDC_INT_AS_SSIZE_T,__NOTHROW_NCX,_vsnwprintf_l,(wchar_t *__buf, size_t __bufsize, wchar_t const *__format, __locale_t __locale, __builtin_va_list __args),(__buf,__bufsize,__format,__locale,__args))
#else /* __CRT_HAVE__vsnwprintf_l */
#include <libc/local/corecrt_wstdio/_vsnwprintf_l.h>
__NAMESPACE_LOCAL_USING_OR_IMPL(_vsnwprintf_l, __FORCELOCAL __ATTR_ARTIFICIAL __ATTR_NONNULL((3)) __STDC_INT_AS_SSIZE_T __NOTHROW_NCX(__LIBCCALL _vsnwprintf_l)(wchar_t *__buf, size_t __bufsize, wchar_t const *__format, __locale_t __locale, __builtin_va_list __args) { return (__NAMESPACE_LOCAL_SYM __LIBC_LOCAL_NAME(_vsnwprintf_l))(__buf, __bufsize, __format, __locale, __args); })
#endif /* !__CRT_HAVE__vsnwprintf_l */
#ifdef __CRT_HAVE__vswprintf_s_l
__CDECLARE(__ATTR_NONNULL((3)),__STDC_INT_AS_SSIZE_T,__NOTHROW_NCX,_vswprintf_s_l,(wchar_t *__buf, size_t __bufsize, wchar_t const *__format, __locale_t __locale, __builtin_va_list __args),(__buf,__bufsize,__format,__locale,__args))
#else /* __CRT_HAVE__vswprintf_s_l */
#include <libc/local/corecrt_wstdio/_vswprintf_s_l.h>
__NAMESPACE_LOCAL_USING_OR_IMPL(_vswprintf_s_l, __FORCELOCAL __ATTR_ARTIFICIAL __ATTR_NONNULL((3)) __STDC_INT_AS_SSIZE_T __NOTHROW_NCX(__LIBCCALL _vswprintf_s_l)(wchar_t *__buf, size_t __bufsize, wchar_t const *__format, __locale_t __locale, __builtin_va_list __args) { return (__NAMESPACE_LOCAL_SYM __LIBC_LOCAL_NAME(_vswprintf_s_l))(__buf, __bufsize, __format, __locale, __args); })
#endif /* !__CRT_HAVE__vswprintf_s_l */
#ifdef __CRT_HAVE__vswprintf_p_l
__CDECLARE(__ATTR_NONNULL((3)),__STDC_INT_AS_SSIZE_T,__NOTHROW_NCX,_vswprintf_p_l,(wchar_t *__buf, size_t __bufsize, wchar_t const *__format, __locale_t __locale, __builtin_va_list __args),(__buf,__bufsize,__format,__locale,__args))
#else /* __CRT_HAVE__vswprintf_p_l */
#include <libc/local/corecrt_wstdio/_vswprintf_p_l.h>
__NAMESPACE_LOCAL_USING_OR_IMPL(_vswprintf_p_l, __FORCELOCAL __ATTR_ARTIFICIAL __ATTR_NONNULL((3)) __STDC_INT_AS_SSIZE_T __NOTHROW_NCX(__LIBCCALL _vswprintf_p_l)(wchar_t *__buf, size_t __bufsize, wchar_t const *__format, __locale_t __locale, __builtin_va_list __args) { return (__NAMESPACE_LOCAL_SYM __LIBC_LOCAL_NAME(_vswprintf_p_l))(__buf, __bufsize, __format, __locale, __args); })
#endif /* !__CRT_HAVE__vswprintf_p_l */
#ifdef __CRT_HAVE__vscwprintf_l
__CDECLARE(__ATTR_NONNULL((1)),__STDC_INT_AS_SSIZE_T,__NOTHROW_NCX,_vscwprintf_l,(wchar_t const *__format, __locale_t __locale, __builtin_va_list __args),(__format,__locale,__args))
#else /* __CRT_HAVE__vscwprintf_l */
#include <libc/local/corecrt_wstdio/_vscwprintf_l.h>
__NAMESPACE_LOCAL_USING_OR_IMPL(_vscwprintf_l, __FORCELOCAL __ATTR_ARTIFICIAL __ATTR_NONNULL((1)) __STDC_INT_AS_SSIZE_T __NOTHROW_NCX(__LIBCCALL _vscwprintf_l)(wchar_t const *__format, __locale_t __locale, __builtin_va_list __args) { return (__NAMESPACE_LOCAL_SYM __LIBC_LOCAL_NAME(_vscwprintf_l))(__format, __locale, __args); })
#endif /* !__CRT_HAVE__vscwprintf_l */
#ifdef __CRT_HAVE__vscwprintf_p_l
__CDECLARE(__ATTR_NONNULL((1)),__STDC_INT_AS_SSIZE_T,__NOTHROW_NCX,_vscwprintf_p_l,(wchar_t const *__format, __locale_t __locale, __builtin_va_list __args),(__format,__locale,__args))
#else /* __CRT_HAVE__vscwprintf_p_l */
#include <libc/local/corecrt_wstdio/_vscwprintf_p_l.h>
__NAMESPACE_LOCAL_USING_OR_IMPL(_vscwprintf_p_l, __FORCELOCAL __ATTR_ARTIFICIAL __ATTR_NONNULL((1)) __STDC_INT_AS_SSIZE_T __NOTHROW_NCX(__LIBCCALL _vscwprintf_p_l)(wchar_t const *__format, __locale_t __locale, __builtin_va_list __args) { return (__NAMESPACE_LOCAL_SYM __LIBC_LOCAL_NAME(_vscwprintf_p_l))(__format, __locale, __args); })
#endif /* !__CRT_HAVE__vscwprintf_p_l */
#ifdef __CRT_HAVE__vswprintf_l
__CREDIRECT(__ATTR_NONNULL((3)),__STDC_INT_AS_SSIZE_T,__NOTHROW_NCX,_vswprintf_c_l,(wchar_t *__buf, size_t __bufsize, wchar_t const *__format, __locale_t __locale, __builtin_va_list __args),_vswprintf_l,(__buf,__bufsize,__format,__locale,__args))
#elif defined(__CRT_HAVE__vswprintf_c_l)
__CDECLARE(__ATTR_NONNULL((3)),__STDC_INT_AS_SSIZE_T,__NOTHROW_NCX,_vswprintf_c_l,(wchar_t *__buf, size_t __bufsize, wchar_t const *__format, __locale_t __locale, __builtin_va_list __args),(__buf,__bufsize,__format,__locale,__args))
#else /* ... */
#include <libc/local/corecrt_wstdio/_vswprintf_c_l.h>
__NAMESPACE_LOCAL_USING_OR_IMPL(_vswprintf_c_l, __FORCELOCAL __ATTR_ARTIFICIAL __ATTR_NONNULL((3)) __STDC_INT_AS_SSIZE_T __NOTHROW_NCX(__LIBCCALL _vswprintf_c_l)(wchar_t *__buf, size_t __bufsize, wchar_t const *__format, __locale_t __locale, __builtin_va_list __args) { return (__NAMESPACE_LOCAL_SYM __LIBC_LOCAL_NAME(_vswprintf_c_l))(__buf, __bufsize, __format, __locale, __args); })
#endif /* !... */
#ifdef __CRT_HAVE__vsnwprintf_s_l
__CDECLARE(__ATTR_NONNULL((4)),__STDC_INT_AS_SSIZE_T,__NOTHROW_NCX,_vsnwprintf_s_l,(wchar_t *__buf, size_t __bufsize, size_t __maxsize, wchar_t const *__format, __locale_t __locale, __builtin_va_list __args),(__buf,__bufsize,__maxsize,__format,__locale,__args))
#else /* __CRT_HAVE__vsnwprintf_s_l */
#include <libc/local/corecrt_wstdio/_vsnwprintf_s_l.h>
__NAMESPACE_LOCAL_USING_OR_IMPL(_vsnwprintf_s_l, __FORCELOCAL __ATTR_ARTIFICIAL __ATTR_NONNULL((4)) __STDC_INT_AS_SSIZE_T __NOTHROW_NCX(__LIBCCALL _vsnwprintf_s_l)(wchar_t *__buf, size_t __bufsize, size_t __maxsize, wchar_t const *__format, __locale_t __locale, __builtin_va_list __args) { return (__NAMESPACE_LOCAL_SYM __LIBC_LOCAL_NAME(_vsnwprintf_s_l))(__buf, __bufsize, __maxsize, __format, __locale, __args); })
#endif /* !__CRT_HAVE__vsnwprintf_s_l */
#ifdef __CRT_HAVE__vfwprintf_s_l
__CDECLARE(__ATTR_NONNULL((1, 2)),__STDC_INT_AS_SSIZE_T,__NOTHROW_NCX,_vfwprintf_s_l,(FILE *__stream, wchar_t const *__format, __locale_t __locale, __builtin_va_list __args),(__stream,__format,__locale,__args))
#elif defined(__CRT_HAVE___stdio_common_vfwprintf_s) || defined(__CRT_HAVE___stdio_common_vfwprintf) || defined(__CRT_HAVE_vfwprintf) || defined(__CRT_HAVE_file_wprinter) || defined(__CRT_HAVE_file_wprinter_unlocked) || defined(__CRT_HAVE_fputwc) || defined(__CRT_HAVE_putwc)
#include <libc/local/corecrt_wstdio/_vfwprintf_s_l.h>
__NAMESPACE_LOCAL_USING_OR_IMPL(_vfwprintf_s_l, __FORCELOCAL __ATTR_ARTIFICIAL __ATTR_NONNULL((1, 2)) __STDC_INT_AS_SSIZE_T __NOTHROW_NCX(__LIBCCALL _vfwprintf_s_l)(FILE *__stream, wchar_t const *__format, __locale_t __locale, __builtin_va_list __args) { return (__NAMESPACE_LOCAL_SYM __LIBC_LOCAL_NAME(_vfwprintf_s_l))(__stream, __format, __locale, __args); })
#endif /* ... */
#ifdef __CRT_HAVE__vfwprintf_p_l
__CDECLARE(__ATTR_NONNULL((1, 2)),__STDC_INT_AS_SSIZE_T,__NOTHROW_NCX,_vfwprintf_p_l,(FILE *__stream, wchar_t const *__format, __locale_t __locale, __builtin_va_list __args),(__stream,__format,__locale,__args))
#elif defined(__CRT_HAVE___stdio_common_vfwprintf_p) || defined(__CRT_HAVE_vfwprintf) || defined(__CRT_HAVE_file_wprinter) || defined(__CRT_HAVE_file_wprinter_unlocked) || defined(__CRT_HAVE_fputwc) || defined(__CRT_HAVE_putwc)
#include <libc/local/corecrt_wstdio/_vfwprintf_p_l.h>
__NAMESPACE_LOCAL_USING_OR_IMPL(_vfwprintf_p_l, __FORCELOCAL __ATTR_ARTIFICIAL __ATTR_NONNULL((1, 2)) __STDC_INT_AS_SSIZE_T __NOTHROW_NCX(__LIBCCALL _vfwprintf_p_l)(FILE *__stream, wchar_t const *__format, __locale_t __locale, __builtin_va_list __args) { return (__NAMESPACE_LOCAL_SYM __LIBC_LOCAL_NAME(_vfwprintf_p_l))(__stream, __format, __locale, __args); })
#endif /* ... */
#ifdef __CRT_HAVE__vfwscanf_l
__CDECLARE(__ATTR_WUNUSED __ATTR_NONNULL((1, 2)),__STDC_INT_AS_SSIZE_T,__NOTHROW_NCX,_vfwscanf_l,(FILE *__stream, wchar_t const *__format, __locale_t __locale, __builtin_va_list __args),(__stream,__format,__locale,__args))
#elif defined(__CRT_HAVE___stdio_common_vfscanf) || defined(__CRT_HAVE_vfscanf) || defined(__CRT_HAVE__vfscanf) || defined(__CRT_HAVE__vfscanf_s) || defined(__CRT_HAVE__IO_vfscanf) || defined(__CRT_HAVE_vfscanf_unlocked) || defined(__CRT_HAVE___vfscanf) || ((defined(__CRT_HAVE_fgetc) || defined(__CRT_HAVE_getc) || defined(__CRT_HAVE__IO_getc) || defined(__CRT_HAVE_fgetc_unlocked) || defined(__CRT_HAVE_getc_unlocked) || (defined(__CRT_DOS) && (defined(__CRT_HAVE__filbuf) || defined(__CRT_HAVE___uflow))) || defined(__CRT_HAVE_fread) || defined(__CRT_HAVE__IO_fread) || defined(__CRT_HAVE_fread_unlocked) || defined(__CRT_HAVE__fread_nolock)) && (defined(__CRT_HAVE_ungetc) || defined(__CRT_HAVE__IO_ungetc) || defined(__CRT_HAVE_ungetc_unlocked) || defined(__CRT_HAVE__ungetc_nolock)))
#include <libc/local/corecrt_wstdio/_vfwscanf_l.h>
__NAMESPACE_LOCAL_USING_OR_IMPL(_vfwscanf_l, __FORCELOCAL __ATTR_ARTIFICIAL __ATTR_WUNUSED __ATTR_NONNULL((1, 2)) __STDC_INT_AS_SSIZE_T __NOTHROW_NCX(__LIBCCALL _vfwscanf_l)(FILE *__stream, wchar_t const *__format, __locale_t __locale, __builtin_va_list __args) { return (__NAMESPACE_LOCAL_SYM __LIBC_LOCAL_NAME(_vfwscanf_l))(__stream, __format, __locale, __args); })
#endif /* ... */
#ifdef __CRT_HAVE__vfwscanf_s_l
__CDECLARE(__ATTR_WUNUSED __ATTR_NONNULL((1, 2)),__STDC_INT_AS_SSIZE_T,__NOTHROW_NCX,_vfwscanf_s_l,(FILE *__stream, wchar_t const *__format, __locale_t __locale, __builtin_va_list __args),(__stream,__format,__locale,__args))
#elif defined(__CRT_HAVE___stdio_common_vfscanf) || defined(__CRT_HAVE_vfscanf) || defined(__CRT_HAVE__vfscanf) || defined(__CRT_HAVE__vfscanf_s) || defined(__CRT_HAVE__IO_vfscanf) || defined(__CRT_HAVE_vfscanf_unlocked) || defined(__CRT_HAVE___vfscanf) || ((defined(__CRT_HAVE_fgetc) || defined(__CRT_HAVE_getc) || defined(__CRT_HAVE__IO_getc) || defined(__CRT_HAVE_fgetc_unlocked) || defined(__CRT_HAVE_getc_unlocked) || (defined(__CRT_DOS) && (defined(__CRT_HAVE__filbuf) || defined(__CRT_HAVE___uflow))) || defined(__CRT_HAVE_fread) || defined(__CRT_HAVE__IO_fread) || defined(__CRT_HAVE_fread_unlocked) || defined(__CRT_HAVE__fread_nolock)) && (defined(__CRT_HAVE_ungetc) || defined(__CRT_HAVE__IO_ungetc) || defined(__CRT_HAVE_ungetc_unlocked) || defined(__CRT_HAVE__ungetc_nolock)))
#include <libc/local/corecrt_wstdio/_vfwscanf_s_l.h>
__NAMESPACE_LOCAL_USING_OR_IMPL(_vfwscanf_s_l, __FORCELOCAL __ATTR_ARTIFICIAL __ATTR_WUNUSED __ATTR_NONNULL((1, 2)) __STDC_INT_AS_SSIZE_T __NOTHROW_NCX(__LIBCCALL _vfwscanf_s_l)(FILE *__stream, wchar_t const *__format, __locale_t __locale, __builtin_va_list __args) { return (__NAMESPACE_LOCAL_SYM __LIBC_LOCAL_NAME(_vfwscanf_s_l))(__stream, __format, __locale, __args); })
#endif /* ... */
#ifdef __CRT_HAVE__vwprintf_l
__CDECLARE(__ATTR_NONNULL((1)),__STDC_INT_AS_SSIZE_T,__NOTHROW_NCX,_vwprintf_l,(wchar_t const *__format, __locale_t __locale, __builtin_va_list __args),(__format,__locale,__args))
#else /* __CRT_HAVE__vwprintf_l */
#include <__crt.h>
#if defined(__CRT_HAVE_vwprintf) || ((defined(__CRT_HAVE_vfwprintf) || defined(__CRT_HAVE_file_wprinter) || defined(__CRT_HAVE_file_wprinter_unlocked) || defined(__CRT_HAVE_fputwc) || defined(__CRT_HAVE_putwc)) && !defined(__NO_STDSTREAMS))
#include <libc/local/corecrt_wstdio/_vwprintf_l.h>
__NAMESPACE_LOCAL_USING_OR_IMPL(_vwprintf_l, __FORCELOCAL __ATTR_ARTIFICIAL __ATTR_NONNULL((1)) __STDC_INT_AS_SSIZE_T __NOTHROW_NCX(__LIBCCALL _vwprintf_l)(wchar_t const *__format, __locale_t __locale, __builtin_va_list __args) { return (__NAMESPACE_LOCAL_SYM __LIBC_LOCAL_NAME(_vwprintf_l))(__format, __locale, __args); })
#endif /* __CRT_HAVE_vwprintf || ((__CRT_HAVE_vfwprintf || __CRT_HAVE_file_wprinter || __CRT_HAVE_file_wprinter_unlocked || __CRT_HAVE_fputwc || __CRT_HAVE_putwc) && !__NO_STDSTREAMS) */
#endif /* !__CRT_HAVE__vwprintf_l */
#ifdef __CRT_HAVE__vfwprintf_l
__CDECLARE(__ATTR_NONNULL((1, 2)),__STDC_INT_AS_SSIZE_T,__NOTHROW_NCX,_vfwprintf_l,(FILE *__stream, wchar_t const *__format, __locale_t __locale, __builtin_va_list __args),(__stream,__format,__locale,__args))
#elif defined(__CRT_HAVE_vfwprintf) || defined(__CRT_HAVE_file_wprinter) || defined(__CRT_HAVE_file_wprinter_unlocked) || defined(__CRT_HAVE_fputwc) || defined(__CRT_HAVE_putwc)
#include <libc/local/corecrt_wstdio/_vfwprintf_l.h>
__NAMESPACE_LOCAL_USING_OR_IMPL(_vfwprintf_l, __FORCELOCAL __ATTR_ARTIFICIAL __ATTR_NONNULL((1, 2)) __STDC_INT_AS_SSIZE_T __NOTHROW_NCX(__LIBCCALL _vfwprintf_l)(FILE *__stream, wchar_t const *__format, __locale_t __locale, __builtin_va_list __args) { return (__NAMESPACE_LOCAL_SYM __LIBC_LOCAL_NAME(_vfwprintf_l))(__stream, __format, __locale, __args); })
#endif /* ... */
#ifdef __CRT_HAVE___vswprintf_l
__CDECLARE(__ATTR_NONNULL((1, 2)),__STDC_INT_AS_SSIZE_T,__NOTHROW_NCX,__vswprintf_l,(wchar_t *__buf, wchar_t const *__format, __locale_t __locale, __builtin_va_list __args),(__buf,__format,__locale,__args))
#else /* __CRT_HAVE___vswprintf_l */
#include <libc/local/corecrt_wstdio/__vswprintf_l.h>
__NAMESPACE_LOCAL_USING_OR_IMPL(__vswprintf_l, __FORCELOCAL __ATTR_ARTIFICIAL __ATTR_NONNULL((1, 2)) __STDC_INT_AS_SSIZE_T __NOTHROW_NCX(__LIBCCALL __vswprintf_l)(wchar_t *__buf, wchar_t const *__format, __locale_t __locale, __builtin_va_list __args) { return (__NAMESPACE_LOCAL_SYM __LIBC_LOCAL_NAME(__vswprintf_l))(__buf, __format, __locale, __args); })
#endif /* !__CRT_HAVE___vswprintf_l */
#ifdef __CRT_HAVE__vwscanf_l
__CDECLARE(__ATTR_WUNUSED __ATTR_NONNULL((1)),__STDC_INT_AS_SSIZE_T,__NOTHROW_NCX,_vwscanf_l,(wchar_t const *__format, __locale_t __locale, __builtin_va_list __args),(__format,__locale,__args))
#else /* __CRT_HAVE__vwscanf_l */
#include <__crt.h>
#if defined(__CRT_HAVE_vwscanf) || (defined(__CRT_HAVE_vfwscanf) && !defined(__NO_STDSTREAMS))
#include <libc/local/corecrt_wstdio/_vwscanf_l.h>
__NAMESPACE_LOCAL_USING_OR_IMPL(_vwscanf_l, __FORCELOCAL __ATTR_ARTIFICIAL __ATTR_WUNUSED __ATTR_NONNULL((1)) __STDC_INT_AS_SSIZE_T __NOTHROW_NCX(__LIBCCALL _vwscanf_l)(wchar_t const *__format, __locale_t __locale, __builtin_va_list __args) { return (__NAMESPACE_LOCAL_SYM __LIBC_LOCAL_NAME(_vwscanf_l))(__format, __locale, __args); })
#endif /* __CRT_HAVE_vwscanf || (__CRT_HAVE_vfwscanf && !__NO_STDSTREAMS) */
#endif /* !__CRT_HAVE__vwscanf_l */
#ifdef __CRT_HAVE__vswprintf_l
__CDECLARE(__ATTR_NONNULL((3)),__STDC_INT_AS_SSIZE_T,__NOTHROW_NCX,_vswprintf_l,(wchar_t *__buf, size_t __bufsize, wchar_t const *__format, __locale_t __locale, __builtin_va_list __args),(__buf,__bufsize,__format,__locale,__args))
#elif defined(__CRT_HAVE__vswprintf_c_l)
__CREDIRECT(__ATTR_NONNULL((3)),__STDC_INT_AS_SSIZE_T,__NOTHROW_NCX,_vswprintf_l,(wchar_t *__buf, size_t __bufsize, wchar_t const *__format, __locale_t __locale, __builtin_va_list __args),_vswprintf_c_l,(__buf,__bufsize,__format,__locale,__args))
#else /* ... */
#include <libc/local/corecrt_wstdio/_vswprintf_c_l.h>
__FORCELOCAL __ATTR_ARTIFICIAL __ATTR_NONNULL((3)) __STDC_INT_AS_SSIZE_T __NOTHROW_NCX(__LIBCCALL _vswprintf_l)(wchar_t *__buf, size_t __bufsize, wchar_t const *__format, __locale_t __locale, __builtin_va_list __args) { return (__NAMESPACE_LOCAL_SYM __LIBC_LOCAL_NAME(_vswprintf_c_l))(__buf, __bufsize, __format, __locale, __args); }
#endif /* !... */
#ifdef __CRT_HAVE__vfwprintf_p
__CDECLARE(__ATTR_NONNULL((1, 2)),__STDC_INT_AS_SSIZE_T,__NOTHROW_NCX,_vfwprintf_p,(FILE *__stream, wchar_t const *__format, __builtin_va_list __args),(__stream,__format,__args))
#elif defined(__CRT_HAVE__vfwprintf_p_l) || defined(__CRT_HAVE___stdio_common_vfwprintf_p) || defined(__CRT_HAVE_vfwprintf) || defined(__CRT_HAVE_file_wprinter) || defined(__CRT_HAVE_file_wprinter_unlocked) || defined(__CRT_HAVE_fputwc) || defined(__CRT_HAVE_putwc)
#include <libc/local/corecrt_wstdio/_vfwprintf_p.h>
__NAMESPACE_LOCAL_USING_OR_IMPL(_vfwprintf_p, __FORCELOCAL __ATTR_ARTIFICIAL __ATTR_NONNULL((1, 2)) __STDC_INT_AS_SSIZE_T __NOTHROW_NCX(__LIBCCALL _vfwprintf_p)(FILE *__stream, wchar_t const *__format, __builtin_va_list __args) { return (__NAMESPACE_LOCAL_SYM __LIBC_LOCAL_NAME(_vfwprintf_p))(__stream, __format, __args); })
#endif /* ... */
#ifdef __CRT_HAVE__vsnwprintf_s
__CDECLARE(__ATTR_NONNULL((4)),__STDC_INT_AS_SSIZE_T,__NOTHROW_NCX,_vsnwprintf_s,(wchar_t *__buf, size_t __bufsize, size_t __maxsize, wchar_t const *__format, __builtin_va_list __args),(__buf,__bufsize,__maxsize,__format,__args))
#else /* __CRT_HAVE__vsnwprintf_s */
#include <libc/local/corecrt_wstdio/_vsnwprintf_s.h>
__NAMESPACE_LOCAL_USING_OR_IMPL(_vsnwprintf_s, __FORCELOCAL __ATTR_ARTIFICIAL __ATTR_NONNULL((4)) __STDC_INT_AS_SSIZE_T __NOTHROW_NCX(__LIBCCALL _vsnwprintf_s)(wchar_t *__buf, size_t __bufsize, size_t __maxsize, wchar_t const *__format, __builtin_va_list __args) { return (__NAMESPACE_LOCAL_SYM __LIBC_LOCAL_NAME(_vsnwprintf_s))(__buf, __bufsize, __maxsize, __format, __args); })
#endif /* !__CRT_HAVE__vsnwprintf_s */
#ifdef __CRT_HAVE__vsnwprintf
__CDECLARE(__ATTR_NONNULL((3)),__STDC_INT_AS_SSIZE_T,__NOTHROW_NCX,_vsnwprintf,(wchar_t *__buf, size_t __bufsize, wchar_t const *__format, __builtin_va_list __args),(__buf,__bufsize,__format,__args))
#else /* __CRT_HAVE__vsnwprintf */
#include <libc/local/corecrt_wstdio/_vsnwprintf.h>
__NAMESPACE_LOCAL_USING_OR_IMPL(_vsnwprintf, __FORCELOCAL __ATTR_ARTIFICIAL __ATTR_NONNULL((3)) __STDC_INT_AS_SSIZE_T __NOTHROW_NCX(__LIBCCALL _vsnwprintf)(wchar_t *__buf, size_t __bufsize, wchar_t const *__format, __builtin_va_list __args) { return (__NAMESPACE_LOCAL_SYM __LIBC_LOCAL_NAME(_vsnwprintf))(__buf, __bufsize, __format, __args); })
#endif /* !__CRT_HAVE__vsnwprintf */
#ifdef __CRT_HAVE__vswprintf_c
__CDECLARE(__ATTR_NONNULL((3)),__STDC_INT_AS_SSIZE_T,__NOTHROW_NCX,_vswprintf_c,(wchar_t *__buf, size_t __bufsize, wchar_t const *__format, __builtin_va_list __args),(__buf,__bufsize,__format,__args))
#else /* __CRT_HAVE__vswprintf_c */
#include <libc/local/corecrt_wstdio/_vswprintf_c.h>
__NAMESPACE_LOCAL_USING_OR_IMPL(_vswprintf_c, __FORCELOCAL __ATTR_ARTIFICIAL __ATTR_NONNULL((3)) __STDC_INT_AS_SSIZE_T __NOTHROW_NCX(__LIBCCALL _vswprintf_c)(wchar_t *__buf, size_t __bufsize, wchar_t const *__format, __builtin_va_list __args) { return (__NAMESPACE_LOCAL_SYM __LIBC_LOCAL_NAME(_vswprintf_c))(__buf, __bufsize, __format, __args); })
#endif /* !__CRT_HAVE__vswprintf_c */
#ifdef __CRT_HAVE__vswprintf
__CDECLARE(__ATTR_NONNULL((1, 2)),__STDC_INT_AS_SSIZE_T,__NOTHROW_NCX,_vswprintf,(wchar_t *__buf, wchar_t const *__format, __builtin_va_list __args),(__buf,__format,__args))
#else /* __CRT_HAVE__vswprintf */
#include <libc/local/corecrt_wstdio/_vswprintf.h>
__NAMESPACE_LOCAL_USING_OR_IMPL(_vswprintf, __FORCELOCAL __ATTR_ARTIFICIAL __ATTR_NONNULL((1, 2)) __STDC_INT_AS_SSIZE_T __NOTHROW_NCX(__LIBCCALL _vswprintf)(wchar_t *__buf, wchar_t const *__format, __builtin_va_list __args) { return (__NAMESPACE_LOCAL_SYM __LIBC_LOCAL_NAME(_vswprintf))(__buf, __format, __args); })
#endif /* !__CRT_HAVE__vswprintf */
#ifdef __CRT_HAVE__vwprintf_p
__CDECLARE(__ATTR_NONNULL((1)),__STDC_INT_AS_SSIZE_T,__NOTHROW_NCX,_vwprintf_p,(wchar_t const *__format, __builtin_va_list __args),(__format,__args))
#elif defined(__CRT_HAVE__vwprintf_p_l) || ((defined(__CRT_HAVE__vfwprintf_s_l) || defined(__CRT_HAVE___stdio_common_vfwprintf_s) || defined(__CRT_HAVE___stdio_common_vfwprintf) || defined(__CRT_HAVE_vfwprintf) || defined(__CRT_HAVE_file_wprinter) || defined(__CRT_HAVE_file_wprinter_unlocked) || defined(__CRT_HAVE_fputwc) || defined(__CRT_HAVE_putwc)) && !defined(__NO_STDSTREAMS))
#include <libc/local/corecrt_wstdio/_vwprintf_p.h>
__NAMESPACE_LOCAL_USING_OR_IMPL(_vwprintf_p, __FORCELOCAL __ATTR_ARTIFICIAL __ATTR_NONNULL((1)) __STDC_INT_AS_SSIZE_T __NOTHROW_NCX(__LIBCCALL _vwprintf_p)(wchar_t const *__format, __builtin_va_list __args) { return (__NAMESPACE_LOCAL_SYM __LIBC_LOCAL_NAME(_vwprintf_p))(__format, __args); })
#endif /* ... */
#ifdef __CRT_HAVE__vswprintf_p
__CDECLARE(__ATTR_NONNULL((3)),__STDC_INT_AS_SSIZE_T,__NOTHROW_NCX,_vswprintf_p,(wchar_t *__buf, size_t __bufsize, wchar_t const *__format, __builtin_va_list __args),(__buf,__bufsize,__format,__args))
#else /* __CRT_HAVE__vswprintf_p */
#include <libc/local/corecrt_wstdio/_vswprintf_p.h>
__NAMESPACE_LOCAL_USING_OR_IMPL(_vswprintf_p, __FORCELOCAL __ATTR_ARTIFICIAL __ATTR_NONNULL((3)) __STDC_INT_AS_SSIZE_T __NOTHROW_NCX(__LIBCCALL _vswprintf_p)(wchar_t *__buf, size_t __bufsize, wchar_t const *__format, __builtin_va_list __args) { return (__NAMESPACE_LOCAL_SYM __LIBC_LOCAL_NAME(_vswprintf_p))(__buf, __bufsize, __format, __args); })
#endif /* !__CRT_HAVE__vswprintf_p */
#ifdef __CRT_HAVE__vscwprintf
__CDECLARE(__ATTR_NONNULL((1)),__STDC_INT_AS_SSIZE_T,__NOTHROW_NCX,_vscwprintf,(wchar_t const *__format, __builtin_va_list __args),(__format,__args))
#else /* __CRT_HAVE__vscwprintf */
#include <libc/local/corecrt_wstdio/_vscwprintf.h>
__NAMESPACE_LOCAL_USING_OR_IMPL(_vscwprintf, __FORCELOCAL __ATTR_ARTIFICIAL __ATTR_NONNULL((1)) __STDC_INT_AS_SSIZE_T __NOTHROW_NCX(__LIBCCALL _vscwprintf)(wchar_t const *__format, __builtin_va_list __args) { return (__NAMESPACE_LOCAL_SYM __LIBC_LOCAL_NAME(_vscwprintf))(__format, __args); })
#endif /* !__CRT_HAVE__vscwprintf */
#ifdef __CRT_HAVE__vscwprintf_p
__CDECLARE(__ATTR_NONNULL((1)),__STDC_INT_AS_SSIZE_T,__NOTHROW_NCX,_vscwprintf_p,(wchar_t const *__format, __builtin_va_list __args),(__format,__args))
#else /* __CRT_HAVE__vscwprintf_p */
#include <libc/local/corecrt_wstdio/_vscwprintf_p.h>
__NAMESPACE_LOCAL_USING_OR_IMPL(_vscwprintf_p, __FORCELOCAL __ATTR_ARTIFICIAL __ATTR_NONNULL((1)) __STDC_INT_AS_SSIZE_T __NOTHROW_NCX(__LIBCCALL _vscwprintf_p)(wchar_t const *__format, __builtin_va_list __args) { return (__NAMESPACE_LOCAL_SYM __LIBC_LOCAL_NAME(_vscwprintf_p))(__format, __args); })
#endif /* !__CRT_HAVE__vscwprintf_p */
#ifdef __CRT_HAVE__vswscanf_l
__CDECLARE(__ATTR_NONNULL((1, 2)),__STDC_INT_AS_SSIZE_T,__NOTHROW_NCX,_vswscanf_l,(wchar_t const *__buf, wchar_t const *__format, __locale_t __locale, __builtin_va_list __args),(__buf,__format,__locale,__args))
#else /* __CRT_HAVE__vswscanf_l */
#include <libc/local/corecrt_wstdio/_vswscanf_l.h>
__NAMESPACE_LOCAL_USING_OR_IMPL(_vswscanf_l, __FORCELOCAL __ATTR_ARTIFICIAL __ATTR_NONNULL((1, 2)) __STDC_INT_AS_SSIZE_T __NOTHROW_NCX(__LIBCCALL _vswscanf_l)(wchar_t const *__buf, wchar_t const *__format, __locale_t __locale, __builtin_va_list __args) { return (__NAMESPACE_LOCAL_SYM __LIBC_LOCAL_NAME(_vswscanf_l))(__buf, __format, __locale, __args); })
#endif /* !__CRT_HAVE__vswscanf_l */
#ifdef __CRT_HAVE__vswscanf_s_l
__CDECLARE(__ATTR_NONNULL((1, 2)),__STDC_INT_AS_SSIZE_T,__NOTHROW_NCX,_vswscanf_s_l,(wchar_t const *__buf, wchar_t const *__format, __locale_t __locale, __builtin_va_list __args),(__buf,__format,__locale,__args))
#else /* __CRT_HAVE__vswscanf_s_l */
#include <libc/local/corecrt_wstdio/_vswscanf_s_l.h>
__NAMESPACE_LOCAL_USING_OR_IMPL(_vswscanf_s_l, __FORCELOCAL __ATTR_ARTIFICIAL __ATTR_NONNULL((1, 2)) __STDC_INT_AS_SSIZE_T __NOTHROW_NCX(__LIBCCALL _vswscanf_s_l)(wchar_t const *__buf, wchar_t const *__format, __locale_t __locale, __builtin_va_list __args) { return (__NAMESPACE_LOCAL_SYM __LIBC_LOCAL_NAME(_vswscanf_s_l))(__buf, __format, __locale, __args); })
#endif /* !__CRT_HAVE__vswscanf_s_l */
#ifdef __CRT_HAVE__vwprintf_s_l
__CDECLARE(__ATTR_NONNULL((1)),__STDC_INT_AS_SSIZE_T,__NOTHROW_NCX,_vwprintf_s_l,(wchar_t const *__format, __locale_t __locale, __builtin_va_list __args),(__format,__locale,__args))
#elif (defined(__CRT_HAVE__vfwprintf_s_l) || defined(__CRT_HAVE___stdio_common_vfwprintf_s) || defined(__CRT_HAVE___stdio_common_vfwprintf) || defined(__CRT_HAVE_vfwprintf) || defined(__CRT_HAVE_file_wprinter) || defined(__CRT_HAVE_file_wprinter_unlocked) || defined(__CRT_HAVE_fputwc) || defined(__CRT_HAVE_putwc)) && !defined(__NO_STDSTREAMS)
#include <libc/local/corecrt_wstdio/_vwprintf_s_l.h>
__NAMESPACE_LOCAL_USING_OR_IMPL(_vwprintf_s_l, __FORCELOCAL __ATTR_ARTIFICIAL __ATTR_NONNULL((1)) __STDC_INT_AS_SSIZE_T __NOTHROW_NCX(__LIBCCALL _vwprintf_s_l)(wchar_t const *__format, __locale_t __locale, __builtin_va_list __args) { return (__NAMESPACE_LOCAL_SYM __LIBC_LOCAL_NAME(_vwprintf_s_l))(__format, __locale, __args); })
#endif /* ... */
#ifdef __CRT_HAVE__vwprintf_p_l
__CDECLARE(__ATTR_NONNULL((1)),__STDC_INT_AS_SSIZE_T,__NOTHROW_NCX,_vwprintf_p_l,(wchar_t const *__format, __locale_t __locale, __builtin_va_list __args),(__format,__locale,__args))
#elif (defined(__CRT_HAVE__vfwprintf_s_l) || defined(__CRT_HAVE___stdio_common_vfwprintf_s) || defined(__CRT_HAVE___stdio_common_vfwprintf) || defined(__CRT_HAVE_vfwprintf) || defined(__CRT_HAVE_file_wprinter) || defined(__CRT_HAVE_file_wprinter_unlocked) || defined(__CRT_HAVE_fputwc) || defined(__CRT_HAVE_putwc)) && !defined(__NO_STDSTREAMS)
#include <libc/local/corecrt_wstdio/_vwprintf_p_l.h>
__NAMESPACE_LOCAL_USING_OR_IMPL(_vwprintf_p_l, __FORCELOCAL __ATTR_ARTIFICIAL __ATTR_NONNULL((1)) __STDC_INT_AS_SSIZE_T __NOTHROW_NCX(__LIBCCALL _vwprintf_p_l)(wchar_t const *__format, __locale_t __locale, __builtin_va_list __args) { return (__NAMESPACE_LOCAL_SYM __LIBC_LOCAL_NAME(_vwprintf_p_l))(__format, __locale, __args); })
#endif /* ... */
#ifdef __CRT_HAVE__vwscanf_s_l
__CDECLARE(__ATTR_NONNULL((1)),__STDC_INT_AS_SSIZE_T,__NOTHROW_NCX,_vwscanf_s_l,(wchar_t const *__format, __locale_t __locale, __builtin_va_list __args),(__format,__locale,__args))
#elif (defined(__CRT_HAVE__vfwprintf_s_l) || defined(__CRT_HAVE___stdio_common_vfwprintf_s) || defined(__CRT_HAVE___stdio_common_vfwprintf) || defined(__CRT_HAVE_vfwprintf) || defined(__CRT_HAVE_file_wprinter) || defined(__CRT_HAVE_file_wprinter_unlocked) || defined(__CRT_HAVE_fputwc) || defined(__CRT_HAVE_putwc)) && !defined(__NO_STDSTREAMS)
#include <libc/local/corecrt_wstdio/_vwscanf_s_l.h>
__NAMESPACE_LOCAL_USING_OR_IMPL(_vwscanf_s_l, __FORCELOCAL __ATTR_ARTIFICIAL __ATTR_NONNULL((1)) __STDC_INT_AS_SSIZE_T __NOTHROW_NCX(__LIBCCALL _vwscanf_s_l)(wchar_t const *__format, __locale_t __locale, __builtin_va_list __args) { return (__NAMESPACE_LOCAL_SYM __LIBC_LOCAL_NAME(_vwscanf_s_l))(__format, __locale, __args); })
#endif /* ... */
#ifdef __CRT_HAVE__fwprintf_l
__LIBC __ATTR_NONNULL((1, 2)) __STDC_INT_AS_SSIZE_T __NOTHROW_NCX(__VLIBCCALL _fwprintf_l)(FILE *__stream, wchar_t const *__format, __locale_t __locale, ...) __CASMNAME_SAME("_fwprintf_l");
#elif defined(__CRT_HAVE__vfwprintf_l) || defined(__CRT_HAVE_vfwprintf) || defined(__CRT_HAVE_file_wprinter) || defined(__CRT_HAVE_file_wprinter_unlocked) || defined(__CRT_HAVE_fputwc) || defined(__CRT_HAVE_putwc)
#include <libc/local/corecrt_wstdio/_fwprintf_l.h>
#ifdef __cplusplus
__NAMESPACE_LOCAL_USING_OR_IMPL(_fwprintf_l, __FORCELOCAL __ATTR_ARTIFICIAL __ATTR_NONNULL((1, 2)) __STDC_INT_AS_SSIZE_T __NOTHROW_NCX(__VLIBCCALL _fwprintf_l)(FILE *__stream, wchar_t const *__format, __locale_t __locale, ...) { return (__NAMESPACE_LOCAL_SYM __LIBC_LOCAL_NAME(_fwprintf_l))(__stream, __format, __locale, __builtin_va_arg_pack()); })
#else /* __cplusplus */
#define _fwprintf_l (__NAMESPACE_LOCAL_SYM __LIBC_LOCAL_NAME(_fwprintf_l))
#endif /* !__cplusplus */
#endif /* ... */
#ifdef __CRT_HAVE__fwprintf_s_l
__LIBC __ATTR_NONNULL((1, 2)) __STDC_INT_AS_SSIZE_T __NOTHROW_NCX(__VLIBCCALL _fwprintf_s_l)(FILE *__stream, wchar_t const *__format, __locale_t __locale, ...) __CASMNAME_SAME("_fwprintf_s_l");
#elif defined(__CRT_HAVE__vfwprintf_s_l) || defined(__CRT_HAVE___stdio_common_vfwprintf_s) || defined(__CRT_HAVE___stdio_common_vfwprintf) || defined(__CRT_HAVE_vfwprintf) || defined(__CRT_HAVE_file_wprinter) || defined(__CRT_HAVE_file_wprinter_unlocked) || defined(__CRT_HAVE_fputwc) || defined(__CRT_HAVE_putwc)
#include <libc/local/corecrt_wstdio/_fwprintf_s_l.h>
#ifdef __cplusplus
__NAMESPACE_LOCAL_USING_OR_IMPL(_fwprintf_s_l, __FORCELOCAL __ATTR_ARTIFICIAL __ATTR_NONNULL((1, 2)) __STDC_INT_AS_SSIZE_T __NOTHROW_NCX(__VLIBCCALL _fwprintf_s_l)(FILE *__stream, wchar_t const *__format, __locale_t __locale, ...) { return (__NAMESPACE_LOCAL_SYM __LIBC_LOCAL_NAME(_fwprintf_s_l))(__stream, __format, __locale, __builtin_va_arg_pack()); })
#else /* __cplusplus */
#define _fwprintf_s_l (__NAMESPACE_LOCAL_SYM __LIBC_LOCAL_NAME(_fwprintf_s_l))
#endif /* !__cplusplus */
#endif /* ... */
#ifdef __CRT_HAVE__fwprintf_p_l
__LIBC __ATTR_NONNULL((1, 2)) __STDC_INT_AS_SSIZE_T __NOTHROW_NCX(__VLIBCCALL _fwprintf_p_l)(FILE *__stream, wchar_t const *__format, __locale_t __locale, ...) __CASMNAME_SAME("_fwprintf_p_l");
#elif defined(__CRT_HAVE__vfwprintf_p_l) || defined(__CRT_HAVE___stdio_common_vfwprintf_p) || defined(__CRT_HAVE_vfwprintf) || defined(__CRT_HAVE_file_wprinter) || defined(__CRT_HAVE_file_wprinter_unlocked) || defined(__CRT_HAVE_fputwc) || defined(__CRT_HAVE_putwc)
#include <libc/local/corecrt_wstdio/_fwprintf_p_l.h>
#ifdef __cplusplus
__NAMESPACE_LOCAL_USING_OR_IMPL(_fwprintf_p_l, __FORCELOCAL __ATTR_ARTIFICIAL __ATTR_NONNULL((1, 2)) __STDC_INT_AS_SSIZE_T __NOTHROW_NCX(__VLIBCCALL _fwprintf_p_l)(FILE *__stream, wchar_t const *__format, __locale_t __locale, ...) { return (__NAMESPACE_LOCAL_SYM __LIBC_LOCAL_NAME(_fwprintf_p_l))(__stream, __format, __locale, __builtin_va_arg_pack()); })
#else /* __cplusplus */
#define _fwprintf_p_l (__NAMESPACE_LOCAL_SYM __LIBC_LOCAL_NAME(_fwprintf_p_l))
#endif /* !__cplusplus */
#endif /* ... */
#ifdef __CRT_HAVE__wprintf_l
__LIBC __ATTR_NONNULL((1)) __STDC_INT_AS_SSIZE_T __NOTHROW_NCX(__VLIBCCALL _wprintf_l)(wchar_t const *__format, __locale_t __locale, ...) __CASMNAME_SAME("_wprintf_l");
#else /* __CRT_HAVE__wprintf_l */
#include <__crt.h>
#if defined(__CRT_HAVE__vwprintf_l) || defined(__CRT_HAVE_vwprintf) || ((defined(__CRT_HAVE_vfwprintf) || defined(__CRT_HAVE_file_wprinter) || defined(__CRT_HAVE_file_wprinter_unlocked) || defined(__CRT_HAVE_fputwc) || defined(__CRT_HAVE_putwc)) && !defined(__NO_STDSTREAMS))
#include <libc/local/corecrt_wstdio/_wprintf_l.h>
#ifdef __cplusplus
__NAMESPACE_LOCAL_USING_OR_IMPL(_wprintf_l, __FORCELOCAL __ATTR_ARTIFICIAL __ATTR_NONNULL((1)) __STDC_INT_AS_SSIZE_T __NOTHROW_NCX(__VLIBCCALL _wprintf_l)(wchar_t const *__format, __locale_t __locale, ...) { return (__NAMESPACE_LOCAL_SYM __LIBC_LOCAL_NAME(_wprintf_l))(__format, __locale, __builtin_va_arg_pack()); })
#else /* __cplusplus */
#define _wprintf_l (__NAMESPACE_LOCAL_SYM __LIBC_LOCAL_NAME(_wprintf_l))
#endif /* !__cplusplus */
#endif /* __CRT_HAVE__vwprintf_l || __CRT_HAVE_vwprintf || ((__CRT_HAVE_vfwprintf || __CRT_HAVE_file_wprinter || __CRT_HAVE_file_wprinter_unlocked || __CRT_HAVE_fputwc || __CRT_HAVE_putwc) && !__NO_STDSTREAMS) */
#endif /* !__CRT_HAVE__wprintf_l */
#ifdef __CRT_HAVE__wprintf_s_l
__LIBC __ATTR_NONNULL((1)) __STDC_INT_AS_SSIZE_T __NOTHROW_NCX(__VLIBCCALL _wprintf_s_l)(wchar_t const *__format, __locale_t __locale, ...) __CASMNAME_SAME("_wprintf_s_l");
#elif defined(__CRT_HAVE__vwprintf_s_l) || ((defined(__CRT_HAVE__vfwprintf_s_l) || defined(__CRT_HAVE___stdio_common_vfwprintf_s) || defined(__CRT_HAVE___stdio_common_vfwprintf) || defined(__CRT_HAVE_vfwprintf) || defined(__CRT_HAVE_file_wprinter) || defined(__CRT_HAVE_file_wprinter_unlocked) || defined(__CRT_HAVE_fputwc) || defined(__CRT_HAVE_putwc)) && !defined(__NO_STDSTREAMS))
#include <libc/local/corecrt_wstdio/_wprintf_s_l.h>
#ifdef __cplusplus
__NAMESPACE_LOCAL_USING_OR_IMPL(_wprintf_s_l, __FORCELOCAL __ATTR_ARTIFICIAL __ATTR_NONNULL((1)) __STDC_INT_AS_SSIZE_T __NOTHROW_NCX(__VLIBCCALL _wprintf_s_l)(wchar_t const *__format, __locale_t __locale, ...) { return (__NAMESPACE_LOCAL_SYM __LIBC_LOCAL_NAME(_wprintf_s_l))(__format, __locale, __builtin_va_arg_pack()); })
#else /* __cplusplus */
#define _wprintf_s_l (__NAMESPACE_LOCAL_SYM __LIBC_LOCAL_NAME(_wprintf_s_l))
#endif /* !__cplusplus */
#endif /* ... */
#ifdef __CRT_HAVE__wprintf_p_l
__LIBC __ATTR_NONNULL((1)) __STDC_INT_AS_SSIZE_T __NOTHROW_NCX(__VLIBCCALL _wprintf_p_l)(wchar_t const *__format, __locale_t __locale, ...) __CASMNAME_SAME("_wprintf_p_l");
#elif defined(__CRT_HAVE__vwprintf_p_l) || ((defined(__CRT_HAVE__vfwprintf_s_l) || defined(__CRT_HAVE___stdio_common_vfwprintf_s) || defined(__CRT_HAVE___stdio_common_vfwprintf) || defined(__CRT_HAVE_vfwprintf) || defined(__CRT_HAVE_file_wprinter) || defined(__CRT_HAVE_file_wprinter_unlocked) || defined(__CRT_HAVE_fputwc) || defined(__CRT_HAVE_putwc)) && !defined(__NO_STDSTREAMS))
#include <libc/local/corecrt_wstdio/_wprintf_p_l.h>
#ifdef __cplusplus
__NAMESPACE_LOCAL_USING_OR_IMPL(_wprintf_p_l, __FORCELOCAL __ATTR_ARTIFICIAL __ATTR_NONNULL((1)) __STDC_INT_AS_SSIZE_T __NOTHROW_NCX(__VLIBCCALL _wprintf_p_l)(wchar_t const *__format, __locale_t __locale, ...) { return (__NAMESPACE_LOCAL_SYM __LIBC_LOCAL_NAME(_wprintf_p_l))(__format, __locale, __builtin_va_arg_pack()); })
#else /* __cplusplus */
#define _wprintf_p_l (__NAMESPACE_LOCAL_SYM __LIBC_LOCAL_NAME(_wprintf_p_l))
#endif /* !__cplusplus */
#endif /* ... */
#ifdef __CRT_HAVE___swprintf_l
__LIBC __ATTR_NONNULL((1, 2)) __STDC_INT_AS_SSIZE_T __NOTHROW_NCX(__VLIBCCALL __swprintf_l)(wchar_t *__buf, wchar_t const *__format, __locale_t __locale, ...) __CASMNAME_SAME("__swprintf_l");
#else /* __CRT_HAVE___swprintf_l */
#include <libc/local/corecrt_wstdio/__swprintf_l.h>
#ifdef __cplusplus
__NAMESPACE_LOCAL_USING_OR_IMPL(__swprintf_l, __FORCELOCAL __ATTR_ARTIFICIAL __ATTR_NONNULL((1, 2)) __STDC_INT_AS_SSIZE_T __NOTHROW_NCX(__VLIBCCALL __swprintf_l)(wchar_t *__buf, wchar_t const *__format, __locale_t __locale, ...) { return (__NAMESPACE_LOCAL_SYM __LIBC_LOCAL_NAME(__swprintf_l))(__buf, __format, __locale, __builtin_va_arg_pack()); })
#else /* __cplusplus */
#define __swprintf_l (__NAMESPACE_LOCAL_SYM __LIBC_LOCAL_NAME(__swprintf_l))
#endif /* !__cplusplus */
#endif /* !__CRT_HAVE___swprintf_l */
#ifdef __CRT_HAVE__fwprintf_p
__LIBC __ATTR_NONNULL((1, 2)) __STDC_INT_AS_SSIZE_T __NOTHROW_NCX(__VLIBCCALL _fwprintf_p)(FILE *__stream, wchar_t const *__format, ...) __CASMNAME_SAME("_fwprintf_p");
#elif defined(__CRT_HAVE__vfwprintf_p) || defined(__CRT_HAVE__vfwprintf_p_l) || defined(__CRT_HAVE___stdio_common_vfwprintf_p) || defined(__CRT_HAVE_vfwprintf) || defined(__CRT_HAVE_file_wprinter) || defined(__CRT_HAVE_file_wprinter_unlocked) || defined(__CRT_HAVE_fputwc) || defined(__CRT_HAVE_putwc)
#include <libc/local/corecrt_wstdio/_fwprintf_p.h>
#ifdef __cplusplus
__NAMESPACE_LOCAL_USING_OR_IMPL(_fwprintf_p, __FORCELOCAL __ATTR_ARTIFICIAL __ATTR_NONNULL((1, 2)) __STDC_INT_AS_SSIZE_T __NOTHROW_NCX(__VLIBCCALL _fwprintf_p)(FILE *__stream, wchar_t const *__format, ...) { return (__NAMESPACE_LOCAL_SYM __LIBC_LOCAL_NAME(_fwprintf_p))(__stream, __format, __builtin_va_arg_pack()); })
#else /* __cplusplus */
#define _fwprintf_p (__NAMESPACE_LOCAL_SYM __LIBC_LOCAL_NAME(_fwprintf_p))
#endif /* !__cplusplus */
#endif /* ... */
#ifdef __CRT_HAVE__wprintf_p
__LIBC __ATTR_NONNULL((1)) __STDC_INT_AS_SSIZE_T __NOTHROW_NCX(__VLIBCCALL _wprintf_p)(wchar_t const *__format, ...) __CASMNAME_SAME("_wprintf_p");
#elif defined(__CRT_HAVE__vwprintf_p) || defined(__CRT_HAVE__vwprintf_p_l) || ((defined(__CRT_HAVE__vfwprintf_s_l) || defined(__CRT_HAVE___stdio_common_vfwprintf_s) || defined(__CRT_HAVE___stdio_common_vfwprintf) || defined(__CRT_HAVE_vfwprintf) || defined(__CRT_HAVE_file_wprinter) || defined(__CRT_HAVE_file_wprinter_unlocked) || defined(__CRT_HAVE_fputwc) || defined(__CRT_HAVE_putwc)) && !defined(__NO_STDSTREAMS))
#include <libc/local/corecrt_wstdio/_wprintf_p.h>
#ifdef __cplusplus
__NAMESPACE_LOCAL_USING_OR_IMPL(_wprintf_p, __FORCELOCAL __ATTR_ARTIFICIAL __ATTR_NONNULL((1)) __STDC_INT_AS_SSIZE_T __NOTHROW_NCX(__VLIBCCALL _wprintf_p)(wchar_t const *__format, ...) { return (__NAMESPACE_LOCAL_SYM __LIBC_LOCAL_NAME(_wprintf_p))(__format, __builtin_va_arg_pack()); })
#else /* __cplusplus */
#define _wprintf_p (__NAMESPACE_LOCAL_SYM __LIBC_LOCAL_NAME(_wprintf_p))
#endif /* !__cplusplus */
#endif /* ... */
#ifdef __CRT_HAVE__swprintf
__LIBC __ATTR_NONNULL((1, 2)) __STDC_INT_AS_SSIZE_T __NOTHROW_NCX(__VLIBCCALL _swprintf)(wchar_t *__buf, wchar_t const *__format, ...) __CASMNAME_SAME("_swprintf");
#else /* __CRT_HAVE__swprintf */
#include <libc/local/corecrt_wstdio/_swprintf.h>
#ifdef __cplusplus
__NAMESPACE_LOCAL_USING_OR_IMPL(_swprintf, __FORCELOCAL __ATTR_ARTIFICIAL __ATTR_NONNULL((1, 2)) __STDC_INT_AS_SSIZE_T __NOTHROW_NCX(__VLIBCCALL _swprintf)(wchar_t *__buf, wchar_t const *__format, ...) { return (__NAMESPACE_LOCAL_SYM __LIBC_LOCAL_NAME(_swprintf))(__buf, __format, __builtin_va_arg_pack()); })
#else /* __cplusplus */
#define _swprintf (__NAMESPACE_LOCAL_SYM __LIBC_LOCAL_NAME(_swprintf))
#endif /* !__cplusplus */
#endif /* !__CRT_HAVE__swprintf */
#ifdef __CRT_HAVE__swprintf_l
__LIBC __ATTR_NONNULL((3)) __STDC_INT_AS_SSIZE_T __NOTHROW_NCX(__VLIBCCALL _swprintf_l)(wchar_t *__buf, size_t __bufsize, wchar_t const *__format, __locale_t __locale, ...) __CASMNAME_SAME("_swprintf_l");
#elif defined(__CRT_HAVE__swprintf_c_l)
__LIBC __ATTR_NONNULL((3)) __STDC_INT_AS_SSIZE_T __NOTHROW_NCX(__VLIBCCALL _swprintf_l)(wchar_t *__buf, size_t __bufsize, wchar_t const *__format, __locale_t __locale, ...) __CASMNAME("_swprintf_c_l");
#else /* ... */
#include <libc/local/corecrt_wstdio/_swprintf_c_l.h>
#define _swprintf_l (__NAMESPACE_LOCAL_SYM __LIBC_LOCAL_NAME(_swprintf_c_l))
#endif /* !... */
#ifdef __CRT_HAVE__swprintf_s_l
__LIBC __ATTR_NONNULL((3)) __STDC_INT_AS_SSIZE_T __NOTHROW_NCX(__VLIBCCALL _swprintf_s_l)(wchar_t *__buf, size_t __bufsize, wchar_t const *__format, __locale_t __locale, ...) __CASMNAME_SAME("_swprintf_s_l");
#else /* __CRT_HAVE__swprintf_s_l */
#include <libc/local/corecrt_wstdio/_swprintf_s_l.h>
#ifdef __cplusplus
__NAMESPACE_LOCAL_USING_OR_IMPL(_swprintf_s_l, __FORCELOCAL __ATTR_ARTIFICIAL __ATTR_NONNULL((3)) __STDC_INT_AS_SSIZE_T __NOTHROW_NCX(__VLIBCCALL _swprintf_s_l)(wchar_t *__buf, size_t __bufsize, wchar_t const *__format, __locale_t __locale, ...) { return (__NAMESPACE_LOCAL_SYM __LIBC_LOCAL_NAME(_swprintf_s_l))(__buf, __bufsize, __format, __locale, __builtin_va_arg_pack()); })
#else /* __cplusplus */
#define _swprintf_s_l (__NAMESPACE_LOCAL_SYM __LIBC_LOCAL_NAME(_swprintf_s_l))
#endif /* !__cplusplus */
#endif /* !__CRT_HAVE__swprintf_s_l */
#ifdef __CRT_HAVE__swprintf_p_l
__LIBC __ATTR_NONNULL((3)) __STDC_INT_AS_SSIZE_T __NOTHROW_NCX(__VLIBCCALL _swprintf_p_l)(wchar_t *__buf, size_t __bufsize, wchar_t const *__format, __locale_t __locale, ...) __CASMNAME_SAME("_swprintf_p_l");
#else /* __CRT_HAVE__swprintf_p_l */
#include <libc/local/corecrt_wstdio/_swprintf_p_l.h>
#ifdef __cplusplus
__NAMESPACE_LOCAL_USING_OR_IMPL(_swprintf_p_l, __FORCELOCAL __ATTR_ARTIFICIAL __ATTR_NONNULL((3)) __STDC_INT_AS_SSIZE_T __NOTHROW_NCX(__VLIBCCALL _swprintf_p_l)(wchar_t *__buf, size_t __bufsize, wchar_t const *__format, __locale_t __locale, ...) { return (__NAMESPACE_LOCAL_SYM __LIBC_LOCAL_NAME(_swprintf_p_l))(__buf, __bufsize, __format, __locale, __builtin_va_arg_pack()); })
#else /* __cplusplus */
#define _swprintf_p_l (__NAMESPACE_LOCAL_SYM __LIBC_LOCAL_NAME(_swprintf_p_l))
#endif /* !__cplusplus */
#endif /* !__CRT_HAVE__swprintf_p_l */
#ifdef __CRT_HAVE__swprintf_l
__LIBC __ATTR_NONNULL((3)) __STDC_INT_AS_SSIZE_T __NOTHROW_NCX(__VLIBCCALL _swprintf_c_l)(wchar_t *__buf, size_t __bufsize, wchar_t const *__format, __locale_t __locale, ...) __CASMNAME("_swprintf_l");
#elif defined(__CRT_HAVE__swprintf_c_l)
__LIBC __ATTR_NONNULL((3)) __STDC_INT_AS_SSIZE_T __NOTHROW_NCX(__VLIBCCALL _swprintf_c_l)(wchar_t *__buf, size_t __bufsize, wchar_t const *__format, __locale_t __locale, ...) __CASMNAME_SAME("_swprintf_c_l");
#else /* ... */
#include <libc/local/corecrt_wstdio/_swprintf_c_l.h>
#ifdef __cplusplus
__NAMESPACE_LOCAL_USING_OR_IMPL(_swprintf_c_l, __FORCELOCAL __ATTR_ARTIFICIAL __ATTR_NONNULL((3)) __STDC_INT_AS_SSIZE_T __NOTHROW_NCX(__VLIBCCALL _swprintf_c_l)(wchar_t *__buf, size_t __bufsize, wchar_t const *__format, __locale_t __locale, ...) { return (__NAMESPACE_LOCAL_SYM __LIBC_LOCAL_NAME(_swprintf_c_l))(__buf, __bufsize, __format, __locale, __builtin_va_arg_pack()); })
#else /* __cplusplus */
#define _swprintf_c_l (__NAMESPACE_LOCAL_SYM __LIBC_LOCAL_NAME(_swprintf_c_l))
#endif /* !__cplusplus */
#endif /* !... */
#ifdef __CRT_HAVE__snwprintf_l
__LIBC __ATTR_NONNULL((3)) __STDC_INT_AS_SSIZE_T __NOTHROW_NCX(__VLIBCCALL _snwprintf_l)(wchar_t *__buf, size_t __bufsize, wchar_t const *__format, __locale_t __locale, ...) __CASMNAME_SAME("_snwprintf_l");
#else /* __CRT_HAVE__snwprintf_l */
#include <libc/local/corecrt_wstdio/_snwprintf_l.h>
#ifdef __cplusplus
__NAMESPACE_LOCAL_USING_OR_IMPL(_snwprintf_l, __FORCELOCAL __ATTR_ARTIFICIAL __ATTR_NONNULL((3)) __STDC_INT_AS_SSIZE_T __NOTHROW_NCX(__VLIBCCALL _snwprintf_l)(wchar_t *__buf, size_t __bufsize, wchar_t const *__format, __locale_t __locale, ...) { return (__NAMESPACE_LOCAL_SYM __LIBC_LOCAL_NAME(_snwprintf_l))(__buf, __bufsize, __format, __locale, __builtin_va_arg_pack()); })
#else /* __cplusplus */
#define _snwprintf_l (__NAMESPACE_LOCAL_SYM __LIBC_LOCAL_NAME(_snwprintf_l))
#endif /* !__cplusplus */
#endif /* !__CRT_HAVE__snwprintf_l */
#ifdef __CRT_HAVE__snwprintf_s_l
__LIBC __ATTR_NONNULL((4)) __STDC_INT_AS_SSIZE_T __NOTHROW_NCX(__VLIBCCALL _snwprintf_s_l)(wchar_t *__buf, size_t __bufsize, size_t __maxsize, wchar_t const *__format, __locale_t __locale, ...) __CASMNAME_SAME("_snwprintf_s_l");
#else /* __CRT_HAVE__snwprintf_s_l */
#include <libc/local/corecrt_wstdio/_snwprintf_s_l.h>
#ifdef __cplusplus
__NAMESPACE_LOCAL_USING_OR_IMPL(_snwprintf_s_l, __FORCELOCAL __ATTR_ARTIFICIAL __ATTR_NONNULL((4)) __STDC_INT_AS_SSIZE_T __NOTHROW_NCX(__VLIBCCALL _snwprintf_s_l)(wchar_t *__buf, size_t __bufsize, size_t __maxsize, wchar_t const *__format, __locale_t __locale, ...) { return (__NAMESPACE_LOCAL_SYM __LIBC_LOCAL_NAME(_snwprintf_s_l))(__buf, __bufsize, __maxsize, __format, __locale, __builtin_va_arg_pack()); })
#else /* __cplusplus */
#define _snwprintf_s_l (__NAMESPACE_LOCAL_SYM __LIBC_LOCAL_NAME(_snwprintf_s_l))
#endif /* !__cplusplus */
#endif /* !__CRT_HAVE__snwprintf_s_l */
#ifdef __CRT_HAVE__swprintf_p
__LIBC __ATTR_NONNULL((3)) __STDC_INT_AS_SSIZE_T __NOTHROW_NCX(__VLIBCCALL _swprintf_p)(wchar_t *__buf, size_t __bufsize, wchar_t const *__format, ...) __CASMNAME_SAME("_swprintf_p");
#else /* __CRT_HAVE__swprintf_p */
#include <libc/local/corecrt_wstdio/_swprintf_p.h>
#ifdef __cplusplus
__NAMESPACE_LOCAL_USING_OR_IMPL(_swprintf_p, __FORCELOCAL __ATTR_ARTIFICIAL __ATTR_NONNULL((3)) __STDC_INT_AS_SSIZE_T __NOTHROW_NCX(__VLIBCCALL _swprintf_p)(wchar_t *__buf, size_t __bufsize, wchar_t const *__format, ...) { return (__NAMESPACE_LOCAL_SYM __LIBC_LOCAL_NAME(_swprintf_p))(__buf, __bufsize, __format, __builtin_va_arg_pack()); })
#else /* __cplusplus */
#define _swprintf_p (__NAMESPACE_LOCAL_SYM __LIBC_LOCAL_NAME(_swprintf_p))
#endif /* !__cplusplus */
#endif /* !__CRT_HAVE__swprintf_p */
#ifdef __CRT_HAVE__swprintf_c
__LIBC __ATTR_NONNULL((3)) __STDC_INT_AS_SSIZE_T __NOTHROW_NCX(__VLIBCCALL _swprintf_c)(wchar_t *__buf, size_t __bufsize, wchar_t const *__format, ...) __CASMNAME_SAME("_swprintf_c");
#else /* __CRT_HAVE__swprintf_c */
#include <libc/local/corecrt_wstdio/_swprintf_c.h>
#ifdef __cplusplus
__NAMESPACE_LOCAL_USING_OR_IMPL(_swprintf_c, __FORCELOCAL __ATTR_ARTIFICIAL __ATTR_NONNULL((3)) __STDC_INT_AS_SSIZE_T __NOTHROW_NCX(__VLIBCCALL _swprintf_c)(wchar_t *__buf, size_t __bufsize, wchar_t const *__format, ...) { return (__NAMESPACE_LOCAL_SYM __LIBC_LOCAL_NAME(_swprintf_c))(__buf, __bufsize, __format, __builtin_va_arg_pack()); })
#else /* __cplusplus */
#define _swprintf_c (__NAMESPACE_LOCAL_SYM __LIBC_LOCAL_NAME(_swprintf_c))
#endif /* !__cplusplus */
#endif /* !__CRT_HAVE__swprintf_c */
#ifdef __CRT_HAVE__snwprintf
__LIBC __ATTR_NONNULL((3)) __STDC_INT_AS_SSIZE_T __NOTHROW_NCX(__VLIBCCALL _snwprintf)(wchar_t *__buf, size_t __bufsize, wchar_t const *__format, ...) __CASMNAME_SAME("_snwprintf");
#else /* __CRT_HAVE__snwprintf */
#include <libc/local/corecrt_wstdio/_snwprintf.h>
#ifdef __cplusplus
__NAMESPACE_LOCAL_USING_OR_IMPL(_snwprintf, __FORCELOCAL __ATTR_ARTIFICIAL __ATTR_NONNULL((3)) __STDC_INT_AS_SSIZE_T __NOTHROW_NCX(__VLIBCCALL _snwprintf)(wchar_t *__buf, size_t __bufsize, wchar_t const *__format, ...) { return (__NAMESPACE_LOCAL_SYM __LIBC_LOCAL_NAME(_snwprintf))(__buf, __bufsize, __format, __builtin_va_arg_pack()); })
#else /* __cplusplus */
#define _snwprintf (__NAMESPACE_LOCAL_SYM __LIBC_LOCAL_NAME(_snwprintf))
#endif /* !__cplusplus */
#endif /* !__CRT_HAVE__snwprintf */
#ifdef __CRT_HAVE__snwprintf_s
__LIBC __ATTR_NONNULL((4)) __STDC_INT_AS_SSIZE_T __NOTHROW_NCX(__VLIBCCALL _snwprintf_s)(wchar_t *__buf, size_t __bufsize, size_t __maxsize, wchar_t const *__format, ...) __CASMNAME_SAME("_snwprintf_s");
#else /* __CRT_HAVE__snwprintf_s */
#include <libc/local/corecrt_wstdio/_snwprintf_s.h>
#ifdef __cplusplus
__NAMESPACE_LOCAL_USING_OR_IMPL(_snwprintf_s, __FORCELOCAL __ATTR_ARTIFICIAL __ATTR_NONNULL((4)) __STDC_INT_AS_SSIZE_T __NOTHROW_NCX(__VLIBCCALL _snwprintf_s)(wchar_t *__buf, size_t __bufsize, size_t __maxsize, wchar_t const *__format, ...) { return (__NAMESPACE_LOCAL_SYM __LIBC_LOCAL_NAME(_snwprintf_s))(__buf, __bufsize, __maxsize, __format, __builtin_va_arg_pack()); })
#else /* __cplusplus */
#define _snwprintf_s (__NAMESPACE_LOCAL_SYM __LIBC_LOCAL_NAME(_snwprintf_s))
#endif /* !__cplusplus */
#endif /* !__CRT_HAVE__snwprintf_s */
#ifdef __CRT_HAVE__scwprintf_l
__LIBC __ATTR_NONNULL((1)) __STDC_INT_AS_SSIZE_T __NOTHROW_NCX(__VLIBCCALL _scwprintf_l)(wchar_t const *__format, __locale_t __locale, ...) __CASMNAME_SAME("_scwprintf_l");
#else /* __CRT_HAVE__scwprintf_l */
#include <libc/local/corecrt_wstdio/_scwprintf_l.h>
#ifdef __cplusplus
__NAMESPACE_LOCAL_USING_OR_IMPL(_scwprintf_l, __FORCELOCAL __ATTR_ARTIFICIAL __ATTR_NONNULL((1)) __STDC_INT_AS_SSIZE_T __NOTHROW_NCX(__VLIBCCALL _scwprintf_l)(wchar_t const *__format, __locale_t __locale, ...) { return (__NAMESPACE_LOCAL_SYM __LIBC_LOCAL_NAME(_scwprintf_l))(__format, __locale, __builtin_va_arg_pack()); })
#else /* __cplusplus */
#define _scwprintf_l (__NAMESPACE_LOCAL_SYM __LIBC_LOCAL_NAME(_scwprintf_l))
#endif /* !__cplusplus */
#endif /* !__CRT_HAVE__scwprintf_l */
#ifdef __CRT_HAVE__scwprintf_p_l
__LIBC __ATTR_NONNULL((1)) __STDC_INT_AS_SSIZE_T __NOTHROW_NCX(__VLIBCCALL _scwprintf_p_l)(wchar_t const *__format, __locale_t __locale, ...) __CASMNAME_SAME("_scwprintf_p_l");
#else /* __CRT_HAVE__scwprintf_p_l */
#include <libc/local/corecrt_wstdio/_scwprintf_p_l.h>
#ifdef __cplusplus
__NAMESPACE_LOCAL_USING_OR_IMPL(_scwprintf_p_l, __FORCELOCAL __ATTR_ARTIFICIAL __ATTR_NONNULL((1)) __STDC_INT_AS_SSIZE_T __NOTHROW_NCX(__VLIBCCALL _scwprintf_p_l)(wchar_t const *__format, __locale_t __locale, ...) { return (__NAMESPACE_LOCAL_SYM __LIBC_LOCAL_NAME(_scwprintf_p_l))(__format, __locale, __builtin_va_arg_pack()); })
#else /* __cplusplus */
#define _scwprintf_p_l (__NAMESPACE_LOCAL_SYM __LIBC_LOCAL_NAME(_scwprintf_p_l))
#endif /* !__cplusplus */
#endif /* !__CRT_HAVE__scwprintf_p_l */
#ifdef __CRT_HAVE__scwprintf
__LIBC __ATTR_NONNULL((1)) __STDC_INT_AS_SSIZE_T __NOTHROW_NCX(__VLIBCCALL _scwprintf)(wchar_t const *__format, ...) __CASMNAME_SAME("_scwprintf");
#else /* __CRT_HAVE__scwprintf */
#include <libc/local/corecrt_wstdio/_scwprintf.h>
#ifdef __cplusplus
__NAMESPACE_LOCAL_USING_OR_IMPL(_scwprintf, __FORCELOCAL __ATTR_ARTIFICIAL __ATTR_NONNULL((1)) __STDC_INT_AS_SSIZE_T __NOTHROW_NCX(__VLIBCCALL _scwprintf)(wchar_t const *__format, ...) { return (__NAMESPACE_LOCAL_SYM __LIBC_LOCAL_NAME(_scwprintf))(__format, __builtin_va_arg_pack()); })
#else /* __cplusplus */
#define _scwprintf (__NAMESPACE_LOCAL_SYM __LIBC_LOCAL_NAME(_scwprintf))
#endif /* !__cplusplus */
#endif /* !__CRT_HAVE__scwprintf */
#ifdef __CRT_HAVE__scwprintf_p
__LIBC __ATTR_NONNULL((1)) __STDC_INT_AS_SSIZE_T __NOTHROW_NCX(__VLIBCCALL _scwprintf_p)(wchar_t const *__format, ...) __CASMNAME_SAME("_scwprintf_p");
#else /* __CRT_HAVE__scwprintf_p */
#include <libc/local/corecrt_wstdio/_scwprintf_p.h>
#ifdef __cplusplus
__NAMESPACE_LOCAL_USING_OR_IMPL(_scwprintf_p, __FORCELOCAL __ATTR_ARTIFICIAL __ATTR_NONNULL((1)) __STDC_INT_AS_SSIZE_T __NOTHROW_NCX(__VLIBCCALL _scwprintf_p)(wchar_t const *__format, ...) { return (__NAMESPACE_LOCAL_SYM __LIBC_LOCAL_NAME(_scwprintf_p))(__format, __builtin_va_arg_pack()); })
#else /* __cplusplus */
#define _scwprintf_p (__NAMESPACE_LOCAL_SYM __LIBC_LOCAL_NAME(_scwprintf_p))
#endif /* !__cplusplus */
#endif /* !__CRT_HAVE__scwprintf_p */
#ifdef __CRT_HAVE__fwscanf_l
__LIBC __ATTR_WUNUSED __ATTR_NONNULL((1, 2)) __STDC_INT_AS_SSIZE_T __NOTHROW_NCX(__VLIBCCALL _fwscanf_l)(FILE *__stream, wchar_t const *__format, __locale_t __locale, ...) __CASMNAME_SAME("_fwscanf_l");
#elif defined(__CRT_HAVE__vfwscanf_l) || defined(__CRT_HAVE___stdio_common_vfscanf) || defined(__CRT_HAVE_vfscanf) || defined(__CRT_HAVE__vfscanf) || defined(__CRT_HAVE__vfscanf_s) || defined(__CRT_HAVE__IO_vfscanf) || defined(__CRT_HAVE_vfscanf_unlocked) || defined(__CRT_HAVE___vfscanf) || ((defined(__CRT_HAVE_fgetc) || defined(__CRT_HAVE_getc) || defined(__CRT_HAVE__IO_getc) || defined(__CRT_HAVE_fgetc_unlocked) || defined(__CRT_HAVE_getc_unlocked) || (defined(__CRT_DOS) && (defined(__CRT_HAVE__filbuf) || defined(__CRT_HAVE___uflow))) || defined(__CRT_HAVE_fread) || defined(__CRT_HAVE__IO_fread) || defined(__CRT_HAVE_fread_unlocked) || defined(__CRT_HAVE__fread_nolock)) && (defined(__CRT_HAVE_ungetc) || defined(__CRT_HAVE__IO_ungetc) || defined(__CRT_HAVE_ungetc_unlocked) || defined(__CRT_HAVE__ungetc_nolock)))
#include <libc/local/corecrt_wstdio/_fwscanf_l.h>
#ifdef __cplusplus
__NAMESPACE_LOCAL_USING_OR_IMPL(_fwscanf_l, __FORCELOCAL __ATTR_ARTIFICIAL __ATTR_WUNUSED __ATTR_NONNULL((1, 2)) __STDC_INT_AS_SSIZE_T __NOTHROW_NCX(__VLIBCCALL _fwscanf_l)(FILE *__stream, wchar_t const *__format, __locale_t __locale, ...) { return (__NAMESPACE_LOCAL_SYM __LIBC_LOCAL_NAME(_fwscanf_l))(__stream, __format, __locale, __builtin_va_arg_pack()); })
#else /* __cplusplus */
#define _fwscanf_l (__NAMESPACE_LOCAL_SYM __LIBC_LOCAL_NAME(_fwscanf_l))
#endif /* !__cplusplus */
#endif /* ... */
#ifdef __CRT_HAVE__fwscanf_s_l
__LIBC __ATTR_WUNUSED __ATTR_NONNULL((1, 2)) __STDC_INT_AS_SSIZE_T __NOTHROW_NCX(__VLIBCCALL _fwscanf_s_l)(FILE *__stream, wchar_t const *__format, __locale_t __locale, ...) __CASMNAME_SAME("_fwscanf_s_l");
#elif defined(__CRT_HAVE__vfwscanf_s_l) || defined(__CRT_HAVE___stdio_common_vfscanf) || defined(__CRT_HAVE_vfscanf) || defined(__CRT_HAVE__vfscanf) || defined(__CRT_HAVE__vfscanf_s) || defined(__CRT_HAVE__IO_vfscanf) || defined(__CRT_HAVE_vfscanf_unlocked) || defined(__CRT_HAVE___vfscanf) || ((defined(__CRT_HAVE_fgetc) || defined(__CRT_HAVE_getc) || defined(__CRT_HAVE__IO_getc) || defined(__CRT_HAVE_fgetc_unlocked) || defined(__CRT_HAVE_getc_unlocked) || (defined(__CRT_DOS) && (defined(__CRT_HAVE__filbuf) || defined(__CRT_HAVE___uflow))) || defined(__CRT_HAVE_fread) || defined(__CRT_HAVE__IO_fread) || defined(__CRT_HAVE_fread_unlocked) || defined(__CRT_HAVE__fread_nolock)) && (defined(__CRT_HAVE_ungetc) || defined(__CRT_HAVE__IO_ungetc) || defined(__CRT_HAVE_ungetc_unlocked) || defined(__CRT_HAVE__ungetc_nolock)))
#include <libc/local/corecrt_wstdio/_fwscanf_s_l.h>
#ifdef __cplusplus
__NAMESPACE_LOCAL_USING_OR_IMPL(_fwscanf_s_l, __FORCELOCAL __ATTR_ARTIFICIAL __ATTR_WUNUSED __ATTR_NONNULL((1, 2)) __STDC_INT_AS_SSIZE_T __NOTHROW_NCX(__VLIBCCALL _fwscanf_s_l)(FILE *__stream, wchar_t const *__format, __locale_t __locale, ...) { return (__NAMESPACE_LOCAL_SYM __LIBC_LOCAL_NAME(_fwscanf_s_l))(__stream, __format, __locale, __builtin_va_arg_pack()); })
#else /* __cplusplus */
#define _fwscanf_s_l (__NAMESPACE_LOCAL_SYM __LIBC_LOCAL_NAME(_fwscanf_s_l))
#endif /* !__cplusplus */
#endif /* ... */
#ifdef __CRT_HAVE__wscanf_l
__LIBC __ATTR_WUNUSED __ATTR_NONNULL((1)) __STDC_INT_AS_SSIZE_T __NOTHROW_NCX(__VLIBCCALL _wscanf_l)(wchar_t const *__format, __locale_t __locale, ...) __CASMNAME_SAME("_wscanf_l");
#else /* __CRT_HAVE__wscanf_l */
#include <__crt.h>
#if defined(__CRT_HAVE__vwscanf_l) || defined(__CRT_HAVE_vwscanf) || (defined(__CRT_HAVE_vfwscanf) && !defined(__NO_STDSTREAMS))
#include <libc/local/corecrt_wstdio/_wscanf_l.h>
#ifdef __cplusplus
__NAMESPACE_LOCAL_USING_OR_IMPL(_wscanf_l, __FORCELOCAL __ATTR_ARTIFICIAL __ATTR_WUNUSED __ATTR_NONNULL((1)) __STDC_INT_AS_SSIZE_T __NOTHROW_NCX(__VLIBCCALL _wscanf_l)(wchar_t const *__format, __locale_t __locale, ...) { return (__NAMESPACE_LOCAL_SYM __LIBC_LOCAL_NAME(_wscanf_l))(__format, __locale, __builtin_va_arg_pack()); })
#else /* __cplusplus */
#define _wscanf_l (__NAMESPACE_LOCAL_SYM __LIBC_LOCAL_NAME(_wscanf_l))
#endif /* !__cplusplus */
#endif /* __CRT_HAVE__vwscanf_l || __CRT_HAVE_vwscanf || (__CRT_HAVE_vfwscanf && !__NO_STDSTREAMS) */
#endif /* !__CRT_HAVE__wscanf_l */
#ifdef __CRT_HAVE__wscanf_s_l
__LIBC __ATTR_WUNUSED __ATTR_NONNULL((1)) __STDC_INT_AS_SSIZE_T __NOTHROW_NCX(__VLIBCCALL _wscanf_s_l)(wchar_t const *__format, __locale_t __locale, ...) __CASMNAME_SAME("_wscanf_s_l");
#elif defined(__CRT_HAVE__vwscanf_s_l) || ((defined(__CRT_HAVE__vfwprintf_s_l) || defined(__CRT_HAVE___stdio_common_vfwprintf_s) || defined(__CRT_HAVE___stdio_common_vfwprintf) || defined(__CRT_HAVE_vfwprintf) || defined(__CRT_HAVE_file_wprinter) || defined(__CRT_HAVE_file_wprinter_unlocked) || defined(__CRT_HAVE_fputwc) || defined(__CRT_HAVE_putwc)) && !defined(__NO_STDSTREAMS))
#include <libc/local/corecrt_wstdio/_wscanf_s_l.h>
#ifdef __cplusplus
__NAMESPACE_LOCAL_USING_OR_IMPL(_wscanf_s_l, __FORCELOCAL __ATTR_ARTIFICIAL __ATTR_WUNUSED __ATTR_NONNULL((1)) __STDC_INT_AS_SSIZE_T __NOTHROW_NCX(__VLIBCCALL _wscanf_s_l)(wchar_t const *__format, __locale_t __locale, ...) { return (__NAMESPACE_LOCAL_SYM __LIBC_LOCAL_NAME(_wscanf_s_l))(__format, __locale, __builtin_va_arg_pack()); })
#else /* __cplusplus */
#define _wscanf_s_l (__NAMESPACE_LOCAL_SYM __LIBC_LOCAL_NAME(_wscanf_s_l))
#endif /* !__cplusplus */
#endif /* ... */
#ifdef __CRT_HAVE__swscanf_l
__LIBC __ATTR_WUNUSED __ATTR_NONNULL((1, 2)) __STDC_INT_AS_SSIZE_T __NOTHROW_NCX(__VLIBCCALL _swscanf_l)(wchar_t const *__buf, wchar_t const *__format, __locale_t __locale, ...) __CASMNAME_SAME("_swscanf_l");
#else /* __CRT_HAVE__swscanf_l */
#include <libc/local/corecrt_wstdio/_swscanf_l.h>
#ifdef __cplusplus
__NAMESPACE_LOCAL_USING_OR_IMPL(_swscanf_l, __FORCELOCAL __ATTR_ARTIFICIAL __ATTR_WUNUSED __ATTR_NONNULL((1, 2)) __STDC_INT_AS_SSIZE_T __NOTHROW_NCX(__VLIBCCALL _swscanf_l)(wchar_t const *__buf, wchar_t const *__format, __locale_t __locale, ...) { return (__NAMESPACE_LOCAL_SYM __LIBC_LOCAL_NAME(_swscanf_l))(__buf, __format, __locale, __builtin_va_arg_pack()); })
#else /* __cplusplus */
#define _swscanf_l (__NAMESPACE_LOCAL_SYM __LIBC_LOCAL_NAME(_swscanf_l))
#endif /* !__cplusplus */
#endif /* !__CRT_HAVE__swscanf_l */
#ifdef __CRT_HAVE__swscanf_s_l
__LIBC __ATTR_WUNUSED __ATTR_NONNULL((1, 2)) __STDC_INT_AS_SSIZE_T __NOTHROW_NCX(__VLIBCCALL _swscanf_s_l)(wchar_t const *__buf, wchar_t const *__format, __locale_t __locale, ...) __CASMNAME_SAME("_swscanf_s_l");
#else /* __CRT_HAVE__swscanf_s_l */
#include <libc/local/corecrt_wstdio/_swscanf_s_l.h>
#ifdef __cplusplus
__NAMESPACE_LOCAL_USING_OR_IMPL(_swscanf_s_l, __FORCELOCAL __ATTR_ARTIFICIAL __ATTR_WUNUSED __ATTR_NONNULL((1, 2)) __STDC_INT_AS_SSIZE_T __NOTHROW_NCX(__VLIBCCALL _swscanf_s_l)(wchar_t const *__buf, wchar_t const *__format, __locale_t __locale, ...) { return (__NAMESPACE_LOCAL_SYM __LIBC_LOCAL_NAME(_swscanf_s_l))(__buf, __format, __locale, __builtin_va_arg_pack()); })
#else /* __cplusplus */
#define _swscanf_s_l (__NAMESPACE_LOCAL_SYM __LIBC_LOCAL_NAME(_swscanf_s_l))
#endif /* !__cplusplus */
#endif /* !__CRT_HAVE__swscanf_s_l */
#ifdef __CRT_HAVE__snwscanf_l
__LIBC __ATTR_WUNUSED __ATTR_NONNULL((3)) __STDC_INT_AS_SSIZE_T __NOTHROW_NCX(__VLIBCCALL _snwscanf_l)(wchar_t const *__buf, size_t __bufsize, wchar_t const *__format, __locale_t __locale, ...) __CASMNAME_SAME("_snwscanf_l");
#else /* __CRT_HAVE__snwscanf_l */
#include <libc/local/corecrt_wstdio/_snwscanf_l.h>
#ifdef __cplusplus
__NAMESPACE_LOCAL_USING_OR_IMPL(_snwscanf_l, __FORCELOCAL __ATTR_ARTIFICIAL __ATTR_WUNUSED __ATTR_NONNULL((3)) __STDC_INT_AS_SSIZE_T __NOTHROW_NCX(__VLIBCCALL _snwscanf_l)(wchar_t const *__buf, size_t __bufsize, wchar_t const *__format, __locale_t __locale, ...) { return (__NAMESPACE_LOCAL_SYM __LIBC_LOCAL_NAME(_snwscanf_l))(__buf, __bufsize, __format, __locale, __builtin_va_arg_pack()); })
#else /* __cplusplus */
#define _snwscanf_l (__NAMESPACE_LOCAL_SYM __LIBC_LOCAL_NAME(_snwscanf_l))
#endif /* !__cplusplus */
#endif /* !__CRT_HAVE__snwscanf_l */
#ifdef __CRT_HAVE__snwscanf
__LIBC __ATTR_WUNUSED __ATTR_NONNULL((3)) __STDC_INT_AS_SSIZE_T __NOTHROW_NCX(__VLIBCCALL _snwscanf)(wchar_t const *__buf, size_t __bufsize, wchar_t const *__format, ...) __CASMNAME_SAME("_snwscanf");
#else /* __CRT_HAVE__snwscanf */
#include <libc/local/corecrt_wstdio/_snwscanf.h>
#ifdef __cplusplus
__NAMESPACE_LOCAL_USING_OR_IMPL(_snwscanf, __FORCELOCAL __ATTR_ARTIFICIAL __ATTR_WUNUSED __ATTR_NONNULL((3)) __STDC_INT_AS_SSIZE_T __NOTHROW_NCX(__VLIBCCALL _snwscanf)(wchar_t const *__buf, size_t __bufsize, wchar_t const *__format, ...) { return (__NAMESPACE_LOCAL_SYM __LIBC_LOCAL_NAME(_snwscanf))(__buf, __bufsize, __format, __builtin_va_arg_pack()); })
#else /* __cplusplus */
#define _snwscanf (__NAMESPACE_LOCAL_SYM __LIBC_LOCAL_NAME(_snwscanf))
#endif /* !__cplusplus */
#endif /* !__CRT_HAVE__snwscanf */
#ifdef __CRT_HAVE__snwscanf_s_l
__LIBC __ATTR_WUNUSED __ATTR_NONNULL((3)) __STDC_INT_AS_SSIZE_T __NOTHROW_NCX(__VLIBCCALL _snwscanf_s_l)(wchar_t const *__buf, size_t __bufsize, wchar_t const *__format, __locale_t __locale, ...) __CASMNAME_SAME("_snwscanf_s_l");
#else /* __CRT_HAVE__snwscanf_s_l */
#include <libc/local/corecrt_wstdio/_snwscanf_s_l.h>
#ifdef __cplusplus
__NAMESPACE_LOCAL_USING_OR_IMPL(_snwscanf_s_l, __FORCELOCAL __ATTR_ARTIFICIAL __ATTR_WUNUSED __ATTR_NONNULL((3)) __STDC_INT_AS_SSIZE_T __NOTHROW_NCX(__VLIBCCALL _snwscanf_s_l)(wchar_t const *__buf, size_t __bufsize, wchar_t const *__format, __locale_t __locale, ...) { return (__NAMESPACE_LOCAL_SYM __LIBC_LOCAL_NAME(_snwscanf_s_l))(__buf, __bufsize, __format, __locale, __builtin_va_arg_pack()); })
#else /* __cplusplus */
#define _snwscanf_s_l (__NAMESPACE_LOCAL_SYM __LIBC_LOCAL_NAME(_snwscanf_s_l))
#endif /* !__cplusplus */
#endif /* !__CRT_HAVE__snwscanf_s_l */
#ifdef __CRT_HAVE__snwscanf_s
__LIBC __ATTR_WUNUSED __ATTR_NONNULL((3)) __STDC_INT_AS_SSIZE_T __NOTHROW_NCX(__VLIBCCALL _snwscanf_s)(wchar_t const *__buf, size_t __bufsize, wchar_t const *__format, ...) __CASMNAME_SAME("_snwscanf_s");
#else /* __CRT_HAVE__snwscanf_s */
#include <libc/local/corecrt_wstdio/_snwscanf_s.h>
#ifdef __cplusplus
__NAMESPACE_LOCAL_USING_OR_IMPL(_snwscanf_s, __FORCELOCAL __ATTR_ARTIFICIAL __ATTR_WUNUSED __ATTR_NONNULL((3)) __STDC_INT_AS_SSIZE_T __NOTHROW_NCX(__VLIBCCALL _snwscanf_s)(wchar_t const *__buf, size_t __bufsize, wchar_t const *__format, ...) { return (__NAMESPACE_LOCAL_SYM __LIBC_LOCAL_NAME(_snwscanf_s))(__buf, __bufsize, __format, __builtin_va_arg_pack()); })
#else /* __cplusplus */
#define _snwscanf_s (__NAMESPACE_LOCAL_SYM __LIBC_LOCAL_NAME(_snwscanf_s))
#endif /* !__cplusplus */
#endif /* !__CRT_HAVE__snwscanf_s */
#ifdef __USE_DOS_SLIB
#ifdef __CRT_HAVE_vfwprintf_s
__CDECLARE(__ATTR_NONNULL((1, 2)),__STDC_INT_AS_SSIZE_T,__NOTHROW_NCX,vfwprintf_s,(FILE *__stream, wchar_t const *__format, __builtin_va_list __args),(__stream,__format,__args))
#elif defined(__CRT_HAVE__vfwprintf_s_l) || defined(__CRT_HAVE___stdio_common_vfwprintf_s) || defined(__CRT_HAVE___stdio_common_vfwprintf) || defined(__CRT_HAVE_vfwprintf) || defined(__CRT_HAVE_file_wprinter) || defined(__CRT_HAVE_file_wprinter_unlocked) || defined(__CRT_HAVE_fputwc) || defined(__CRT_HAVE_putwc)
#include <libc/local/corecrt_wstdio/vfwprintf_s.h>
__NAMESPACE_LOCAL_USING_OR_IMPL(vfwprintf_s, __FORCELOCAL __ATTR_ARTIFICIAL __ATTR_NONNULL((1, 2)) __STDC_INT_AS_SSIZE_T __NOTHROW_NCX(__LIBCCALL vfwprintf_s)(FILE *__stream, wchar_t const *__format, __builtin_va_list __args) { return (__NAMESPACE_LOCAL_SYM __LIBC_LOCAL_NAME(vfwprintf_s))(__stream, __format, __args); })
#endif /* ... */
#ifdef __CRT_HAVE_vwprintf_s
__CDECLARE(__ATTR_NONNULL((1)),__STDC_INT_AS_SSIZE_T,__NOTHROW_NCX,vwprintf_s,(wchar_t const *__format, __builtin_va_list __args),(__format,__args))
#elif defined(__CRT_HAVE__vfwprintf_s_l) || defined(__CRT_HAVE___stdio_common_vfwprintf_s) || defined(__CRT_HAVE___stdio_common_vfwprintf) || defined(__CRT_HAVE_vfwprintf) || defined(__CRT_HAVE_file_wprinter) || defined(__CRT_HAVE_file_wprinter_unlocked) || defined(__CRT_HAVE_fputwc) || defined(__CRT_HAVE_putwc)
#include <libc/local/corecrt_wstdio/vwprintf_s.h>
__NAMESPACE_LOCAL_USING_OR_IMPL(vwprintf_s, __FORCELOCAL __ATTR_ARTIFICIAL __ATTR_NONNULL((1)) __STDC_INT_AS_SSIZE_T __NOTHROW_NCX(__LIBCCALL vwprintf_s)(wchar_t const *__format, __builtin_va_list __args) { return (__NAMESPACE_LOCAL_SYM __LIBC_LOCAL_NAME(vwprintf_s))(__format, __args); })
#endif /* ... */
#ifdef __CRT_HAVE_vswprintf_s
__CDECLARE(__ATTR_NONNULL((3)),__STDC_INT_AS_SSIZE_T,__NOTHROW_NCX,vswprintf_s,(wchar_t *__buf, size_t __bufsize, wchar_t const *__format, __builtin_va_list __args),(__buf,__bufsize,__format,__args))
#else /* __CRT_HAVE_vswprintf_s */
#include <libc/local/corecrt_wstdio/vswprintf_s.h>
__NAMESPACE_LOCAL_USING_OR_IMPL(vswprintf_s, __FORCELOCAL __ATTR_ARTIFICIAL __ATTR_NONNULL((3)) __STDC_INT_AS_SSIZE_T __NOTHROW_NCX(__LIBCCALL vswprintf_s)(wchar_t *__buf, size_t __bufsize, wchar_t const *__format, __builtin_va_list __args) { return (__NAMESPACE_LOCAL_SYM __LIBC_LOCAL_NAME(vswprintf_s))(__buf, __bufsize, __format, __args); })
#endif /* !__CRT_HAVE_vswprintf_s */
#ifdef __CRT_HAVE_vfwscanf_s
__CDECLARE(__ATTR_WUNUSED __ATTR_NONNULL((1, 2)),__STDC_INT_AS_SSIZE_T,__NOTHROW_NCX,vfwscanf_s,(FILE *__stream, wchar_t const *__format, __builtin_va_list __args),(__stream,__format,__args))
#elif defined(__CRT_HAVE__vfwscanf_s_l) || defined(__CRT_HAVE___stdio_common_vfscanf) || defined(__CRT_HAVE_vfscanf) || defined(__CRT_HAVE__vfscanf) || defined(__CRT_HAVE__vfscanf_s) || defined(__CRT_HAVE__IO_vfscanf) || defined(__CRT_HAVE_vfscanf_unlocked) || defined(__CRT_HAVE___vfscanf) || ((defined(__CRT_HAVE_fgetc) || defined(__CRT_HAVE_getc) || defined(__CRT_HAVE__IO_getc) || defined(__CRT_HAVE_fgetc_unlocked) || defined(__CRT_HAVE_getc_unlocked) || (defined(__CRT_DOS) && (defined(__CRT_HAVE__filbuf) || defined(__CRT_HAVE___uflow))) || defined(__CRT_HAVE_fread) || defined(__CRT_HAVE__IO_fread) || defined(__CRT_HAVE_fread_unlocked) || defined(__CRT_HAVE__fread_nolock)) && (defined(__CRT_HAVE_ungetc) || defined(__CRT_HAVE__IO_ungetc) || defined(__CRT_HAVE_ungetc_unlocked) || defined(__CRT_HAVE__ungetc_nolock)))
#include <libc/local/corecrt_wstdio/vfwscanf_s.h>
__NAMESPACE_LOCAL_USING_OR_IMPL(vfwscanf_s, __FORCELOCAL __ATTR_ARTIFICIAL __ATTR_WUNUSED __ATTR_NONNULL((1, 2)) __STDC_INT_AS_SSIZE_T __NOTHROW_NCX(__LIBCCALL vfwscanf_s)(FILE *__stream, wchar_t const *__format, __builtin_va_list __args) { return (__NAMESPACE_LOCAL_SYM __LIBC_LOCAL_NAME(vfwscanf_s))(__stream, __format, __args); })
#endif /* ... */
#ifdef __CRT_HAVE_vwscanf_s
__CDECLARE(__ATTR_WUNUSED __ATTR_NONNULL((1)),__STDC_INT_AS_SSIZE_T,__NOTHROW_NCX,vwscanf_s,(wchar_t const *__format, __builtin_va_list __args),(__format,__args))
#elif defined(__CRT_HAVE__vfwscanf_s_l) || defined(__CRT_HAVE___stdio_common_vfscanf) || defined(__CRT_HAVE_vfscanf) || defined(__CRT_HAVE__vfscanf) || defined(__CRT_HAVE__vfscanf_s) || defined(__CRT_HAVE__IO_vfscanf) || defined(__CRT_HAVE_vfscanf_unlocked) || defined(__CRT_HAVE___vfscanf) || ((defined(__CRT_HAVE_fgetc) || defined(__CRT_HAVE_getc) || defined(__CRT_HAVE__IO_getc) || defined(__CRT_HAVE_fgetc_unlocked) || defined(__CRT_HAVE_getc_unlocked) || (defined(__CRT_DOS) && (defined(__CRT_HAVE__filbuf) || defined(__CRT_HAVE___uflow))) || defined(__CRT_HAVE_fread) || defined(__CRT_HAVE__IO_fread) || defined(__CRT_HAVE_fread_unlocked) || defined(__CRT_HAVE__fread_nolock)) && (defined(__CRT_HAVE_ungetc) || defined(__CRT_HAVE__IO_ungetc) || defined(__CRT_HAVE_ungetc_unlocked) || defined(__CRT_HAVE__ungetc_nolock)))
#include <libc/local/corecrt_wstdio/vwscanf_s.h>
__NAMESPACE_LOCAL_USING_OR_IMPL(vwscanf_s, __FORCELOCAL __ATTR_ARTIFICIAL __ATTR_WUNUSED __ATTR_NONNULL((1)) __STDC_INT_AS_SSIZE_T __NOTHROW_NCX(__LIBCCALL vwscanf_s)(wchar_t const *__format, __builtin_va_list __args) { return (__NAMESPACE_LOCAL_SYM __LIBC_LOCAL_NAME(vwscanf_s))(__format, __args); })
#endif /* ... */
#ifdef __CRT_HAVE_vswscanf_s
__CDECLARE(__ATTR_WUNUSED __ATTR_NONNULL((1, 2)),__STDC_INT_AS_SSIZE_T,__NOTHROW_NCX,vswscanf_s,(wchar_t const *__buf, wchar_t const *__format, __builtin_va_list __args),(__buf,__format,__args))
#else /* __CRT_HAVE_vswscanf_s */
#include <libc/local/corecrt_wstdio/vswscanf_s.h>
__NAMESPACE_LOCAL_USING_OR_IMPL(vswscanf_s, __FORCELOCAL __ATTR_ARTIFICIAL __ATTR_WUNUSED __ATTR_NONNULL((1, 2)) __STDC_INT_AS_SSIZE_T __NOTHROW_NCX(__LIBCCALL vswscanf_s)(wchar_t const *__buf, wchar_t const *__format, __builtin_va_list __args) { return (__NAMESPACE_LOCAL_SYM __LIBC_LOCAL_NAME(vswscanf_s))(__buf, __format, __args); })
#endif /* !__CRT_HAVE_vswscanf_s */
#ifdef __CRT_HAVE_fwprintf_s
__LIBC __ATTR_NONNULL((1, 2)) __STDC_INT_AS_SSIZE_T __NOTHROW_NCX(__VLIBCCALL fwprintf_s)(FILE *__stream, wchar_t const *__format, ...) __CASMNAME_SAME("fwprintf_s");
#elif defined(__CRT_HAVE_vfwprintf_s) || defined(__CRT_HAVE__vfwprintf_s_l) || defined(__CRT_HAVE___stdio_common_vfwprintf_s) || defined(__CRT_HAVE___stdio_common_vfwprintf) || defined(__CRT_HAVE_vfwprintf) || defined(__CRT_HAVE_file_wprinter) || defined(__CRT_HAVE_file_wprinter_unlocked) || defined(__CRT_HAVE_fputwc) || defined(__CRT_HAVE_putwc)
#include <libc/local/corecrt_wstdio/fwprintf_s.h>
#ifdef __cplusplus
__NAMESPACE_LOCAL_USING_OR_IMPL(fwprintf_s, __FORCELOCAL __ATTR_ARTIFICIAL __ATTR_NONNULL((1, 2)) __STDC_INT_AS_SSIZE_T __NOTHROW_NCX(__VLIBCCALL fwprintf_s)(FILE *__stream, wchar_t const *__format, ...) { return (__NAMESPACE_LOCAL_SYM __LIBC_LOCAL_NAME(fwprintf_s))(__stream, __format, __builtin_va_arg_pack()); })
#else /* __cplusplus */
#define fwprintf_s (__NAMESPACE_LOCAL_SYM __LIBC_LOCAL_NAME(fwprintf_s))
#endif /* !__cplusplus */
#endif /* ... */
#ifdef __CRT_HAVE_wprintf_s
__LIBC __ATTR_NONNULL((1)) __STDC_INT_AS_SSIZE_T __NOTHROW_NCX(__VLIBCCALL wprintf_s)(wchar_t const *__format, ...) __CASMNAME_SAME("wprintf_s");
#elif defined(__CRT_HAVE_vwprintf_s) || defined(__CRT_HAVE__vfwprintf_s_l) || defined(__CRT_HAVE___stdio_common_vfwprintf_s) || defined(__CRT_HAVE___stdio_common_vfwprintf) || defined(__CRT_HAVE_vfwprintf) || defined(__CRT_HAVE_file_wprinter) || defined(__CRT_HAVE_file_wprinter_unlocked) || defined(__CRT_HAVE_fputwc) || defined(__CRT_HAVE_putwc)
#include <libc/local/corecrt_wstdio/wprintf_s.h>
#ifdef __cplusplus
__NAMESPACE_LOCAL_USING_OR_IMPL(wprintf_s, __FORCELOCAL __ATTR_ARTIFICIAL __ATTR_NONNULL((1)) __STDC_INT_AS_SSIZE_T __NOTHROW_NCX(__VLIBCCALL wprintf_s)(wchar_t const *__format, ...) { return (__NAMESPACE_LOCAL_SYM __LIBC_LOCAL_NAME(wprintf_s))(__format, __builtin_va_arg_pack()); })
#else /* __cplusplus */
#define wprintf_s (__NAMESPACE_LOCAL_SYM __LIBC_LOCAL_NAME(wprintf_s))
#endif /* !__cplusplus */
#endif /* ... */
#ifdef __CRT_HAVE_swprintf_s
__LIBC __ATTR_NONNULL((3)) __STDC_INT_AS_SSIZE_T __NOTHROW_NCX(__VLIBCCALL swprintf_s)(wchar_t *__buf, size_t __bufsize, wchar_t const *__format, ...) __CASMNAME_SAME("swprintf_s");
#else /* __CRT_HAVE_swprintf_s */
#include <libc/local/corecrt_wstdio/swprintf_s.h>
#ifdef __cplusplus
__NAMESPACE_LOCAL_USING_OR_IMPL(swprintf_s, __FORCELOCAL __ATTR_ARTIFICIAL __ATTR_NONNULL((3)) __STDC_INT_AS_SSIZE_T __NOTHROW_NCX(__VLIBCCALL swprintf_s)(wchar_t *__buf, size_t __bufsize, wchar_t const *__format, ...) { return (__NAMESPACE_LOCAL_SYM __LIBC_LOCAL_NAME(swprintf_s))(__buf, __bufsize, __format, __builtin_va_arg_pack()); })
#else /* __cplusplus */
#define swprintf_s (__NAMESPACE_LOCAL_SYM __LIBC_LOCAL_NAME(swprintf_s))
#endif /* !__cplusplus */
#endif /* !__CRT_HAVE_swprintf_s */
#ifdef __CRT_HAVE_fwscanf_s
__LIBC __ATTR_WUNUSED __ATTR_NONNULL((1, 2)) __STDC_INT_AS_SSIZE_T __NOTHROW_NCX(__VLIBCCALL fwscanf_s)(FILE *__stream, wchar_t const *__format, ...) __CASMNAME_SAME("fwscanf_s");
#elif defined(__CRT_HAVE_vfwscanf_s) || defined(__CRT_HAVE__vfwscanf_s_l) || defined(__CRT_HAVE___stdio_common_vfscanf) || defined(__CRT_HAVE_vfscanf) || defined(__CRT_HAVE__vfscanf) || defined(__CRT_HAVE__vfscanf_s) || defined(__CRT_HAVE__IO_vfscanf) || defined(__CRT_HAVE_vfscanf_unlocked) || defined(__CRT_HAVE___vfscanf) || ((defined(__CRT_HAVE_fgetc) || defined(__CRT_HAVE_getc) || defined(__CRT_HAVE__IO_getc) || defined(__CRT_HAVE_fgetc_unlocked) || defined(__CRT_HAVE_getc_unlocked) || (defined(__CRT_DOS) && (defined(__CRT_HAVE__filbuf) || defined(__CRT_HAVE___uflow))) || defined(__CRT_HAVE_fread) || defined(__CRT_HAVE__IO_fread) || defined(__CRT_HAVE_fread_unlocked) || defined(__CRT_HAVE__fread_nolock)) && (defined(__CRT_HAVE_ungetc) || defined(__CRT_HAVE__IO_ungetc) || defined(__CRT_HAVE_ungetc_unlocked) || defined(__CRT_HAVE__ungetc_nolock)))
#include <libc/local/corecrt_wstdio/fwscanf_s.h>
#ifdef __cplusplus
__NAMESPACE_LOCAL_USING_OR_IMPL(fwscanf_s, __FORCELOCAL __ATTR_ARTIFICIAL __ATTR_WUNUSED __ATTR_NONNULL((1, 2)) __STDC_INT_AS_SSIZE_T __NOTHROW_NCX(__VLIBCCALL fwscanf_s)(FILE *__stream, wchar_t const *__format, ...) { return (__NAMESPACE_LOCAL_SYM __LIBC_LOCAL_NAME(fwscanf_s))(__stream, __format, __builtin_va_arg_pack()); })
#else /* __cplusplus */
#define fwscanf_s (__NAMESPACE_LOCAL_SYM __LIBC_LOCAL_NAME(fwscanf_s))
#endif /* !__cplusplus */
#endif /* ... */
#ifdef __CRT_HAVE_wscanf_s
__LIBC __ATTR_WUNUSED __ATTR_NONNULL((1)) __STDC_INT_AS_SSIZE_T __NOTHROW_NCX(__VLIBCCALL wscanf_s)(wchar_t const *__format, ...) __CASMNAME_SAME("wscanf_s");
#elif defined(__CRT_HAVE_vwscanf_s) || defined(__CRT_HAVE__vfwscanf_s_l) || defined(__CRT_HAVE___stdio_common_vfscanf) || defined(__CRT_HAVE_vfscanf) || defined(__CRT_HAVE__vfscanf) || defined(__CRT_HAVE__vfscanf_s) || defined(__CRT_HAVE__IO_vfscanf) || defined(__CRT_HAVE_vfscanf_unlocked) || defined(__CRT_HAVE___vfscanf) || ((defined(__CRT_HAVE_fgetc) || defined(__CRT_HAVE_getc) || defined(__CRT_HAVE__IO_getc) || defined(__CRT_HAVE_fgetc_unlocked) || defined(__CRT_HAVE_getc_unlocked) || (defined(__CRT_DOS) && (defined(__CRT_HAVE__filbuf) || defined(__CRT_HAVE___uflow))) || defined(__CRT_HAVE_fread) || defined(__CRT_HAVE__IO_fread) || defined(__CRT_HAVE_fread_unlocked) || defined(__CRT_HAVE__fread_nolock)) && (defined(__CRT_HAVE_ungetc) || defined(__CRT_HAVE__IO_ungetc) || defined(__CRT_HAVE_ungetc_unlocked) || defined(__CRT_HAVE__ungetc_nolock)))
#include <libc/local/corecrt_wstdio/wscanf_s.h>
#ifdef __cplusplus
__NAMESPACE_LOCAL_USING_OR_IMPL(wscanf_s, __FORCELOCAL __ATTR_ARTIFICIAL __ATTR_WUNUSED __ATTR_NONNULL((1)) __STDC_INT_AS_SSIZE_T __NOTHROW_NCX(__VLIBCCALL wscanf_s)(wchar_t const *__format, ...) { return (__NAMESPACE_LOCAL_SYM __LIBC_LOCAL_NAME(wscanf_s))(__format, __builtin_va_arg_pack()); })
#else /* __cplusplus */
#define wscanf_s (__NAMESPACE_LOCAL_SYM __LIBC_LOCAL_NAME(wscanf_s))
#endif /* !__cplusplus */
#endif /* ... */
#ifdef __CRT_HAVE_swscanf_s
__LIBC __ATTR_WUNUSED __ATTR_NONNULL((1, 2)) __STDC_INT_AS_SSIZE_T __NOTHROW_NCX(__VLIBCCALL swscanf_s)(wchar_t const *__buf, wchar_t const *__format, ...) __CASMNAME_SAME("swscanf_s");
#else /* __CRT_HAVE_swscanf_s */
#include <libc/local/corecrt_wstdio/swscanf_s.h>
#ifdef __cplusplus
__NAMESPACE_LOCAL_USING_OR_IMPL(swscanf_s, __FORCELOCAL __ATTR_ARTIFICIAL __ATTR_WUNUSED __ATTR_NONNULL((1, 2)) __STDC_INT_AS_SSIZE_T __NOTHROW_NCX(__VLIBCCALL swscanf_s)(wchar_t const *__buf, wchar_t const *__format, ...) { return (__NAMESPACE_LOCAL_SYM __LIBC_LOCAL_NAME(swscanf_s))(__buf, __format, __builtin_va_arg_pack()); })
#else /* __cplusplus */
#define swscanf_s (__NAMESPACE_LOCAL_SYM __LIBC_LOCAL_NAME(swscanf_s))
#endif /* !__cplusplus */
#endif /* !__CRT_HAVE_swscanf_s */
#endif /* __USE_DOS_SLIB */
#endif /* !_WSTDIO_DEFINED */

__SYSDECL_END
#endif /* __CC__ */

#ifdef __USE_UTF
#if defined(_UCHAR_H) && !defined(_PARTS_UCHAR_STDIO_H)
#include <parts/uchar/stdio.h>
#endif /* _UCHAR_H && !_PARTS_UCHAR_STDIO_H */
#endif /* __USE_UTF */

#endif /* !_CORECRT_WSTDIO_H */