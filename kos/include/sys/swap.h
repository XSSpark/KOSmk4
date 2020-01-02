/* HASH CRC-32:0xfc3ee3bb */
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
#ifndef _SYS_SWAP_H
#define _SYS_SWAP_H 1

#include <__stdinc.h>
#include <__crt.h>

#ifdef __COMPILER_HAVE_PRAGMA_GCC_SYSTEM_HEADER
#pragma GCC system_header
#endif /* __COMPILER_HAVE_PRAGMA_GCC_SYSTEM_HEADER */


__SYSDECL_BEGIN

/* The swap priority is encoded as:
 *  - (prio << SWAP_FLAG_PRIO_SHIFT) & SWAP_FLAG_PRIO_MASK */
#define SWAP_FLAG_PREFER     0x08000 /* Set if swap priority is specified. */
#define SWAP_FLAG_PRIO_MASK  0x07fff
#define SWAP_FLAG_PRIO_SHIFT       0
#define SWAP_FLAG_DISCARD    0x10000 /* Discard swap cluster after use. */

#ifdef __CC__

#ifdef __CRT_HAVE_swapon
/* @param swapflags: Set of `SWAP_FLAG_*' */
__CDECLARE(__ATTR_NONNULL((1)),int,__NOTHROW_RPC,swapon,(char const *__path, int __swapflags),(__path,__swapflags))
#endif /* swapon... */
#ifdef __CRT_HAVE_swapoff
__CDECLARE(__ATTR_NONNULL((1)),int,__NOTHROW_RPC,swapoff,(char const *__path),(__path))
#endif /* swapoff... */

#endif /* __CC__ */

__SYSDECL_END

#endif /* !_SYS_SWAP_H */
