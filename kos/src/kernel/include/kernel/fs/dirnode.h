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
#ifndef GUARD_KERNEL_INCLUDE_KERNEL_FS_DIRNODE_H
#define GUARD_KERNEL_INCLUDE_KERNEL_FS_DIRNODE_H 1

#include <kernel/compiler.h>

#ifndef CONFIG_USE_NEW_FS
#include <fs/node.h>
#else /* !CONFIG_USE_NEW_FS */
#include <kernel/fs/node.h>
#include <kernel/types.h>

#include <bits/os/timespec.h>
#include <kos/io.h>

#ifdef __CC__
DECL_BEGIN

struct fdirent;
struct fdirnode;

#ifndef __fsmode_t_defined
#define __fsmode_t_defined
typedef uintptr_t fsmode_t; /* Set of `FS_MODE_F*' */
#endif /* !__fsmode_t_defined */


/* Info descriptor for looking up directory entries by-name. */
struct flookup_info {
	CHECKED USER /*utf-8*/ char const *flu_name;    /* [?..flu_namelen] Name for the new file. */
	uintptr_t                          flu_hash;    /* Hash for `mkf_name' (s.a. `fdirent_hash()') */
	u16                                flu_namelen; /* Length of `mkf_name' */
	u16                               _flu_pad;     /* ... */
	u32                                flu_flags;   /* Set of `0 | FS_MODE_FDOSPATH' */
};

/* Flags for `fmkfile_info::mkf_flags' */
#define FMKFILE_F_NORMAL 0x0000           /* [in] Normal flags */
#define FMKFILE_F_NOCASE FS_MODE_FDOSPATH /* [in] Ignore case */
#define FMKFILE_F_EXISTS 0x0001           /* [out] File already existed (no new file created) */

struct fcreatfile_info {
	uid_t                      c_owner;     /* [in] File owner */
	gid_t                      c_group;     /* [in] File group */
	struct timespec            c_atime;     /* [in] File access timestamp */
	struct timespec            c_mtime;     /* [in] File modified timestamp */
	struct timespec            c_ctime;     /* [in] File created timestamp */
	union {
		dev_t                  c_rdev;      /* [valid_if(S_ISDEV(mkf_fmode))][in] Referenced device. */
		struct {
			CHECKED USER /*utf-8*/ char const *s_text; /* [?..s_size][in] Symlink text. */
			size_t                             s_size; /* [in] Symlink text length (in characters; excluding trailing \0). */
		}                      c_symlink;   /* [valid_if(S_ISLNK(mkf_fmode))] Symlink text. */
	};
};

/* Info descriptor for: creat(2), mkdir(2), mknod(2), symlink(2) and link(2) */
struct fmkfile_info {
	union {
		struct {
			CHECKED USER /*utf-8*/ char const *mkf_name;    /* [?..mkf_namelen][in] Name for the new file. */
			uintptr_t                          mkf_hash;    /* [in] Hash for `mkf_name' (s.a. `fdirent_hash()') */
			u16                                mkf_namelen; /* [in] Length of `mkf_name' */
			u16                               _mkf_pad;     /* ... */
			u32                                mkf_flags;   /* [in|out] Set of `FMKFILE_F_*' */
		};
		struct flookup_info            mkf_lookup_info;     /* [in] Lookup info for the new file being created. */
	};
	REF struct fdirent                *mkf_dent;    /* [1..1][out] Directory entry for the new file (s.a. `dno_lookup') */
	mode_t                             mkf_fmode;   /* [in] File  type & access  permissions for the new  file-node. If a hard-
	                                                 * link  should be created, this is field is set to ZERO. For this purpose,
	                                                 * node that all S_IF*-file-type-flags are non-zero, meaning you can simply
	                                                 * switch on this field and use `0' for hard-link. */
	union {
		REF struct fnode              *mkf_rnode;   /* [1..1][out] The newly constructed file-node. */
		REF struct fregnode           *mkf_rreg;    /* [1..1][out][valid_if(S_ISREG(mkf_fmode) || (mkf_fmode == 0 && fnode_isreg(mkf_hrdlnk.hl_node)))] */
		REF struct fdirnode           *mkf_rdir;    /* [1..1][out][valid_if(S_ISDIR(mkf_fmode) || (mkf_fmode == 0 && fnode_isdir(mkf_hrdlnk.hl_node)))] */
		REF struct flnknode           *mkf_rlnk;    /* [1..1][out][valid_if(S_ISLNK(mkf_fmode) || (mkf_fmode == 0 && fnode_islnk(mkf_hrdlnk.hl_node)))] */
		REF struct ffifonode          *mkf_rfifo;   /* [1..1][out][valid_if(S_ISFIFO(mkf_fmode) || (mkf_fmode == 0 && fnode_isfifo(mkf_hrdlnk.hl_node)))] */
		REF struct fsocknode          *mkf_rsock;   /* [1..1][out][valid_if(S_ISSOCK(mkf_fmode) || (mkf_fmode == 0 && fnode_issock(mkf_hrdlnk.hl_node)))] */
		REF struct fdevnode           *mkf_rdev;    /* [1..1][out][valid_if(S_ISDEV(mkf_fmode) || (mkf_fmode == 0 && fnode_isdev(mkf_hrdlnk.hl_node)))] */
	};
	union {
		struct {
			struct fnode              *hl_node;     /* [1..1][in] The file to which to create a hard-link. */
		}                      mkf_hrdlnk; /* [valid_if(mkf_fmode == 0)] Hardlink creation info. */
		struct fcreatfile_info mkf_creat;  /* [valid_if(mkf_fmode != 0)] File creation info. */
	};
};

/* Info descriptor for: rename(2) */
struct frename_info {
	union {
		struct {
			CHECKED USER /*utf-8*/ char const *frn_name;    /* [?..frn_namelen][in] Name for the new file. */
			uintptr_t                          frn_hash;    /* [in] Hash for `frn_name' (s.a. `fdirent_hash()') */
			u16                                frn_namelen; /* [in] Length of `frn_name' */
			u16                               _frn_pad;     /* ... */
			u32                                frn_flags;   /* [in] Set of `0 | FS_MODE_FDOSPATH' */
		};
		struct flookup_info            frn_lookup_info;     /* [in] Lookup info for the new file being created. */
	};
	REF struct fdirent                *frn_dent;    /* [1..1][out] New directory entry for the file. */
	struct fnode                      *frn_file;    /* [1..1][in] The file that should be renamed. */
	struct fdirent                    *frn_oldent;  /* [1..1][in] Directory of `frn_file' in `frn_olddir' */
	struct fdirnode                   *frn_olddir;  /* [1..1][in] Old containing directory. (Part of the same superblock!)
	                                                 * NOTE: The new  directory  is  guarantied  not to  be  a  child  of  this
	                                                 *       directory! (this must be asserted by the caller of `dno_rename()') */
};


struct fdirenum;
struct fdirenum_ops {
	/* [1..1] Finalization callback. */
	NOBLOCK NONNULL((1)) void
	/*NOTHROW*/ (KCALL *deo_fini)(struct fdirenum *__restrict self);

	/* [1..1] Directory reader callback.
	 * When end-of-file has been reached, `0' is returned. */
	NONNULL((1)) size_t
	(KCALL *deo_readdir)(struct fdirenum *__restrict self, USER CHECKED struct dirent *buf,
	                     size_t bufsize, readdir_mode_t readdir_mode, iomode_t mode)
			THROWS(...);

	/* [1..1] Directory stream seeking.
	 * This callback must at least be capable of  `offset=0,whence=SEEK_SET',
	 * which may be used to rewind the directory and re-read all entries that
	 * had already been read before. (s.a. `rewinddir(3)')
	 * Other  than this, it is undefined what an absolute `offset' actually
	 * represents.  - It may be an index (seeking to the n'th entry), or an
	 * absolute position within the containing directory file, or something
	 * entirety different. (multiple directory entires are even allowed  to
	 * share the same  ~position~, meaning that  reading the position,  and
	 * later restoring it is _not_ a  usage-case that has to be  supported)
	 * NOTE: This function mustn't enumerate the `.' and `..' entires!
	 * @return: * : The new position (however that may be defined) within
	 *              the directory stream. */
	NONNULL((1)) pos_t
	(KCALL *deo_seekdir)(struct fdirenum *__restrict self,
	                     off_t offset, unsigned int whence)
			THROWS(...);
};

struct fdirenum {
	struct fdirenum_ops const *de_ops;      /* [1..1][const] Operators. */
	void                      *de_data[15]; /* Filesystem-specific data. */
};

/* Helper macros for operating with `struct fdirenum' */
#define fdirenum_fini(self)                                      (*(self)->de_ops->deo_fini)(self)
#define fdirenum_readdir(self, buf, bufsize, readdir_mode, mode) (*(self)->de_ops->deo_readdir)(self, buf, bufsize, readdir_mode, mode)
#define fdirenum_seekdir(self, offset, whence)                   (*(self)->de_ops->deo_seekdir)(self, offset, whence)


struct fdirnode_ops {
	struct fnode_ops dno_node; /* FNode operators */

	/* [1..1] Lookup a directory entry within `self', given its name.
	 * @return: NULL: No entry exists that is matching the given name. */
	WUNUSED NONNULL((1, 2)) REF struct fdirent *
	(KCALL *dno_lookup)(struct fdirnode *__restrict self,
	                    struct flookup_info *__restrict info);

	/* [0..1] Optional helper to directly lookup a file-node w/o going
	 *        through the fdirent indirection.
	 * @return: NULL: No entry exists that is matching the given name. */
	WUNUSED NONNULL((1, 2)) REF struct fnode *
	(KCALL *dno_lookup_fnode)(struct fdirnode *__restrict self,
	                          struct flookup_info *__restrict info);

	/* [1..1] Construct a directory enumerator object in `*result'.
	 * This  function  must  initialize _all_  fields  of `*result'
	 * It  is undefined if files created or  deleted after the creation of an
	 * enumerator, will still be enumerated by said enumerator. It is however
	 * guarantied that all files  not created/deleted afterwards will  always
	 * be enumerated */
	NONNULL((1, 2)) void
	(KCALL *dno_enum)(struct fdirnode *__restrict self,
	                  struct fdirenum *__restrict result);

	/* [0..1] Create  new files within a given directory. If
	 * another file with the same name already existed, then
	 * `FMKFILE_F_EXISTS'  is set, and that file is returned
	 * instead.
	 * @throw: E_FSERROR_ILLEGAL_PATH:        `info->mkf_name' contains bad characters
	 * @throw: E_FSERROR_DISK_FULL:           Disk full
	 * @throw: E_FSERROR_READONLY:            Read-only filesystem
	 * @throw: E_FSERROR_TOO_MANY_HARD_LINKS: ... */
	NONNULL((1, 2)) void
	(KCALL *dno_mkfile)(struct fdirnode *__restrict self,
	                    struct fmkfile_info *__restrict info)
			THROWS(E_FSERROR_ILLEGAL_PATH, E_FSERROR_DISK_FULL,
			       E_FSERROR_READONLY, E_FSERROR_TOO_MANY_HARD_LINKS);

	/* [0..1] Delete   the  specified  file  from  this  directory.
	 * This function is also responsible to update `file->fn_nlink'
	 * @throw: E_FSERROR_FILE_NOT_FOUND:      The file had  already been deleted,  or
	 *                                        renamed (it no longer exists as `entry'
	 *                                        within `self').
	 * @throw: E_FSERROR_DIRECTORY_NOT_EMPTY: `file' is a non-empty directory.
	 * @throw: E_FSERROR_READONLY:            Read-only filesystem
	 * @throw: E_FSERROR_DELETED:             The given `entry' was already deleted */
	NONNULL((1, 2, 3)) void
	(KCALL *dno_unlink)(struct fdirnode *__restrict self,
	                    struct fdirent *__restrict entry,
	                    struct fnode *__restrict file)
			THROWS(E_FSERROR_FILE_NOT_FOUND, E_FSERROR_DIRECTORY_NOT_EMPTY,
			       E_FSERROR_READONLY, E_FSERROR_DELETED);

	/* [0..1] Rename/move the specified file from one location to another
	 * @throw: E_FSERROR_ILLEGAL_PATH:        `info->frn_name' contains bad characters
	 * @throw: E_FSERROR_DISK_FULL:           Disk full
	 * @throw: E_FSERROR_READONLY:            Read-only filesystem
	 * @throw: E_FSERROR_FILE_ALREADY_EXISTS: `info->frn_name' already exists
	 * @throw: E_FSERROR_DELETED:             The given `entry' was already deleted */
	NONNULL((1, 2)) void
	(KCALL *dno_rename)(struct fdirnode *__restrict self,
	                    struct frename_info *__restrict info)
			THROWS(E_FSERROR_ILLEGAL_PATH, E_FSERROR_DISK_FULL,
			       E_FSERROR_READONLY, E_FSERROR_FILE_ALREADY_EXISTS,
			       E_FSERROR_DELETED);
};

struct fdirnode
#ifndef __WANT_FS_INLINE_STRUCTURES
    : fnode                          /* Underlying file-node */
#endif /* !__WANT_FS_INLINE_STRUCTURES */
{
#ifdef __WANT_FS_INLINE_STRUCTURES
	struct fnode         dn_node;    /* Underlying file-node */
#define _fdirnode_node_     dn_node.
#define _fdirnode_asnode(x) &(x)->dn_node
#ifdef __WANT_FS_INIT
#define FDIRNODE_INIT_EX(refcnt, ops, parts, blockshift, fn_nlink, fn_mode, fn_uid, fn_gid, fn_ino, fn_super, dn_parent) \
	{ FNODE_INIT_EX(refcnt, &(ops)->dno_node, parts, blockshift, fn_nlink, (fn_mode) | S_IFDIR, fn_uid, fn_gid, fn_ino, fn_super), dn_parent }
#endif /* __WANT_FS_INIT */
#else /* __WANT_FS_INLINE_STRUCTURES */
#define _fdirnode_node_     /* nothing */
#define _fdirnode_asnode(x) x
#endif /* !__WANT_FS_INLINE_STRUCTURES */
	REF struct fdirnode *dn_parent;  /* [0..1][const] Parent directory (or `NULL' if `fnode_issuper(self)'). */
};


/* Return a pointer to directory-node operators of `self' */
#define fdirnode_getops(self)                                                                                       \
	__COMPILER_CONTAINER_OF(__COMPILER_REQTYPE(struct fdirnode const *, self)->_fdirnode_node_ _fnode_file_ mf_ops, \
	                        struct fdirnode_ops, dno_node.no_file)
#define _fdirnode_assert_ops_(ops) \
	_fnode_assert_ops_(&(ops)->dno_node)

/* Check if a given `struct fdirnode *self' is a fsuper. */
#define fdirnode_issuper(self)  (&(self)->_fdirnode_node_ fn_super->fs_root == (self))
#define fdirnode_assuper(self)  __COMPILER_CONTAINER_OF(self, struct fsuper, fs_root)



/* Initialize common+basic fields. The caller must still initialize:
 *  - self->_fdirnode_node_ _fnode_file_ mf_atime
 *  - self->_fdirnode_node_ _fnode_file_ mf_mtime
 *  - self->_fdirnode_node_ _fnode_file_ mf_ctime
 *  - self->_fdirnode_node_ fn_uid
 *  - self->_fdirnode_node_ fn_gid
 *  - self->_fdirnode_node_ fn_allnodes
 *  - self->_fdirnode_node_ fn_supent
 *  - self->_fdirnode_node_ fn_ino
 *  - self->_fdirnode_node_ fn_mode (with something or'd with S_IFDIR)
 * @param: struct fdirnode     *self:   Directory node to initialize.
 * @param: struct fdirnode_ops *ops:    Directory operators.
 * @param: struct fdirnode     *parent: Parent directory. */
#define _fdirnode_init(self, ops, parent)                                                                       \
	(_fdirnode_assert_ops_(ops) _fnode_init_common(_fdirnode_asnode(self)),                                     \
	 (self)->_fdirnode_node_ _fnode_file_ mf_parts = MFILE_PARTS_ANONYMOUS,                                     \
	 (self)->_fdirnode_node_ _fnode_file_ mf_flags = (parent)->_fdirnode_node_ _fnode_file_ mf_flags &          \
	                                                 (MFILE_F_DELETED | MFILE_F_PERSISTENT |                    \
	                                                  MFILE_F_READONLY | MFILE_F_NOATIME |                      \
	                                                  MFILE_F_NOMTIME | MFILE_F_NOUSRMMAP |                     \
	                                                  MFILE_F_NOUSRIO | MFILE_F_FIXEDFILESIZE),                 \
	 __hybrid_assert(((self)->_fdirnode_node_ _fnode_file_ mf_flags &                                           \
	                  (MFILE_F_NOUSRMMAP | MFILE_F_NOUSRIO | MFILE_F_FIXEDFILESIZE)) ==                         \
	                 (MFILE_F_NOUSRMMAP | MFILE_F_NOUSRIO | MFILE_F_FIXEDFILESIZE)),                            \
	 (self)->_fdirnode_node_ _fnode_file_ mf_ops        = &(ops)->dno_node.no_file,                             \
	 (self)->_fdirnode_node_ _fnode_file_ mf_blockshift = (parent)->_fdirnode_node_ _fnode_file_ mf_blockshift, \
	 (self)->_fdirnode_node_ _fnode_file_ mf_part_amask = (parent)->_fdirnode_node_ _fnode_file_ mf_part_amask, \
	 (self)->_fdirnode_node_ _fnode_file_ mf_filesize   = (pos_t)-1,                                            \
	 (self)->_fdirnode_node_ fn_nlink                   = 1,                                                    \
	 (self)->_fdirnode_node_ fn_super                   = incref((parent)->_fdirnode_node_ fn_super),           \
	 (self)->dn_parent                                  = incref(parent))
#define _fdirnode_cinit(self, ops, super)                                                                       \
	(_fdirnode_assert_ops_(ops) _fnode_cinit_common(_fdirnode_asnode(self)),                                    \
	 (self)->mf_parts                              = MFILE_PARTS_ANONYMOUS,                                     \
	 (self)->_fdirnode_node_ _fnode_file_ mf_flags = (parent)->_fdirnode_node_ _fnode_file_ mf_flags &          \
	                                                 (MFILE_F_DELETED | MFILE_F_PERSISTENT |                    \
	                                                  MFILE_F_READONLY | MFILE_F_NOATIME |                      \
	                                                  MFILE_F_NOMTIME | MFILE_F_NOUSRMMAP |                     \
	                                                  MFILE_F_NOUSRIO | MFILE_F_FIXEDFILESIZE),                 \
	 __hybrid_assert(((self)->_fdirnode_node_ _fnode_file_ mf_flags &                                           \
	                  (MFILE_F_NOUSRMMAP | MFILE_F_NOUSRIO | MFILE_F_FIXEDFILESIZE)) ==                         \
	                 (MFILE_F_NOUSRMMAP | MFILE_F_NOUSRIO | MFILE_F_FIXEDFILESIZE)),                            \
	 (self)->_fdirnode_node_ _fnode_file_ mf_ops        = &(ops)->dno_node.no_file,                             \
	 (self)->_fdirnode_node_ _fnode_file_ mf_blockshift = (parent)->_fdirnode_node_ _fnode_file_ mf_blockshift, \
	 (self)->_fdirnode_node_ _fnode_file_ mf_part_amask = (parent)->_fdirnode_node_ _fnode_file_ mf_part_amask, \
	 (self)->_fdirnode_node_ _fnode_file_ mf_filesize   = (pos_t)-1,                                            \
	 (self)->_fdirnode_node_ fn_nlink                   = 1,                                                    \
	 (self)->_fdirnode_node_ fn_super                   = incref((parent)->_fdirnode_node_ fn_super),           \
	 (self)->dn_parent                                  = incref(parent))
/* Finalize a partially initialized `struct fdirnode' (as initialized by `_fdirnode_init()') */
#define _fdirnode_fini(self) \
	(decref_nokill((self)->fn_super), decref_nokill((self)->dn_parent))

/* Default operators for `struct fdirnode_ops' */
FUNDEF NOBLOCK NONNULL((1)) void
NOTHROW(KCALL fdirnode_v_destroy)(struct mfile *__restrict self);


/* Public API (high-level wrappers around low-level operators) */

/* Lookup a directory entry within `self', given its name.
 * @return: NULL: No entry exists that is matching the given name. */
#define fdirnode_lookup(self, info) \
	(*fdirnode_getops(self)->dno_lookup)(self, info)

/* Lookup the INode associated with a given name within `self'.
 * @return: NULL: No entry exists that is matching the given name. */
FUNDEF WUNUSED NONNULL((1, 2)) REF struct fnode *KCALL
fdirnode_lookup_fnode(struct fdirnode *__restrict self,
                      struct flookup_info *__restrict info);


/* Construct a directory  enumerator object in  `*result'.
 * This function must initialize _all_ fields of `*result'
 * It  is undefined if files created or  deleted after the creation of an
 * enumerator, will still be enumerated by said enumerator. It is however
 * guarantied that all files  not created/deleted afterwards will  always
 * be enumerated */
#define fdirnode_enum(self, result) \
	(*fdirnode_getops(self)->dno_enum)(self, result)


/* Create new files within a given directory.
 * If another  file with  the same  name already  existed,  then
 * `FMKFILE_F_EXISTS' is set, and that file is returned instead.
 * @throw: E_FSERROR_ILLEGAL_PATH:        `info->mkf_name' contains bad characters
 * @throw: E_FSERROR_DISK_FULL:           Disk full
 * @throw: E_FSERROR_READONLY:            Read-only filesystem (or unsupported operation)
 * @throw: E_FSERROR_TOO_MANY_HARD_LINKS: ... */
FUNDEF NONNULL((1, 2)) void KCALL
fdirnode_mkfile(struct fdirnode *__restrict self,
                struct fmkfile_info *__restrict info)
		THROWS(E_FSERROR_ILLEGAL_PATH, E_FSERROR_DISK_FULL,
		       E_FSERROR_READONLY, E_FSERROR_TOO_MANY_HARD_LINKS);

/* Delete the specified file from this directory
 * @throw: E_FSERROR_FILE_NOT_FOUND:      The file had  already been deleted,  or
 *                                        renamed (it no longer exists as `entry'
 *                                        within `self').
 * @throw: E_FSERROR_DIRECTORY_NOT_EMPTY: `file' is a non-empty directory.
 * @throw: E_FSERROR_READONLY:            Read-only filesystem (or unsupported operation)
 * @throw: E_FSERROR_DELETED:             The given `entry' was already deleted */
FUNDEF NONNULL((1, 2)) void KCALL
fdirnode_unlink(struct fdirnode *__restrict self,
                struct fdirent *__restrict entry,
                struct fnode *__restrict file)
		THROWS(E_FSERROR_FILE_NOT_FOUND, E_FSERROR_DIRECTORY_NOT_EMPTY,
		       E_FSERROR_READONLY, E_FSERROR_DELETED);

/* Rename/move the specified file from one location to another
 * @throw: E_FSERROR_ILLEGAL_PATH:            `info->frn_name' contains bad characters
 * @throw: E_FSERROR_DISK_FULL:               Disk full
 * @throw: E_FSERROR_READONLY:                Read-only filesystem
 * @throw: E_FSERROR_FILE_ALREADY_EXISTS:     `info->frn_name' already exists
 * @throw: E_FSERROR_DELETED:                 The given `entry' was already deleted
 * @throw: E_FSERROR_CROSS_DEVICE_LINK:       ...
 * @throw: E_FSERROR_DIRECTORY_MOVE_TO_CHILD: ... */
FUNDEF NONNULL((1, 2)) void KCALL
fdirnode_rename(struct fdirnode *__restrict self,
                struct frename_info *__restrict info)
		THROWS(E_FSERROR_ILLEGAL_PATH, E_FSERROR_DISK_FULL,
		       E_FSERROR_READONLY, E_FSERROR_FILE_ALREADY_EXISTS,
		       E_FSERROR_DELETED, E_FSERROR_CROSS_DEVICE_LINK,
		       E_FSERROR_DIRECTORY_MOVE_TO_CHILD);


DECL_END
#endif /* __CC__ */
#endif /* CONFIG_USE_NEW_FS */

#endif /* !GUARD_KERNEL_INCLUDE_KERNEL_FS_DIRNODE_H */