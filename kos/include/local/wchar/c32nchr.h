/* HASH CRC-32:0xb6f7dec3 */
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
#ifndef __local_c32nchr_defined
#define __local_c32nchr_defined 1
__NAMESPACE_LOCAL_BEGIN
/* Same as `c32chr', but don't exceed `MAX_CHARS' characters. */
__LOCAL_LIBC(c32nchr) __ATTR_PURE __ATTR_WUNUSED __ATTR_NONNULL((1)) __CHAR32_TYPE__ *
__NOTHROW_NCX(__LIBCCALL __LIBC_LOCAL_NAME(c32nchr))(__CHAR32_TYPE__ const *__restrict __haystack,
                                                     __CHAR32_TYPE__ __needle,
                                                     __SIZE_TYPE__ __maxlen) {
#line 2514 "kos/src/libc/magic/string.c"
	for (; __maxlen-- && *__haystack; ++__haystack) {
		if __unlikely((__CHAR32_TYPE__)*__haystack == (__CHAR32_TYPE__)__needle)
			return (__CHAR32_TYPE__ *)__haystack;
	}
	return __NULLPTR;
}
__NAMESPACE_LOCAL_END
#endif /* !__local_c32nchr_defined */
