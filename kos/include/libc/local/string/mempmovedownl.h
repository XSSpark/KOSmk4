/* HASH CRC-32:0x1ef14239 */
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
#ifndef __local_mempmovedownl_defined
#define __local_mempmovedownl_defined
#include <__crt.h>
#include <hybrid/typecore.h>
__NAMESPACE_LOCAL_BEGIN
#ifndef __local___localdep_memmovedownl_defined
#define __local___localdep_memmovedownl_defined
#ifdef __CRT_HAVE_memmovedownl
__CREDIRECT(__ATTR_LEAF __ATTR_RETNONNULL __ATTR_NONNULL((1, 2)),__UINT32_TYPE__ *,__NOTHROW_NCX,__localdep_memmovedownl,(void *__dst, void const *__src, __SIZE_TYPE__ __n_dwords),memmovedownl,(__dst,__src,__n_dwords))
#elif defined(__CRT_HAVE_memmovel)
__CREDIRECT(__ATTR_LEAF __ATTR_RETNONNULL __ATTR_NONNULL((1, 2)),__UINT32_TYPE__ *,__NOTHROW_NCX,__localdep_memmovedownl,(void *__dst, void const *__src, __SIZE_TYPE__ __n_dwords),memmovel,(__dst,__src,__n_dwords))
#elif defined(__CRT_HAVE_wmemmove) && __SIZEOF_WCHAR_T__ == 4
__CREDIRECT(__ATTR_LEAF __ATTR_RETNONNULL __ATTR_NONNULL((1, 2)),__UINT32_TYPE__ *,__NOTHROW_NCX,__localdep_memmovedownl,(void *__dst, void const *__src, __SIZE_TYPE__ __n_dwords),wmemmove,(__dst,__src,__n_dwords))
#elif defined(__CRT_HAVE_KOS$wmemmove) && defined(__LIBCCALL_IS_LIBKCALL)
__COMPILER_REDIRECT(__LIBC,__ATTR_LEAF __ATTR_RETNONNULL __ATTR_NONNULL((1, 2)),__UINT32_TYPE__ *,__NOTHROW_NCX,__LIBCCALL,__localdep_memmovedownl,(void *__dst, void const *__src, __SIZE_TYPE__ __n_dwords),KOS$wmemmove,(__dst,__src,__n_dwords))
#else /* ... */
__NAMESPACE_LOCAL_END
#include <libc/local/string/memmovedownl.h>
__NAMESPACE_LOCAL_BEGIN
#define __localdep_memmovedownl __LIBC_LOCAL_NAME(memmovedownl)
#endif /* !... */
#endif /* !__local___localdep_memmovedownl_defined */
__LOCAL_LIBC(mempmovedownl) __ATTR_LEAF __ATTR_RETNONNULL __ATTR_NONNULL((1, 2)) __UINT32_TYPE__ *
__NOTHROW_NCX(__LIBCCALL __LIBC_LOCAL_NAME(mempmovedownl))(void *__dst, void const *__src, __SIZE_TYPE__ __n_dwords) {
	return (__UINT32_TYPE__ *)(__NAMESPACE_LOCAL_SYM __localdep_memmovedownl)(__dst, __src, __n_dwords) + __n_dwords;
}
__NAMESPACE_LOCAL_END
#ifndef __local___localdep_mempmovedownl_defined
#define __local___localdep_mempmovedownl_defined
#define __localdep_mempmovedownl __LIBC_LOCAL_NAME(mempmovedownl)
#endif /* !__local___localdep_mempmovedownl_defined */
#endif /* !__local_mempmovedownl_defined */
