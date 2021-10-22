#define __WANT_FNODE__fn_alllop
#define __WANT_FNODE__fn_suplop
#include <kernel/fs/blkdev.h>
#include <kernel/fs/devfs.h>
#include <kernel/fs/ramfs.h>
#include <kos/except/reason/fs.h>
#define RBTREE_WANT_TRYINSERT
	if (obj->fs_changednodes.lh_first != FSUPER_NODES_DELETED) {
		bool is_first = LIST_EMPTY(&obj->fs_changednodes);
		LIST_INSERT_HEAD(&obj->fs_changednodes, me, fn_changed);
		/* First changed node was added to the superblock.
		 * -> Mark the superblock as changed (for `sync(2)') */
		if (is_first)
			fsuper_add2changed(obj);
	}
	if (fsuper_changednodes_tryacquire(super)) {
		if (!LIST_ISBOUND(self, fn_changed) &&
		    super->fs_changednodes.lh_first != FSUPER_NODES_DELETED) {
		fsuper_changednodes_release(super);
	assert(me->fs_root._fn_alllop.lo_func != &fnode_addtoall_lop);
	if unlikely(me->_fn_alllop.lo_func == &fnode_addtoall_lop) {
		/* Must let `fnode_addtoall_lop()' finish first! */
		me->_mf_lop.lo_func = &fnode_v_destroy_rmallnodes_lop;
		lockop_enqueue(&fallnodes_lockops, &me->_mf_lop);
		return NULL;
	}
again_unbind_allnodes:
	COMPILER_READ_BARRIER();
			COMPILER_READ_BARRIER();
			if unlikely(me->_fn_alllop.lo_func == &fnode_addtoall_lop) {
				fallnodes_release(); /* This should reap & invoke fnode_addtoall_lop() */
				goto again_unbind_allnodes;
			}
	COMPILER_READ_BARRIER();
	assertf(me->_fn_suplop.olo_func != &fnode_add2sup_lop,
	        "The `fnode_add2sup_lop' function owns a reference to the node, "
	        "and we only get here then the reference counter reaches 0, so "
	        "there should be no way the object is still trying to async-add "
	        "itself to the nodes list!");
	if (me->fn_supent.rb_rhs != FSUPER_NODES_DELETED) {
	assertf(!(self->mf_flags & MFILE_FN_GLOBAL_REF),
				assert(me->fs_root._fn_alllop.lo_func != &fnode_addtoall_lop);
		if (ATOMIC_READ(self->fn_supent.rb_rhs) != FSUPER_NODES_DELETED) {
			if (fsuper_nodes_trywrite(super)) {
				COMPILER_READ_BARRIER();
				if (self->fn_supent.rb_rhs != FSUPER_NODES_DELETED) {
					assertf(self->_fn_suplop.olo_func != &fnode_add2sup_lop,
					        "The `fnode_add2sup_lop' function owns a reference to the node, "
					        "and we only get here then the reference counter reaches 0, so "
					        "there should be no way the object is still trying to async-add "
					        "itself to the nodes list!");
				fsuper_nodes_endwrite(super);
				_fsuper_nodes_reap(super);
again_unbind_allnodes:
		COMPILER_READ_BARRIER();
				COMPILER_READ_BARRIER();
				if unlikely(self->_fn_alllop.lo_func == &fnode_addtoall_lop) {
					fallnodes_release(); /* This should reap & invoke fnode_addtoall_lop() */
					goto again_unbind_allnodes;
				}
/* For use with `_fn_alllop': asynchronously add the node to the list of all nodes.
 * This function needs to be exposed publicly because it being set requires special
 * care if set during custom fnode destructors.
 * Note that the default `fnode_v_destroy()' includes correct handling for this. */
PUBLIC NOBLOCK NONNULL((1)) struct postlockop *
NOTHROW(LOCKOP_CC fnode_addtoall_lop)(struct lockop *__restrict self) {
	STATIC_ASSERT(offsetof(struct fnode, fn_allnodes.le_prev) == offsetof(struct fnode, _fn_alllop.lo_func));
	struct fnode *me = container_of(self, struct fnode, _fn_alllop);
	assert(me->_fn_alllop.lo_func == &fnode_addtoall_lop);
	ATOMIC_WRITE(me->fn_allnodes.le_prev, &fallnodes_list.lh_first); /* This write needs to be atomic */
	if ((me->fn_allnodes.le_next = fallnodes_list.lh_first) != NULL)
		me->fn_allnodes.le_next->fn_allnodes.le_prev = &me->fn_allnodes.le_next;
	return NULL;
}



/* Add the given node `self'  to the list of all  nodes. The caller must  ensure
 * that this function is _NOT_ called such that it would violate the REMOVE_ONCE
 * constraint  of `fn_allnodes'. Iow:  don't call this  function when `self' has
 * already become globally visible by some other means.
 *
 * This function can be used to initialize:
 *  - self->fn_allnodes
 * ... but  may only be called once _all_ other fields of `self' have already been
 *     initialized, and only if `self' isn't globally visible by some other means. */
PUBLIC NOBLOCK NONNULL((1)) void
NOTHROW(FCALL fnode_init_addtoall)(struct fnode *__restrict self) {
	/* Yes: we do a raw init! */
	DBG_memset(&self->fn_allnodes, 0xcc, sizeof(self->fn_allnodes));
	if (fallnodes_tryacquire()) {
		LIST_INSERT_HEAD(&fallnodes_list, self, fn_allnodes);
		fallnodes_release();
	} else {
		/* Use a lock operator. */
		self->_fn_alllop.lo_func = &fnode_addtoall_lop;
		COMPILER_WRITE_BARRIER();
		lockop_enqueue(&fallnodes_lockops, &self->_fn_alllop);
		_fallnodes_reap();
	}
}


PRIVATE NOBLOCK NONNULL((1, 2)) void
NOTHROW(LOCKOP_CC destroy_node_from_super_postlop)(Tobpostlockop(fsuper) *__restrict self,
                                                   struct fsuper *__restrict obj) {
	struct fnode *me;
	me = container_of(self, struct fnode, _mf_fsuperplop);
	mfile_destroy(me);
}

/* For use with `_fn_suplop': The node is currently adding itself to the superblock
 * by  means of an asynchronous operation. NOTE: This lock operator unconditionally
 * inherits a reference  to `container_of(self, struct fnode, _fn_suplop)', and  as
 * such  you may assume that a fnode which  has been destroyed would no longer make
 * use of this.
 *
 * However, in situations where `fsuper_nodes_removenode(node)' is called, you  must
 * first check  if  `node->_fn_suplop.olo_func == &fnode_add2sup_lop',  and  if  so,
 * must  release your lock to the superblock before reacquiring it in order to allow
 * the node to fully add itself to the superblock's node tree so that you may safely
 * remove it later.
 *
 * Similarly, `fsuper_nodes_locate()' or `fsuper_nodes_remove()' returning NULL when
 * `fsuper_nodes_mustreap()' does not necessarily mean  that the node you're  trying
 * to lookup/remove doesn't exist; it may just be trying to add itself right now. */
PUBLIC NOBLOCK NONNULL((1, 2)) Tobpostlockop(fsuper) *
NOTHROW(LOCKOP_CC fnode_add2sup_lop)(Toblockop(fsuper) *__restrict self,
                                     struct fsuper *__restrict obj) {
	REF struct fnode *me;
	me = container_of(self, struct fnode, _fn_suplop);
	assert(obj == me->fn_super);

	/* Check for special  case: if  the node has  been marked  as
	 * deleted, then we mustn't add it to the super's Inode tree. */
	if unlikely(me->mf_flags & MFILE_F_DELETED) {
		ATOMIC_WRITE(me->fn_supent.rb_rhs, FSUPER_NODES_DELETED);
		return NULL;
	}
	if unlikely(!fsuper_nodes_tryinsert(obj, me)) {
		struct fnode *existing;
		existing = fsuper_nodes_remove(obj, me->fn_ino);
		assert(existing);
		existing->fn_supent.rb_rhs = FSUPER_NODES_DELETED;
		fsuper_nodes_insert(obj, me);
	}

	/* Inherit one reference from `me' */
	if unlikely(ATOMIC_DECFETCH(me->mf_refcnt) == 0) {
		/* Don't do the destroy here; use a post-operator for that! */
		me->_mf_fsuperplop.oplo_func = &destroy_node_from_super_postlop;
		return &me->_mf_fsuperplop;
	}
	return NULL;
}

/* The async combination of adding `self' to its superblock's node tree, as  well
 * as to the global list of all nodes. The caller must ensure that `self->fn_ino'
 * isn't already in use any  other file-node. If another  node with the same  INO
 * number  already exists, said other node is  first removed from the INode tree.
 *
 * This function can be used to initialize:
 *  - self->fn_supent
 *  - self->fn_allnodes
 * ... but  may only be called once _all_ other fields of `self' have already been
 *     initialized, and only if `self' isn't globally visible by some other means,
 *     though the fields initialized  by this are pretty  much the only fields  by
 *     which nodes *normally* become globally visible. */
PUBLIC NOBLOCK NONNULL((1)) void
NOTHROW(FCALL fnode_init_addtosuper_and_all)(struct fnode *__restrict self) {
	struct fsuper *super = self->fn_super;
	assert(!(self->mf_flags & MFILE_F_DELETED));
	if (fsuper_nodes_trywrite(super)) {
		if unlikely(!fsuper_nodes_tryinsert(super, self)) {
			struct fnode *existing;
			existing = fsuper_nodes_remove(super, self->fn_ino);
			assert(existing);
			existing->fn_supent.rb_rhs = FSUPER_NODES_DELETED;
			fsuper_nodes_insert(super, self);
		}

		/* Also add to the list of all nodes.
		 * NOTE: Like required by `fnode_init_addtoall()', this function must be
		 *       called _BEFORE_ `self' becomes globally visible, which is still
		 *       the case so-long as we've not `fsuper_nodes_endwrite(super)'. */
		fnode_init_addtoall(self);

		/* Release the super-nodes lock. */
		fsuper_nodes_endwrite(super);
	} else {
		STATIC_ASSERT_MSG(offsetof(struct fnode, _fn_suplop.olo_func) ==
		                  offsetof(struct fnode, fn_supent.rb_rhs),
		                  "This is a requirement to ensure that the node can't "
		                  "accidentally appear as `rb_rhs == FSUPER_NODES_DELETED', "
		                  "which would indicate that it had been removed from the "
		                  "super's node tree, when in fact it's still in limbo as "
		                  "to whether or not it's part of said tree.");

		/* Use lock operations to have the INode add itself to the  */
		++self->mf_refcnt; /* +1: inherited by `fnode_add2sup_lop()' */
		self->_fn_suplop.olo_func = &fnode_add2sup_lop;
		COMPILER_WRITE_BARRIER();

		/* Also add to the all-nodes list. */
		fnode_init_addtoall(self);
		COMPILER_BARRIER();

		/* At this point we would normally have a race condition since `node'
		 * has yet to be added to its superblock's node-tree, but (may)  have
		 * already become globally visible (via the all-nodes list).
		 *
		 * But this is resolved by  `node->_fn_suplop.olo_func = &fnode_add2sup_lop',
		 * which for one makes it so that `fn_supent.rb_rhs != FSUPER_NODES_DELETED',
		 * marking  the file as _NOT_ removed from its super's Inode tree, as well as
		 * by this special scenario being publicly documented by requiring other code
		 * to reap a superblock's node-lops before trying to remove an INode that  is
		 * marked  as  `node->_fn_suplop.olo_func == &fnode_add2sup_lop', as  well as
		 * also reaping if trying (and failing) to remove from a super that's got any
		 * pending lops. */
		oblockop_enqueue(&super->fs_nodeslockops, &self->_fn_suplop);
		_fsuper_nodes_reap(super);
	}
}





 * @throw: E_FSERROR_READONLY:    The `MFILE_FM_ATTRREADONLY' flag is (or was) set.
	/* Check if file attributes may be modified. */
	if unlikely(self->mf_flags & MFILE_FM_ATTRREADONLY)
		THROW(E_FSERROR_READONLY);

 * @throw: E_FSERROR_READONLY:    The `MFILE_FM_ATTRREADONLY' flag is (or was) set.
	/* Check if file attributes may be modified. */
	if unlikely(self->mf_flags & MFILE_FM_ATTRREADONLY)
		THROW(E_FSERROR_READONLY);


/* Change all non-NULL the timestamp that are given.
 * @throw: E_FSERROR_DELETED:E_FILESYSTEM_DELETED_FILE: The `MFILE_F_DELETED' is set.
 * @throw: E_FSERROR_READONLY: The `MFILE_FM_ATTRREADONLY' flag is (or was) set. */
PUBLIC NONNULL((1)) void KCALL
mfile_chtime(struct mfile *__restrict self,
             struct timespec const *new_atime,
             struct timespec const *new_mtime,
             struct timespec const *new_ctime)
		THROWS(E_FSERROR_READONLY) {
	if unlikely(self->mf_flags & MFILE_FM_ATTRREADONLY)
		THROW(E_FSERROR_READONLY);
	mfile_tslock_acquire(self);
	/* Check if the file was deleted. (If it was,
	 * then timestamps must no longer be modified
	 * sine they're re-used for lops & the like) */
	COMPILER_READ_BARRIER();
	if (self->mf_flags & MFILE_F_DELETED) {
		mfile_tslock_release_br(self);
		THROW(E_FSERROR_DELETED, E_FILESYSTEM_DELETED_FILE);
	}
	/* Fill in new timestamps. */
	if (new_atime) {
		self->mf_atime.tv_sec  = new_atime->tv_sec;
		self->mf_atime.tv_nsec = new_atime->tv_nsec;
	}
	if (new_mtime) {
		self->mf_mtime.tv_sec  = new_mtime->tv_sec;
		self->mf_mtime.tv_nsec = new_mtime->tv_nsec;
	}
	if (new_ctime) {
		self->mf_ctime.tv_sec  = new_ctime->tv_sec;
		self->mf_ctime.tv_nsec = new_ctime->tv_nsec;
	}
	mfile_tslock_release(self);

	/* Mark attributes of this file as having changed. */
	mfile_changed(self, MFILE_F_ATTRCHANGED);
}



	do {
		old_flags = ATOMIC_READ(self->mf_flags);
		if (!(old_flags & MFILE_F_ATTRCHANGED) || (old_flags & MFILE_F_DELETED))
			return;
	} while (!ATOMIC_CMPXCH_WEAK(self->mf_flags, old_flags,
	                             old_flags & ~MFILE_F_ATTRCHANGED));
			fsuper_changednodes_acquire(super);
				fsuper_changednodes_release(super);
		fsuper_changednodes_release(super);
		 * should  actually appear within the changed list. */
	do {
		old_flags = ATOMIC_READ(self->mf_flags);
		if (!(old_flags & MFILE_F_CHANGED) || (old_flags & MFILE_F_DELETED))
			return;
	} while (!ATOMIC_CMPXCH_WEAK(self->mf_flags, old_flags,
	                             old_flags & ~MFILE_F_CHANGED));
			fsuper_changednodes_acquire(super);
				fsuper_changednodes_release(super);
		fsuper_changednodes_release(super);
		 * should  actually appear within the changed list. */

		/* Special case: if we're a block device, also invoke its sync operator. */
		if (fnode_isblkdev(self)) {
			struct blkdev *me;
			struct blkdev_ops const *ops;
			me  = fnode_asblkdev(self);
			ops = blkdev_getops(me);
			if (ops->bdo_sync != NULL)
				(*ops->bdo_sync)(me);
		}


/* Check if the calling thread is allowed to access `self' as described by `type'
 * @param: type:   Set of `R_OK | W_OK | X_OK' (all specified types must be allowed)
 * @return: true:  Access granted
 * @return: false: Access denied. */
PUBLIC ATTR_PURE WUNUSED NONNULL((1)) bool FCALL
fnode_mayaccess(struct fnode *__restrict self,
                unsigned int type)
		THROWS(E_WOULDBLOCK) {
	unsigned int how;
	mode_t mode = ATOMIC_READ(self->fn_mode);
	for (how = 1; how <= 4; how <<= 1) {
		if (!(type & how))
			continue; /* Access not checked. */
		if likely(mode & how)
			continue; /* Access is allowed for everyone. */
		if (mode & (how << 3)) {
			gid_t gid = ATOMIC_READ(self->fn_gid);
			if likely(cred_isfsgroupmember(gid))
				continue; /* The calling thread's user is part of the file owner's group */
		}
		if (mode & (how << 6)) {
			uid_t uid = ATOMIC_READ(self->fn_uid);
			if likely(uid == cred_getfsuid())
				continue; /* The calling thread's user is the file's owner */
			/* CAP_FOWNER: ... Ignore filesystem-uid checks ... */
			if likely(capable(CAP_FOWNER))
				continue;
		}
		return false;
	}
	return true;
}

/* Helper wrapper for `fnode_mayaccess()' that asserts access
 * and throws `E_FSERROR_ACCESS_DENIED' is access was denied.
 * @param: type: Set of `R_OK | W_OK | X_OK' (all specified types must be allowed)
 * @return:                         Access granted
 * @throw: E_FSERROR_ACCESS_DENIED: Access denied. */
PUBLIC NONNULL((1)) void FCALL
fnode_access(struct fnode *__restrict self, unsigned int type)
		THROWS(E_FSERROR_ACCESS_DENIED) {
	if unlikely(!fnode_mayaccess(self, type))
		THROW(E_FSERROR_ACCESS_DENIED);
}






/************************************************************************/
/* Implementation of `fnode_delete()'                                   */
/************************************************************************/

PRIVATE NOBLOCK NONNULL((1)) void
NOTHROW(LOCKOP_CC fnode_delete_remove_from_allnodes_postlop)(struct postlockop *__restrict self) {
	REF struct fnode *me;
	me = container_of(self, struct fnode, _mf_plop);

	/* At this point, all of the global hooks of `me' have been  removed.
	 * The rest of the deletion work relates only to the underlying mfile
	 * and includes stuff like anonymizing any bound mem-parts. */
	mfile_delete_impl(me); /* Inherit reference */
}

PRIVATE NOBLOCK NONNULL((1)) struct postlockop *
NOTHROW(LOCKOP_CC fnode_delete_remove_from_allnodes_lop)(struct lockop *__restrict self) {
	REF struct fnode *me;
	me = container_of(self, struct fnode, _mf_lop);
	COMPILER_READ_BARRIER();
	if (LIST_ISBOUND(me, fn_allnodes))
		LIST_UNBIND(me, fn_allnodes);
	me->_mf_plop.plo_func = &fnode_delete_remove_from_allnodes_postlop; /* Inherit reference */
	return &me->_mf_plop;
}


PRIVATE NOBLOCK NONNULL((1, 2)) void
NOTHROW(LOCKOP_CC fnode_delete_remove_from_changed_postlop)(Tobpostlockop(fsuper) *__restrict self,
                                                            struct fsuper *__restrict UNUSED(obj)) {
	REF struct fnode *me;
	me = container_of(self, struct fnode, _mf_fsuperplop);

	/* Remove from `fallnodes_list'. */
	COMPILER_READ_BARRIER();
	if (LIST_ISBOUND(me, fn_allnodes)) {
		COMPILER_READ_BARRIER();
		if (fallnodes_tryacquire()) {
			COMPILER_READ_BARRIER();
			if (LIST_ISBOUND(me, fn_allnodes))
				LIST_UNBIND(me, fn_allnodes);
			COMPILER_READ_BARRIER();
			fallnodes_release();
		} else {
			me->_mf_lop.lo_func = &fnode_delete_remove_from_allnodes_lop; /* Inherit reference */
			lockop_enqueue(&fallnodes_lockops, &me->_mf_lop);
			_fallnodes_reap();
			return;
		}
	}
	fnode_delete_remove_from_allnodes_postlop(&me->_mf_plop); /* Inherit reference */
}

PRIVATE NOBLOCK NONNULL((1, 2)) Tobpostlockop(fsuper) *
NOTHROW(LOCKOP_CC fnode_delete_remove_from_changed_lop)(Toblockop(fsuper) *__restrict self,
                                                        struct fsuper *__restrict UNUSED(obj)) {
	REF struct fnode *me;
	me = container_of(self, struct fnode, _mf_fsuperlop);
	if (LIST_ISBOUND(me, fn_changed)) {
		LIST_UNBIND(me, fn_changed);
		decref_nokill(me); /* Reference from the `fn_changed' list. */
	}

	me->_mf_fsuperplop.oplo_func = &fnode_delete_remove_from_changed_postlop; /* Inherit reference */
	return &me->_mf_fsuperplop;
}

PRIVATE NOBLOCK NONNULL((1, 2)) void
NOTHROW(LOCKOP_CC fnode_delete_remove_from_byino_postlop)(Tobpostlockop(fsuper) *__restrict self,
                                                          struct fsuper *__restrict obj) {
	REF struct fnode *me;
	me = container_of(self, struct fnode, _mf_fsuperplop);

	/* Remove from `obj->fs_changednodes'. */
	COMPILER_READ_BARRIER();
	if (LIST_ISBOUND(me, fn_changed)) {
		COMPILER_READ_BARRIER();
		if (fsuper_changednodes_tryacquire(obj)) {
			COMPILER_READ_BARRIER();
			if (LIST_ISBOUND(me, fn_changed)) {
				LIST_UNBIND(me, fn_changed);
				decref_nokill(me); /* Reference from the `fn_changed' list. */
			}
			COMPILER_READ_BARRIER();
			fsuper_changednodes_release(obj);
		} else {
			me->_mf_fsuperlop.olo_func = &fnode_delete_remove_from_changed_lop; /* Inherit reference */
			oblockop_enqueue(&obj->fs_changednodes_lops, &me->_mf_fsuperlop);
			_fsuper_changednodes_reap(obj);
			return;
		}
	}

	fnode_delete_remove_from_changed_postlop(&me->_mf_fsuperplop, obj); /* Inherit reference */
}

PRIVATE NOBLOCK NONNULL((1, 2)) Tobpostlockop(fsuper) *
NOTHROW(LOCKOP_CC fnode_delete_remove_from_byino_lop)(Toblockop(fsuper) *__restrict self,
                                                      struct fsuper *__restrict obj) {
	REF struct fnode *me;
	me = container_of(self, struct fnode, _mf_fsuperlop);
	COMPILER_READ_BARRIER();
	if (me->fn_supent.rb_rhs != FSUPER_NODES_DELETED) {
		if unlikely(me->_fn_suplop.olo_func == &fnode_add2sup_lop) {
			/* Device is still being async-added to the nodes tree.
			 * In this case, we must wait for it to finish doing so
			 * before trying to repeat the remove-from-tree lop. */
			me->_mf_fsuperlop.olo_func = &fnode_delete_remove_from_byino_lop;
			oblockop_enqueue(&obj->fs_nodeslockops, &me->_mf_fsuperlop); /* Retry later... */
			return NULL;
		}
		fsuper_nodes_removenode(obj, me);
		ATOMIC_WRITE(me->fn_supent.rb_rhs, FSUPER_NODES_DELETED);
	}
	me->_mf_fsuperplop.oplo_func = &fnode_delete_remove_from_byino_postlop; /* Inherit reference */
	return &me->_mf_fsuperplop;
}



/* Internal implementation of `fnode_delete()' (don't call this one
 * unless you know that you're doing; otherwise, you may cause race
 * conditions that can result in data corruption) */
PUBLIC NOBLOCK NONNULL((1)) void
NOTHROW(FCALL fnode_delete_impl)(/*inherit(always)*/ REF struct fnode *__restrict self) {
	struct fsuper *super = self->fn_super;

	/* Remove from the superblock's INode tree. */
	if (ATOMIC_READ(self->fn_supent.rb_rhs) != FSUPER_NODES_DELETED) {
again_lock_super:
		if (fsuper_nodes_trywrite(super)) {
			COMPILER_READ_BARRIER();
			if (self->fn_supent.rb_rhs != FSUPER_NODES_DELETED) {
				if unlikely(self->_fn_suplop.olo_func == &fnode_add2sup_lop) {
					/* Reap+retry */
					_fsuper_nodes_endwrite(super);
					_fsuper_nodes_reap(super);
					goto again_lock_super;
				}
				fsuper_nodes_removenode(super, self);
				ATOMIC_WRITE(self->fn_supent.rb_rhs, FSUPER_NODES_DELETED);
			}
			fsuper_nodes_endwrite(super);
		} else {
			self->_mf_fsuperlop.olo_func = &fnode_delete_remove_from_byino_lop; /* Inherit reference */
			oblockop_enqueue(&super->fs_nodeslockops, &self->_mf_fsuperlop);
			_fsuper_nodes_reap(super);
			return;
		}
	}
	fnode_delete_remove_from_byino_postlop(&self->_mf_fsuperplop, super); /* Inherit reference */
}

/* Perform all of the async work needed for deleting `self' as the result of `fn_nlink == 0'
 * This function will do the following (asynchronously if necessary)
 *  - Set flags: MFILE_F_DELETED,  MFILE_F_NOATIME,  MFILE_F_NOMTIME, MFILE_F_CHANGED,
 *               MFILE_F_ATTRCHANGED,  MFILE_F_FIXEDFILESIZE,   MFILE_FM_ATTRREADONLY,
 *               MFILE_F_NOUSRMMAP, MFILE_F_NOUSRIO, MFILE_FS_NOSUID, MFILE_FS_NOEXEC,
 *               MFILE_F_READONLY
 *    If `MFILE_F_DELETED' was already set, none of the below are done!
 *  - Unlink `self->fn_supent' (if bound)
 *  - Unlink `self->fn_changed' (if bound)
 *  - Unlink `self->fn_allnodes' (if bound)
 *  - Call `mfile_delete()' (technically `mfile_delete_impl()') */
PUBLIC NOBLOCK NONNULL((1)) void
NOTHROW(FCALL fnode_delete)(struct fnode *__restrict self) {
	uintptr_t old_flags;

	/* Delete global reference to the file-node. */
	if (ATOMIC_FETCHAND(self->mf_flags, ~(MFILE_FN_GLOBAL_REF |
	                                      MFILE_F_PERSISTENT)) &
	    MFILE_FN_GLOBAL_REF) {
		decref_nokill(self);
	}

	/* Mark the file as deleted (and make available use of the timestamp fields) */
	mfile_tslock_acquire(self);
	old_flags = ATOMIC_FETCHOR(self->mf_flags,
	                           MFILE_F_DELETED | MFILE_F_NOATIME | MFILE_F_NOMTIME |
	                           MFILE_F_CHANGED | MFILE_F_ATTRCHANGED | MFILE_F_FIXEDFILESIZE |
	                           MFILE_FM_ATTRREADONLY | MFILE_F_NOUSRMMAP | MFILE_F_NOUSRIO |
	                           MFILE_FS_NOSUID | MFILE_FS_NOEXEC | MFILE_F_READONLY);
	if (old_flags & MFILE_F_PERSISTENT)
		ATOMIC_AND(self->mf_flags, ~MFILE_F_PERSISTENT); /* Also clear the PERSISTENT flag */
	mfile_tslock_release(self);

	if (old_flags & MFILE_F_DELETED)
		return; /* Already deleted, or deletion already in progress. */

	incref(self);
	fnode_delete_impl(self);
}


