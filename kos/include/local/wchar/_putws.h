/* HASH CRC-32:0x2c976e91 */
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
#ifndef __local__putws_defined
#define __local__putws_defined 1
#include <__crt.h>
#if ((defined(__CRT_HAVE_DOS$fputws_unlocked) && defined(__USE_STDIO_UNLOCKED) && __SIZEOF_WCHAR_T__ == 4) || (defined(__CRT_HAVE_DOS$fputws_unlocked) && defined(__USE_STDIO_UNLOCKED) && __SIZEOF_WCHAR_T__ == 2) || (defined(__CRT_HAVE_DOS$_fputws_nolock) && defined(__USE_STDIO_UNLOCKED) && __SIZEOF_WCHAR_T__ == 4) || (defined(__CRT_HAVE_DOS$_fputws_nolock) && defined(__USE_STDIO_UNLOCKED) && __SIZEOF_WCHAR_T__ == 2) || defined(__CRT_HAVE_fputws) || (defined(__CRT_HAVE_DOS$fputws) && __SIZEOF_WCHAR_T__ == 4) || (defined(__CRT_HAVE_DOS$fputws) && __SIZEOF_WCHAR_T__ == 2) || defined(__CRT_HAVE_fputws_unlocked) || (defined(__CRT_HAVE_DOS$fputws_unlocked) && __SIZEOF_WCHAR_T__ == 4) || (defined(__CRT_HAVE_DOS$fputws_unlocked) && __SIZEOF_WCHAR_T__ == 2) || defined(__CRT_HAVE__fputws_nolock) || (defined(__CRT_HAVE_DOS$_fputws_nolock) && __SIZEOF_WCHAR_T__ == 4) || (defined(__CRT_HAVE_DOS$_fputws_nolock) && __SIZEOF_WCHAR_T__ == 2) || (defined(__CRT_HAVE_DOS$file_wprinter_unlocked) && defined(__USE_STDIO_UNLOCKED) && __SIZEOF_WCHAR_T__ == 4) || (defined(__CRT_HAVE_DOS$file_wprinter_unlocked) && defined(__USE_STDIO_UNLOCKED) && __SIZEOF_WCHAR_T__ == 2) || defined(__CRT_HAVE_file_wprinter) || (defined(__CRT_HAVE_DOS$file_wprinter) && __SIZEOF_WCHAR_T__ == 4) || (defined(__CRT_HAVE_DOS$file_wprinter) && __SIZEOF_WCHAR_T__ == 2) || defined(__CRT_HAVE_file_wprinter_unlocked) || (defined(__CRT_HAVE_DOS$file_wprinter_unlocked) && __SIZEOF_WCHAR_T__ == 4) || (defined(__CRT_HAVE_DOS$file_wprinter_unlocked) && __SIZEOF_WCHAR_T__ == 2) || defined(__CRT_HAVE_fputwc) || (defined(__CRT_HAVE_DOS$fputwc) && __SIZEOF_WCHAR_T__ == 4) || (defined(__CRT_HAVE_DOS$fputwc) && __SIZEOF_WCHAR_T__ == 2) || defined(__CRT_HAVE_putwc) || (defined(__CRT_HAVE_DOS$putwc) && __SIZEOF_WCHAR_T__ == 4) || (defined(__CRT_HAVE_DOS$putwc) && __SIZEOF_WCHAR_T__ == 2)) && !defined(__NO_STDSTREAMS)
#include <features.h>
__NAMESPACE_LOCAL_BEGIN
/* Dependency: fputws from wchar */
#ifndef __local___localdep_fputws_defined
#define __local___localdep_fputws_defined 1
#if defined(__CRT_HAVE_fputws_unlocked) && defined(__USE_STDIO_UNLOCKED)
__CREDIRECT(__ATTR_NONNULL((1, 2)),__STDC_INT_AS_SIZE_T,__THROWING,__localdep_fputws,(__WCHAR_TYPE__ const *__restrict __string, __FILE *__restrict __stream),fputws_unlocked,(__string,__stream))
#elif defined(__CRT_HAVE_DOS$fputws_unlocked) && defined(__USE_STDIO_UNLOCKED) && __SIZEOF_WCHAR_T__ == 4
__CREDIRECT_KOS(__ATTR_NONNULL((1, 2)),__STDC_INT_AS_SIZE_T,__THROWING,__localdep_fputws,(__WCHAR_TYPE__ const *__restrict __string, __FILE *__restrict __stream),fputws_unlocked,(__string,__stream))
#elif defined(__CRT_HAVE_DOS$fputws_unlocked) && defined(__USE_STDIO_UNLOCKED) && __SIZEOF_WCHAR_T__ == 2
__CREDIRECT_DOS(__ATTR_NONNULL((1, 2)),__STDC_INT_AS_SIZE_T,__THROWING,__localdep_fputws,(__WCHAR_TYPE__ const *__restrict __string, __FILE *__restrict __stream),fputws_unlocked,(__string,__stream))
#elif defined(__CRT_HAVE__fputws_nolock) && defined(__USE_STDIO_UNLOCKED)
__CREDIRECT(__ATTR_NONNULL((1, 2)),__STDC_INT_AS_SIZE_T,__THROWING,__localdep_fputws,(__WCHAR_TYPE__ const *__restrict __string, __FILE *__restrict __stream),_fputws_nolock,(__string,__stream))
#elif defined(__CRT_HAVE_DOS$_fputws_nolock) && defined(__USE_STDIO_UNLOCKED) && __SIZEOF_WCHAR_T__ == 4
__CREDIRECT_KOS(__ATTR_NONNULL((1, 2)),__STDC_INT_AS_SIZE_T,__THROWING,__localdep_fputws,(__WCHAR_TYPE__ const *__restrict __string, __FILE *__restrict __stream),_fputws_nolock,(__string,__stream))
#elif defined(__CRT_HAVE_DOS$_fputws_nolock) && defined(__USE_STDIO_UNLOCKED) && __SIZEOF_WCHAR_T__ == 2
__CREDIRECT_DOS(__ATTR_NONNULL((1, 2)),__STDC_INT_AS_SIZE_T,__THROWING,__localdep_fputws,(__WCHAR_TYPE__ const *__restrict __string, __FILE *__restrict __stream),_fputws_nolock,(__string,__stream))
#elif defined(__CRT_HAVE_fputws)
__CREDIRECT(__ATTR_NONNULL((1, 2)),__STDC_INT_AS_SIZE_T,__THROWING,__localdep_fputws,(__WCHAR_TYPE__ const *__restrict __string, __FILE *__restrict __stream),fputws,(__string,__stream))
#elif defined(__CRT_HAVE_DOS$fputws) && __SIZEOF_WCHAR_T__ == 4
__CREDIRECT_KOS(__ATTR_NONNULL((1, 2)),__STDC_INT_AS_SIZE_T,__THROWING,__localdep_fputws,(__WCHAR_TYPE__ const *__restrict __string, __FILE *__restrict __stream),fputws,(__string,__stream))
#elif defined(__CRT_HAVE_DOS$fputws) && __SIZEOF_WCHAR_T__ == 2
__CREDIRECT_DOS(__ATTR_NONNULL((1, 2)),__STDC_INT_AS_SIZE_T,__THROWING,__localdep_fputws,(__WCHAR_TYPE__ const *__restrict __string, __FILE *__restrict __stream),fputws,(__string,__stream))
#elif defined(__CRT_HAVE_fputws_unlocked)
__CREDIRECT(__ATTR_NONNULL((1, 2)),__STDC_INT_AS_SIZE_T,__THROWING,__localdep_fputws,(__WCHAR_TYPE__ const *__restrict __string, __FILE *__restrict __stream),fputws_unlocked,(__string,__stream))
#elif defined(__CRT_HAVE_DOS$fputws_unlocked) && __SIZEOF_WCHAR_T__ == 4
__CREDIRECT_KOS(__ATTR_NONNULL((1, 2)),__STDC_INT_AS_SIZE_T,__THROWING,__localdep_fputws,(__WCHAR_TYPE__ const *__restrict __string, __FILE *__restrict __stream),fputws_unlocked,(__string,__stream))
#elif defined(__CRT_HAVE_DOS$fputws_unlocked) && __SIZEOF_WCHAR_T__ == 2
__CREDIRECT_DOS(__ATTR_NONNULL((1, 2)),__STDC_INT_AS_SIZE_T,__THROWING,__localdep_fputws,(__WCHAR_TYPE__ const *__restrict __string, __FILE *__restrict __stream),fputws_unlocked,(__string,__stream))
#elif defined(__CRT_HAVE__fputws_nolock)
__CREDIRECT(__ATTR_NONNULL((1, 2)),__STDC_INT_AS_SIZE_T,__THROWING,__localdep_fputws,(__WCHAR_TYPE__ const *__restrict __string, __FILE *__restrict __stream),_fputws_nolock,(__string,__stream))
#elif defined(__CRT_HAVE_DOS$_fputws_nolock) && __SIZEOF_WCHAR_T__ == 4
__CREDIRECT_KOS(__ATTR_NONNULL((1, 2)),__STDC_INT_AS_SIZE_T,__THROWING,__localdep_fputws,(__WCHAR_TYPE__ const *__restrict __string, __FILE *__restrict __stream),_fputws_nolock,(__string,__stream))
#elif defined(__CRT_HAVE_DOS$_fputws_nolock) && __SIZEOF_WCHAR_T__ == 2
__CREDIRECT_DOS(__ATTR_NONNULL((1, 2)),__STDC_INT_AS_SIZE_T,__THROWING,__localdep_fputws,(__WCHAR_TYPE__ const *__restrict __string, __FILE *__restrict __stream),_fputws_nolock,(__string,__stream))
#elif (defined(__CRT_HAVE_DOS$file_wprinter_unlocked) && defined(__USE_STDIO_UNLOCKED) && __SIZEOF_WCHAR_T__ == 4) || (defined(__CRT_HAVE_DOS$file_wprinter_unlocked) && defined(__USE_STDIO_UNLOCKED) && __SIZEOF_WCHAR_T__ == 2) || defined(__CRT_HAVE_file_wprinter) || (defined(__CRT_HAVE_DOS$file_wprinter) && __SIZEOF_WCHAR_T__ == 4) || (defined(__CRT_HAVE_DOS$file_wprinter) && __SIZEOF_WCHAR_T__ == 2) || defined(__CRT_HAVE_file_wprinter_unlocked) || (defined(__CRT_HAVE_DOS$file_wprinter_unlocked) && __SIZEOF_WCHAR_T__ == 4) || (defined(__CRT_HAVE_DOS$file_wprinter_unlocked) && __SIZEOF_WCHAR_T__ == 2) || defined(__CRT_HAVE_fputwc) || (defined(__CRT_HAVE_DOS$fputwc) && __SIZEOF_WCHAR_T__ == 4) || (defined(__CRT_HAVE_DOS$fputwc) && __SIZEOF_WCHAR_T__ == 2) || defined(__CRT_HAVE_putwc) || (defined(__CRT_HAVE_DOS$putwc) && __SIZEOF_WCHAR_T__ == 4) || (defined(__CRT_HAVE_DOS$putwc) && __SIZEOF_WCHAR_T__ == 2)
__NAMESPACE_LOCAL_END
#include <local/wchar/fputws.h>
__NAMESPACE_LOCAL_BEGIN
#define __localdep_fputws __LIBC_LOCAL_NAME(fputws)
#else /* ... */
#undef __local___localdep_fputws_defined
#endif /* !... */
#endif /* !__local___localdep_fputws_defined */
__NAMESPACE_LOCAL_END
#include <local/stdstreams.h>
__NAMESPACE_LOCAL_BEGIN
__LOCAL_LIBC(_putws) __ATTR_NONNULL((1)) __STDC_INT_AS_SIZE_T
__NOTHROW_NCX(__LIBCCALL __LIBC_LOCAL_NAME(_putws))(__WCHAR_TYPE__ const *__string) {
	return __localdep_fputws(__string, __LOCAL_stdout);
}
__NAMESPACE_LOCAL_END
#ifndef __local___localdep__putws_defined
#define __local___localdep__putws_defined 1
#define __localdep__putws __LIBC_LOCAL_NAME(_putws)
#endif /* !__local___localdep__putws_defined */
#else /* ((__CRT_HAVE_DOS$fputws_unlocked && __USE_STDIO_UNLOCKED && __SIZEOF_WCHAR_T__ == 4) || (__CRT_HAVE_DOS$fputws_unlocked && __USE_STDIO_UNLOCKED && __SIZEOF_WCHAR_T__ == 2) || (__CRT_HAVE_DOS$_fputws_nolock && __USE_STDIO_UNLOCKED && __SIZEOF_WCHAR_T__ == 4) || (__CRT_HAVE_DOS$_fputws_nolock && __USE_STDIO_UNLOCKED && __SIZEOF_WCHAR_T__ == 2) || __CRT_HAVE_fputws || (__CRT_HAVE_DOS$fputws && __SIZEOF_WCHAR_T__ == 4) || (__CRT_HAVE_DOS$fputws && __SIZEOF_WCHAR_T__ == 2) || __CRT_HAVE_fputws_unlocked || (__CRT_HAVE_DOS$fputws_unlocked && __SIZEOF_WCHAR_T__ == 4) || (__CRT_HAVE_DOS$fputws_unlocked && __SIZEOF_WCHAR_T__ == 2) || __CRT_HAVE__fputws_nolock || (__CRT_HAVE_DOS$_fputws_nolock && __SIZEOF_WCHAR_T__ == 4) || (__CRT_HAVE_DOS$_fputws_nolock && __SIZEOF_WCHAR_T__ == 2) || (__CRT_HAVE_DOS$file_wprinter_unlocked && __USE_STDIO_UNLOCKED && __SIZEOF_WCHAR_T__ == 4) || (__CRT_HAVE_DOS$file_wprinter_unlocked && __USE_STDIO_UNLOCKED && __SIZEOF_WCHAR_T__ == 2) || __CRT_HAVE_file_wprinter || (__CRT_HAVE_DOS$file_wprinter && __SIZEOF_WCHAR_T__ == 4) || (__CRT_HAVE_DOS$file_wprinter && __SIZEOF_WCHAR_T__ == 2) || __CRT_HAVE_file_wprinter_unlocked || (__CRT_HAVE_DOS$file_wprinter_unlocked && __SIZEOF_WCHAR_T__ == 4) || (__CRT_HAVE_DOS$file_wprinter_unlocked && __SIZEOF_WCHAR_T__ == 2) || __CRT_HAVE_fputwc || (__CRT_HAVE_DOS$fputwc && __SIZEOF_WCHAR_T__ == 4) || (__CRT_HAVE_DOS$fputwc && __SIZEOF_WCHAR_T__ == 2) || __CRT_HAVE_putwc || (__CRT_HAVE_DOS$putwc && __SIZEOF_WCHAR_T__ == 4) || (__CRT_HAVE_DOS$putwc && __SIZEOF_WCHAR_T__ == 2)) && !__NO_STDSTREAMS */
#undef __local__putws_defined
#endif /* ((!__CRT_HAVE_DOS$fputws_unlocked || !__USE_STDIO_UNLOCKED || !__SIZEOF_WCHAR_T__ == 4) && (!__CRT_HAVE_DOS$fputws_unlocked || !__USE_STDIO_UNLOCKED || !__SIZEOF_WCHAR_T__ == 2) && (!__CRT_HAVE_DOS$_fputws_nolock || !__USE_STDIO_UNLOCKED || !__SIZEOF_WCHAR_T__ == 4) && (!__CRT_HAVE_DOS$_fputws_nolock || !__USE_STDIO_UNLOCKED || !__SIZEOF_WCHAR_T__ == 2) && !__CRT_HAVE_fputws && (!__CRT_HAVE_DOS$fputws || !__SIZEOF_WCHAR_T__ == 4) && (!__CRT_HAVE_DOS$fputws || !__SIZEOF_WCHAR_T__ == 2) && !__CRT_HAVE_fputws_unlocked && (!__CRT_HAVE_DOS$fputws_unlocked || !__SIZEOF_WCHAR_T__ == 4) && (!__CRT_HAVE_DOS$fputws_unlocked || !__SIZEOF_WCHAR_T__ == 2) && !__CRT_HAVE__fputws_nolock && (!__CRT_HAVE_DOS$_fputws_nolock || !__SIZEOF_WCHAR_T__ == 4) && (!__CRT_HAVE_DOS$_fputws_nolock || !__SIZEOF_WCHAR_T__ == 2) && (!__CRT_HAVE_DOS$file_wprinter_unlocked || !__USE_STDIO_UNLOCKED || !__SIZEOF_WCHAR_T__ == 4) && (!__CRT_HAVE_DOS$file_wprinter_unlocked || !__USE_STDIO_UNLOCKED || !__SIZEOF_WCHAR_T__ == 2) && !__CRT_HAVE_file_wprinter && (!__CRT_HAVE_DOS$file_wprinter || !__SIZEOF_WCHAR_T__ == 4) && (!__CRT_HAVE_DOS$file_wprinter || !__SIZEOF_WCHAR_T__ == 2) && !__CRT_HAVE_file_wprinter_unlocked && (!__CRT_HAVE_DOS$file_wprinter_unlocked || !__SIZEOF_WCHAR_T__ == 4) && (!__CRT_HAVE_DOS$file_wprinter_unlocked || !__SIZEOF_WCHAR_T__ == 2) && !__CRT_HAVE_fputwc && (!__CRT_HAVE_DOS$fputwc || !__SIZEOF_WCHAR_T__ == 4) && (!__CRT_HAVE_DOS$fputwc || !__SIZEOF_WCHAR_T__ == 2) && !__CRT_HAVE_putwc && (!__CRT_HAVE_DOS$putwc || !__SIZEOF_WCHAR_T__ == 4) && (!__CRT_HAVE_DOS$putwc || !__SIZEOF_WCHAR_T__ == 2)) || __NO_STDSTREAMS */
#endif /* !__local__putws_defined */
