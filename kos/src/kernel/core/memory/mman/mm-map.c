/* Copyright (c) 2019-2021 Griefer@Work                                       *
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
 *    Portions Copyright (c) 2019-2021 Griefer@Work                           *
 * 2. Altered source versions must be plainly marked as such, and must not be *
 *    misrepresented as being the original software.                          *
 * 3. This notice may not be removed or altered from any source distribution. *
 */
#ifndef GUARD_KERNEL_SRC_MEMORY_MMAN_MM_MAP_C
#define GUARD_KERNEL_SRC_MEMORY_MMAN_MM_MAP_C 1
#define __WANT_MNODE__mn_alloc
#define _KOS_SOURCE 1

#include <kernel/compiler.h>

#include <kernel/mman.h>
#include <kernel/mman/mfile.h>
#include <kernel/mman/mm-map.h>
#include <kernel/mman/mnode.h>
#include <kernel/mman/mpart.h>

#include <kos/except.h>

#include <assert.h>
#include <stdbool.h>
#include <string.h>

DECL_BEGIN

#ifndef NDEBUG
#define DBG_memset(dst, byte, num_bytes) memset(dst, byte, num_bytes)
#else /* !NDEBUG */
#define DBG_memset(dst, byte, num_bytes) (void)0
#endif /* NDEBUG */

/* Sync all changes made to file mappings within the given
 * address range with on-disk file images. (s.a. `mfile_sync()')
 * NOTE: Memory ranges that aren't actually mapped, aren't mapped
 *       with WRITE and SHARED, or aren't mapped to write-back files
 *       are simply ignored. */
PUBLIC void FCALL
mman_syncmem(struct mman *__restrict self,
             UNCHECKED void *addr, size_t num_bytes)
		THROWS(E_WOULDBLOCK, ...) {
	void *maxaddr = (byte_t *)addr + num_bytes - 1;
	while (addr <= maxaddr) {
		REF struct mpart *part;
		struct mnode_tree_minmax mima;
		mman_lock_read(self);
		mnode_tree_minmaxlocate(self->mm_mappings, addr, maxaddr, &mima);
		if (!mima.mm_min) {
			mman_lock_endread(self);
			break;
		}
		addr = mnode_getendaddr(mima.mm_min);
		part = xincref(mima.mm_min->mn_part);
		mman_lock_endread(self);
		if (part) {
			FINALLY_DECREF_UNLIKELY(part);
			/* Sync the backing data-part. */
			mpart_sync(part);
		}
	}
}


DECL_END

#ifndef __INTELLISENSE__
#define DEFINE_mman_map
#include "mm-map.c.inl"
#define DEFINE_mman_map_subrange
#include "mm-map.c.inl"
#define DEFINE_mman_map_res
#include "mm-map.c.inl"
#endif /* !__INTELLISENSE__ */


#endif /* !GUARD_KERNEL_SRC_MEMORY_MMAN_MM_MAP_C */
