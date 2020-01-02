/* HASH CRC-32:0xe70bae60 */
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
#ifndef __local_imaxabs_defined
#define __local_imaxabs_defined 1
__NAMESPACE_LOCAL_BEGIN
__LOCAL_LIBC(imaxabs) __ATTR_CONST __INTMAX_TYPE__
__NOTHROW(__LIBCCALL __LIBC_LOCAL_NAME(imaxabs))(__INTMAX_TYPE__ __x) {
#line 477 "kos/src/libc/magic/inttypes.c"
	return __x < 0 ? -__x : __x;
}
__NAMESPACE_LOCAL_END
#endif /* !__local_imaxabs_defined */
