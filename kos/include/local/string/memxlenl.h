/* HASH CRC-32:0xb9d2e7f9 */
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
#ifndef __local_memxlenl_defined
#define __local_memxlenl_defined 1
/* Dependency: "memxendl" from "string" */
#ifndef ____localdep_memxendl_defined
#define ____localdep_memxendl_defined 1
#ifdef __CRT_HAVE_memxendl
/* Same as `memendl', but search for non-matching locations. */
__CREDIRECT(__ATTR_PURE __ATTR_RETNONNULL __ATTR_WUNUSED __ATTR_NONNULL((1)),__UINT32_TYPE__ *,__NOTHROW_NCX,__localdep_memxendl,(void const *__restrict __haystack, __UINT32_TYPE__ __dword, __SIZE_TYPE__ __n_bytes),memxendl,(__haystack,__dword,__n_bytes))
#else /* LIBC: memxendl */
#include <local/string/memxendl.h>
/* Same as `memendl', but search for non-matching locations. */
#define __localdep_memxendl (__NAMESPACE_LOCAL_SYM __LIBC_LOCAL_NAME(memxendl))
#endif /* memxendl... */
#endif /* !____localdep_memxendl_defined */

__NAMESPACE_LOCAL_BEGIN
/* Same as `memlenl', but search for non-matching locations. */
__LOCAL_LIBC(memxlenl) __ATTR_PURE __ATTR_WUNUSED __ATTR_NONNULL((1)) __SIZE_TYPE__
__NOTHROW_NCX(__LIBCCALL __LIBC_LOCAL_NAME(memxlenl))(void const *__restrict __haystack,
                                                      __UINT32_TYPE__ __dword,
                                                      __SIZE_TYPE__ __n_dwords) {
#line 1974 "kos/src/libc/magic/string.c"
	return (__SIZE_TYPE__)(__localdep_memxendl(__haystack, __dword, __n_dwords) - (__UINT32_TYPE__ *)__haystack);
}
__NAMESPACE_LOCAL_END
#endif /* !__local_memxlenl_defined */
