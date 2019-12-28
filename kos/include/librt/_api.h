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
#ifndef _LIBRT__API_H
#define _LIBRT__API_H 1

#include <__crt.h>
#include <__stdinc.h>

#define __LIBRT_CC __LIBCCALL
#ifdef __LIBRT_STATIC
#define __LIBRT_DECL __INTDEF
#else /* __LIBRT_STATIC */
#define __LIBRT_DECL __IMPDEF
#endif /* !__LIBRT_STATIC */

/* Library name for use with `dlopen()' */
#define __LIBRT_LIBRARY_NAME "librt.so"

#endif /* !_LIBRT__API_H */
