/* HASH CRC-32:0x256b0d11 */
/* Copyright (c) 2019-2022 Griefer@Work                                       *
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
 *    Portions Copyright (c) 2019-2022 Griefer@Work                           *
 * 2. Altered source versions must be plainly marked as such, and must not be *
 *    misrepresented as being the original software.                          *
 * 3. This notice may not be removed or altered from any source distribution. *
 */
/* (#) Portability: EMX kLIBC     (/libc/include/sgtty.h) */
/* (#) Portability: GNU C Library (/misc/sgtty.h) */
/* (#) Portability: NetBSD        (/include/sgtty.h) */
/* (#) Portability: OpenSolaris   (/usr/src/head/sgtty.h) */
/* (#) Portability: libc6         (/include/sgtty.h) */
/* (#) Portability: mintlib       (/include/sgtty.h) */
/* (#) Portability: uClibc        (/include/sgtty.h) */
#ifndef _SGTTY_H
#define _SGTTY_H 1

#include "__stdinc.h"
#include "__crt.h"

#ifdef __COMPILER_HAVE_PRAGMA_GCC_SYSTEM_HEADER
#pragma GCC system_header
#endif /* __COMPILER_HAVE_PRAGMA_GCC_SYSTEM_HEADER */

#include <features.h>
#include <bits/types.h>

#ifdef __USE_GLIBC
#include <sys/ioctl.h>
#endif /* __USE_GLIBC */

#ifdef __CC__
__SYSDECL_BEGIN

struct sgttyb;

__CDECLARE_OPT(__ATTR_ACCESS_WR(2),int,__NOTHROW_NCX,gtty,(__fd_t __fd, struct sgttyb *__params),(__fd,__params))
__CDECLARE_OPT(__ATTR_ACCESS_RO(2),int,__NOTHROW_NCX,stty,(__fd_t __fd, struct sgttyb const *__params),(__fd,__params))

__SYSDECL_END
#endif /* __CC__ */

#endif /* !_SGTTY_H */
