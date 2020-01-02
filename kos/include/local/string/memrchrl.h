/* HASH CRC-32:0xe0d8b639 */
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
 *    in a product, an acknowledgement in the product documentation would be  *
 *    appreciated but is not required.                                        *
 * 2. Altered source versions must be plainly marked as such, and must not be *
 *    misrepresented as being the original software.                          *
 * 3. This notice may not be removed or altered from any source distribution. *
 */
#ifndef __local_memrchrl_defined
#define __local_memrchrl_defined 1
__NAMESPACE_LOCAL_BEGIN
/* Descendingly search for `NEEDLE', starting at `HAYSTACK+N_DWORDS'. - Return `NULL' if `NEEDLE' wasn't found. */
__LOCAL_LIBC(memrchrl) __ATTR_PURE __ATTR_WUNUSED __ATTR_NONNULL((1)) __UINT32_TYPE__ *
__NOTHROW_NCX(__LIBCCALL __LIBC_LOCAL_NAME(memrchrl))(/*aligned(4)*/ void const *__restrict __haystack,
                                                      __UINT32_TYPE__ __dword,
                                                      __SIZE_TYPE__ __n_dwords) {
#line 1347 "kos/src/libc/magic/string.c"
	__UINT32_TYPE__ *__iter = (__UINT32_TYPE__ *)__haystack + __n_dwords;
	while (__n_dwords--) {
		if __unlikely(*--__iter == __dword)
			return __iter;
	}
	return __NULLPTR;
}
__NAMESPACE_LOCAL_END
#endif /* !__local_memrchrl_defined */
