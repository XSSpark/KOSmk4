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
#ifndef _I386_KOS_BITS_UCONTEXT_CYGWIN64_H
#define _I386_KOS_BITS_UCONTEXT_CYGWIN64_H 1

/* File:
 *    <i386-kos/bits/ucontext-linux64.h>
 * 
 * Definitions:
 *    - struct __cygwin64_ucontext { ... };
 * #if defined(__CRT_CYG) && defined(__x86_64__)
 *    - struct ucontext { ... };
 * #endif
 */

#include <__stdinc.h>

#include <hybrid/__pointer.h> /* __HYBRID_PTR64 */
#include <hybrid/host.h>      /* __x86_64__, __i386__ */
#include <hybrid/typecore.h>  /* __ULONG64_TYPE__ */

#include <bits/mcontext-cygwin64.h> /* struct __cygwin64_mcontext */
#include <bits/sigset.h>            /* struct __sigset_struct */
#include <bits/sigstack64.h>        /* struct __sigaltstackx64 */

#if defined(__CRT_CYG) && defined(__x86_64__)
#define __cygwin64_ucontext ucontext
#endif /* __CRT_CYG && __x86_64__ */

#ifdef __CC__
__DECL_BEGIN

struct __cygwin64_ucontext /*[NAME(cygwin64_ucontext)][PREFIX(uc_)]*/ {
	/* Userlevel context. */
	struct __cygwin64_mcontext                 uc_mcontext;
	__HYBRID_PTR64(struct __cygwin64_ucontext) uc_link;
	struct __sigset_struct                     uc_sigmask;
	struct __sigaltstackx64                    uc_stack;
	__ULONG64_TYPE__                           uc_flags;
};

__DECL_END
#endif /* __CC__ */


#endif /* !_I386_KOS_BITS_UCONTEXT_CYGWIN64_H */
