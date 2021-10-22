 *             |        +-----> device                       Base class for device nodes within devfs
 *             |                  +-----> chrdev  (S_IFCHR)  (TODO: replacement for `struct chrdev')
 *      - always: mf_parts == MFILE_PARTS_ANONYMOUS; // iow: mfile_isanon(self);
 *      - always: mf_parts == MFILE_PARTS_ANONYMOUS; // iow: mfile_isanon(self);
 *      - always: mf_parts == MFILE_PARTS_ANONYMOUS; // iow: mfile_isanon(self);
 *      - usually: mf_parts == MFILE_PARTS_ANONYMOUS; // iow: mfile_isanon(self);
#ifdef __WANT_FS_INIT
#define FNODE_INIT_fn_nlink(fn_nlink)  fn_nlink
#define FNODE_INIT_fn_mode(fn_mode)    fn_mode
#define FNODE_INIT_fn_uid(fn_uid)      fn_uid
#define FNODE_INIT_fn_gid(fn_gid)      fn_gid
#define FNODE_INIT_fn_ino(fn_ino)      fn_ino
#define FNODE_INIT_fn_super(fn_super)  fn_super
#define FNODE_INIT_fn_allnodes         { LIST_ENTRY_UNBOUND_INITIALIZER }
#define FNODE_INIT_fn_allnodes_EX(...) { __VA_ARGS__ }
#define FNODE_INIT_fn_allsuper_EX(...) { .fn_allsuper = __VA_ARGS__ }
#endif /* __WANT_FS_INIT */
#define FNODE_INIT_fn_changed         { LIST_ENTRY_UNBOUND_INITIALIZER }
#define FNODE_INIT_fn_changed_EX(...) { __VA_ARGS__ }
#define FNODE_INIT_fn_changed         LIST_ENTRY_UNBOUND_INITIALIZER
#define FNODE_INIT_fn_changed_EX(...) __VA_ARGS__
#ifdef __WANT_FNODE__fn_suplop
	union {
		Toblockop(fsuper)      _fn_suplop;   /* Used for `fnode_add2sup_lop' */
		LLRBTREE_NODE(fnode)    fn_supent;   /* ... */
	};
#ifdef __WANT_FS_INIT
#define FNODE_INIT_fn_supent           { { __NULLPTR, __NULLPTR } }
#define FNODE_INIT_fn_supent_EX(...)   { __VA_ARGS__ }
#endif /* __WANT_FS_INIT */
#else /* __WANT_FNODE__fn_suplop */
#ifdef __WANT_FS_INIT
#define FNODE_INIT_fn_supent           { __NULLPTR, __NULLPTR }
#define FNODE_INIT_fn_supent_EX(...)   __VA_ARGS__
#endif /* __WANT_FS_INIT */
	                                          * When  `rb_rhs' is set to `FSUPER_NODES_DELETED', then the
#endif /* !__WANT_FNODE__fn_suplop */
		LIST_ENTRY(fnode)       fn_allnodes; /* [0..1][lock(:fnode_all_lock && REMOVE_ONCE)][valid_if(!fnode_issuper(self))]
		                                      * Link entry within the global list of all file nodes. When `MFILE_FN_GLOBAL_REF'
		                                      * is set, then the global  list holds a reference to  this node. (Even when  this
		                                      * link isn't bound) */
		LIST_ENTRY(fsuper)      fn_allsuper; /* [0..1][lock(:fsuper_all_lock && REMOVE_ONCE)][valid_if(fnode_issuper(self))]
		                                      * Link entry within the global list of all super blocks. When `MFILE_FN_GLOBAL_REF'
		                                      * is set, then  the global list  holds a reference  to this node.  (Even when  this
		                                      * link isn't bound) */
#ifdef __WANT_FNODE__fn_alllop
		struct lockop _fn_alllop; /* Lock operator used for async add-to-all-nodes */
#endif /* __WANT_FNODE__fn_alllop */
#ifndef FNODE_FSDATA_T
#define FNODE_FSDATA_T void
#endif /* !FNODE_FSDATA_T */
	FNODE_FSDATA_T    *fn_fsdata; /* [?..?][lock(?)] Optional,  fs-specific data  pointer or  data-word.
	                               * Since the different standard sub-classes of `fnode' have  different
	                               * struct sizes, it is normally quite difficult for filesystems to add
	                               * additional  data fields common to _all_ nodes (since the offset for
	                               * such data would be different for every node type). This is what you
	                               * can use this field for: have it point to somewhere else in the node
	                               * structure,  preferably to a fs-specific data blob which can then be
	                               * shared by _all_ nodes allocated for your filesystem. */
#ifdef __WANT_FNODE__fn_alllop
/* For use with `_fn_alllop': asynchronously add the node to the list of all nodes.
 * This function needs to be exposed publicly because it being set requires special
 * care if set during custom fnode destructors.
 * Note that the default `fnode_v_destroy()' includes correct handling for this. */
FUNDEF NOBLOCK NONNULL((1)) struct postlockop *
NOTHROW(LOCKOP_CC fnode_addtoall_lop)(struct lockop *__restrict self);
#endif /* __WANT_FNODE__fn_alllop */

#ifdef __WANT_FNODE__fn_suplop
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
FUNDEF NOBLOCK NONNULL((1, 2)) Tobpostlockop(fsuper) *
NOTHROW(LOCKOP_CC fnode_add2sup_lop)(Toblockop(fsuper) *__restrict self,
                                     struct fsuper *__restrict obj);
#endif /* __WANT_FNODE__fn_suplop */

/* Add the given node `self'  to the list of all  nodes. The caller must  ensure
 * that this function is _NOT_ called such that it would violate the REMOVE_ONCE
 * constraint  of `fn_allnodes'. Iow:  don't call this  function when `self' has
 * already become globally visible by some other means.
 *
 * This function can be used to initialize:
 *  - self->fn_allnodes
 * ... but  may only be called once _all_ other fields of `self' have already been
 *     initialized, and only if `self' isn't globally visible by some other means. */
FUNDEF NOBLOCK NONNULL((1)) void
NOTHROW(FCALL fnode_init_addtoall)(struct fnode *__restrict self);

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
FUNDEF NOBLOCK NONNULL((1)) void
NOTHROW(FCALL fnode_init_addtosuper_and_all)(struct fnode *__restrict self);

 * @throw: E_FSERROR_READONLY:    The `MFILE_FM_ATTRREADONLY' flag is (or was) set.
 * @throw: E_FSERROR_READONLY:    The `MFILE_FM_ATTRREADONLY' flag is (or was) set.
            USER CHECKED uid_t *pold_owner DFL(__NULLPTR),
            USER CHECKED gid_t *pold_group DFL(__NULLPTR),
/* Change all non-NULL the timestamp that are given.
 * @throw: E_FSERROR_DELETED:E_FILESYSTEM_DELETED_FILE: The `MFILE_F_DELETED' is set.
 * @throw: E_FSERROR_READONLY: The `MFILE_FM_ATTRREADONLY' flag is (or was) set. */
PUBLIC NONNULL((1)) void KCALL
mfile_chtime(struct mfile *__restrict self,
             struct timespec const *new_atime,
             struct timespec const *new_mtime,
             struct timespec const *new_ctime)
		THROWS(E_FSERROR_READONLY);


/* Check if the calling thread is allowed to access `self' as described by `type'
 * @param: type:   Set of `R_OK | W_OK | X_OK' (all specified types must be allowed)
 * @return: true:  Access granted
 * @return: false: Access denied. */
FUNDEF ATTR_PURE WUNUSED NONNULL((1)) __BOOL FCALL
fnode_mayaccess(struct fnode *__restrict self,
                unsigned int type)
		THROWS(E_WOULDBLOCK);

/* Helper wrapper for `fnode_mayaccess()' that asserts access
 * and throws `E_FSERROR_ACCESS_DENIED' is access was denied.
 * @param: type: Set of `R_OK | W_OK | X_OK' (all specified types must be allowed)
 * @return:                         Access granted
 * @throw: E_FSERROR_ACCESS_DENIED: Access denied. */
FUNDEF NONNULL((1)) void FCALL
fnode_access(struct fnode *__restrict self, unsigned int type)
		THROWS(E_WOULDBLOCK, E_FSERROR_ACCESS_DENIED);


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
FUNDEF NOBLOCK NONNULL((1)) void
NOTHROW(FCALL fnode_delete)(struct fnode *__restrict self);

/* Internal implementation of `fnode_delete()' (don't call this one
 * unless you know that you're doing; otherwise, you may cause race
 * conditions that can result in data corruption) */
FUNDEF NOBLOCK NONNULL((1)) void
NOTHROW(FCALL fnode_delete_impl)(/*inherit(always)*/ REF struct fnode *__restrict self);

#define fnode_isreg(self)     S_ISREG((self)->fn_mode)
#define fnode_isdir(self)     S_ISDIR((self)->fn_mode)
#define fnode_issuper(self)   (_fdirnode_asnode(&(self)->fn_super->fs_root) == (self))
#define fnode_islnk(self)     S_ISLNK((self)->fn_mode)
#define fnode_isfifo(self)    S_ISFIFO((self)->fn_mode)
#define fnode_issock(self)    S_ISSOCK((self)->fn_mode)
#define fnode_isdevnode(self) S_ISDEV((self)->fn_mode)
#define fnode_isdevice(self)  (S_ISDEV((self)->fn_mode) && (self)->fn_super == _ramfs_super_assuper(&devfs) && (self)->_fnode_file_ mf_ops != &ramfs_devnode_ops.dno_node.no_file)
#define fnode_ischrdev(self)  (S_ISCHR((self)->fn_mode) && (self)->fn_super == _ramfs_super_assuper(&devfs) && (self)->_fnode_file_ mf_ops != &ramfs_devnode_ops.dno_node.no_file)
#define fnode_isblkdev(self)  (S_ISBLK((self)->fn_mode) && (self)->fn_super == _ramfs_super_assuper(&devfs) && (self)->_fnode_file_ mf_ops != &ramfs_devnode_ops.dno_node.no_file)
#define fnode_isblkroot(self) mfile_isblkroot(_fnode_asfile(self))
#define fnode_isblkpart(self) mfile_isblkpart(_fnode_asfile(self))
#define fnode_asreg(self)     ((struct fregnode *)(self))
#define fnode_asdir(self)     ((struct fdirnode *)(self))
#define fnode_assuper(self)   __COMPILER_CONTAINER_OF((struct fdirnode *)(self), struct fsuper, fs_root)
#define fnode_aslnk(self)     ((struct flnknode *)(self))
#define fnode_asfifo(self)    ((struct ffifonode *)(self))
#define fnode_assock(self)    ((struct fsocknode *)(self))
#define fnode_asdevnode(self) ((struct fdevnode *)(self))
#define fnode_asdevice(self)  ((struct device *)(self))
#define fnode_aschrdev(self)  ((struct chrdev *)(self))
#define fnode_asblkdev(self)  ((struct blkdev *)(self))
#define mfile_isnode(self)    ((self)->mf_ops->mo_changed == &fnode_v_changed)
#define mfile_isreg(self)     (mfile_isnode(self) && fnode_isreg(mfile_asnode(self)))
#define mfile_isdir(self)     (mfile_isnode(self) && fnode_isdir(mfile_asnode(self)))
#define mfile_issuper(self)   (mfile_isnode(self) && fnode_issuper(mfile_asnode(self)))
#define mfile_islnk(self)     (mfile_isnode(self) && fnode_islnk(mfile_asnode(self)))
#define mfile_isfifo(self)    (mfile_isnode(self) && fnode_isfifo(mfile_asnode(self)))
#define mfile_issock(self)    (mfile_isnode(self) && fnode_issock(mfile_asnode(self)))
#define mfile_isdevnode(self) (mfile_isnode(self) && fnode_isdevnode(mfile_asnode(self)))
#define mfile_isdevice(self)  (mfile_isnode(self) && fnode_isdevice(mfile_asnode(self)))
#define mfile_ischrdev(self)  (mfile_isnode(self) && fnode_ischrdev(mfile_asnode(self)))
#define mfile_isblkdev(self)  (mfile_isnode(self) && fnode_isblkdev(mfile_asnode(self)))
#define mfile_isblkroot(self) (mfile_isblkdev(self) && !mfile_isblkpart(self))
#define mfile_isblkpart(self) ((self)->mf_ops == &blkpart_ops.bdo_dev.do_node.dno_node.no_file)
#define mfile_asnode(self)    ((struct fnode *)(self))
#define mfile_asreg(self)     fnode_asreg(mfile_asnode(self))
#define mfile_asdir(self)     fnode_asdir(mfile_asnode(self))
#define mfile_assuper(self)   fnode_assuper(mfile_asnode(self))
#define mfile_aslnk(self)     fnode_aslnk(mfile_asnode(self))
#define mfile_asfifo(self)    fnode_asfifo(mfile_asnode(self))
#define mfile_assock(self)    fnode_assock(mfile_asnode(self))
#define mfile_asdevnode(self) fnode_asdevnode(mfile_asnode(self))
#define mfile_asdevice(self)  fnode_asdevice(mfile_asnode(self))
#define mfile_aschrdev(self)  fnode_aschrdev(mfile_asnode(self))
#define mfile_asblkdev(self)  fnode_asblkdev(mfile_asnode(self))
FUNDEF NOBLOCK WUNUSED NONNULL((1, 2)) __BOOL NOTHROW(FCALL fnode_tree_tryinsert)(struct fnode **__restrict proot, struct fnode *__restrict node);