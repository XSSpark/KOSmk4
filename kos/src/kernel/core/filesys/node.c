#define RBTREE_WANT_MINMAXLOCATE
#define RBTREE_MINMAX_T_DEFINED
#define fnode_tree_minmax_t        struct fnode_tree_minmax
/* Get the uid/gid of a given file-node. These functions respect `npo_getown' */
PUBLIC BLOCKING NONNULL((1)) uid_t KCALL
fnode_getuid(struct fnode *__restrict self) {
	uid_t result;
	struct fnode_perm_ops const *perm_ops;
	perm_ops = fnode_getops(self)->no_perm;
	if (perm_ops && perm_ops->npo_getown) {
		gid_t _ignored;
		(*perm_ops->npo_getown)(self, &result, &_ignored);
	} else {
#if 1
		result = ATOMIC_READ(self->fn_uid);
#else
		mfile_tslock_acquire(self);
		result = self->fn_uid;
		mfile_tslock_release(self);
#endif
	}
	return result;
}

PUBLIC BLOCKING NONNULL((1)) gid_t KCALL
fnode_getgid(struct fnode *__restrict self) {
	gid_t result;
	struct fnode_perm_ops const *perm_ops;
	perm_ops = fnode_getops(self)->no_perm;
	if (perm_ops && perm_ops->npo_getown) {
		uid_t _ignored;
		(*perm_ops->npo_getown)(self, &_ignored, &result);
	} else {
#if 1
		result = ATOMIC_READ(self->fn_gid);
#else
		mfile_tslock_acquire(self);
		result = self->fn_gid;
		mfile_tslock_release(self);
#endif
	}
	return result;
}

PUBLIC BLOCKING NONNULL((1, 2, 3)) void KCALL
fnode_getugid(struct fnode *__restrict self,
              uid_t *__restrict puid,
              gid_t *__restrict pgid) {
	struct fnode_perm_ops const *perm_ops;
	perm_ops = fnode_getops(self)->no_perm;
	if (perm_ops && perm_ops->npo_getown) {
		(*perm_ops->npo_getown)(self, puid, pgid);
	} else {
		mfile_tslock_acquire(self);
		*puid = self->fn_uid;
		*pgid = self->fn_gid;
		mfile_tslock_release(self);
	}
}




PUBLIC BLOCKING NONNULL((1)) mode_t KCALL
	if (check_permissions) {
		struct fnode_perm_ops const *perm_ops;
		perm_ops = fnode_getops(self)->no_perm;
		if (perm_ops && perm_ops->npo_getown) {
			uid_t uid;
			gid_t gid;
			(*perm_ops->npo_getown)(self, &uid, &gid);
			if (uid != cred_getfsuid() && !capable(CAP_FOWNER))
				THROW(E_INSUFFICIENT_RIGHTS, CAP_FOWNER);
			if ((perm_flag & S_ISGID) || (perm_mask & S_ISGID)) {
				/* The Set-gid bit can must  be turned off (i.e.  cannot be turned on/or  left
				 * on) when the caller isn't apart of the group associated with `self->fn_gid' */
				if (!capable(CAP_FSETID) && !cred_isfsgroupmember(gid)) {
					perm_mask &= ~S_ISGID;
					perm_flag &= ~S_ISGID;
				}
			}
			check_permissions = false;
		}
	}

	{
		struct fnode_perm_ops const *perm_ops;
		perm_ops = fnode_getops(self)->no_perm;
		if (perm_ops && perm_ops->npo_getown) {
			/* Owner/group are fixed and cannot be changed. */
			(*perm_ops->npo_getown)(self, &old_owner, &old_group);
			if ((owner != (uid_t)-1 && owner != old_owner) ||
			    (group != (gid_t)-1 && group != old_group))
				THROW(E_FSERROR_READONLY);
			if (pold_owner)
				*pold_owner = old_owner;
			if (pold_group)
				*pold_group = old_group;
			return;
		}
	}


		/* Check if file attributes may be modified. */
		if unlikely(self->mf_flags & MFILE_FN_ATTRREADONLY) {
			mfile_tslock_release_br(self);
			THROW(E_FSERROR_READONLY);
		}

PUBLIC BLOCKING NONNULL((1)) void KCALL
PUBLIC BLOCKING NONNULL((1)) void KCALL
		struct mfile_stream_ops const *stream_ops;
		/* If defined, invoke the streams-sync operator */
		stream_ops = self->mf_ops->mo_stream;
		if (stream_ops != NULL && stream_ops->mso_sync != NULL)
			(*stream_ops->mso_sync)(self);
PUBLIC NOBLOCK ATTR_PURE WUNUSED NONNULL((1)) bool
NOTHROW(FCALL fnode_mayaccess)(struct fnode *__restrict self,
                               unsigned int type) {
	struct fnode_perm_ops const *perm_ops;
	perm_ops = fnode_getops(self)->no_perm;
	if (perm_ops && perm_ops->npo_getown) {
		uid_t uid;
		gid_t gid;
		(*perm_ops->npo_getown)(self, &uid, &gid);
		for (how = 1; how <= 4; how <<= 1) {
			if (!(type & how))
				continue; /* Access not checked. */
			if likely(mode & how)
				continue; /* Access is allowed for everyone. */
			if (mode & (how << 3)) {
				if likely(cred_isfsgroupmember(gid))
					continue; /* The calling thread's user is part of the file owner's group */
			}
			if (mode & (how << 6)) {
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
PUBLIC NOBLOCK NONNULL((1)) void FCALL
	assertf(!fnode_isdevice(self), "You need to use `device_delete()' in this case!");
	assertf(!fnode_issuper(self), "You need to use `super_delete()' in this case!");
