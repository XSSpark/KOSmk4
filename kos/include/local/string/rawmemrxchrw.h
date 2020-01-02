/* HASH CRC-32:0x410b0606 */
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
#ifndef __local_rawmemrxchrw_defined
#define __local_rawmemrxchrw_defined 1
__NAMESPACE_LOCAL_BEGIN
/* Same as `rawmemrchrw', but search for non-matching locations. */
__LOCAL_LIBC(rawmemrxchrw) __ATTR_PURE __ATTR_RETNONNULL __ATTR_WUNUSED __ATTR_NONNULL((1)) __UINT16_TYPE__ *
__NOTHROW_NCX(__LIBCCALL __LIBC_LOCAL_NAME(rawmemrxchrw))(/*aligned(2)*/ void const *__restrict __haystack,
                                                          __UINT16_TYPE__ __word) {
#line 2096 "kos/src/libc/magic/string.c"
	__UINT16_TYPE__ *__iter = (__UINT16_TYPE__ *)__haystack;
	for (;;) {
		if __unlikely(*--__iter != __word)
			break;
	}
	return __iter;
}
__NAMESPACE_LOCAL_END
#endif /* !__local_rawmemrxchrw_defined */
