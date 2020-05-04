/* HASH CRC-32:0xf781a6b7 */
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
 *    in a product, an acknowledgement (see the following) in the product     *
 *    documentation is required:                                              *
 *    Portions Copyright (c) 2019-2020 Griefer@Work                           *
 * 2. Altered source versions must be plainly marked as such, and must not be *
 *    misrepresented as being the original software.                          *
 * 3. This notice may not be removed or altered from any source distribution. *
 */
#ifndef __local_unicode_writeutf8_defined
#define __local_unicode_writeutf8_defined 1
#include <__crt.h>
__NAMESPACE_LOCAL_BEGIN
/* Write a given Unicode character `ch' to `dst' and return a pointer to its end location.
 * This function will write at most `UNICODE_UTF8_CURLEN' bytes to `dst' */
__LOCAL_LIBC(unicode_writeutf8) __ATTR_RETNONNULL __ATTR_NONNULL((1)) char *
__NOTHROW_NCX(__LIBCCALL __LIBC_LOCAL_NAME(unicode_writeutf8))(/*utf-8*/ char *__restrict __dst,
                                                               __CHAR32_TYPE__ __ch) {
#line 689 "kos/src/libc/magic/unicode.c"
	if __likely(__ch <= ((__UINT32_TYPE__)1 << 7)-1) {
		*__dst++ = (char)(__UINT8_TYPE__)__ch;
	} else if (__ch <= ((__UINT32_TYPE__)1 << 11)-1) {
		*__dst++ = (char)(0xc0 | (__UINT8_TYPE__)((__ch >> 6)/* & 0x1f*/));
		*__dst++ = (char)(0x80 | (__UINT8_TYPE__)((__ch) & 0x3f));
	} else if (__ch <= ((__UINT32_TYPE__)1 << 16)-1) {
		*__dst++ = (char)(0xe0 | (__UINT8_TYPE__)((__ch >> 12)/* & 0x0f*/));
		*__dst++ = (char)(0x80 | (__UINT8_TYPE__)((__ch >> 6) & 0x3f));
		*__dst++ = (char)(0x80 | (__UINT8_TYPE__)((__ch) & 0x3f));
	} else if (__ch <= ((__UINT32_TYPE__)1 << 21)-1) {
		*__dst++ = (char)(0xf0 | (__UINT8_TYPE__)((__ch >> 18)/* & 0x07*/));
		*__dst++ = (char)(0x80 | (__UINT8_TYPE__)((__ch >> 12) & 0x3f));
		*__dst++ = (char)(0x80 | (__UINT8_TYPE__)((__ch >> 6) & 0x3f));
		*__dst++ = (char)(0x80 | (__UINT8_TYPE__)((__ch) & 0x3f));
	} else if (__ch <= ((__UINT32_TYPE__)1 << 26)-1) {
		*__dst++ = (char)(0xf8 | (__UINT8_TYPE__)((__ch >> 24)/* & 0x03*/));
		*__dst++ = (char)(0x80 | (__UINT8_TYPE__)((__ch >> 18) & 0x3f));
		*__dst++ = (char)(0x80 | (__UINT8_TYPE__)((__ch >> 12) & 0x3f));
		*__dst++ = (char)(0x80 | (__UINT8_TYPE__)((__ch >> 6) & 0x3f));
		*__dst++ = (char)(0x80 | (__UINT8_TYPE__)((__ch) & 0x3f));
	} else if (__ch <= ((__UINT32_TYPE__)1 << 31)-1) {
		*__dst++ = (char)(0xfc | (__UINT8_TYPE__)((__ch >> 30)/* & 0x01*/));
		*__dst++ = (char)(0x80 | (__UINT8_TYPE__)((__ch >> 24) & 0x3f));
		*__dst++ = (char)(0x80 | (__UINT8_TYPE__)((__ch >> 18) & 0x3f));
		*__dst++ = (char)(0x80 | (__UINT8_TYPE__)((__ch >> 12) & 0x3f));
		*__dst++ = (char)(0x80 | (__UINT8_TYPE__)((__ch >> 6) & 0x3f));
		*__dst++ = (char)(0x80 | (__UINT8_TYPE__)((__ch) & 0x3f));
	} else {
		*__dst++ = (char)(0xfe);
		*__dst++ = (char)(0x80 | (__UINT8_TYPE__)((__ch >> 30) & 0x03/* & 0x3f*/));
		*__dst++ = (char)(0x80 | (__UINT8_TYPE__)((__ch >> 24) & 0x3f));
		*__dst++ = (char)(0x80 | (__UINT8_TYPE__)((__ch >> 18) & 0x3f));
		*__dst++ = (char)(0x80 | (__UINT8_TYPE__)((__ch >> 12) & 0x3f));
		*__dst++ = (char)(0x80 | (__UINT8_TYPE__)((__ch >> 6) & 0x3f));
		*__dst++ = (char)(0x80 | (__UINT8_TYPE__)((__ch) & 0x3f));
	}
	return __dst;
}
__NAMESPACE_LOCAL_END
#endif /* !__local_unicode_writeutf8_defined */
