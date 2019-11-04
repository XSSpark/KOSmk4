/* HASH CRC-32:0x8a314de9 */
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
#ifndef __local_wprintf_unlocked_defined
#if ((defined(__CRT_HAVE_fputwc_unlocked) || defined(__CRT_HAVE__fputwc_nolock) || defined(__CRT_HAVE_file_wprinter_unlocked) || defined(__CRT_HAVE_file_wprinter) || defined(__CRT_HAVE_vfwprintf_unlocked)) && !defined(__NO_STDSTREAMS)) || defined(__CRT_HAVE_vwprintf_unlocked)
#define __local_wprintf_unlocked_defined 1
#include <kos/anno.h>
/* Dependency: "vwprintf_unlocked" from "wchar" */
#ifndef ____localdep_vwprintf_unlocked_defined
#define ____localdep_vwprintf_unlocked_defined 1
#if defined(__CRT_HAVE_vwprintf_unlocked)
__CREDIRECT(__ATTR_NONNULL((1)) __ATTR_LIBC_WPRINTF(1, 0),__STDC_INT_AS_SIZE_T,,__localdep_vwprintf_unlocked,(__WCHAR_TYPE__ const *__restrict __format, __builtin_va_list __args),vwprintf_unlocked,(__format,__args)) __THROWS(...)
#elif (defined(__CRT_HAVE_fputwc_unlocked) || defined(__CRT_HAVE__fputwc_nolock) || defined(__CRT_HAVE_file_wprinter_unlocked) || defined(__CRT_HAVE_file_wprinter) || defined(__CRT_HAVE_vfwprintf_unlocked)) && !defined(__NO_STDSTREAMS)
#include <local/wchar/vwprintf_unlocked.h>
#define __localdep_vwprintf_unlocked (__NAMESPACE_LOCAL_SYM __LIBC_LOCAL_NAME(vwprintf_unlocked))
#else /* CUSTOM: vwprintf_unlocked */
#undef ____localdep_vwprintf_unlocked_defined
#endif /* vwprintf_unlocked... */
#endif /* !____localdep_vwprintf_unlocked_defined */

__NAMESPACE_LOCAL_BEGIN
__LOCAL_LIBC(wprintf_unlocked) __ATTR_NONNULL((1)) __ATTR_LIBC_WPRINTF(1, 2) __STDC_INT_AS_SIZE_T
(__VLIBCCALL __LIBC_LOCAL_NAME(wprintf_unlocked))(__WCHAR_TYPE__ const *__restrict __format,
                                                  ...) __THROWS(...) {
#line 1090 "kos/src/libc/magic/wchar.c"
	__STDC_INT_AS_SIZE_T __result;
	__builtin_va_list __args;
	__builtin_va_start(__args, __format);
	__result = __localdep_vwprintf_unlocked(__format, __args);
	__builtin_va_end(__args);
	return __result;
}
__NAMESPACE_LOCAL_END
#endif /* ((defined(__CRT_HAVE_fputwc_unlocked) || defined(__CRT_HAVE__fputwc_nolock) || defined(__CRT_HAVE_file_wprinter_unlocked) || defined(__CRT_HAVE_file_wprinter) || defined(__CRT_HAVE_vfwprintf_unlocked)) && !defined(__NO_STDSTREAMS)) || defined(__CRT_HAVE_vwprintf_unlocked) */
#endif /* !__local_wprintf_unlocked_defined */
