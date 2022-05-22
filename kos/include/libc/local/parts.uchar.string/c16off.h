/* HASH CRC-32:0x3cc09343 */
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
#ifndef __local_c16off_defined
#define __local_c16off_defined
#include <__crt.h>
#include <hybrid/typecore.h>
__NAMESPACE_LOCAL_BEGIN
#ifndef __local___localdep_c16chrnul_defined
#define __local___localdep_c16chrnul_defined
#if defined(__CRT_HAVE_wcschrnul) && __SIZEOF_WCHAR_T__ == 2 && defined(__LIBCCALL_IS_LIBDCALL)
__CREDIRECT(__ATTR_PURE __ATTR_WUNUSED __ATTR_ACCESS_RO(1),__CHAR16_TYPE__ *,__NOTHROW_NCX,__localdep_c16chrnul,(__CHAR16_TYPE__ const *__haystack, __CHAR16_TYPE__ __needle),wcschrnul,(__haystack,__needle))
#elif defined(__CRT_HAVE_DOS$wcschrnul)
__CREDIRECT_DOS(__ATTR_PURE __ATTR_WUNUSED __ATTR_ACCESS_RO(1),__CHAR16_TYPE__ *,__NOTHROW_NCX,__localdep_c16chrnul,(__CHAR16_TYPE__ const *__haystack, __CHAR16_TYPE__ __needle),wcschrnul,(__haystack,__needle))
#elif __SIZEOF_WCHAR_T__ == 2
__NAMESPACE_LOCAL_END
#include <libc/local/wchar/wcschrnul.h>
__NAMESPACE_LOCAL_BEGIN
#define __localdep_c16chrnul __NAMESPACE_LOCAL_TYPEHAX(__CHAR16_TYPE__ *(__LIBDCALL*)(__CHAR16_TYPE__ const *,__CHAR16_TYPE__),__CHAR16_TYPE__ *(__LIBDCALL&)(__CHAR16_TYPE__ const *,__CHAR16_TYPE__),wcschrnul)
#else /* ... */
__NAMESPACE_LOCAL_END
#include <libc/local/parts.uchar.string/c16chrnul.h>
__NAMESPACE_LOCAL_BEGIN
#define __localdep_c16chrnul __LIBC_LOCAL_NAME(c16chrnul)
#endif /* !... */
#endif /* !__local___localdep_c16chrnul_defined */
__LOCAL_LIBC(c16off) __ATTR_PURE __ATTR_WUNUSED __ATTR_ACCESS_RO(1) __SIZE_TYPE__
__NOTHROW_NCX(__LIBDCALL __LIBC_LOCAL_NAME(c16off))(__CHAR16_TYPE__ const *__restrict __haystack, __CHAR16_TYPE__ __needle) {
	return (__SIZE_TYPE__)((__NAMESPACE_LOCAL_SYM __localdep_c16chrnul)(__haystack, __needle) - __haystack);
}
__NAMESPACE_LOCAL_END
#ifndef __local___localdep_c16off_defined
#define __local___localdep_c16off_defined
#define __localdep_c16off __LIBC_LOCAL_NAME(c16off)
#endif /* !__local___localdep_c16off_defined */
#endif /* !__local_c16off_defined */
