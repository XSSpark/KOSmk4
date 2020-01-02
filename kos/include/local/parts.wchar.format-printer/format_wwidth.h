/* HASH CRC-32:0xa5736fb2 */
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
#ifndef __local_format_wwidth_defined
#define __local_format_wwidth_defined 1
__NAMESPACE_LOCAL_BEGIN
/* Returns the width (number of characters; not bytes) of the given unicode string */
__LOCAL_LIBC(format_wwidth) __ATTR_PURE __ATTR_NONNULL((2)) __SSIZE_TYPE__
__NOTHROW_NCX(__LIBCCALL __LIBC_LOCAL_NAME(format_wwidth))(void *__arg,
                                                           __WCHAR_TYPE__ const *__restrict __data,
                                                           __SIZE_TYPE__ __datalen) {
#line 124 "kos/src/libc/magic/parts.wchar.format-printer.c"
#if __SIZEOF_WCHAR_T__ == 2
	__SIZE_TYPE__ __result = 0;
	__WCHAR_TYPE__ const *__iter, *__end;
	(void)__arg;
	__end = (__iter = __data) + __datalen;
	while (__iter < __end) {
		__WCHAR_TYPE__ __ch;
		__ch = *__iter++;
		if (__ch >= 0xd800 && __ch <= 0xd800 && __iter < __end) {
			__ch = *__iter;
			if (__ch >= 0xdc00 && __ch <= 0xdfff)
				++__iter;
		}
		++__result;
	}
	return (__SSIZE_TYPE__)__result;
#else
	(void)__arg;
	(void)__data;
	/* XXX: Not necessarily correct, as the 32-bit variant is actually ATTR_CONST.
	 *      However, magic headers don't support conditional attributes, so we can't just do
	 *      [if(__SIZEOF_WCHAR_T__ == 2), ATTR_PURE]
	 *      [if(__SIZEOF_WCHAR_T__ != 2), ATTR_CONST] */
	__COMPILER_IMPURE();
	return (__SSIZE_TYPE__)__datalen;
#endif
}
__NAMESPACE_LOCAL_END
#endif /* !__local_format_wwidth_defined */
