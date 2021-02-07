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
#ifndef GUARD_KERNEL_INCLUDE_KERNEL_VM_FUTEX_H
#define GUARD_KERNEL_INCLUDE_KERNEL_VM_FUTEX_H 1

#include <kernel/compiler.h>

#ifdef CONFIG_USE_NEW_VM
#include <kernel/mman/mpartmeta.h>

#define fc_tree     mpm_ftx
#define fc_dead     mpm_ftx_dead.slh_first
#define fc_rtm_vers mpm_rtm_vers
#define vmf_refcnt  mfu_refcnt
#define vmf_part    mfu_part
#define vmf_tree    mfu_mtaent
#define vmf_signal  mfu_signal
#define vmf_ndead   _mfu_dead.sle_next

#define vm_futex_alloc()                         ((struct mfutex *)kmalloc(sizeof(struct mfutex), GFP_NORMAL))
#define vm_futex_allocf(gfp_flags)               ((struct mfutex *)kmalloc(sizeof(struct mfutex), gfp_flags))
#define vm_futex_allocf_nx(gfp_flags)            ((struct mfutex *)kmalloc_nx(sizeof(struct mfutex), gfp_flags))
#define vm_futex_free(p)                         mfutex_free(p)
#define vm_futex_destroy(self)                   mfutex_destroy(self)
#define vm_futex_controller_alloc()              ((struct mpartmeta *)kmalloc(sizeof(struct mpartmeta), GFP_CALLOC))
#define vm_futex_controller_allocf(gfp_flags)    ((struct mpartmeta *)kmalloc(sizeof(struct mpartmeta), (gfp_flags) | GFP_CALLOC))
#define vm_futex_controller_allocf_nx(gfp_flags) ((struct mpartmeta *)kmalloc_nx(sizeof(struct mpartmeta), (gfp_flags) | GFP_CALLOC))
#define vm_futex_controller_free(p)              kfree(p)

/* Futex API */
#define VM_DATAPART_GETFUTEX_OUTOFRANGE                      MPART_FUTEX_OOB
#define vm_datablock_getfutex(self, offset)                  mfile_createfutex(self, offset)
#define vm_datablock_getfutex_existing(self, offset)         mfile_lookupfutex(self, offset)
#define vm_getfutex(effective_vm, addr)                      mman_createfutex(effective_vm, addr)
#define vm_getfutex_existing(effective_vm, addr)             mman_lookupfutex(effective_vm, addr)
#define vm_futex_broadcast(futex_address)                    mman_broadcastfutex(futex_address)

#else /* CONFIG_USE_NEW_VM */
#define mpm_ftx                                  fc_tree
#define mpm_rtm_vers                             fc_rtm_vers
#define mfu_refcnt                               vmf_refcnt
#define mfu_part                                 vmf_part
#define mfu_mtaent                               vmf_tree
#define mfu_signal                               vmf_signal
#define mfutex_free(p)                           vm_futex_free(p)
#define mfutex_destroy(self)                     vm_futex_destroy(self)
#define MPART_FUTEX_OOB                          VM_DATAPART_GETFUTEX_OUTOFRANGE
#define mfile_createfutex(self, offset)          vm_datablock_getfutex(self, offset)
#define mfile_lookupfutex(self, offset)          vm_datablock_getfutex_existing(self, offset)
#define mman_createfutex(effective_vm, addr)     vm_getfutex(effective_vm, addr)
#define mman_lookupfutex(effective_vm, addr)     vm_getfutex_existing(effective_vm, addr)
#define mman_broadcastfutex(futex_address)       vm_futex_broadcast(futex_address)

#include <kernel/arch/mman/rtm.h>
#include <kernel/types.h>
#include <kernel/vm.h>

#include <hybrid/sequence/atree.h>

#include <kos/aref.h>

#ifdef ARCH_HAVE_RTM
#include <hybrid/sync/atomic-rwlock.h>
#endif /* ARCH_HAVE_RTM */

DECL_BEGIN

struct vm_futex;
struct vm_datapart;
AWREF(vm_datapart_awref, vm_datapart);

struct vm_futex {
	WEAK refcnt_t        vmf_refcnt; /* Futex reference counter. */
	struct vm_datapart_awref
	                     vmf_part;   /* [0..1] The data part associated with this futex.
	                                * Note that this part may change at any time in order to deal
	                                * with the part being split. Additionally, if the part is destroyed,
	                                * then this pointer is cleared, meaning that a NULL-value indicate
	                                * that the futex is dangling and that `vmf_tree' has become invalid. */
	ATREE_NODE_SINGLE(struct vm_futex, uintptr_t)
	                     vmf_tree;   /* [lock(vmf_part->dp_lock)][valid_if(!wasdestroyed(vmf_tree))]
	                                * The tree of futex objects that exist within the data part.
	                                * The associated tree-root can be found under `vmf_part->dp_futex->fc_tree' */
	union {
		struct sig       vmf_signal; /* [valid_if(vmf_refcnt != 0)] The signal used to implement the futex. */
		struct vm_futex *vmf_ndead;  /* [valid_if(vmf_refcnt == 0)] Next dead futex pointer. */
	};
};

#define vm_futex_alloc()              ((struct vm_futex *)kmalloc(sizeof(struct vm_futex), GFP_NORMAL))
#define vm_futex_allocf(gfp_flags)    ((struct vm_futex *)kmalloc(sizeof(struct vm_futex), gfp_flags))
#define vm_futex_allocf_nx(gfp_flags) ((struct vm_futex *)kmalloc_nx(sizeof(struct vm_futex), gfp_flags))
#define vm_futex_free(p)              kfree(p)

/* Destroy the given futex due to its reference counter having reached zero. */
FUNDEF NOBLOCK NONNULL((1)) void
NOTHROW(KCALL vm_futex_destroy)(struct vm_futex *__restrict self);
DEFINE_REFCOUNT_FUNCTIONS(struct vm_futex, vmf_refcnt, vm_futex_destroy)


struct vm_futex_controller {
	ATREE_HEAD(WEAK struct vm_futex) fc_tree;  /* [lock(:dp_lock)] Futex tree.
	                                            * HINT: Dead futex objects are automatically removed
	                                            *       from the tree whenever `dp_lock' is acquired,
	                                            *       though more futex objects can die even while
	                                            *       a lock to `dp_lock' is being held. */
	uintptr_t                        fc_semi0; /* [lock(:dp_lock)] Futex tree SEMI0 value. */
	unsigned int                     fc_leve0; /* [lock(:dp_lock)] Futex tree LEVEL0 value. */
	WEAK struct vm_futex            *fc_dead;  /* [0..1][LINK(->vmf_ndead)] Chain of dead futex objects.
	                                            * This chain is serviced at the same time as `dp_stale' */
#ifdef ARCH_HAVE_RTM
	/* We keep the RTM version and lock fields in the futex controller, such that
	 * they don't take up space in the base vm_datapart structure, but only exist
	 * conditionally, and upon first access. */
	uintptr_t                        fc_rtm_vers; /* [lock(:dp_lock)]
	                                               * RTM version (incremented for every RTM-driven
	                                               * modifications made to memory). */
#endif /* ARCH_HAVE_RTM */
};

#define vm_futex_controller_alloc()              ((struct vm_futex_controller *)kmalloc(sizeof(struct vm_futex_controller), GFP_CALLOC))
#define vm_futex_controller_allocf(gfp_flags)    ((struct vm_futex_controller *)kmalloc(sizeof(struct vm_futex_controller), (gfp_flags) | GFP_CALLOC))
#define vm_futex_controller_allocf_nx(gfp_flags) ((struct vm_futex_controller *)kmalloc_nx(sizeof(struct vm_futex_controller), (gfp_flags) | GFP_CALLOC))
#define vm_futex_controller_free(p)              kfree(p)


/* Return a reference to the futex associated with `datapart_offset' bytes into the given data part.
 * If no such futex already exists, use this chance to allocate it, as well as a potentially
 * missing `vm_futex_controller' when `self->dp_futex' was `NULL' when this function was called.
 * @return: * : A reference to the futex associated with `datapart_offset'
 * @return: VM_DATAPART_GETFUTEX_OUTOFRANGE:
 *              The given `datapart_offset' is greater than `vm_datapart_numbytes(self)', which
 *              may be the case even if you checked before that it wasn't (or simply
 *              used `vm_paged_datablock_locatepart()' in order to lookup the associated part),
 *              because there always exists the possibility that any data part gets split
 *              into multiple smaller parts. */
FUNDEF ATTR_RETNONNULL WUNUSED NONNULL((1)) REF struct vm_futex *
(KCALL vm_datapart_getfutex)(struct vm_datapart *__restrict self,
                             uintptr_t datapart_offset)
		THROWS(E_BADALLOC, E_WOULDBLOCK);
#define VM_DATAPART_GETFUTEX_OUTOFRANGE ((REF struct vm_futex *)-1)

/* Same as `vm_datapart_getfutex()', but don't allocate a new
 * futex object if none already exists for the given `datapart_offset'
 * @return: * :   A reference to the futex bound to the given `datapart_offset'
 * @return: NULL: No futex exists for the given `datapart_offset', even though `datapart_offset'
 *                was located within the bounds of the given data part at
 *                the time of the call being made.
 * @return: VM_DATAPART_GETFUTEX_OUTOFRANGE:
 *               The given `datapart_offset' is greater than `vm_datapart_numbytes(self)'
 *               s.a. `vm_datapart_getfutex()' */
FUNDEF WUNUSED NONNULL((1)) REF struct vm_futex *
(KCALL vm_datapart_getfutex_existing)(struct vm_datapart *__restrict self,
                                      uintptr_t datapart_offset)
		THROWS(E_WOULDBLOCK);

/* Lookup a futex at a given address that is offset from the start of a given
 * data block. Note though the possibly unintended behavior which applies when
 * the given `vm_datablock' is anonymous at the time of the call being made.
 * WARNING: Using this function when `self' has been, or always was anonymous, will
 *          cause the data part associated with the returned futex to also be anonymous,
 *          meaning that the part would get freshly allocated, and repeated calls with
 *          the same arguments would not yield the same futex object!
 *       -> As such, in the most common case of a futex lookup where you wish to find
 *          the futex associated with some given `uintptr_t', the process would be to
 *          to determine the `vm_node' of the address, and using that node then determine
 *          the associated vm_datapart, and relative offset into that datapart. If a lookup
 *          of the futex then returns `VM_DATAPART_GETFUTEX_OUTOFRANGE', loop back around
 *          and once again lookup the `vm_node'.
 *       -> In the end, there exists no API also found on linux that would make use of this
 *          function, however on KOS it is possible to access this function through use of
 *          the HANDLE_TYPE_DATABLOCK-specific hop() function `HOP_DATABLOCK_OPEN_FUTEX[_EXISTING]'
 * @return: * : The futex associated with the given `offset' */
FUNDEF ATTR_RETNONNULL WUNUSED NONNULL((1)) REF struct vm_futex *
(KCALL vm_datablock_getfutex)(struct vm_datablock *__restrict self, pos_t offset)
		THROWS(E_BADALLOC, E_WOULDBLOCK);

/* Same as `vm_datablock_getfutex()', but don't allocate a new
 * futex object if none already exists for the given `offset'
 * @return: * : The futex associated with the given `offset'
 * @return: NULL: No futex exists for the given address. */
FUNDEF WUNUSED NONNULL((1)) REF struct vm_futex *
(KCALL vm_datablock_getfutex_existing)(struct vm_datablock *__restrict self, pos_t offset)
		THROWS(E_WOULDBLOCK);

/* Return the futex object that is associated with the given virtual memory address.
 * In the event that `addr' isn't  */
FUNDEF ATTR_RETNONNULL WUNUSED NONNULL((1)) REF struct vm_futex *
(KCALL vm_getfutex)(struct vm *__restrict effective_vm, UNCHECKED void *addr)
		THROWS(E_BADALLOC, E_WOULDBLOCK, E_SEGFAULT);

/* Same as `vm_getfutex()', but don't allocate a new
 * futex object if none already exists for the given `addr'
 * @return: * : The futex associated with the given `addr'
 * @return: NULL: No futex exists for the given address. */
FUNDEF WUNUSED NONNULL((1)) REF struct vm_futex *
(KCALL vm_getfutex_existing)(struct vm *__restrict effective_vm, UNCHECKED void *addr)
		THROWS(E_WOULDBLOCK, E_SEGFAULT);

/* Broadcast to all thread waiting for a futex at `futex_address' within the current VM */
FUNDEF void FCALL
vm_futex_broadcast(UNCHECKED void *futex_address)
		THROWS(E_WOULDBLOCK, E_SEGFAULT);


DECL_END
#endif /* !CONFIG_USE_NEW_VM */

#endif /* !GUARD_KERNEL_INCLUDE_KERNEL_VM_FUTEX_H */
