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
#ifndef __GUARD_HYBRID_DEBUG_ALIGNMENT_H
#define __GUARD_HYBRID_DEBUG_ALIGNMENT_H 1

#include <__stdinc.h>
#include "__debug-alignment.h"

#ifdef __NO_hybrid_dbg_alignment
#define NO_DBG_ALIGNMENT 1
#endif /* __NO_hybrid_dbg_alignment */
#define DBG_ALIGNMENT_ENABLE()  __hybrid_dbg_alignment_enable()
#define DBG_ALIGNMENT_DISABLE() __hybrid_dbg_alignment_disable()

#endif /* !__GUARD_HYBRID_DEBUG_ALIGNMENT_H */
