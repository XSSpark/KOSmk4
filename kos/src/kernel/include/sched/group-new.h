/* Copyright (c) 2019-2022 Griefer@Work                                       *
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
 *    Portions Copyright (c) 2019-2022 Griefer@Work                           *
 * 2. Altered source versions must be plainly marked as such, and must not be *
 *    misrepresented as being the original software.                          *
 * 3. This notice may not be removed or altered from any source distribution. *
 */
#ifndef GUARD_KERNEL_INCLUDE_SCHED_GROUP_NEW_H
#define GUARD_KERNEL_INCLUDE_SCHED_GROUP_NEW_H 1

#include <kernel/compiler.h>

#ifdef CONFIG_USE_NEW_GROUP
#include <kernel/types.h>
#include <sched/pid.h>
#include <sched/signal.h>

#include <hybrid/sequence/list.h>
#include <hybrid/sync/atomic-rwlock.h>

#include <kos/aref.h>
#include <kos/lockop.h>


#ifdef __CC__
DECL_BEGIN

struct taskpid;

#ifndef __taskpid_list_defined
#define __taskpid_list_defined
LIST_HEAD(taskpid_list, taskpid);
#endif /* !__taskpid_list_defined */

#ifndef __task_arref_defined
#define __task_arref_defined
ARREF(task_arref, task);
#endif /* !__task_arref_defined */

#ifndef __taskpid_arref_defined
#define __taskpid_arref_defined
ARREF(taskpid_arref, taskpid);
#endif /* !__taskpid_arref_defined */

#ifndef __task_list_defined
#define __task_list_defined
LIST_HEAD(task_list, task);
#endif /* !__task_list_defined */

/* For `posix-signal' */
struct pending_rpc; /* Define in `rpc-internal.h' */
#ifndef __pending_rpc_slist_defined
#define __pending_rpc_slist_defined
SLIST_HEAD(pending_rpc_slist, pending_rpc);
#endif /* !__pending_rpc_slist_defined */


/* ============== Controller for processes / process groups / sessions ==============
 *
 * [1..1] Process of thread: >> struct taskpid *proc        = THIS_TASKPID->tp_proc;
 * [1..1] Process parent:    >> REF struct task *parent     = arref_get(&THIS_TASKPID->tp_pctl->pc_parent);
 * [1..1] Process group:     >> REF struct procgrp *grp     = arref_get(&THIS_TASKPID->tp_pctl->pc_grp);
 * [1..1] Process session:   >> REF struct procsession *ses = arref_get(&grp->pgr_session);
 *
 * is_process_leader:
 *    >> return THIS_TASKPID->tp_proc == THIS_TASKPID;
 * is_group_leader:
 *    >> pid = THIS_TASKPID;
 *    >> grp = arref_get(&pid->tp_pctl->pc_grp);
 *    >> return grp->pgr_ns == pid->tp_ns &&
 *    >>        _taskpid_slot_getpidno(grp->pgr_pids[pid->tp_ns->pn_ind].pgs_pid) ==
 *    >>        _taskpid_slot_getpidno(pid->tp_pids[pid->tp_ns->pn_ind].pgs_pid);
 * is_session_leader:
 *    >> return is_group_leader && grp == grp->pgr_sleader;
 *
 * get_thread_id:              >> THIS_TASKPID->tp_pids[*].tps_pid;
 * get_process_id:             >> THIS_TASKPID->tp_proc->tp_pids[*].tps_pid;
 * get_group_id:               >> arref_get(&THIS_TASKPID->tp_pctl->pc_grp)->pgr_pids[*].pgs_pid
 * get_session_id:             >> arref_get(&THIS_TASKPID->tp_pctl->pc_grp)->pgr_sleader->pgr_pids[*].pgs_pid
 * get_session_id_of_terminal: >> awref_get(&tty->t_cproc)?:->pgr_pids[*].pgs_pid
 * get_fggroup_id_of_terminal: >> awref_get(&tty->t_fproc)?:->pgr_pids[*].pgs_pid
 *
 * NOTE: get_session_id -- Sessions IDs are _required_ to be process group IDs:
 *       Posix says about `getsid(2)':
 *       """
 *       The getsid(2) function  shall obtain  the process group  ID of  the
 *       process that is the session leader of the process specified by pid.
 *       """
 *       In other words: the concept of "session IDs" doesn't actually
 *       exist. Instead, it's all just process and process group  IDs!
 *
 * Enumerate threads/children of process (WARNING: E_WOULDBLOCK):
 * >> struct taskpid *tpid;
 * >> struct procctl *ctl = _task_getproctl();
 * >> procctl_chlds_read(ctl);
 * >> // NOTE: This enumeration does not include task_getprocess()!
 * >> LIST_FOREACH (tpid, &ctl->pc_chlds_list, tp_parsib) {
 * >>     if (taskpid_isaprocess(tpid)) {
 * >>         assert(tpid->tp_pctl != ctl);
 * >>         assert(arref_ptr(&tpid->tp_pctl->pc_parent) == THIS_TASK);
 * >>         // Child process: `tpid'
 * >>     } else {
 * >>         // Thread: `tpid' (but doesn't include `THIS_TASKPID')
 * >>     }
 * >> }
 * >> procctl_chlds_endread(ctl);
 *
 * Enumerate process in a process group (WARNING: E_WOULDBLOCK):
 * >> struct procgrp *grp = ...;
 * >> struct taskpid *tpid;
 * >> procgrp_memb_read(grp);
 * >> LIST_FOREACH (tpid, &grp->pgr_memb_list, tp_pctl->pc_grpmember) {
 * >>     assert(taskpid_isaprocess(tpid));
 * >>     assert(arref_ptr(&tpid->tp_pctl->pc_grp) == grp);
 * >>     // Member process: `tpid'
 * >> }
 * >> procgrp_memb_endread(grp);
 *
 * There  doesn't need to be a way of enumerating process groups
 * apart  of a session.  -- Nothing in  posix would require that
 * sort of functionality (though you could do a system-wide enum
 * of all processes  (e.g. via `pidns_root'),  and filter  those
 * that are apart of your sought-after session)
 *
 */


struct ttydev;
#ifndef __ttydev_axref_defined
#define __ttydev_axref_defined
AXREF(ttydev_axref, ttydev);
#endif /* !__ttydev_axref_defined */

struct procsession {
	struct ttydev_axref ps_ctty; /* [0..1] The controlling terminal; aka. "/dev/tty" (if any) */
	/* Other session data would go here...
	 *
	 * After skimming the posix specs, it seems that the controlling terminal is
	 * the  only thing that needs to be in here. The only other mention of stuff
	 * relating  to session lifetime appears to be return values of `sysconf()',
	 * so maybe stuff like `struct fs::fs_lnkmax' should also go in here?
	 */
};

/* Allocate/Free a `struct procsession' */
#define _procsession_new()      ((struct procsession *)kmalloc(sizeof(struct procsession), GFP_CALLOC))
#define _procsession_free(self) kfree(self)
#define _procsession_fini(self) axref_fini(&(self)->ps_ctty)
#define _procsession_destroy(self) \
	(_procsession_fini(self), _procsession_free(self))

struct procgrp;
struct procgrp_slot {
	LLRBTREE_NODE(procgrp) pgs_link; /* [0..1][lock(:pgr_ns->[pn_par...]->pn_lock)]
	                                  * Link entry within the associated PID namespace */
	upid_t                 pgs_pid;  /* [const] PID relevant to this slot. (s.a. `__TASKPID_SLOT_*') */
};

#define _procgrp_slot_getpidno(self) ((pid_t)((self).pgs_pid & __TASKPID_SLOT_PIDMASK))

struct procgrp {
	WEAK refcnt_t                                pgr_refcnt;    /* Reference counter. */
#ifdef __WANT_PROCGRP__pgr_lop
	union {
		struct {
			REF struct procgrp                  *pgr_sleader;   /* [1..1][ref_if(!= this)][const] Session leader process group. */
			struct procsession                  *pgr_session;   /* [1..1][owned_if(pgr_sleader == this)][const] Session data. */
		};
		Toblockop(pidns)                        _pgr_lop;       /* Used internally during destruction */
		Tobpostlockop(pidns)                    _pgr_plop;      /* Used internally during destruction */
	};
#else /* __WANT_PROCGRP__pgr_lop */
	REF struct procgrp                          *pgr_sleader;   /* [1..1][ref_if(!= this)][const] Session leader process group. */
	struct procsession                          *pgr_session;   /* [1..1][owned_if(pgr_sleader == this)][const] Session data. */
#endif /* !__WANT_PROCGRP__pgr_lop */
	Toblockop_slist(procgrp)                     pgr_memb_lops; /* Lock-ops for `pgr_memb_lock' */
	struct atomic_rwlock                         pgr_memb_lock; /* Lock for `pgr_memb_list' */
	struct WEAK taskpid_list                     pgr_memb_list; /* [0..n][LINK(tp_pctl->pc_grpmember)][lock(pgr_memb_lock)]
	                                                             * List of processes apart of this group (including the group leader).
	                                                             * Members of  this list  automatically remove  themselves upon  being
	                                                             * destroyed (aka. inside of `taskpid_destroy()'). */
	REF struct pidns                            *pgr_ns;        /* [1..1][const] Top-level PID namespace containing this descriptor. */
	COMPILER_FLEXIBLE_ARRAY(struct procgrp_slot, pgr_pids);     /* [const][tp_ns->pn_ind+1] Task PID value from different namespaces. */
};

/* Destroy the given procgrp. */
FUNDEF NOBLOCK NONNULL((1)) void
NOTHROW(FCALL procgrp_destroy)(struct procgrp *__restrict self);
DEFINE_REFCOUNT_FUNCTIONS(struct procgrp, pgr_refcnt, procgrp_destroy)


/* Allocate/Free a `struct procgrp' */
#define _procgrp_sizeof(ns) \
	(__builtin_offsetof(struct procgrp, pgr_pids) + ((ns)->pn_ind + 1) * sizeof(struct procgrp_slot))
#define _procgrp_alloc(ns) ((struct procgrp *)kmalloc(_procgrp_sizeof(ns), GFP_NORMAL))
#define _procgrp_free(self) kfree(self)

/* Helper macros for working with `struct procgrp::pgr_memb_lock' */
#define procgrp_memb_mustreap(self)     oblockop_mustreap(&(self)->pgr_memb_lops)
#define _procgrp_memb_reap(self)        _oblockop_reap_atomic_rwlock(&(self)->pgr_memb_lops, &(self)->pgr_memb_lock, self)
#define procgrp_memb_reap(self)         oblockop_reap_atomic_rwlock(&(self)->pgr_memb_lops, &(self)->pgr_memb_lock, self)
#define procgrp_memb_write(self)        atomic_rwlock_write(&(self)->pgr_memb_lock)
#define procgrp_memb_write_nx(self)     atomic_rwlock_write_nx(&(self)->pgr_memb_lock)
#define procgrp_memb_trywrite(self)     atomic_rwlock_trywrite(&(self)->pgr_memb_lock)
#define procgrp_memb_endwrite(self)     (atomic_rwlock_endwrite(&(self)->pgr_memb_lock), procgrp_memb_reap(self))
#define _procgrp_memb_endwrite(self)    atomic_rwlock_endwrite(&(self)->pgr_memb_lock)
#define procgrp_memb_read(self)         atomic_rwlock_read(&(self)->pgr_memb_lock)
#define procgrp_memb_read_nx(self)      atomic_rwlock_read_nx(&(self)->pgr_memb_lock)
#define procgrp_memb_tryread(self)      atomic_rwlock_tryread(&(self)->pgr_memb_lock)
#define _procgrp_memb_endread(self)     atomic_rwlock_endread(&(self)->pgr_memb_lock)
#define procgrp_memb_endread(self)      (void)(atomic_rwlock_endread(&(self)->pgr_memb_lock) && (procgrp_memb_reap(self), 0))
#define _procgrp_memb_end(self)         atomic_rwlock_end(&(self)->pgr_memb_lock)
#define procgrp_memb_end(self)          (void)(atomic_rwlock_end(&(self)->pgr_memb_lock) && (procgrp_memb_reap(self), 0))
#define procgrp_memb_upgrade(self)      atomic_rwlock_upgrade(&(self)->pgr_memb_lock)
#define procgrp_memb_upgrade_nx(self)   atomic_rwlock_upgrade_nx(&(self)->pgr_memb_lock)
#define procgrp_memb_tryupgrade(self)   atomic_rwlock_tryupgrade(&(self)->pgr_memb_lock)
#define procgrp_memb_downgrade(self)    atomic_rwlock_downgrade(&(self)->pgr_memb_lock)
#define procgrp_memb_reading(self)      atomic_rwlock_reading(&(self)->pgr_memb_lock)
#define procgrp_memb_writing(self)      atomic_rwlock_writing(&(self)->pgr_memb_lock)
#define procgrp_memb_canread(self)      atomic_rwlock_canread(&(self)->pgr_memb_lock)
#define procgrp_memb_canwrite(self)     atomic_rwlock_canwrite(&(self)->pgr_memb_lock)
#define procgrp_memb_waitread(self)     atomic_rwlock_waitread(&(self)->pgr_memb_lock)
#define procgrp_memb_waitwrite(self)    atomic_rwlock_waitwrite(&(self)->pgr_memb_lock)
#define procgrp_memb_waitread_nx(self)  atomic_rwlock_waitread_nx(&(self)->pgr_memb_lock)
#define procgrp_memb_waitwrite_nx(self) atomic_rwlock_waitwrite_nx(&(self)->pgr_memb_lock)

/* Helper macros for adding/removing elements from `struct procgrp::pgr_memb_list'
 * NOTE: For all of these, the caller must be holding a write-lock to `self->pgr_memb_lock' */
#define __procgrp_memb_assert(self, member_struct_taskpid)                    \
	(__hybrid_assertf(taskpid_isaprocess(member_struct_taskpid),              \
	                  "Not a process leader")/*, vvv: cannot assert -- race   \
	 __hybrid_assertf(taskpid_getprocgrpptr(member_struct_taskpid) == (self), \
	                  "Not set-up as a member of this process group")*/)
#define procgrp_memb_insert(self, struct_taskpid_to_insert) \
	(__procgrp_memb_assert(self, struct_taskpid_to_insert), \
	 LIST_INSERT_HEAD(&(self)->pgr_memb_list, struct_taskpid_to_insert, tp_pctl->pc_grpmember))
#define procgrp_memb_remove(self, struct_taskpid_to_remove) \
	(__procgrp_memb_assert(self, struct_taskpid_to_remove), \
	 LIST_REMOVE(struct_taskpid_to_remove, tp_pctl->pc_grpmember))



/* Destroy the given process group. */
FUNDEF NOBLOCK NONNULL((1)) void
NOTHROW(FCALL procgrp_destroy)(struct procgrp *__restrict self);
DEFINE_REFCOUNT_FUNCTIONS(struct procgrp, pgr_refcnt, procgrp_destroy)

/* Return information regarding the session associated with a given process group. */
#define procgrp_getsessionleader(self) (self)->pgr_sleader
#define procgrp_issessionleader(self)  ((self) == procgrp_getsessionleader(self))
#define procgrp_getsession(self)       procgrp_getsessionleader(self)->pgr_session

/* Return the # of PIDs defined by `self' */
#define procgrp_getpgidcount(self) ((self)->pgr_ns->pn_ind + 1)

/* Return `self's PID number associated with `ns' */
#ifdef NDEBUG
#define procgrp_getnspgid(self, ns) \
	_procgrp_slot_getpidno((self)->pgr_pids[(ns)->pn_ind])
#else /* NDEBUG */
#define procgrp_getnspgid(self, ns)                                        \
	({                                                                     \
		struct procgrp const *__pggp_self = (self);                        \
		struct pidns const *__pggp_ns     = (ns);                          \
		__hybrid_assert(__pggp_self->pgr_ns->pn_ind >= __pggp_ns->pn_ind); \
		_procgrp_slot_getpidno(__pggp_self->pgr_pids[__pggp_ns->pn_ind]);  \
	})
#endif /* !NDEBUG */

/* Same as `procgrp_getnspgid()', but return `0' if `self' doesn't appear in `ns' */
#define procgrp_getnspgid_s(self, ns)                                         \
	({                                                                        \
		struct procgrp const *__pggps_self = (self);                          \
		struct pidns const *__tpgps_ns     = (ns);                            \
		(likely(__pggps_self->pgr_ns->pn_ind >= __tpgps_ns->pn_ind)           \
		 ? _procgrp_slot_getpidno(__pggps_self->pgr_pids[__tpgps_ns->pn_ind]) \
		 : 0);                                                                \
	})
#define procgrp_getpgid(self)   procgrp_getnspgid(self, THIS_PIDNS)
#define procgrp_getpgid_s(self) procgrp_getnspgid_s(self, THIS_PIDNS)

/* Return `self's PID number within the root namespace */
#define procgrp_getrootpgid(self) _procgrp_slot_getpidno((self)->pgr_pids[0])

/* Query the session ID of a given process group */
#define procgrp_getnssid(self, ns)   procgrp_getnspgid(procgrp_getsessionleader(self), ns)
#define procgrp_getnssid_s(self, ns) procgrp_getnspgid_s(procgrp_getsessionleader(self), ns)
#define procgrp_getsid(self)         procgrp_getpgid(procgrp_getsessionleader(self))
#define procgrp_getsid_s(self)       procgrp_getpgid_s(procgrp_getsessionleader(self))
#define procgrp_getrootsid(self)     procgrp_getrootpgid(procgrp_getsessionleader(self))




#ifndef __procgrp_arref_defined
#define __procgrp_arref_defined
ARREF(procgrp_arref, procgrp);
#endif /* !__procgrp_arref_defined */

struct procctl {
	struct atomic_rwlock     pc_chlds_lock;   /* Lock for `pc_chlds_list' */
	struct REF taskpid_list  pc_chlds_list;   /* [0..n][lock(pc_chlds_lock)][link(tp_parsib)]
	                                           * Child  tasks of this process. This includes both
	                                           * threads  (other than the main thread) within the
	                                           * process,  as well as  processes that were fork'd
	                                           * from this one (or re-parented to this one). Note
	                                           * that threads with the `TASK_FDETACHED' flag will
	                                           * automatically remove themselves  from this  list
	                                           * during `task_exit()'. Child  processes don't  do
	                                           * this  and must instead  be removed with wait(2),
	                                           * as can  also be  done for  threads (that  aren't
	                                           * detached)
	                                           * NOTE: Elements may only be added to this list for
	                                           *       as long as this process's main thread isn't
	                                           *       marked as `TASK_FTERMINATING'! */
	struct sig               pc_chld_changed; /* Broadcast when one of `pc_chlds_list' changes
	                                           * status.  (s.a.  `struct taskpid::tp_changed') */
	struct task_arref        pc_parent;       /* [1..1][lock(READ(ATOMIC), WRITE(OLD->pc_chlds_lock &&
	                                           *                                 NEW->pc_chlds_lock))]
	                                           * Parent process. When the parent thread exits, it will
	                                           * re-parent  all of its remaining children to boottask.
	                                           * NOTE: You may assume that `task_isaprocess(pc_parent)' */
	struct atomic_rwlock     pc_sig_lock;     /* Lock for `pc_sig_list'. */
	struct pending_rpc_slist pc_sig_list;     /* [0..n][lock(APPEND(ATOMIC), REMOVE(pc_sig_lock))]
	                                           * List of pending RPCs directed at the process as a whole. When
	                                           * set  to `THIS_RPCS_TERMINATED', the  process is considered as
	                                           * terminated and no additional RPCs can be enqueued.
	                                           * NOTE: User-RPCs must not have the `RPC_SYNCMODE_F_REQUIRE_SC'
	                                           *       or `RPC_SYNCMODE_F_REQUIRE_CP'  flag set.  If they  do,
	                                           *       an internal assertion check will trigger. */
	struct sig               pc_sig_more;     /* A signal that is broadcast whenever something is added to `pc_sig_list'
	                                           * This  signal is _only_  used to implement  `signalfd(2)', as you're not
	                                           * normally supposed to "wait" for signals to arrive; you just always  get
	                                           * a sporadic interrupt once they do arrive. */
	struct procgrp_arref     pc_grp;          /* [1..1][lock(READ(ATOMIC), WRITE(OLD->pgr_memb_lock &&
	                                           *                                 NEW->pgr_memb_lock))]
	                                           * Process group controller. */
	LIST_ENTRY(taskpid)      pc_grpmember;    /* [0..1][lock(pc_grp->pgr_memb_lock)] Process group member link. */
};

/* Allocate/Free a `struct procctl' */
#define _procctl_alloc()    ((struct procctl *)kmalloc(sizeof(struct procctl), GFP_NORMAL))
#define _procctl_free(self) kfree(self)

/* Helper macros for working with `struct procgrp::pc_chlds_lock' */
#define procctl_chlds_mustreap(self)     0
#define _procctl_chlds_reap(self)        (void)0
#define procctl_chlds_reap(self)         (void)0
#define procctl_chlds_write(self)        atomic_rwlock_write(&(self)->pc_chlds_lock)
#define procctl_chlds_write_nx(self)     atomic_rwlock_write_nx(&(self)->pc_chlds_lock)
#define procctl_chlds_trywrite(self)     atomic_rwlock_trywrite(&(self)->pc_chlds_lock)
#define procctl_chlds_endwrite(self)     (atomic_rwlock_endwrite(&(self)->pc_chlds_lock), procctl_chlds_reap(self))
#define _procctl_chlds_endwrite(self)    atomic_rwlock_endwrite(&(self)->pc_chlds_lock)
#define procctl_chlds_read(self)         atomic_rwlock_read(&(self)->pc_chlds_lock)
#define procctl_chlds_read_nx(self)      atomic_rwlock_read_nx(&(self)->pc_chlds_lock)
#define procctl_chlds_tryread(self)      atomic_rwlock_tryread(&(self)->pc_chlds_lock)
#define _procctl_chlds_endread(self)     atomic_rwlock_endread(&(self)->pc_chlds_lock)
#define procctl_chlds_endread(self)      (void)(atomic_rwlock_endread(&(self)->pc_chlds_lock) && (procctl_chlds_reap(self), 0))
#define _procctl_chlds_end(self)         atomic_rwlock_end(&(self)->pc_chlds_lock)
#define procctl_chlds_end(self)          (void)(atomic_rwlock_end(&(self)->pc_chlds_lock) && (procctl_chlds_reap(self), 0))
#define procctl_chlds_upgrade(self)      atomic_rwlock_upgrade(&(self)->pc_chlds_lock)
#define procctl_chlds_upgrade_nx(self)   atomic_rwlock_upgrade_nx(&(self)->pc_chlds_lock)
#define procctl_chlds_tryupgrade(self)   atomic_rwlock_tryupgrade(&(self)->pc_chlds_lock)
#define procctl_chlds_downgrade(self)    atomic_rwlock_downgrade(&(self)->pc_chlds_lock)
#define procctl_chlds_reading(self)      atomic_rwlock_reading(&(self)->pc_chlds_lock)
#define procctl_chlds_writing(self)      atomic_rwlock_writing(&(self)->pc_chlds_lock)
#define procctl_chlds_canread(self)      atomic_rwlock_canread(&(self)->pc_chlds_lock)
#define procctl_chlds_canwrite(self)     atomic_rwlock_canwrite(&(self)->pc_chlds_lock)
#define procctl_chlds_waitread(self)     atomic_rwlock_waitread(&(self)->pc_chlds_lock)
#define procctl_chlds_waitwrite(self)    atomic_rwlock_waitwrite(&(self)->pc_chlds_lock)
#define procctl_chlds_waitread_nx(self)  atomic_rwlock_waitread_nx(&(self)->pc_chlds_lock)
#define procctl_chlds_waitwrite_nx(self) atomic_rwlock_waitwrite_nx(&(self)->pc_chlds_lock)

/* Helper macros for adding/removing elements from `struct procctl::pc_chlds_list'
 * NOTE: For all of these, the caller must be holding a write-lock to `self->pc_chlds_lock' */
#define procctl_chlds_insert(self, /*IN_REF*/ struct_taskpid_to_insert) \
	LIST_INSERT_HEAD(&(self)->pc_chlds_list, struct_taskpid_to_insert, tp_parsib)
#define procctl_chlds_remove(self, /*OUT_REF*/ struct_taskpid_to_remove) \
	LIST_REMOVE(struct_taskpid_to_remove, tp_parsib)
#define procctl_chlds_unbind(self, /*OUT_REF*/ struct_taskpid_to_unbind) \
	LIST_UNBIND(struct_taskpid_to_unbind, tp_parsib)

/* Helper macros for working with `struct procgrp::pc_sig_lock' */
#define procctl_sig_mustreap(self)     0
#define _procctl_sig_reap(self)        (void)0
#define procctl_sig_reap(self)         (void)0
#define procctl_sig_write(self)        atomic_rwlock_write(&(self)->pc_sig_lock)
#define procctl_sig_write_nx(self)     atomic_rwlock_write_nx(&(self)->pc_sig_lock)
#define procctl_sig_trywrite(self)     atomic_rwlock_trywrite(&(self)->pc_sig_lock)
#define procctl_sig_endwrite(self)     (atomic_rwlock_endwrite(&(self)->pc_sig_lock), procctl_sig_reap(self))
#define _procctl_sig_endwrite(self)    atomic_rwlock_endwrite(&(self)->pc_sig_lock)
#define procctl_sig_read(self)         atomic_rwlock_read(&(self)->pc_sig_lock)
#define procctl_sig_read_nx(self)      atomic_rwlock_read_nx(&(self)->pc_sig_lock)
#define procctl_sig_tryread(self)      atomic_rwlock_tryread(&(self)->pc_sig_lock)
#define _procctl_sig_endread(self)     atomic_rwlock_endread(&(self)->pc_sig_lock)
#define procctl_sig_endread(self)      (void)(atomic_rwlock_endread(&(self)->pc_sig_lock) && (procctl_sig_reap(self), 0))
#define _procctl_sig_end(self)         atomic_rwlock_end(&(self)->pc_sig_lock)
#define procctl_sig_end(self)          (void)(atomic_rwlock_end(&(self)->pc_sig_lock) && (procctl_sig_reap(self), 0))
#define procctl_sig_upgrade(self)      atomic_rwlock_upgrade(&(self)->pc_sig_lock)
#define procctl_sig_upgrade_nx(self)   atomic_rwlock_upgrade_nx(&(self)->pc_sig_lock)
#define procctl_sig_tryupgrade(self)   atomic_rwlock_tryupgrade(&(self)->pc_sig_lock)
#define procctl_sig_downgrade(self)    atomic_rwlock_downgrade(&(self)->pc_sig_lock)
#define procctl_sig_reading(self)      atomic_rwlock_reading(&(self)->pc_sig_lock)
#define procctl_sig_writing(self)      atomic_rwlock_writing(&(self)->pc_sig_lock)
#define procctl_sig_canread(self)      atomic_rwlock_canread(&(self)->pc_sig_lock)
#define procctl_sig_canwrite(self)     atomic_rwlock_canwrite(&(self)->pc_sig_lock)
#define procctl_sig_waitread(self)     atomic_rwlock_waitread(&(self)->pc_sig_lock)
#define procctl_sig_waitwrite(self)    atomic_rwlock_waitwrite(&(self)->pc_sig_lock)
#define procctl_sig_waitread_nx(self)  atomic_rwlock_waitread_nx(&(self)->pc_sig_lock)
#define procctl_sig_waitwrite_nx(self) atomic_rwlock_waitwrite_nx(&(self)->pc_sig_lock)

/* Helper macros for working with `struct procgrp::pc_sig_list' */
#define procctl_sig_insert(self, struct_pending_rpc_to_add)                                   \
	(__hybrid_assertf(!((struct_pending_rpc_to_add)->pr_flags & (RPC_SYNCMODE_F_REQUIRE_SC |  \
	                                                             RPC_SYNCMODE_F_REQUIRE_CP)), \
	                  "These flags cannot be used for process-directed signals/RPCs"),        \
	 SLIST_ATOMIC_INSERT(&(self)->pc_sig_list, struct_pending_rpc_to_add, pr_link))
#define procctl_sig_insert_r(self, lo_elem, hi_elem) \
	SLIST_ATOMIC_INSERT_R(&(self)->pc_sig_list, lo_elem, hi_elem, pr_link)
/* NOTE: For `procctl_sig_clear()', the caller must be holding a write-lock to `pc_sig_list'! */
#define procctl_sig_clear(self) SLIST_ATOMIC_CLEAR(&(self)->pc_sig_list)

/* Return a reference to the parent-process of the given element. */
#define procctl_getparentprocess(self)   arref_get(&(self)->pc_parent)                        /* REF struct task * [1..1] */
#define taskpid_getparentprocess(self)   procctl_getparentprocess(taskpid_getprocctl(self))   /* REF struct task * [0..1] */
#define task_getparentprocess()          procctl_getparentprocess(task_getprocctl())          /* REF struct task * [0..1] */
#define task_getparentprocess_of(thread) procctl_getparentprocess(task_getprocctl_of(thread)) /* REF struct task * [0..1] */

#define procctl_getparentprocessptr(self)   arref_ptr(&(self)->pc_parent)
#define taskpid_getparentprocessptr(self)   procctl_getparentprocessptr(taskpid_getprocctl(self))
#define task_getparentprocessptr()          procctl_getparentprocessptr(task_getprocctl())
#define task_getparentprocessptr_of(thread) procctl_getparentprocessptr(task_getprocctl_of(thread))


/* Session used by `procctl_boottask' (and consequently also by `/bin/init') */
DATDEF struct procsession boottask_procsession;

/* Special process group with `pgid == 0'  used for kernel threads (and  /bin/init)
 * Because we use  `pgid == 0', `setpgid(2)'  cannot be  used to  join this  group,
 * since passing it `0' as the target process group ID is an alias for `getpid(2)'. */
DATDEF struct procgrp boottask_procgrp;

/* Process controller for `boottask' (aka. `/bin/init') */
DATDEF struct procctl boottask_procctl;


/* Return a reference to the process group to which element belongs. */
#define procctl_getprocgrp(self) /* >> REF struct procgrp * [1..1] */ \
	({ struct procctl *__pcgpg_self = (self); arref_get(&__pcgpg_self->pc_grp); })
#define taskpid_getprocgrp(self)      procctl_getprocgrp(taskpid_getprocctl(self))   /* >> REF struct procgrp * [1..1] */
#define task_getprocgrp()             procctl_getprocgrp(task_getprocctl())          /* >> REF struct procgrp * [1..1] */
#define task_getprocgrp_of(thread)    procctl_getprocgrp(task_getprocctl_of(thread)) /* >> REF struct procgrp * [1..1] */
#define procctl_getprocgrpptr(self)   arref_ptr(&(self)->pc_grp)
#define taskpid_getprocgrpptr(self)   procctl_getprocgrpptr(taskpid_getprocctl(self))
#define task_getprocgrpptr()          procctl_getprocgrpptr(task_getprocctl())
#define task_getprocgrpptr_of(thread) procctl_getprocgrpptr(task_getprocctl_of(thread))


#define __task_getpgid(inherted_grp_ref, how)                 \
	({                                                        \
		REF struct procgrp *__tgpgi_grp = (inherted_grp_ref); \
		pid_t __tgpgi_res               = how;                \
		decref_unlikely(__tgpgi_grp);                         \
		__tgpgi_res;                                          \
	})
#define __task_getmypgid(how)                                                \
	({                                                                       \
		struct taskpid *__tgmpgi_pid     = task_gettaskpid();                \
		REF struct procgrp *__tgmpgi_grp = taskpid_getprocgrp(__tgmpgi_pid); \
		pid_t __tgmpgi_res               = how;                              \
		decref_unlikely(__tgmpgi_grp);                                       \
		__tgmpgi_res;                                                        \
	})

/* Return process group ID of a given `struct procctl' or `struct taskpid'. */
#define procctl_getpgid(self)         __task_getpgid(procctl_getprocgrp(self), procgrp_getpgid(__tgpgi_grp))         /* Return PGID of given `struct procctl' (in caller's namespace; panic/undefined if not mapped) */
#define procctl_getpgid_s(self)       __task_getpgid(procctl_getprocgrp(self), procgrp_getpgid_s(__tgpgi_grp))       /* Return PGID of given `struct procctl' (in caller's namespace; `0' if not mapped) */
#define procctl_getrootpgid(self)     __task_getpgid(procctl_getprocgrp(self), procgrp_getrootpgid(__tgpgi_grp))     /* Return PGID of given `struct procctl' (in root namespace) */
#define procctl_getnspgid(self, ns)   __task_getpgid(procctl_getprocgrp(self), procgrp_getnspgid(__tgpgi_grp, ns))   /* Return PGID of given `struct procctl' (in caller's namespace; panic/undefined if not mapped) */
#define procctl_getnspgid_s(self, ns) __task_getpgid(procctl_getprocgrp(self), procgrp_getnspgid_s(__tgpgi_grp, ns)) /* Return PGID of given `struct procctl' (in caller's namespace; `0' if not mapped) */
#define taskpid_getpgid(self)         procctl_getpgid(taskpid_getprocctl(self))                                       /* Return PGID of given `struct taskpid' (in caller's namespace; panic/undefined if not mapped) */
#define taskpid_getpgid_s(self)       procctl_getpgid_s(taskpid_getprocctl(self))                                     /* Return PGID of given `struct taskpid' (in caller's namespace; `0' if not mapped) */
#define taskpid_getrootpgid(self)     procctl_getrootpgid(taskpid_getprocctl(self))                                   /* Return PGID of given `struct taskpid' (in root namespace) */
#define taskpid_getnspgid(self, ns)   procctl_getnspgid(taskpid_getprocctl(self), ns)                                 /* Return PGID of given `struct taskpid' (in given namespace; panic/undefined if not mapped) */
#define taskpid_getnspgid_s(self, ns) procctl_getnspgid_s(taskpid_getprocctl(self), ns)                               /* Return PGID of given `struct taskpid' (in given namespace; `0' if not mapped) */

/* Return process group ID of the calling/given thread. */
#define task_getpgid()                  __task_getmypgid(procgrp_getnspgid(__tgmpgi_grp, __tgmpgi_pid->tp_ns))   /* Return PGID of calling thread (in caller's namespace; panic/undefined if not mapped) */
#define task_getpgid_s()                __task_getmypgid(procgrp_getnspgid_s(__tgmpgi_grp, __tgmpgi_pid->tp_ns)) /* Return PGID of calling thread (in caller's namespace; `0' if not mapped) */
#define task_getpgid_of(thread)         taskpid_getpgid(task_gettaskpid_of(thread))                               /* Return PGID of given thread (in caller's namespace; panic/undefined if not mapped) */
#define task_getpgid_of_s(thread)       taskpid_getpgid_s(task_gettaskpid_of(thread))                             /* Return PGID of given thread (in caller's namespace; `0' if not mapped) */
#define task_getrootpgid()              taskpid_getrootpgid(task_gettaskpid())                                    /* Return PGID of calling thread (in root namespace) */
#define task_getrootpgid_of(thread)     taskpid_getrootpgid(task_gettaskpid_of(thread))                           /* Return PGID of given thread (in root namespace) */
#define task_getnspgid(ns)              taskpid_getnspgid(task_gettaskpid(), ns)                                  /* Return PGID of calling thread (in given namespace; panic/undefined if not mapped) */
#define task_getnspgid_s(ns)            taskpid_getnspgid_s(task_gettaskpid(), ns)                                /* Return PGID of calling thread (in given namespace; `0' if not mapped) */
#define task_getnspgid_of(thread, ns)   taskpid_getnspgid(task_gettaskpid_of(thread), ns)                         /* Return PGID of given thread (in given namespace; panic/undefined if not mapped) */
#define task_getnspgid_of_s(thread, ns) taskpid_getnspgid_s(task_gettaskpid_of(thread), ns)                       /* Return PGID of given thread (in given namespace; `0' if not mapped) */

/* Return session ID of a given `struct procctl' or `struct taskpid'. */
#define procctl_getsid(self)         __task_getpgid(procctl_getprocgrp(self), procgrp_getsid(__tgpgi_grp))         /* Return SID of given `struct procctl' (in caller's namespace; panic/undefined if not mapped) */
#define procctl_getsid_s(self)       __task_getpgid(procctl_getprocgrp(self), procgrp_getsid_s(__tgpgi_grp))       /* Return SID of given `struct procctl' (in caller's namespace; `0' if not mapped) */
#define procctl_getrootsid(self)     __task_getpgid(procctl_getprocgrp(self), procgrp_getrootsid(__tgpgi_grp))     /* Return SID of given `struct procctl' (in root namespace) */
#define procctl_getnssid(self, ns)   __task_getpgid(procctl_getprocgrp(self), procgrp_getnssid(__tgpgi_grp, ns))   /* Return SID of given `struct procctl' (in caller's namespace; panic/undefined if not mapped) */
#define procctl_getnssid_s(self, ns) __task_getpgid(procctl_getprocgrp(self), procgrp_getnssid_s(__tgpgi_grp, ns)) /* Return SID of given `struct procctl' (in caller's namespace; `0' if not mapped) */
#define taskpid_getsid(self)         procctl_getsid(taskpid_getprocctl(self))                                        /* Return SID of given `struct taskpid' (in caller's namespace; panic/undefined if not mapped) */
#define taskpid_getsid_s(self)       procctl_getsid_s(taskpid_getprocctl(self))                                      /* Return SID of given `struct taskpid' (in caller's namespace; `0' if not mapped) */
#define taskpid_getrootsid(self)     procctl_getrootsid(taskpid_getprocctl(self))                                    /* Return SID of given `struct taskpid' (in root namespace) */
#define taskpid_getnssid(self, ns)   procctl_getnssid(taskpid_getprocctl(self), ns)                                  /* Return SID of given `struct taskpid' (in given namespace; panic/undefined if not mapped) */
#define taskpid_getnssid_s(self, ns) procctl_getnssid_s(taskpid_getprocctl(self), ns)                                /* Return SID of given `struct taskpid' (in given namespace; `0' if not mapped) */

/* Return session ID of the calling/given thread. */
#define task_getsid()                  __task_getmypgid(procgrp_getnssid(__tgmpgi_grp, __tgmpgi_pid->tp_ns))   /* Return SID of calling thread (in caller's namespace; panic/undefined if not mapped) */
#define task_getsid_s()                __task_getmypgid(procgrp_getnssid_s(__tgmpgi_grp, __tgmpgi_pid->tp_ns)) /* Return SID of calling thread (in caller's namespace; `0' if not mapped) */
#define task_getsid_of(thread)         taskpid_getsid(task_gettaskpid_of(thread))                                /* Return SID of given thread (in caller's namespace; panic/undefined if not mapped) */
#define task_getsid_of_s(thread)       taskpid_getsid_s(task_gettaskpid_of(thread))                              /* Return SID of given thread (in caller's namespace; `0' if not mapped) */
#define task_getrootsid()              taskpid_getrootsid(task_gettaskpid())                                     /* Return SID of calling thread (in root namespace) */
#define task_getrootsid_of(thread)     taskpid_getrootsid(task_gettaskpid_of(thread))                            /* Return SID of given thread (in root namespace) */
#define task_getnssid(ns)              taskpid_getnssid(task_gettaskpid(), ns)                                   /* Return SID of calling thread (in given namespace; panic/undefined if not mapped) */
#define task_getnssid_s(ns)            taskpid_getnssid_s(task_gettaskpid(), ns)                                 /* Return SID of calling thread (in given namespace; `0' if not mapped) */
#define task_getnssid_of(thread, ns)   taskpid_getnssid(task_gettaskpid_of(thread), ns)                          /* Return SID of given thread (in given namespace; panic/undefined if not mapped) */
#define task_getnssid_of_s(thread, ns) taskpid_getnssid_s(task_gettaskpid_of(thread), ns)                        /* Return SID of given thread (in given namespace; `0' if not mapped) */




#define __task_getppid(inherted_task, how)              \
	({                                                  \
		REF struct task *__tgpp_task = (inherted_task); \
		pid_t __tgpp_res             = how;             \
		decref_unlikely(__tgpp_task);                   \
		__tgpp_res;                                     \
	})
#define __task_getmyppid(how)                                                  \
	({                                                                         \
		struct taskpid *__tgmpp_pid   = task_gettaskpid();                     \
		REF struct task *__tgmpp_task = taskpid_getparentprocess(__tgmpp_pid); \
		pid_t __tgpp_res              = how;                                   \
		decref_unlikely(__tgmpp_task);                                         \
		__tgpp_res;                                                            \
	})

/* Return parent process ID of a given `struct procctl' or `struct taskpid'. */
#define procctl_getppid(self)         __task_getppid(procctl_getparentprocess(self), task_gettid_of(__tgpp_task))         /* Return parent PID of given `struct procctl' (in caller's namespace; panic/undefined if not mapped) */
#define procctl_getppid_s(self)       __task_getppid(procctl_getparentprocess(self), task_gettid_of_s(__tgpp_task))       /* Return parent PID of given `struct procctl' (in caller's namespace; `0' if not mapped) */
#define procctl_getrootppid(self)     __task_getppid(procctl_getparentprocess(self), task_getroottid_of(__tgpp_task))     /* Return parent PID of given `struct procctl' (in root namespace) */
#define procctl_getnsppid(self, ns)   __task_getppid(procctl_getparentprocess(self), task_getnstid_of(__tgpp_task, ns))   /* Return parent PID of given `struct procctl' (in given namespace; panic/undefined if not mapped) */
#define procctl_getnsppid_s(self, ns) __task_getppid(procctl_getparentprocess(self), task_getnstid_of_s(__tgpp_task, ns)) /* Return parent PID of given `struct procctl' (in given namespace; `0' if not mapped) */
#define taskpid_getppid(self)         procctl_getppid(taskpid_getprocctl(self))                                           /* Return parent PID of given `struct taskpid' (in caller's namespace; panic/undefined if not mapped) */
#define taskpid_getppid_s(self)       procctl_getppid_s(taskpid_getprocctl(self))                                         /* Return parent PID of given `struct taskpid' (in caller's namespace; `0' if not mapped) */
#define taskpid_getrootppid(self)     procctl_getrootppid(taskpid_getprocctl(self))                                       /* Return parent PID of given `struct taskpid' (in root namespace) */
#define taskpid_getnsppid(self, ns)   procctl_getnsppid(taskpid_getprocctl(self), ns)                                     /* Return parent PID of given `struct taskpid' (in given namespace; panic/undefined if not mapped) */
#define taskpid_getnsppid_s(self, ns) procctl_getnsppid_s(taskpid_getprocctl(self), ns)                                   /* Return parent PID of given `struct taskpid' (in given namespace; `0' if not mapped) */

/* Return parent process ID of the calling/given thread. */
#define task_getppid()                __task_getmyppid(task_getnstid_of(__tgmpp_task, __tgmpp_pid->tp_ns))   /* Return parent PID of calling thread (in caller's namespace; panic/undefined if not mapped) */
#define task_getppid_s()              __task_getmyppid(task_getnstid_of_s(__tgmpp_task, __tgmpp_pid->tp_ns)) /* Return parent PID of calling thread (in caller's namespace; `0' if not mapped) */
#define task_getppid_of(self)         taskpid_getppid(task_gettaskpid_of(self))                              /* Return parent PID of given thread (in caller's namespace; panic/undefined if not mapped) */
#define task_getppid_of_s(self)       taskpid_getppid_s(task_gettaskpid_of(self))                            /* Return parent PID of given thread (in caller's namespace; `0' if not mapped) */
#define task_getrootppid()            taskpid_getrootppid(task_gettaskpid())                                 /* Return parent PID of calling thread (in root namespace) */
#define task_getrootppid_of(self)     taskpid_getrootppid(task_gettaskpid_of(self))                          /* Return parent PID of given thread (in root namespace) */
#define task_getnsppid(ns)            taskpid_getnsppid(task_gettaskpid(), ns)                               /* Return parent PID of calling thread (in given namespace; panic/undefined if not mapped) */
#define task_getnsppid_s(ns)          taskpid_getnsppid_s(task_gettaskpid(), ns)                             /* Return parent PID of calling thread (in given namespace; `0' if not mapped) */
#define task_getnsppid_of(self, ns)   taskpid_getnsppid(task_gettaskpid_of(self), ns)                        /* Return parent PID of given thread (in given namespace; panic/undefined if not mapped) */
#define task_getnsppid_of_s(self, ns) taskpid_getnsppid_s(task_gettaskpid_of(self), ns)                      /* Return parent PID of given thread (in given namespace; `0' if not mapped) */


DECL_END
#endif /* __CC__ */
#endif /* CONFIG_USE_NEW_GROUP */

#endif /* !GUARD_KERNEL_INCLUDE_SCHED_GROUP_NEW_H */