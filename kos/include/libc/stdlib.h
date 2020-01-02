/* HASH CRC-32:0xa4440f98 */
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
#ifndef _LIBC_STDLIB_H
#define _LIBC_STDLIB_H 1

#include <__stdinc.h>

#ifdef __CC__
#include <__crt.h>
#include "slow/stdlib.h"

#if !defined(____libc_malloc_defined) && defined(____libc_slow_malloc_defined)
#define ____libc_malloc_defined 1
#define __libc_malloc __libc_slow_malloc
#endif /* !____libc_malloc_defined && ____libc_slow_malloc_defined */
#if !defined(____libc_calloc_defined) && defined(____libc_slow_calloc_defined)
#define ____libc_calloc_defined 1
#define __libc_calloc __libc_slow_calloc
#endif /* !____libc_calloc_defined && ____libc_slow_calloc_defined */
#if !defined(____libc_realloc_defined) && defined(____libc_slow_realloc_defined)
#define ____libc_realloc_defined 1
#define __libc_realloc __libc_slow_realloc
#endif /* !____libc_realloc_defined && ____libc_slow_realloc_defined */
#if !defined(____libc_free_defined) && defined(____libc_slow_free_defined)
#define ____libc_free_defined 1
#define __libc_free __libc_slow_free
#endif /* !____libc_free_defined && ____libc_slow_free_defined */

#endif /* __CC__ */

#endif /* !_LIBC_STDLIB_H */
