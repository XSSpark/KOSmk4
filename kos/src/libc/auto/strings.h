/* HASH CRC-32:0xd5440679 */
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
#ifndef GUARD_LIBC_AUTO_STRINGS_H
#define GUARD_LIBC_AUTO_STRINGS_H 1

#include "../api.h"
#include <hybrid/typecore.h>
#include <kos/types.h>
#include <strings.h>

DECL_BEGIN

#ifndef __KERNEL__
INTDEF NONNULL((1, 2)) void NOTHROW_NCX(LIBCCALL libc_bcopy)(void const *src, void *dst, size_t num_bytes);
INTDEF NONNULL((1)) void NOTHROW_NCX(LIBCCALL libc_bzero)(void *__restrict dst, size_t num_bytes);
/* Compare memory buffers and return the difference of the first non-matching byte
 * @return:  < 0: `s1...+=n_bytes'  < `s2...+=n_bytes'
 * @return: == 0: `s1...+=n_bytes' == `s2...+=n_bytes'
 * @return:  > 0: `s1...+=n_bytes'  > `s2...+=n_bytes' */
#define libc_bcmp libc_memcmp
INTDEF ATTR_PURE WUNUSED NONNULL((1)) char *NOTHROW_NCX(LIBCCALL libc_index)(char const *__restrict haystack, int needle);
INTDEF ATTR_PURE WUNUSED NONNULL((1)) char *NOTHROW_NCX(LIBCCALL libc_rindex)(char const *__restrict haystack, int needle);
#endif /* !__KERNEL__ */

DECL_END

#endif /* !GUARD_LIBC_AUTO_STRINGS_H */
