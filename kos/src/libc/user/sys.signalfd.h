/* HASH CRC-32:0x6e38a0ad */
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
#ifndef GUARD_LIBC_USER_SYS_SIGNALFD_H
#define GUARD_LIBC_USER_SYS_SIGNALFD_H 1

#include "../api.h"
#include <hybrid/typecore.h>
#include <kos/types.h>
#include <sys/signalfd.h>

DECL_BEGIN

/* Request notification for delivery of signals in MASK to be performed using descriptor FD */
INTDEF NONNULL((2)) fd_t NOTHROW_NCX(LIBCCALL libc_signalfd)(fd_t fd, sigset_t const *mask, int flags);

DECL_END

#endif /* !GUARD_LIBC_USER_SYS_SIGNALFD_H */
