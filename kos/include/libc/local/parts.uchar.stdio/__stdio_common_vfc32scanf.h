/* HASH CRC-32:0x66340224 */
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
#ifndef __local___stdio_common_vfc32scanf_defined
#define __local___stdio_common_vfc32scanf_defined
#include <__crt.h>
#if (defined(__CRT_HAVE_vfwscanf) && __SIZEOF_WCHAR_T__ == 4 && defined(__LIBCCALL_IS_LIBKCALL)) || defined(__CRT_HAVE_KOS$vfwscanf)
#include <hybrid/typecore.h>
__NAMESPACE_LOCAL_BEGIN
#ifndef __local___localdep_vfc32scanf_defined
#define __local___localdep_vfc32scanf_defined
#if defined(__CRT_HAVE_vfwscanf) && __SIZEOF_WCHAR_T__ == 4 && defined(__LIBCCALL_IS_LIBKCALL)
__NAMESPACE_LOCAL_END
#include <features.h>
#include <kos/anno.h>
__NAMESPACE_LOCAL_BEGIN
__CREDIRECT(__ATTR_WUNUSED __ATTR_LIBC_WSCANF(2, 0) __ATTR_NONNULL((1, 2)),__STDC_INT_AS_SIZE_T,__THROWING,__localdep_vfc32scanf,(__FILE *__restrict __stream, __CHAR32_TYPE__ const *__restrict __format, __builtin_va_list __args),vfwscanf,(__stream,__format,__args))
#elif defined(__CRT_HAVE_KOS$vfwscanf)
__NAMESPACE_LOCAL_END
#include <features.h>
#include <kos/anno.h>
__NAMESPACE_LOCAL_BEGIN
__CREDIRECT_KOS(__ATTR_WUNUSED __ATTR_LIBC_WSCANF(2, 0) __ATTR_NONNULL((1, 2)),__STDC_INT_AS_SIZE_T,__THROWING,__localdep_vfc32scanf,(__FILE *__restrict __stream, __CHAR32_TYPE__ const *__restrict __format, __builtin_va_list __args),vfwscanf,(__stream,__format,__args))
#else /* ... */
#undef __local___localdep_vfc32scanf_defined
#endif /* !... */
#endif /* !__local___localdep_vfc32scanf_defined */
__LOCAL_LIBC(__stdio_common_vfc32scanf) __ATTR_NONNULL((2, 3)) __STDC_INT_AS_SSIZE_T
__NOTHROW_NCX(__LIBKCALL __LIBC_LOCAL_NAME(__stdio_common_vfc32scanf))(__UINT64_TYPE__ __options, __FILE *__stream, __CHAR32_TYPE__ const *__format, __locale_t __locale, __builtin_va_list __args) {
	(void)__options;
	(void)__locale;
	return (__NAMESPACE_LOCAL_SYM __localdep_vfc32scanf)(__stream, __format, __args);
}
__NAMESPACE_LOCAL_END
#ifndef __local___localdep___stdio_common_vfc32scanf_defined
#define __local___localdep___stdio_common_vfc32scanf_defined
#define __localdep___stdio_common_vfc32scanf __LIBC_LOCAL_NAME(__stdio_common_vfc32scanf)
#endif /* !__local___localdep___stdio_common_vfc32scanf_defined */
#else /* (__CRT_HAVE_vfwscanf && __SIZEOF_WCHAR_T__ == 4 && __LIBCCALL_IS_LIBKCALL) || __CRT_HAVE_KOS$vfwscanf */
#undef __local___stdio_common_vfc32scanf_defined
#endif /* (!__CRT_HAVE_vfwscanf || __SIZEOF_WCHAR_T__ != 4 || !__LIBCCALL_IS_LIBKCALL) && !__CRT_HAVE_KOS$vfwscanf */
#endif /* !__local___stdio_common_vfc32scanf_defined */