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
%[default:section(".text.crt{|.dos}.sched.lockop")]

%[insert:prefix(
#include <bits/types.h>
)]%[insert:prefix(
#include <hybrid/__atomic.h>
)]%[insert:prefix(
#include <kos/anno.h>
)]%[insert:prefix(
#include <kos/bits/lockop.h>
)]%{

#ifdef __cplusplus
#include <__stdcxx.h>
#endif /* __cplusplus */


#ifdef __CC__
__SYSDECL_BEGIN

/* Helper  structures to implement  general-purpose lock operation handling.
 * The basic idea here is to make it possible to enqueue a set of operations
 * that will be performed asynchronously once some kind of lock becomes
 * available:
 *
 * >> void _service(void);
 * >> #define must_service() ...
 * >> #define service() (void)(!must_service() || (_service(), 0))
 * >> #define acquire() acquire_lock(&lock)
 * >> #define release() (release_lock(&lock), service())
 * >>
 * >> ...
 * >>
 * >> void _service(void) {
 * >>     do {
 * >>         if (!try_acquire())
 * >>             break;
 * >>         SERVICE_LOCK_OPS();
 * >>         release_lock(&lock);
 * >>         SERVICE_POST_LOCK_OPS();
 * >>     } while (must_service());
 * >> }
 *
 * This kind of implementation is  sequentially consistent in that it  can
 * guaranty the execution of all  enqueued lock operations "at some  point
 * in  the future", so-long  as the user of  the locking functionality can
 * guaranty that locks  are not  held forever, but  are _always_  released
 * eventually. As such, the lockop mechanism works best with atomic locks,
 * since  the eventual release of an atomic lock following its acquisition
 * is  one of  the logical  consequences of  their semantical requirements
 * regarding the  non-blocking-ness of  anything  performed while  such  a
 * lock is held.
 *
 * Furthermore, lock operations can be used to perform otherwise  blocking
 * operations in  the context  of certain  locks being  held, making  them
 * perfect for augmenting and facilitating the non-blocking-ness of object
 * finalizers that need to acquire locks to other data structures, such as
 * lists,  in order  to remove (part  of) the object  being destroyed, and
 * making  this possible by trying to acquire  such a lock, and making use
 * of lock operations  if the  lock cannot be  acquired without  blocking.
 * This then allows for the part  of the object's finalization that  needs
 * a certain lock to be held, to be performed as an asynchronous operation
 * executed once said lock becomes available:
 *
 * >> struct myobj;
 * >> LIST_HEAD(myobj_list, myobj);
 * >> struct myobj {
 * >>     LIST_ENTRY(myobj) ent;
 * >>     union {
 * >>         struct lockop     lop;
 * >>         struct postlockop postlop;
 * >>     };
 * >> };
 * >>
 * >> PRIVATE struct myobj_list   list    = LIST_HEAD_INITIALIZER(list);
 * >> PRIVATE struct atomic_lock  lock    = ATOMIC_LOCK_INIT;
 * >> PRIVATE struct lockop_slist lockops = SLIST_HEAD_INITIALIZER(lockops);
 * >> #define _service()   _lockop_reap(&lockops, &lock)
 * >> #define service()    lockop_reap(&lockops, &lock)
 * >> #define tryacquire() atomic_lock_tryacquire(&lock)
 * >> #define acquire()    atomic_lock_acquire(&lock)
 * >> #define _release()   atomic_lock_release(&lock)
 * >> #define release()    (_release(), service())
 * >>
 * >> PRIVATE NOBLOCK NONNULL((1)) void
 * >> NOTHROW(LOCKOP_CC myobj_destroy_postlop)(struct postlockop *__restrict self) {
 * >>     struct myobj *me = container_of(self, struct myobj, postlop);
 * >>     free(me);
 * >> }
 * >> PRIVATE NOBLOCK NONNULL((1)) struct postlockop *
 * >> NOTHROW(LOCKOP_CC myobj_destroy_lop)(struct lockop *__restrict self) {
 * >>     struct myobj *me = container_of(self, struct myobj, lop);
 * >>     LIST_REMOVE(me, ent);
 * >>     // free() the object _after_ releasing the lock so the lock
 * >>     // itself can be released sonner than it could otherwise be.
 * >>     me->postlop.plo_func = &myobj_destroy_postlop;
 * >>     return &me->postlop;
 * >> }
 * >>
 * >> // Non-blocking async-remove-object-from-locked-list-and-free-object function.
 * >> PRIVATE NOBLOCK NONNULL((1)) void
 * >> NOTHROW(myobj_destroy)(struct myobj *self) {
 * >>     if (tryacquire()) {
 * >>         LIST_REMOVE(self, ent);
 * >>         _release();
 * >>         free(self);
 * >>         service();
 * >>     } else {
 * >>         self->lop.lo_func = &myobj_destroy_lop;
 * >>         lockop_enqueue(&lockops, &self->lop); // or `SLIST_ATOMIC_INSERT(&lockops, &self->lop, lo_link)'
 * >>         _service();
 * >>     }
 * >> }
 */

struct lockop;
struct postlockop;
struct oblockop;
struct obpostlockop;

#ifndef __LOCKOP_CC
#define __LOCKOP_CC __LIBKCALL
#endif /* !__LOCKOP_CC */

/* Calling convention for functions from the lockop API */
#ifndef LOCKOP_CC
#define LOCKOP_CC __LOCKOP_CC
#endif /* !LOCKOP_CC */


/* Callback prototype for an operation to-be performed once locks have been released. */
typedef __NOBLOCK __ATTR_NONNULL((1)) void
/*__NOTHROW*/ (LOCKOP_CC *postlockop_callback_t)(struct postlockop *__restrict __self);
typedef __NOBLOCK __ATTR_NONNULL((1, 2)) void
/*__NOTHROW*/ (LOCKOP_CC *obpostlockop_callback_t)(struct obpostlockop *__restrict __self,
                                                   void *__restrict __obj);

/* Callback prototype for pending locked operations.
 * @return: NULL: Completed.
 * @return: * :   A descriptor for an operation to perform after the lock has been released. */
typedef __NOBLOCK __ATTR_NONNULL((1)) struct postlockop *
/*NOTHROW*/ (LOCKOP_CC *lockop_callback_t)(struct lockop *__restrict __self);
typedef __NOBLOCK __ATTR_NONNULL((1, 2)) struct obpostlockop *
/*NOTHROW*/ (LOCKOP_CC *oblockop_callback_t)(struct oblockop *__restrict __self, void *__restrict __obj);

/* Helper template for copy+paste:
PRIVATE NOBLOCK NONNULL((1, 2)) void
NOTHROW(LOCKOP_CC mystruct_action_postlop)(Tobpostlockop(mystruct) *__restrict self,
                                           struct mystruct *__restrict obj);
PRIVATE NOBLOCK NONNULL((1, 2)) Tobpostlockop(mystruct) *
NOTHROW(LOCKOP_CC mystruct_action_lop)(Toblockop(mystruct) *__restrict self,
                                       struct mystruct *__restrict obj);
*/

struct postlockop {
/*	SLIST_ENTRY(postlockop)                 plo_link; */
	struct { struct postlockop *sle_next; } plo_link; /* [0..1] Next post-lock operation. */
	postlockop_callback_t                   plo_func; /* [1..1][const] Callback to invoke. */
};

struct obpostlockop {
/*	SLIST_ENTRY(obpostlockop)                 oplo_link; */
	struct { struct obpostlockop *sle_next; } oplo_link; /* [0..1] Next post-lock operation. */
	obpostlockop_callback_t                   oplo_func; /* [1..1][const] Callback to invoke. */
};

struct lockop {
/*	SLIST_ENTRY(lockop)                 lo_link; */
	struct { struct lockop *sle_next; } lo_link; /* [0..1] Next lock operation. */
	lockop_callback_t                   lo_func; /* [1..1][const] Operation to perform. */
};

struct oblockop {
/*	SLIST_ENTRY(oblockop)                 olo_link; */
	struct { struct oblockop *sle_next; } olo_link; /* [0..1] Next lock operation. */
	oblockop_callback_t                   olo_func; /* [1..1][const] Operation to perform. */
};

/* Should be `SLIST_HEAD(lockop_slist, lockop);' (but isn't to prevent dependency on <hybrid/sequence/list.h>) */
struct lockop_slist {
	struct lockop *slh_first; /* [0..1] List head */
};

/* `SLIST_HEAD(postlockop_slist, postlockop);' (but isn't to prevent dependency on <hybrid/sequence/list.h>)  */
struct postlockop_slist {
	struct postlockop *slh_first; /* [0..1] List head */
};

/* `SLIST_HEAD(oblockop_slist, oblockop);' (but isn't to prevent dependency on <hybrid/sequence/list.h>)  */
struct oblockop_slist {
	struct oblockop *slh_first; /* [0..1] List head */
};

/* `SLIST_HEAD(obpostlockop_slist, obpostlockop);' (but isn't to prevent dependency on <hybrid/sequence/list.h>)  */
struct obpostlockop_slist {
	struct obpostlockop *slh_first; /* [0..1] List head */
};


/* Helpers to make object-based lockop callbacks easier to integrate */
#ifdef __cplusplus
extern "C++" {
#define Toblockop(type)                    _Toblockop<struct type>
#define Tobpostlockop(type)                _Tobpostlockop<struct type>
#define Toblockop_slist(type)              _Toblockop_slist<struct type>
#define Tobpostlockop_slist(type)          _Tobpostlockop_slist<struct type>
template<class __T> struct _Toblockop;
template<class __T> struct _Tobpostlockop;
#ifdef __COMPILER_HAVE_CXX_TEMPLATE_USING
#define Toblockop_callback_t(type)         _Toblockop_callback_t<struct type>
#define Tobpostlockop_callback_t(type)     _Tobpostlockop_callback_t<struct type>
#define Toblockop_callback_t_tpl(type)     _Toblockop_callback_t<struct type>
#define Tobpostlockop_callback_t_tpl(type) _Tobpostlockop_callback_t<struct type>
template<class __T> using _Toblockop_callback_t     = __NOBLOCK __ATTR_NONNULL((1)) _Tobpostlockop<__T> * /*__NOTHROW*/ (LOCKOP_CC *)(_Toblockop<__T> *__restrict __self, __T *__restrict __obj);
template<class __T> using _Tobpostlockop_callback_t = __NOBLOCK __ATTR_NONNULL((1)) void /*__NOTHROW*/ (LOCKOP_CC *)(_Tobpostlockop<__T> *__restrict __self, __T *__restrict __obj);
#else /* __COMPILER_HAVE_CXX_TEMPLATE_USING */
#define Toblockop_callback_t(type)         _Toblockop_callback_t<struct type>::__T
#define Tobpostlockop_callback_t(type)     _Tobpostlockop_callback_t<struct type>::__T
#define Toblockop_callback_t_tpl(type)     __CXX_DEDUCE_TYPENAME _Toblockop_callback_t<struct type>::__T
#define Tobpostlockop_callback_t_tpl(type) __CXX_DEDUCE_TYPENAME _Tobpostlockop_callback_t<struct type>::__T
template<class __T> struct _Toblockop_callback_t     { typedef __NOBLOCK __ATTR_NONNULL((1)) _Tobpostlockop<__T> * /*__NOTHROW*/ (LOCKOP_CC *__T)(_Toblockop<__T> *__restrict __self, __T *__restrict __obj); };
template<class __T> struct _Tobpostlockop_callback_t { typedef __NOBLOCK __ATTR_NONNULL((1)) void /*__NOTHROW*/ (LOCKOP_CC *__T)(_Tobpostlockop<__T> *__restrict __self, __T *__restrict __obj); };
#endif /* !__COMPILER_HAVE_CXX_TEMPLATE_USING */

template<class __T> struct _Toblockop {
/*	SLIST_ENTRY(_Toblockop<__T>)          olo_link;  * [0..1] Next lock operation. */
	struct { _Toblockop<__T> *sle_next; } olo_link; /* [0..1] Next lock operation. */
/*	_Toblockop_callback_t<__T>            olo_func;  * [1..1][const] Operation to perform. */
	__NOBLOCK __ATTR_NONNULL((1)) _Tobpostlockop<__T> * /*__NOTHROW*/ (LOCKOP_CC *olo_func)(_Toblockop<__T> *__restrict __self, __T *__restrict __obj); /* [1..1][const] Operation to perform. */
};
template<class __T> struct _Tobpostlockop {
/*	SLIST_ENTRY(_Tobpostlockop<__T>)          oplo_link;  * [0..1] Next post-lock operation. */
	struct { _Tobpostlockop<__T> *sle_next; } oplo_link; /* [0..1] Next post-lock operation. */
/*	_Tobpostlockop_callback_t<__T>            oplo_func;  * [1..1][const] Callback to invoke. */
	__NOBLOCK __ATTR_NONNULL((1)) void /*__NOTHROW*/ (LOCKOP_CC *oplo_func)(_Tobpostlockop<__T> *__restrict __self, __T *__restrict __obj); /* [1..1][const] Callback to invoke. */
};
/* template<class __T> SLIST_HEAD(_Toblockop_slist, _Toblockop<__T>); */
/* template<class __T> SLIST_HEAD(_Tobpostlockop_slist, _Tobpostlockop<__T>); */
template<class __T> struct _Toblockop_slist { struct _Toblockop<__T> *slh_first; /* [0..1] List head */ };
template<class __T> struct _Tobpostlockop_slist { struct _Tobpostlockop<__T> *slh_first; /* [0..1] List head */ };
} /* extern "C++" */
#else /* __cplusplus */
#define Toblockop(type)                    struct oblockop
#define Tobpostlockop(type)                struct obpostlockop
#define Toblockop_slist(type)              struct oblockop_slist
#define Tobpostlockop_slist(type)          struct obpostlockop_slist
#define Toblockop_callback_t(type)         oblockop_callback_t
#define Tobpostlockop_callback_t(type)     obpostlockop_callback_t
#define Toblockop_callback_t_tpl(type)     oblockop_callback_t
#define Tobpostlockop_callback_t_tpl(type) obpostlockop_callback_t
#endif /* !__cplusplus */

struct atomic_lock;
struct atomic_rwlock;

}

%[define_replacement(LOCKOP_CC = __LOCKOP_CC)]


%[define(DEFINE_LOCKOP_CC =
@@pp_ifndef __LOCKOP_CC@@
#define __LOCKOP_CC __LIBKCALL
@@pp_endif@@
)]


%{

/* >> void lockop_enqueue(struct lockop_slist *__restrict self, struct lockop *__restrict lop);
 * >> void oblockop_enqueue(struct oblockop_slist *__restrict self, struct oblockop *__restrict lop);
 * >> void oblockop_enqueue(struct Toblockop_slist(T) *__restrict self, struct Toblockop(T) *__restrict lop);
 * NOTE: To add lock ops to a pending list, use `lockop_enqueue(self, lop)',
 *       which is  equivalent  to  `SLIST_ATOMIC_INSERT(self, lop, lo_link)' */
#define lockop_enqueue(self, lop)   __lockop_enqueue(self, lop, (lop)->lo_link)
#define oblockop_enqueue(self, lop) __lockop_enqueue(self, lop, (lop)->olo_link)
#define __lockop_enqueue(self, lop, lop_link)                                          \
	do {                                                                               \
		lop_link.sle_next = __hybrid_atomic_load((self)->slh_first, __ATOMIC_ACQUIRE); \
		__COMPILER_WRITE_BARRIER();                                                    \
	} while (!__hybrid_atomic_cmpxch((self)->slh_first, lop_link.sle_next, lop,        \
	                                 __ATOMIC_RELEASE, __ATOMIC_RELAXED))




/* >> ATTR_PURE WUNUSED bool lockop_mustreap(struct lockop_slist const *__restrict self);
 * >> ATTR_PURE WUNUSED bool lockop_mustreap(struct oblockop_slist const *__restrict self);
 * >> ATTR_PURE WUNUSED bool lockop_mustreap(struct Toblockop_slist(T) const *__restrict self);
 * Check if the given lockop-list `self' must  be reaped (that is: contains pending  callbacks) */
#define lockop_mustreap(self) \
	(__hybrid_atomic_load((self)->slh_first, __ATOMIC_ACQUIRE) != __NULLPTR)
#define oblockop_mustreap(self) lockop_mustreap(self)

/* >> void lockop_reap_ex(struct lockop_slist *__restrict self, bool (LOCKOP_CC *trylock)(void *cookie), void (LOCKOP_CC *unlock)(void *cookie), void *cookie);
 * >> void oblockop_reap_ex(struct oblockop_slist *__restrict self, bool (LOCKOP_CC *trylock)(void *cookie), void (LOCKOP_CC *unlock)(void *cookie), void *cookie, void *obj);
 * >> void oblockop_reap_ex(struct Toblockop_slist(T) *__restrict self, bool (LOCKOP_CC *trylock)(void *cookie), void (LOCKOP_CC *unlock)(void *cookie), void *cookie, T *obj);
 * Functions to reap pending lock operations. */
#ifdef __OPTIMIZE_SIZE__
#define lockop_reap_ex(self, trylock, unlock, cookie) \
	_lockop_reap_ex(self, trylock, unlock, cookie)
#define oblockop_reap_ex(self, trylock, unlock, cookie, obj) \
	_oblockop_reap_ex(self, trylock, unlock, cookie, obj)
#else /* __OPTIMIZE_SIZE__ */
#define lockop_reap_ex(self, trylock, unlock, cookie) \
	(void)(__hybrid_atomic_load((self)->slh_first, __ATOMIC_ACQUIRE) == __NULLPTR || (_lockop_reap_ex(self, trylock, unlock, cookie), 0))
#define oblockop_reap_ex(self, trylock, unlock, cookie, obj) \
	(void)(__hybrid_atomic_load((self)->slh_first, __ATOMIC_ACQUIRE) == __NULLPTR || (_oblockop_reap_ex(self, trylock, unlock, cookie, obj), 0))
#endif /* !__OPTIMIZE_SIZE__ */

}

%(auto_source){
#include <hybrid/host.h>
}

%[define(MAYBE_INCLUDE_KOS_LOCKOP_H =
@@pp_ifndef _KOS_LOCKOP_H@@
#include <kos/lockop.h>
@@pp_endif@@
)]


[[decl_include("<kos/anno.h>"), decl_prefix(DEFINE_LOCKOP_CC)]]
[[nothrow, attribute(__NOBLOCK), crt_name("lockop_reap_ex")]]
[[decl_prefix(struct lockop_slist;)]]
[[kernel, no_crt_dos_wrapper, cc(__LOCKOP_CC)]]
[[if(defined(__OPTIMIZE_SIZE__) && defined(__x86_64__)), crt_intern_kos_alias("libc_oblockop_reap_ex")]]
[[impl_prefix(MAYBE_INCLUDE_KOS_LOCKOP_H)]]
[[impl_include("<hybrid/__atomic.h>")]]
void _lockop_reap_ex([[nonnull]] struct lockop_slist *__restrict self,
                     [[nonnull]] __BOOL (__LOCKOP_CC *trylock)(void *cookie),
                     [[nonnull]] void (__LOCKOP_CC *unlock)(void *cookie),
                     void *cookie) {
#ifndef __INTELLISENSE__
#define __LOCAL_self      self
#undef __LOCAL_obj
#define __LOCAL_trylock() (*trylock)(cookie)
#define __LOCAL_unlock()  (*unlock)(cookie)
#include <libc/template/lockop.h>
#endif /* !__INTELLISENSE__ */
}

[[decl_include("<kos/anno.h>"), decl_prefix(DEFINE_LOCKOP_CC)]]
[[nothrow, attribute(__NOBLOCK), crt_name("oblockop_reap_ex")]]
[[decl_prefix(struct oblockop_slist;)]]
[[kernel, no_crt_dos_wrapper, cc(__LOCKOP_CC)]]
[[impl_prefix(MAYBE_INCLUDE_KOS_LOCKOP_H)]]
[[impl_include("<hybrid/__atomic.h>")]]
void _oblockop_reap_ex([[nonnull]] struct oblockop_slist *__restrict self,
                       [[nonnull]] __BOOL (__LOCKOP_CC *trylock)(void *cookie),
                       [[nonnull]] void (__LOCKOP_CC *unlock)(void *cookie),
                       void *cookie, [[nonnull]] void *__restrict obj) {
#ifndef __INTELLISENSE__
#define __LOCAL_self      self
#define __LOCAL_obj       obj
#define __LOCAL_trylock() (*trylock)(cookie)
#define __LOCAL_unlock()  (*unlock)(cookie)
#include <libc/template/lockop.h>
#endif /* !__INTELLISENSE__ */
}


%{

/* >> void lockop_reap_atomic_lock(struct lockop_slist *__restrict self, struct atomic_lock *__restrict lock);
 * >> void lockop_reap_atomic_rwlock(struct lockop_slist *__restrict self, struct atomic_rwlock *__restrict lock);
 * >> void oblockop_reap_atomic_lock(struct oblockop_slist *__restrict self, struct atomic_lock *__restrict lock, void *obj);
 * >> void oblockop_reap_atomic_rwlock(struct oblockop_slist *__restrict self, struct atomic_rwlock *__restrict lock, void *obj);
 * Functions to reap pending lock operations. */
#ifdef __OPTIMIZE_SIZE__
#define lockop_reap_atomic_lock(self, lock)          _lockop_reap_atomic_lock(self, lock)
#define lockop_reap_atomic_rwlock(self, lock)        _lockop_reap_atomic_rwlock(self, lock)
#define oblockop_reap_atomic_lock(self, lock, obj)   _oblockop_reap_atomic_lock(self, lock, obj)
#define oblockop_reap_atomic_rwlock(self, lock, obj) _oblockop_reap_atomic_rwlock(self, lock, obj)
#else /* __OPTIMIZE_SIZE__ */
#define lockop_reap_atomic_lock(self, lock)          (void)(__hybrid_atomic_load((self)->slh_first, __ATOMIC_ACQUIRE) == __NULLPTR || (_lockop_reap_atomic_lock(self, lock), 0))
#define lockop_reap_atomic_rwlock(self, lock)        (void)(__hybrid_atomic_load((self)->slh_first, __ATOMIC_ACQUIRE) == __NULLPTR || (_lockop_reap_atomic_rwlock(self, lock), 0))
#define oblockop_reap_atomic_lock(self, lock, obj)   (void)(__hybrid_atomic_load((self)->slh_first, __ATOMIC_ACQUIRE) == __NULLPTR || (_oblockop_reap_atomic_lock(self, lock, obj), 0))
#define oblockop_reap_atomic_rwlock(self, lock, obj) (void)(__hybrid_atomic_load((self)->slh_first, __ATOMIC_ACQUIRE) == __NULLPTR || (_oblockop_reap_atomic_rwlock(self, lock, obj), 0))
#endif /* !__OPTIMIZE_SIZE__ */

}

[[decl_include("<kos/anno.h>"), decl_prefix(DEFINE_LOCKOP_CC)]]
[[nothrow, attribute(__NOBLOCK), crt_name("lockop_reap_atomic_lock")]]
[[decl_prefix(struct lockop_slist;), decl_prefix(struct atomic_lock;)]]
[[kernel, no_crt_dos_wrapper, cc(__LOCKOP_CC)]]
[[if(defined(__OPTIMIZE_SIZE__) && defined(__x86_64__)), crt_intern_kos_alias("libc_oblockop_reap_atomic_lock")]]
[[impl_prefix(MAYBE_INCLUDE_KOS_LOCKOP_H)]]
[[impl_include("<hybrid/__atomic.h>", "<hybrid/sync/atomic-lock.h>")]]
void _lockop_reap_atomic_lock([[nonnull]] struct lockop_slist *__restrict self,
                              [[nonnull]] struct atomic_lock *__restrict lock) {
#ifndef __INTELLISENSE__
#define __LOCAL_self      self
#undef __LOCAL_obj
#define __LOCAL_trylock() @atomic_lock_tryacquire@(lock)
#define __LOCAL_unlock()  @atomic_lock_release@(lock)
#include <libc/template/lockop.h>
#endif /* !__INTELLISENSE__ */
}


[[decl_include("<kos/anno.h>"), decl_prefix(DEFINE_LOCKOP_CC)]]
[[nothrow, attribute(__NOBLOCK), crt_name("oblockop_reap_atomic_lock")]]
[[decl_prefix(struct oblockop_slist;), decl_prefix(struct atomic_lock;)]]
[[kernel, no_crt_dos_wrapper, cc(__LOCKOP_CC)]]
[[impl_prefix(MAYBE_INCLUDE_KOS_LOCKOP_H)]]
[[impl_include("<hybrid/__atomic.h>", "<hybrid/sync/atomic-lock.h>")]]
void _oblockop_reap_atomic_lock([[nonnull]] struct oblockop_slist *__restrict self,
                                [[nonnull]] struct atomic_lock *__restrict lock,
                                [[nonnull]] void *__restrict obj) {
#ifndef __INTELLISENSE__
#define __LOCAL_self      self
#define __LOCAL_obj       obj
#define __LOCAL_trylock() @atomic_lock_tryacquire@(lock)
#define __LOCAL_unlock()  @atomic_lock_release@(lock)
#include <libc/template/lockop.h>
#endif /* !__INTELLISENSE__ */
}


[[decl_include("<kos/anno.h>"), decl_prefix(DEFINE_LOCKOP_CC)]]
[[nothrow, attribute(__NOBLOCK), crt_name("lockop_reap_atomic_rwlock")]]
[[decl_prefix(struct lockop_slist;), decl_prefix(struct atomic_rwlock;)]]
[[kernel, no_crt_dos_wrapper, cc(__LOCKOP_CC)]]
[[if(defined(__OPTIMIZE_SIZE__) && defined(__x86_64__)), crt_intern_kos_alias("libc_oblockop_reap_atomic_rwlock")]]
[[impl_prefix(MAYBE_INCLUDE_KOS_LOCKOP_H)]]
[[impl_include("<hybrid/__atomic.h>", "<hybrid/sync/atomic-rwlock.h>")]]
void _lockop_reap_atomic_rwlock([[nonnull]] struct lockop_slist *__restrict self,
                                [[nonnull]] struct atomic_rwlock *__restrict lock) {
#ifndef __INTELLISENSE__
#define __LOCAL_self      self
#undef __LOCAL_obj
#define __LOCAL_trylock() @atomic_rwlock_trywrite@(lock)
#define __LOCAL_unlock()  @atomic_rwlock_endwrite@(lock)
#include <libc/template/lockop.h>
#endif /* !__INTELLISENSE__ */
}


[[decl_include("<kos/anno.h>"), decl_prefix(DEFINE_LOCKOP_CC)]]
[[nothrow, attribute(__NOBLOCK), crt_name("oblockop_reap_atomic_rwlock")]]
[[decl_prefix(struct oblockop_slist;), decl_prefix(struct atomic_rwlock;)]]
[[kernel, no_crt_dos_wrapper, cc(__LOCKOP_CC)]]
[[impl_prefix(MAYBE_INCLUDE_KOS_LOCKOP_H)]]
[[impl_include("<hybrid/__atomic.h>", "<hybrid/sync/atomic-rwlock.h>")]]
void _oblockop_reap_atomic_rwlock([[nonnull]] struct oblockop_slist *__restrict self,
                                  [[nonnull]] struct atomic_rwlock *__restrict lock,
                                  [[nonnull]] void *__restrict obj) {
#ifndef __INTELLISENSE__
#define __LOCAL_self      self
#define __LOCAL_obj       obj
#define __LOCAL_trylock() @atomic_rwlock_trywrite@(lock)
#define __LOCAL_unlock()  @atomic_rwlock_endwrite@(lock)
#include <libc/template/lockop.h>
#endif /* !__INTELLISENSE__ */
}


%
%#ifdef __cplusplus
%{

/* >> void lockop_reap(struct lockop_slist *__restrict self, struct atomic_lock *__restrict lock);
 * >> void lockop_reap(struct lockop_slist *__restrict self, struct atomic_rwlock *__restrict lock);
 * >> void oblockop_reap(struct oblockop_slist *__restrict self, struct atomic_lock *__restrict lock, void *obj);
 * >> void oblockop_reap(struct oblockop_slist *__restrict self, struct atomic_rwlock *__restrict lock, void *obj);
 * Functions to reap pending lock operations. */
#ifdef __OPTIMIZE_SIZE__
#define lockop_reap(self, lock)        _lockop_reap(self, lock)
#define oblockop_reap(self, lock, obj) _oblockop_reap(self, lock, obj)
#else /* __OPTIMIZE_SIZE__ */
#define lockop_reap(self, lock)        (void)(__hybrid_atomic_load((self)->slh_first, __ATOMIC_ACQUIRE) == __NULLPTR || (_lockop_reap(self, lock), 0))
#define oblockop_reap(self, lock, obj) (void)(__hybrid_atomic_load((self)->slh_first, __ATOMIC_ACQUIRE) == __NULLPTR || (_oblockop_reap(self, lock, obj), 0))
#endif /* !__OPTIMIZE_SIZE__ */

}

%[insert:function(_lockop_reap = _lockop_reap_atomic_lock, externLinkageOverride: "C++")]
%[insert:function(_lockop_reap = _lockop_reap_atomic_rwlock, externLinkageOverride: "C++")]
%[insert:function(_oblockop_reap = _oblockop_reap_atomic_lock, externLinkageOverride: "C++")]
%[insert:function(_oblockop_reap = _oblockop_reap_atomic_rwlock, externLinkageOverride: "C++")]

%
%{
extern "C++" {

template<class __T> __FORCELOCAL __NOBLOCK __ATTR_ARTIFICIAL __ATTR_NONNULL((1, 2, 3)) void
__NOTHROW(__LOCKOP_CC _oblockop_reap_atomic_lock)(_Toblockop_slist<__T> *__restrict __self,
                                                  struct atomic_lock *__restrict __lock,
                                                  __T *__restrict __obj) {
	(_oblockop_reap_atomic_lock)((struct oblockop_slist *)__self, __lock, (void *)__obj);
}
template<class __T> __FORCELOCAL __NOBLOCK __ATTR_ARTIFICIAL __ATTR_NONNULL((1, 2, 3)) void
__NOTHROW(__LOCKOP_CC _oblockop_reap_atomic_rwlock)(_Toblockop_slist<__T> *__restrict __self,
                                                    struct atomic_rwlock *__restrict __lock,
                                                    __T *__restrict __obj) {
	(_oblockop_reap_atomic_rwlock)((struct oblockop_slist *)__self, __lock, (void *)__obj);
}
template<class __T> __FORCELOCAL __NOBLOCK __ATTR_ARTIFICIAL __ATTR_NONNULL((1, 2, 3)) void
__NOTHROW(__LOCKOP_CC _oblockop_reap)(_Toblockop_slist<__T> *__restrict __self,
                                      struct atomic_lock *__restrict __lock,
                                      __T *__restrict __obj) {
	(_oblockop_reap_atomic_lock)((struct oblockop_slist *)__self, __lock, (void *)__obj);
}
template<class __T> __FORCELOCAL __NOBLOCK __ATTR_ARTIFICIAL __ATTR_NONNULL((1, 2, 3)) void
__NOTHROW(__LOCKOP_CC _oblockop_reap)(_Toblockop_slist<__T> *__restrict __self,
                                      struct atomic_rwlock *__restrict __lock,
                                      __T *__restrict __obj) {
	(_oblockop_reap_atomic_rwlock)((struct oblockop_slist *)__self, __lock, (void *)__obj);
}

} /* extern "C++" */
} /* % */
%#endif /* __cplusplus */



%{

__SYSDECL_END
#endif /* __CC__ */

}