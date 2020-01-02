/* HASH CRC-32:0x3d611b0b */
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
#ifndef __local_rawmemrxchr_defined
#define __local_rawmemrxchr_defined 1
__NAMESPACE_LOCAL_BEGIN
/* Same as `rawmemrchr', but search for non-matching locations. */
__LOCAL_LIBC(rawmemrxchr) __ATTR_PURE __ATTR_RETNONNULL __ATTR_WUNUSED __ATTR_NONNULL((1)) void *
__NOTHROW_NCX(__LIBCCALL __LIBC_LOCAL_NAME(rawmemrxchr))(void const *__restrict __haystack,
                                                         int __needle) {
#line 1913 "kos/src/libc/magic/string.c"
	__BYTE_TYPE__ *__iter = (__BYTE_TYPE__ *)__haystack;
	for (;;) {
		if __unlikely(*--__iter != (__BYTE_TYPE__)__needle)
			break;
	}
	return __iter;
}
__NAMESPACE_LOCAL_END
#endif /* !__local_rawmemrxchr_defined */
