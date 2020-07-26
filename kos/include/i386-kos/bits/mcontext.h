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
#ifndef _I386_KOS_BITS_MCONTEXT_H
#define _I386_KOS_BITS_MCONTEXT_H 1

/* File:
 *    <i386-kos/bits/mcontext.h>
 * 
 * Definitions:
 *    - struct mcontext { ... };
 */

#include <__stdinc.h>
#include <hybrid/host.h>

#if (defined(__KOS__) && (defined(__x86_64__) || __KOS_VERSION__ >= 300))
#ifdef __x86_64__
#include "mcontext64.h"
#else /* __x86_64__ */
#include "mcontext32.h"
#endif /* !__x86_64__ */
#elif defined(__linux__) || (!defined(__x86_64__) && defined(__KOS__) && __KOS_VERSION__ < 300)
#ifdef __x86_64__
#include "mcontext-linux64.h"
#else /* __x86_64__ */
#include "mcontext-linux32.h"
#endif /* !__x86_64__ */
#else /* ... */
#include <__crt.h>
#ifdef __CRT_CYG
#ifdef __x86_64__
#include "mcontext-cygwin64.h"
#else /* __x86_64__ */
#include "mcontext-cygwin32.h"
#endif /* !__x86_64__ */
#endif /* __CRT_CYG */
#endif /* !... */

#endif /* !_I386_KOS_BITS_MCONTEXT_H */
