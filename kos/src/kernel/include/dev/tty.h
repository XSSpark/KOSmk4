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
#ifndef GUARD_KERNEL_INCLUDE_DEV_TTY_H
#define GUARD_KERNEL_INCLUDE_DEV_TTY_H 1

#include <kernel/compiler.h>

#include <dev/char.h>
#include <kernel/types.h>
#include <sched/pid.h>

#include <kos/dev.h>

#include <libterm/termio.h>

DECL_BEGIN

/* The base type for PTY and TTY devices */
#ifdef __CC__
struct taskpid;
struct stat;

#ifndef __taskpid_axref_defined
#define __taskpid_axref_defined
AXREF(taskpid_axref, taskpid);
#endif /* !__taskpid_axref_defined */

#ifndef __taskpid_awref_defined
#define __taskpid_awref_defined
AWREF(taskpid_awref, taskpid);
#endif /* !__taskpid_awref_defined */


#ifdef CONFIG_USE_NEW_FS
struct ttydev_ops {
	struct chrdev_ops to_cdev; /* Character device operators. */
};
#endif /* CONFIG_USE_NEW_FS */

struct ttydev
#ifndef __WANT_FS_INLINE_STRUCTURES
    : chrdev                      /* The underling character-device */
#endif /* !__WANT_FS_INLINE_STRUCTURES */
{
#ifdef __WANT_FS_INLINE_STRUCTURES
	struct chrdev        t_cdev;  /* The underling character-device */
#define _ttydev_aschr(x) &(x)->t_cdev
#define _ttydev_chr_     t_cdev.
#else /* __WANT_FS_INLINE_STRUCTURES */
#define _ttydev_aschr(x) x
#define _ttydev_chr_     /* nothing */
#endif /* !__WANT_FS_INLINE_STRUCTURES */
	struct terminal      t_term;  /* The associated terminal driver controller. */
	struct taskpid_awref t_cproc; /* [0..1] Controlling terminal support.
	                               * When  non-NULL,  points  to  a  session  leader  thread,  such that
	                               * `FORTASK(taskpid_gettask(t_cproc), this_taskgroup).tg_ctty == self'
	                               * is the case. */
	struct taskpid_axref t_fproc; /* [0..1] PID of the foreground process group leader.
	                               * This process is usually apart of the same session as `t_cproc' */
};


/* Kernel-level implementations for terminal system operators. */
FUNDEF NONNULL((1)) ssize_t LIBTERM_CC __ttydev_v_chk_sigttou(struct terminal *__restrict self) ASMNAME("ttydev_v_chk_sigttou");
FUNDEF NONNULL((1)) ssize_t LIBTERM_CC __ttydev_v_raise(struct terminal *__restrict self, signo_t signo) ASMNAME("ttydev_v_raise");


#ifdef CONFIG_USE_NEW_FS

/* Operator access */
#define ttydev_getops(self) \
	((struct ttydev_ops const *)__COMPILER_REQTYPE(struct ttydev const *, self)->_ttydev_chr_ _chrdev_dev_ _device_devnode_ _fdevnode_node_ _fnode_file_ mf_ops)
#ifdef NDEBUG
#define ___ttydev_assert_ops_(ops) /* nothing */
#else /* NDEBUG */
#define ___ttydev_assert_ops_(ops)                                                                          \
	__hybrid_assert((ops)->to_cdev.cdo_dev.do_node.dno_node.no_file.mo_stream),                             \
	__hybrid_assert((ops)->to_cdev.cdo_dev.do_node.dno_node.no_file.mo_stream->mso_read == &ttydev_v_read), \
	__hybrid_assert((ops)->to_cdev.cdo_dev.do_node.dno_node.no_file.mo_stream->mso_write == &ttydev_v_write),
#endif /* !NDEBUG */
#define _ttydev_assert_ops_(ops) _chrdev_assert_ops_(&(ops)->to_cdev) ___ttydev_assert_ops_(ops)

/* Helper macros */
#define mfile_istty(self)   ((self)->mf_ops->mo_stream && (self)->mf_ops->mo_stream->mso_read == &ttydev_v_read)
#define mfile_astty(self)   ((struct ttydev *)(self))
#define fnode_istty(self)   mfile_istty(_fnode_asfile(self))
#define fnode_astty(self)   mfile_astty(_fnode_asfile(self))
#define devnode_istty(self) fnode_istty(_fdevnode_asnode(self))
#define devnode_astty(self) fnode_astty(_fdevnode_asnode(self))
#define device_istty(self)  devnode_istty(_device_asdevnode(self))
#define device_astty(self)  devnode_astty(_device_asdevnode(self))
#define chrdev_istty(self)  device_istty(_chrdev_asdev(self))
#define chrdev_astty(self)  device_astty(_chrdev_asdev(self))

/* Default tty operators. */
FUNDEF NOBLOCK NONNULL((1)) void
NOTHROW(KCALL ttydev_v_destroy)(struct mfile *__restrict self);
#define ttydev_v_changed chrdev_v_changed
#define ttydev_v_wrattr  chrdev_v_wrattr
#define ttydev_v_tryas   chrdev_v_tryas
#define ttydev_v_hop     chrdev_v_hop
FUNDEF NONNULL((1)) size_t KCALL /* NOTE: This read operator is _MANDATORY_ and may not be overwritten by sub-classes! */
ttydev_v_read(struct mfile *__restrict self, USER CHECKED void *dst,
              size_t num_bytes, iomode_t mode) THROWS(...);
FUNDEF NONNULL((1)) size_t KCALL /* NOTE: This read operator is _MANDATORY_ and may not be overwritten by sub-classes! */
ttydev_v_write(struct mfile *__restrict self, USER CHECKED void const *src,
               size_t num_bytes, iomode_t mode) THROWS(...);
FUNDEF NONNULL((1)) syscall_slong_t KCALL
ttydev_v_ioctl(struct mfile *__restrict self, syscall_ulong_t cmd,
               USER UNCHECKED void *arg, iomode_t mode) THROWS(...);
FUNDEF NONNULL((1)) void KCALL
ttydev_v_stat(struct mfile *__restrict self,
              USER CHECKED struct stat *result) THROWS(...);
FUNDEF NONNULL((1)) void KCALL
ttydev_v_pollconnect(struct mfile *__restrict self,
                     poll_mode_t what) THROWS(...);
FUNDEF NONNULL((1)) poll_mode_t KCALL
ttydev_v_polltest(struct mfile *__restrict self,
                  poll_mode_t what) THROWS(...);

/* @return: -EINVAL: Unsupported `cmd' */
FUNDEF NONNULL((1)) syscall_slong_t KCALL
_ttydev_tryioctl(struct mfile *__restrict self, syscall_ulong_t cmd,
                 USER UNCHECKED void *arg, iomode_t mode) THROWS(...);

/* Initialize common+basic fields. The caller must still initialize:
 *  - self->_ttydev_chr_ _chrdev_dev_ _device_devnode_ _fdevnode_node_ _fnode_file_ mf_flags |= MFILE_FN_GLOBAL_REF;  # s.a. `device_registerf()'
 *  - self->_ttydev_chr_ _chrdev_dev_ _device_devnode_ _fdevnode_node_ fn_allnodes;  # s.a. `device_registerf()'
 *  - self->_ttydev_chr_ _chrdev_dev_ _device_devnode_ _fdevnode_node_ fn_supent;    # s.a. `device_registerf()'
 *  - self->_ttydev_chr_ _chrdev_dev_ _device_devnode_ _fdevnode_node_ fn_ino;       # s.a. `device_registerf()'
 *  - self->_ttydev_chr_ _chrdev_dev_ _device_devnode_ _fdevnode_node_ fn_mode;      # Something or'd with S_IFCHR
 *  - self->_ttydev_chr_ _chrdev_dev_ _device_devnode_ dn_devno;                     # s.a. `device_registerf()'
 *  - self->_ttydev_chr_ _chrdev_dev_ dv_driver;                                     # As `incref(drv_self)'
 *  - self->_ttydev_chr_ _chrdev_dev_ dv_dirent;                                     # s.a. `device_registerf()'
 *  - self->_ttydev_chr_ _chrdev_dev_ dv_byname_node;                                # s.a. `device_registerf()'
 * @param: struct ttydev       *self:     TTY to initialize.
 * @param: struct ttydev_ops   *ops:      TTY operators.
 * @param: pterminal_oprinter_t oprinter: [1..1] Terminal output printer. */
#define _ttydev_init(self, ops, oprinter)                \
	(___ttydev_assert_ops_(ops)                          \
	 _chrdev_init(_ttydev_aschr(self), &(ops)->to_cdev), \
	 terminal_init(&(self)->t_term, oprinter,            \
	               &__ttydev_v_raise,                    \
	               &__ttydev_v_chk_sigttou),             \
	 awref_init(&(self)->t_cproc, __NULLPTR),            \
	 axref_init(&(self)->t_fproc, __NULLPTR))
#define _ttydev_cinit(self, ops)                          \
	(___ttydev_assert_ops_(ops)                           \
	 _chrdev_cinit(_ttydev_aschr(self), &(ops)->to_cdev), \
	 terminal_init(&(self)->t_term, oprinter,             \
	               &__ttydev_v_raise,                     \
	               &__ttydev_v_chk_sigttou),              \
	 awref_cinit(&(self)->t_cproc, __NULLPTR),            \
	 axref_cinit(&(self)->t_fproc, __NULLPTR))
/* Finalize a partially initialized `struct ttydev' (as initialized by `_ttydev_init()') */
#define _ttydev_fini(self) _chrdev_fini(_ttydev_aschr(self))


#else /* CONFIG_USE_NEW_FS */
/* Check if a given character device is actually a ttybase */
#define chrdev_istty(self)                                                         \
	((self)->cd_heapsize >= sizeof(struct ttydev) &&                               \
	 ((struct ttydev *)(self))->t_term.t_chk_sigttou == &__ttydev_v_chk_sigttou && \
	 ((struct ttydev *)(self))->t_term.t_raise == &__ttydev_v_raise)


/* Default character-device read/write  operator implementations  for tty  devices
 * These functions will call forward to `terminal_iread()' and `terminal_owrite()'
 * NOTE: The implementation of these functions assumes that the oprinter associated
 *       with the terminal never returns negative values! */
FUNDEF NONNULL((1)) size_t KCALL ttydev_v_read(struct chrdev *__restrict self, USER CHECKED void *dst, size_t num_bytes, iomode_t mode) THROWS(...);
FUNDEF NONNULL((1)) size_t KCALL ttydev_v_write(struct chrdev *__restrict self, USER CHECKED void const *src, size_t num_bytes, iomode_t mode) THROWS(...);
FUNDEF NONNULL((1)) syscall_slong_t KCALL ttydev_v_ioctl(struct chrdev *__restrict self, syscall_ulong_t cmd, USER UNCHECKED void *arg, iomode_t mode) THROWS(...);
/* @return: -EINVAL: Unsupported `cmd' */
FUNDEF NONNULL((1)) syscall_slong_t KCALL _ttydev_tryioctl(struct chrdev *__restrict self, syscall_ulong_t cmd, USER UNCHECKED void *arg, iomode_t mode) THROWS(...);
FUNDEF NONNULL((1)) void KCALL ttydev_v_pollconnect(struct chrdev *__restrict self, poll_mode_t what) THROWS(...);
FUNDEF NONNULL((1)) poll_mode_t KCALL ttydev_v_polltest(struct chrdev *__restrict self, poll_mode_t what) THROWS(...);
FUNDEF NONNULL((1)) void KCALL ttydev_v_stat(struct chrdev *__restrict self, USER CHECKED struct stat *result) THROWS(...);

/* Initialize a given TTY character device.
 * NOTE: This function initializes the following operators:
 *   - cd_type.ct_fini        = &ttydev_v_fini;  // Must be called as fallback by overrides
 *   - cd_type.ct_read        = &ttydev_v_read;
 *   - cd_type.ct_write       = &ttydev_v_write;
 *   - cd_type.ct_ioctl       = &ttydev_v_ioctl; // Must be called as fallback by overrides
 *   - cd_type.ct_pollconnect = &ttydev_v_pollconnect;
 *   - cd_type.ct_polltest    = &ttydev_v_polltest;
 *   - cd_type.ct_stat        = &ttydev_v_stat; */
FUNDEF NOBLOCK void
NOTHROW(KCALL ttydev_cinit)(struct ttydev *__restrict self,
                            pterminal_oprinter_t oprinter);

/* Finalize a given TTY character device.
 * NOTE: This function must be called from a user-provided, device-level finalizer,
 *       or  in other words: You must call this from a function which you must then
 *       assign to `self->cd_type.ct_fini'! */
FUNDEF NOBLOCK NONNULL((1)) void
NOTHROW(KCALL ttydev_v_fini)(struct chrdev *__restrict self);
#endif /* !CONFIG_USE_NEW_FS */




/* [IMPL(TIOCSCTTY)] Set the given tty device as the controlling terminal of the calling session.
 * @param: steal_from_other_session: Allow the terminal to be stolen from another session.
 * @param: override_different_ctty:  If the calling session already had a CTTY assigned, override it.
 * @param: caller_must_be_leader:    Fail if the calling process isn't the session leader.
 * @return: * : One of `TTYDEV_SETCTTY_*' */
FUNDEF NOBLOCK int
NOTHROW(KCALL ttydev_setctty)(struct ttydev *__restrict self,
                              __BOOL caller_must_be_leader DFL(1),
                              __BOOL steal_from_other_session DFL(0),
                              __BOOL override_different_ctty DFL(0));
#define TTYDEV_SETCTTY_ALREADY     1  /* `self' was already the controlling terminal of the calling session. */
#define TTYDEV_SETCTTY_SUCCESS     0  /* Successfully assigned `self' as CTTY. */
#define TTYDEV_SETCTTY_NOTLEADER (-1) /* The calling process isn't the session leader, and `caller_must_be_leader' was true. */
#define TTYDEV_SETCTTY_DIFFERENT (-2) /* The calling session already had a CTTY assigned, and `override_different_ctty' was false. */
#define TTYDEV_SETCTTY_INUSE     (-3) /* The tty is already used as the CTTY of another session, and `steal_from_other_session' was false. */

/* [IMPL(TIOCNOTTY)] Give up the controlling terminal of the calling session.
 * @param: required_old_ctty: The expected old CTTY, or NULL if the CTTY should always be given up.
 * @param: pold_ctty:         When non-NULL, store the old CTTY here upon success.
 * @return: * : One of `TTYDEV_HUPCTTY_*' */
FUNDEF NOBLOCK int
NOTHROW(KCALL ttydev_hupctty)(struct ttydev *required_old_ctty DFL(__NULLPTR),
                              __BOOL caller_must_be_leader DFL(1),
                              REF struct ttydev **pold_ctty DFL(__NULLPTR));
#define TTYDEV_HUPCTTY_ALREADY      1  /* The calling session didn't have a CTTY to begin with */
#define TTYDEV_HUPCTTY_SUCCESS      0  /* Successfully gave up control of the CTTY (when `pold_ctty' was non-NULL, that old CTTY is stored there) */
#define TTYDEV_HUPCTTY_NOTLEADER  (-1) /* The calling process isn't the session leader, and `caller_must_be_leader' was true. */
#define TTYDEV_HUPCTTY_DIFFERENT  (-2) /* `required_old_ctty' was non-NULL and differed from the actually set old CTTY */

/* Returns a reference to the controlling- or foreground process's
 * PID descriptor, or NULL if the specified field hasn't been set. */
#define ttydev_getcproc(self) awref_get(&(self)->t_cproc)
#define ttydev_getfproc(self) axref_get(&(self)->t_fproc)

#endif /* __CC__ */

DECL_END

#ifdef GUARD_KERNEL_INCLUDE_SCHED_PID_H
#ifndef GUARD_KERNEL_INCLUDE_SCHED_PID_CTTY_H
#include <sched/pid-ctty.h>
#endif /* !GUARD_KERNEL_INCLUDE_SCHED_PID_CTTY_H */
#endif /* GUARD_KERNEL_INCLUDE_SCHED_PID_H */

#endif /* !GUARD_KERNEL_INCLUDE_DEV_TTY_H */
