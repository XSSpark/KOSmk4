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

%[default_impl_section(.text.crt.sched.pthread)]
%[define_replacement(clockid_t = __clockid_t)]
%[define_replacement(pthread_t = __pthread_t)]
%[define_replacement(pthread_attr_t = __pthread_attr_t)]
%[define_replacement(pthread_mutex_t = __pthread_mutex_t)]
%[define_replacement(pthread_mutexattr_t = __pthread_mutexattr_t)]
%[define_replacement(pthread_cond_t = __pthread_cond_t)]
%[define_replacement(pthread_condattr_t = __pthread_condattr_t)]
%[define_replacement(pthread_key_t = __pthread_key_t)]
%[define_replacement(pthread_once_t = __pthread_once_t)]
%[define_replacement(pthread_rwlock_t = __pthread_rwlock_t)]
%[define_replacement(pthread_rwlockattr_t = __pthread_rwlockattr_t)]
%[define_replacement(pthread_spinlock_t = __pthread_spinlock_t)]
%[define_replacement(pthread_barrier_t = __pthread_barrier_t)]
%[define_replacement(pthread_barrierattr_t = __pthread_barrierattr_t)]


%{
#include <features.h>
#include <endian.h>
#include <sched.h>
#include <time.h>

#include <bits/pthreadtypes.h>
#include <bits/pthreadvalues.h>
#include <bits/pthreadinit.h>
#include <bits/setjmp.h>
#include <bits/wordsize.h>

__SYSDECL_BEGIN

/* Comments and definitions are derived from GNU C /usr/include/pthread.h: */
/* Copyright (C) 2002-2016 Free Software Foundation, Inc.
   This file is part of the GNU C Library.

   The GNU C Library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Lesser General Public
   License as published by the Free Software Foundation; either
   version 2.1 of the License, or (at your option) any later version.

   The GNU C Library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Lesser General Public License for more details.

   You should have received a copy of the GNU Lesser General Public
   License along with the GNU C Library; if not, see
   <http://www.gnu.org/licenses/>.  */


/* Detach state. */
}%[enum @macro @undef {
	PTHREAD_CREATE_JOINABLE = __PTHREAD_CREATE_JOINABLE,
	PTHREAD_CREATE_DETACHED = __PTHREAD_CREATE_DETACHED
}]%{

/* Mutex types. */
}%[enum @macro @undef {
	PTHREAD_MUTEX_TIMED_NP      = __PTHREAD_MUTEX_TIMED,
	PTHREAD_MUTEX_RECURSIVE_NP  = __PTHREAD_MUTEX_RECURSIVE,
	PTHREAD_MUTEX_ERRORCHECK_NP = __PTHREAD_MUTEX_ERRORCHECK,
	PTHREAD_MUTEX_ADAPTIVE_NP   = __PTHREAD_MUTEX_ADAPTIVE,
%#if defined(__USE_UNIX98) || defined(__USE_XOPEN2K8)
	PTHREAD_MUTEX_NORMAL     = __PTHREAD_MUTEX_TIMED,
	PTHREAD_MUTEX_RECURSIVE  = __PTHREAD_MUTEX_RECURSIVE,
	PTHREAD_MUTEX_ERRORCHECK = __PTHREAD_MUTEX_ERRORCHECK,
	PTHREAD_MUTEX_DEFAULT    = PTHREAD_MUTEX_NORMAL,
%#endif /* __USE_UNIX98 || __USE_XOPEN2K8 */
%#ifdef __USE_GNU
	PTHREAD_MUTEX_FAST_NP = __PTHREAD_MUTEX_TIMED, /* For compatibility. */
%#endif /* __USE_GNU */
%#ifdef __USE_XOPEN2K
	PTHREAD_MUTEX_STALLED    = __PTHREAD_MUTEX_STALLED, /* Robust mutex or not flags. */
	PTHREAD_MUTEX_ROBUST     = __PTHREAD_MUTEX_ROBUST,  /* ... */
	PTHREAD_MUTEX_STALLED_NP = __PTHREAD_MUTEX_STALLED, /* ... */
	PTHREAD_MUTEX_ROBUST_NP  = __PTHREAD_MUTEX_ROBUST,  /* ... */
%#endif /* __USE_XOPEN2K */
}]%{


#if defined(__USE_POSIX199506) || defined(__USE_UNIX98)
/* Mutex protocols. */
}%[enum @macro @undef {
	PTHREAD_PRIO_NONE    = __PTHREAD_PRIO_NONE,
	PTHREAD_PRIO_INHERIT = __PTHREAD_PRIO_INHERIT,
	PTHREAD_PRIO_PROTECT = __PTHREAD_PRIO_PROTECT,
}]%{
#endif /* __USE_POSIX199506 || __USE_UNIX98 */

/* Read-write lock types. */
#if defined(__USE_UNIX98) || defined(__USE_XOPEN2K)
}%[enum @macro @undef {
	PTHREAD_RWLOCK_PREFER_READER_NP              = __PTHREAD_RWLOCK_PREFER_READER,
	PTHREAD_RWLOCK_PREFER_WRITER_NP              = __PTHREAD_RWLOCK_PREFER_WRITER,
	PTHREAD_RWLOCK_PREFER_WRITER_NONRECURSIVE_NP = __PTHREAD_RWLOCK_PREFER_WRITER_NONRECURSIVE,
	PTHREAD_RWLOCK_DEFAULT_NP                    = __PTHREAD_RWLOCK_PREFER_READER,
}]%{
#endif  /* __USE_UNIX98 || __USE_XOPEN2K */

/* Scheduler inheritance. */
}%[enum @macro @undef {
	PTHREAD_INHERIT_SCHED  = __PTHREAD_INHERIT_SCHED,
	PTHREAD_EXPLICIT_SCHED = __PTHREAD_EXPLICIT_SCHED
}]%{

/* Scope handling. */
}%[enum @macro @undef {
	PTHREAD_SCOPE_SYSTEM  = __PTHREAD_SCOPE_SYSTEM,
	PTHREAD_SCOPE_PROCESS = __PTHREAD_SCOPE_PROCESS
}]%{

/* Process shared or private flag. */
}%[enum @macro @undef {
	PTHREAD_PROCESS_PRIVATE = __PTHREAD_PROCESS_PRIVATE,
	PTHREAD_PROCESS_SHARED  = __PTHREAD_PROCESS_SHARED
}]%{

/* Cancellation */
}%[enum @macro @undef {
	PTHREAD_CANCEL_ENABLE       = __PTHREAD_CANCEL_ENABLE,
	PTHREAD_CANCEL_DISABLE      = __PTHREAD_CANCEL_DISABLE,
	PTHREAD_CANCEL_DEFERRED     = __PTHREAD_CANCEL_DEFERRED,
	PTHREAD_CANCEL_ASYNCHRONOUS = __PTHREAD_CANCEL_ASYNCHRONOUS
}]%{
#define PTHREAD_CANCELED __PTHREAD_CANCELED

/* Single execution handling. */
#define PTHREAD_ONCE_INIT __PTHREAD_ONCE_INIT

#ifdef __USE_XOPEN2K
/* Value returned by 'pthread_barrier_wait' for one of the threads
 * after the required number of threads have called this function.
 * -1 is distinct from 0 and all errno constants */
#define PTHREAD_BARRIER_SERIAL_THREAD __PTHREAD_BARRIER_SERIAL_THREAD
#endif /* __USE_XOPEN2K */



#ifdef __CC__

#define PTHREAD_MUTEX_INITIALIZER               __PTHREAD_MUTEX_INITIALIZER
#ifdef __USE_GNU
#define PTHREAD_RECURSIVE_MUTEX_INITIALIZER_NP  __PTHREAD_RECURSIVE_MUTEX_INITIALIZER
#define PTHREAD_ERRORCHECK_MUTEX_INITIALIZER_NP __PTHREAD_ERRORCHECK_MUTEX_INITIALIZER
#define PTHREAD_ADAPTIVE_MUTEX_INITIALIZER_NP   __PTHREAD_ADAPTIVE_MUTEX_INITIALIZER
#endif /* __USE_GNU */

/* Read-write lock types. */
#if defined(__USE_UNIX98) || defined(__USE_XOPEN2K)

/* Read-write lock initializers. */
#define PTHREAD_RWLOCK_INITIALIZER __PTHREAD_RWLOCK_INITIALIZER
#ifdef __USE_GNU
#define PTHREAD_RWLOCK_WRITER_NONRECURSIVE_INITIALIZER_NP __PTHREAD_RWLOCK_WRITER_NONRECURSIVE_INITIALIZER
#endif /* __USE_GNU */
#endif  /* __USE_UNIX98 || __USE_XOPEN2K */

/* Conditional variable handling. */
#define PTHREAD_COND_INITIALIZER __PTHREAD_COND_INITIALIZER

/* Cleanup buffers */
struct _pthread_cleanup_buffer {
	void                          (*__routine)(void *); /* Function to call. */
	void                           *__arg;              /* Its argument. */
	int                             __canceltype;       /* Saved cancellation type. */
	struct _pthread_cleanup_buffer *__prev;             /* Chaining of cleanup functions. */
};

#ifndef ____pthread_start_routine_t_defined
#define ____pthread_start_routine_t_defined 1
typedef void *(*__pthread_start_routine_t)(void *);
#endif /* !____pthread_start_routine_t_defined */

typedef __pthread_t pthread_t;
#ifndef __pthread_attr_t_defined
#define __pthread_attr_t_defined 1
typedef __pthread_attr_t pthread_attr_t;
#endif /* !__pthread_attr_t_defined */
typedef __pthread_mutex_t pthread_mutex_t;
typedef __pthread_mutexattr_t pthread_mutexattr_t;
typedef __pthread_cond_t pthread_cond_t;
typedef __pthread_condattr_t pthread_condattr_t;
typedef __pthread_key_t pthread_key_t;
typedef __pthread_once_t pthread_once_t;
#if defined(__USE_UNIX98) || defined(__USE_XOPEN2K)
typedef __pthread_rwlock_t pthread_rwlock_t;
typedef __pthread_rwlockattr_t pthread_rwlockattr_t;
#endif /* __USE_UNIX98 || __USE_XOPEN2K */
#ifdef __USE_XOPEN2K
typedef __pthread_spinlock_t pthread_spinlock_t;
typedef __pthread_barrier_t pthread_barrier_t;
typedef __pthread_barrierattr_t pthread_barrierattr_t;
#endif /* __USE_XOPEN2K */


}



%
@@Create a new thread, starting with execution of START-ROUTINE
@@getting passed ARG. Creation attributed come from ATTR. The new
@@handle is stored in *NEWTHREAD
[decl_include(<bits/pthreadtypes.h>)]
[decl_prefix(
#ifndef ____pthread_start_routine_t_defined
#define ____pthread_start_routine_t_defined 1
typedef void *(*__pthread_start_routine_t)(void *);
#endif /* !____pthread_start_routine_t_defined */
)]
pthread_create:([nonnull] pthread_t *__restrict newthread, pthread_attr_t const *__restrict attr,
                [nonnull] __pthread_start_routine_t start_routine, void *__restrict arg) -> int;

%
@@Terminate calling thread.
@@The registered cleanup handlers are called via exception handling
[throws()][ATTR_NORETURN] pthread_exit:(void *retval);

%
@@Make calling thread wait for termination of the thread THREAD. The
@@exit status of the thread is stored in *THREAD_RETURN, if THREAD_RETURN
@@is not NULL
[decl_include(<bits/pthreadtypes.h>)]
[cp] pthread_join:(pthread_t pthread, void **thread_return) -> int;

%#ifdef __USE_GNU
@@Check whether thread THREAD has terminated. If yes return the status of
@@the thread in *THREAD_RETURN, if THREAD_RETURN is not NULL
[decl_include(<bits/pthreadtypes.h>)]
pthread_tryjoin_np:(pthread_t pthread, void **thread_return) -> int;

%
%struct timespec;
%
@@Make calling thread wait for termination of the thread THREAD, but only
@@until TIMEOUT. The exit status of the thread is stored in
@@*THREAD_RETURN, if THREAD_RETURN is not NULL.
[if(defined(__USE_TIME_BITS64)), preferred_alias(pthread_timedjoin64_np)]
[if(!defined(__USE_TIME_BITS64)), preferred_alias(pthread_timedjoin_np)][cp]
[decl_include(<bits/pthreadtypes.h>)]
[decl_include(<bits/timespec.h>)]
[requires(defined(__CRT_HAVE_pthread_timedjoin_np) || defined(__CRT_HAVE_pthread_timedjoin64_np))]
pthread_timedjoin_np:(pthread_t pthread, void **thread_return,
                      struct timespec const *abstime) -> int {
#ifdef __CRT_HAVE_pthread_timedjoin_np
	int result;
	struct timespec32 abstime32;
	abstime32.@tv_sec@  = (time32_t)abstime->@tv_sec@;
	abstime32.@tv_nsec@ = abstime->@tv_nsec@;
	result = pthread_timedjoin32_np(pthread, thread_return, &abstime32);
	return result;
#else /* __CRT_HAVE_pthread_timedjoin_np */
	int result;
	struct timespec64 abstime64;
	abstime64.@tv_sec@  = (time64_t)abstime->@tv_sec@;
	abstime64.@tv_nsec@ = abstime->@tv_nsec@;
	result = pthread_timedjoin64_np(pthread, thread_return, &abstime32);
	return result;
#endif /* !__CRT_HAVE_pthread_timedjoin_np */
}

%#ifdef __USE_TIME64
[decl_include(<bits/pthreadtypes.h>)][doc_alias(pthread_timedjoin_np)]
[decl_include(<bits/timespec.h>)][ignore][cp]
pthread_timedjoin32_np:(pthread_t pthread, void **thread_return,
                        struct timespec32 const *abstime) -> int = pthread_timedjoin_np?;

[cp][time64_variant_of(pthread_timedjoin_np)]
[decl_include(<bits/pthreadtypes.h>)]
[decl_include(<bits/timespec.h>)]
[requires($has_function(pthread_timedjoin32_np))]
pthread_timedjoin64_np:(pthread_t pthread, void **thread_return,
                        struct timespec64 const *abstime) -> int {
	int result;
	struct timespec32 abstime32;
	abstime32.@tv_sec@  = (time32_t)abstime->@tv_sec@;
	abstime32.@tv_nsec@ = abstime->@tv_nsec@;
	result = pthread_timedjoin32_np(pthread, thread_return, &abstime32);
	return result;
}

%#endif /* __USE_TIME64 */
%#endif /* __USE_GNU */

%
@@Indicate that the thread THREAD is never to be joined with PTHREAD_JOIN.
@@The resources of THREAD will therefore be freed immediately when it
@@terminates, instead of waiting for another thread to perform PTHREAD_JOIN on it
pthread_detach:(pthread_t pthread) -> int;

%
@@Obtain the identifier of the current thread
[export_alias(thrd_current)][ATTR_CONST] pthread_self:() -> pthread_t;

@@Compare two thread identifiers
[ATTR_CONST] pthread_equal:(pthread_t pthread1, pthread_t pthread2) -> int {
	return pthread1 == pthread2;
}

%
%/* Thread attribute handling. */
%
@@Initialize thread attribute *ATTR with default attributes
@@(detachstate is PTHREAD_JOINABLE, scheduling policy is SCHED_OTHER, no user-provided stack)
pthread_attr_init:([nonnull] pthread_attr_t *attr) -> int;

%
@@Destroy thread attribute *ATTR
pthread_attr_destroy:([nonnull] pthread_attr_t *attr) -> int;

%
@@Get detach state attribute
pthread_attr_getdetachstate:([nonnull] pthread_attr_t const *attr, [nonnull] int *detachstate) -> int;

%
@@Set detach state attribute
pthread_attr_setdetachstate:([nonnull] pthread_attr_t *attr, int detachstate) -> int;

%
@@Get the size of the guard area created for stack overflow protection
pthread_attr_getguardsize:([nonnull] pthread_attr_t const *attr, [nonnull] size_t *guardsize) -> int;

%
@@Set the size of the guard area created for stack overflow protection
pthread_attr_setguardsize:([nonnull] pthread_attr_t *attr, size_t guardsize) -> int;

%
@@Return in *PARAM the scheduling parameters of *ATTR
pthread_attr_getschedparam:([nonnull] pthread_attr_t const *__restrict attr,
                            [nonnull] struct sched_param *__restrict param) -> int;

%
@@Set scheduling parameters (priority, etc) in *ATTR according to PARAM
pthread_attr_setschedparam:([nonnull] pthread_attr_t *__restrict attr,
                            [nonnull] struct sched_param const *__restrict param) -> int;

%
@@Return in *POLICY the scheduling policy of *ATTR
pthread_attr_getschedpolicy:([nonnull] pthread_attr_t const *__restrict attr,
                             [nonnull] int *__restrict policy) -> int;

%
@@Set scheduling policy in *ATTR according to POLICY
pthread_attr_setschedpolicy:([nonnull] pthread_attr_t *attr, int policy) -> int;

%
@@Return in *INHERIT the scheduling inheritance mode of *ATTR
pthread_attr_getinheritsched:([nonnull] pthread_attr_t const *__restrict attr,
                              [nonnull] int *__restrict inherit) -> int;

%
@@Set scheduling inheritance mode in *ATTR according to INHERIT
pthread_attr_setinheritsched:([nonnull] pthread_attr_t *attr, int inherit) -> int;

%
@@Return in *SCOPE the scheduling contention scope of *ATTR
pthread_attr_getscope:([nonnull] pthread_attr_t const *__restrict attr,
                       [nonnull] int *__restrict scope) -> int;

%
@@Set scheduling contention scope in *ATTR according to SCOPE
pthread_attr_setscope:([nonnull] pthread_attr_t *attr, int scope) -> int;

%
@@Return the previously set address for the stack
[ATTR_DEPRECATED_]
pthread_attr_getstackaddr:([nonnull] pthread_attr_t const *__restrict attr,
                           [nonnull] void **__restrict stackaddr) -> int;


%
@@Set the starting address of the stack of the thread to be created.
@@Depending on whether the stack grows up or down the value must either
@@be higher or lower than all the address in the memory block. The
@@minimal size of the block must be PTHREAD_STACK_MIN
[ATTR_DEPRECATED_]
pthread_attr_setstackaddr:([nonnull] pthread_attr_t *attr, void *stackaddr) -> int;

%
@@Return the currently used minimal stack size
pthread_attr_getstacksize:([nonnull] pthread_attr_t const *__restrict attr,
                           [nonnull] size_t *__restrict stacksize) -> int;

%
@@Add information about the minimum stack size needed for the thread
@@to be started. This size must never be less than PTHREAD_STACK_MIN
@@and must also not exceed the system limits
pthread_attr_setstacksize:([nonnull] pthread_attr_t *attr, size_t stacksize) -> int;

%#ifdef __USE_XOPEN2K
@@Return the previously set address for the stack
pthread_attr_getstack:([nonnull] pthread_attr_t const *__restrict attr,
                       [nonnull] void **__restrict stackaddr,
                       [nonnull] size_t *__restrict stacksize) -> int;

%
@@The following two interfaces are intended to replace the last two. They
@@require setting the address as well as the size since only setting the
@@address will make the implementation on some architectures impossible
pthread_attr_setstack:([nonnull] pthread_attr_t *attr, void *stackaddr, size_t stacksize) -> int;
%#endif /* __USE_XOPEN2K */

%#ifdef __USE_GNU
@@Thread created with attribute ATTR will be limited to run only on
@@the processors represented in CPUSET
pthread_attr_setaffinity_np:([nonnull] pthread_attr_t *attr, size_t cpusetsize,
                             [nonnull] cpu_set_t const *cpuset) -> int;

%
@@Get bit set in CPUSET representing the processors threads created with ATTR can run on
pthread_attr_getaffinity_np:([nonnull] pthread_attr_t const *attr, size_t cpusetsize,
                             [nonnull] cpu_set_t *cpuset) -> int;

%
@@Get the default attributes used by pthread_create in this process
pthread_getattr_default_np:([nonnull] pthread_attr_t *attr) -> int;

%
@@Set the default attributes to be used by pthread_create in this process
pthread_setattr_default_np:([nonnull] pthread_attr_t const *attr) -> int;

%
@@Initialize thread attribute *ATTR with attributes corresponding to the
@@already running thread THREAD. It shall be called on uninitialized ATTR
@@and destroyed with pthread_attr_destroy when no longer needed
pthread_getattr_np:(pthread_t pthread, [nonnull] pthread_attr_t *attr) -> int;
%#endif /* __USE_GNU */

%
%/* Functions for scheduling control. */
%
@@Set the scheduling parameters for TARGET_THREAD according to POLICY and *PARAM
pthread_setschedparam:(pthread_t target_thread, int policy, [nonnull] struct sched_param const *param) -> int;

%
@@Return in *POLICY and *PARAM the scheduling parameters for TARGET_THREAD
pthread_getschedparam:(pthread_t target_thread, [nonnull] int *__restrict policy, [nonnull] struct sched_param *__restrict param) -> int;

%
@@Set the scheduling priority for TARGET_THREAD
pthread_setschedprio:(pthread_t target_thread, int prio) -> int;


%#ifdef __USE_GNU
@@Get thread name visible in the kernel and its interfaces
pthread_getname_np:(pthread_t target_thread, [outp(buflen)] char *buf, size_t buflen) -> int;

%
@@Set thread name visible in the kernel and its interfaces
pthread_setname_np:(pthread_t target_thread, [nonnull] const char *name) -> int;
%#endif /* __USE_GNU */


%#ifdef __USE_KOS
@@Return the TID of the given `target_thread'
@@If the given `target_thread' has already terminated, 0 is returned
[guard][ATTR_WUNUSED][ATTR_CONST]
pthread_gettid_np:(pthread_t target_thread) -> $pid_t;
%#endif /* __USE_KOS */


%#ifdef __USE_UNIX98
@@Determine level of concurrency
[ATTR_PURE] pthread_getconcurrency:() -> int;

%
@@Set new concurrency level to LEVEL
pthread_setconcurrency:(int level) -> int;
%#endif /* __USE_UNIX98 */

%#ifdef __USE_GNU
@@Yield the processor to another thread or process.
@@This function is similar to the POSIX `sched_yield' function but
@@might be differently implemented in the case of a m-on-n thread
@@implementation
[alias(sched_yield, thrd_yield)] pthread_yield:() -> int;

%
@@Limit specified thread THREAD to run only on the processors represented in CPUSET
pthread_setaffinity_np:(pthread_t pthread, size_t cpusetsize, [nonnull] cpu_set_t const *cpuset) -> int;

%
@@Get bit set in CPUSET representing the processors THREAD can run on
pthread_getaffinity_np:(pthread_t pthread, size_t cpusetsize, [nonnull] cpu_set_t *cpuset) -> int;
%#endif /* __USE_GNU */


%
%typedef void (__LIBCCALL *__pthread_once_routine_t)(void);

%
%/* Functions for handling initialization. */
%

@@Guarantee that the initialization function INIT_ROUTINE will be called
@@only once, even if pthread_once is executed several times with the
@@same ONCE_CONTROL argument. ONCE_CONTROL must point to a static or
@@extern variable initialized to PTHREAD_ONCE_INIT.
[throws][export_alias(call_once)]
pthread_once:([nonnull] pthread_once_t *once_control,
              [nonnull] __pthread_once_routine_t init_routine) -> int;

%
%/* Functions for handling cancellation.
% * Note that these functions are explicitly not marked to not throw an
% * exception in C++ code. If cancellation is implemented by unwinding
% * this is necessary to have the compiler generate the unwind information. */


%
@@Set cancelability state of current thread to STATE,
@@returning old state in *OLDSTATE if OLDSTATE is not NULL
pthread_setcancelstate:(int state, [nullable] int *oldstate) -> int;

%
@@Set cancellation state of current thread to TYPE, returning the old
@@type in *OLDTYPE if OLDTYPE is not NULL
[guard] pthread_setcanceltype:(int type, [nullable] int *oldtype) -> int;

%
@@Cancel THREAD immediately or at the next possibility
pthread_cancel:(pthread_t pthread) -> int;

%
@@Test for pending cancellation for the current thread and terminate
@@the thread as per pthread_exit(PTHREAD_CANCELED) if it has been canceled
[cp] pthread_testcancel:();

%
%/* Cancellation handling with integration into exception handling. */

%{
typedef struct {
	struct {
		struct __jmp_buf __cancel_jmp_buf;
		int              __mask_was_saved;
	}                    __cancel_jmp_buf[1];
	void                *__pad[4];
} __pthread_unwind_buf_t __attribute__((__aligned__)); /* XXX: Portable attribute? */

/* No special attributes by default. */
#ifndef __cleanup_fct_attribute
#define __cleanup_fct_attribute /* nothing */
#endif /* !__cleanup_fct_attribute */

/* Structure to hold the cleanup handler information. */
struct __pthread_cleanup_frame {
	void (__LIBCCALL *__cancel_routine) (void *);
	void             *__cancel_arg;
	int               __do_it;
	int               __cancel_type;
};
}


%#if defined(__GNUC__) && defined(__EXCEPTIONS)
%{
#ifdef __cplusplus
/* Class to handle cancellation handler invocation. */
class __pthread_cleanup_class {
	void (__LIBCCALL *__cancel_routine) (void *);
	void             *__cancel_arg;
	int               __do_it;
	int               __cancel_type;
public:
	__CXX_CLASSMEMBER __pthread_cleanup_class (void (__LIBCCALL *__fct)(void *), void *__arg)
		: __cancel_routine(__fct), __cancel_arg(__arg), __do_it(1)
	{ }
	__CXX_CLASSMEMBER ~__pthread_cleanup_class() {
		if (__do_it)
			(*__cancel_routine)(__cancel_arg);
	}
	__CXX_CLASSMEMBER void __setdoit(int __newval) {
		__do_it = __newval;
	}
#ifdef __pthread_setcanceltype_defined
	__CXX_CLASSMEMBER void __defer() { pthread_setcanceltype(PTHREAD_CANCEL_DEFERRED, &__cancel_type); }
	__CXX_CLASSMEMBER void __restore() const { pthread_setcanceltype(__cancel_type, 0); }
#endif /* __pthread_setcanceltype_defined */
};

/* Install a cleanup handler: ROUTINE will be called with arguments ARG
 * when the thread is canceled or calls pthread_exit. ROUTINE will also
 * be called with arguments ARG when the matching pthread_cleanup_pop
 * is executed with non-zero EXECUTE argument.
 * pthread_cleanup_push and pthread_cleanup_pop are macros and must always
 * be used in matching pairs at the same nesting level of braces. */
#define pthread_cleanup_push(routine, arg) \
	do {                                   \
		__pthread_cleanup_class __clframe(routine, arg)

/* Remove a cleanup handler installed by the matching pthread_cleanup_push.
 * If EXECUTE is non-zero, the handler function is called. */
#define pthread_cleanup_pop(execute)  \
		__clframe.__setdoit(execute); \
	} __WHILE0

#ifdef __USE_GNU
/* Install a cleanup handler as pthread_cleanup_push does, but also
 * saves the current cancellation type and sets it to deferred
 * cancellation. */
#define pthread_cleanup_push_defer_np(routine, arg)      \
	do {                                                 \
		__pthread_cleanup_class __clframe(routine, arg); \
		__clframe.__defer()

/* Remove a cleanup handler as pthread_cleanup_pop does, but also
 * restores the cancellation type that was in effect when the matching
 * pthread_cleanup_push_defer was called. */
#define pthread_cleanup_pop_restore_np(execute) \
		__clframe.__restore();                  \
		__clframe.__setdoit(execute);           \
	} __WHILE0
#endif /* __USE_GNU */
#else /* __cplusplus */
}

@@Function called to call the cleanup handler. As an extern inline
@@function the compiler is free to decide inlining the change when
@@needed or fall back on the copy which must exist somewhere else
__pthread_cleanup_routine:([nonnull] struct __pthread_cleanup_frame *frame) {
	if (frame->@__do_it@)
		(*frame->@__cancel_routine)@(frame->@__cancel_arg@);
}

%{
/* Install a cleanup handler: ROUTINE will be called with arguments ARG
 * when the thread is canceled or calls pthread_exit. ROUTINE will also
 * be called with arguments ARG when the matching pthread_cleanup_pop
 * is executed with non-zero EXECUTE argument.
 * pthread_cleanup_push and pthread_cleanup_pop are macros and must always
 * be used in matching pairs at the same nesting level of braces. */
#define pthread_cleanup_push(routine, arg)                          \
	do {                                                            \
		struct __pthread_cleanup_frame __clframe                    \
		__attribute__((__cleanup__(__pthread_cleanup_routine))) = { \
			.__cancel_routine = (routine),                          \
			.__cancel_arg = (arg),                                  \
			.__do_it = 1                                            \
		};

/* Remove a cleanup handler installed by the matching pthread_cleanup_push.
 * If EXECUTE is non-zero, the handler function is called. */
#define pthread_cleanup_pop(execute)   \
		__clframe.__do_it = (execute); \
	} __WHILE0

#ifdef __USE_GNU
/* Install a cleanup handler as pthread_cleanup_push does, but also
 * saves the current cancellation type and sets it to deferred
 * cancellation. */
#define pthread_cleanup_push_defer_np(routine, arg)                 \
	do {                                                            \
		struct __pthread_cleanup_frame __clframe                    \
		__attribute__((__cleanup__(__pthread_cleanup_routine))) = { \
			.__cancel_routine = (routine),                          \
			.__cancel_arg = (arg),                                  \
			.__do_it = 1                                            \
		};                                                          \
		(void)pthread_setcanceltype(PTHREAD_CANCEL_DEFERRED, &__clframe.__cancel_type)

/* Remove a cleanup handler as pthread_cleanup_pop does, but also
 * restores the cancellation type that was in effect when the matching
 * pthread_cleanup_push_defer was called. */
#define pthread_cleanup_pop_restore_np(execute)                     \
		(void)pthread_setcanceltype(__clframe.__cancel_type, NULL); \
		__clframe.__do_it = (execute);                              \
	} __WHILE0

#endif /* __USE_GNU */
#endif /* !__cplusplus */
}

%#else /* __GNUC__ && __EXCEPTIONS */

%/* Install a cleanup handler: ROUTINE will be called with arguments ARG
% * when the thread is canceled or calls pthread_exit. ROUTINE will also
% * be called with arguments ARG when the matching pthread_cleanup_pop
% * is executed with non-zero EXECUTE argument.
% * pthread_cleanup_push and pthread_cleanup_pop are macros and must always
% * be used in matching pairs at the same nesting level of braces. */
%#define pthread_cleanup_push(routine, arg)                                                                    \
%	do {                                                                                                      \
%		__pthread_unwind_buf_t __cancel_buf;                                                                  \
%		void (__LIBCCALL *__cancel_routine)(void *) = (routine);                                              \
%		void *__cancel_arg                          = (arg);                                                  \
%		int __not_first_call = __sigsetjmp((struct __jmp_buf_tag *)(void *)__cancel_buf.__cancel_jmp_buf, 0); \
%		if __unlikely(__not_first_call) {                                                                     \
%			(*__cancel_routine)(__cancel_arg);                                                                \
%			__pthread_unwind_next(&__cancel_buf);                                                             \
%			__builtin_unreachable();                                                                          \
%		}                                                                                                     \
%		__pthread_register_cancel(&__cancel_buf);                                                             \
%		do {
%

[attribute(@__cleanup_fct_attribute@)]
__pthread_register_cancel:(__pthread_unwind_buf_t *buf);

%/* Remove a cleanup handler installed by the matching pthread_cleanup_push.
% * If EXECUTE is non-zero, the handler function is called. */
%#define pthread_cleanup_pop(execute)                                           \
%			do {                                                               \
%			} __WHILE0; /* Empty to allow label before pthread_cleanup_pop. */ \
%		} __WHILE0;                                                            \
%		__pthread_unregister_cancel(&__cancel_buf);                            \
%		if (execute)                                                           \
%			(*__cancel_routine)(__cancel_arg);                                 \
%	} __WHILE0

[attribute(@__cleanup_fct_attribute@)]
__pthread_unregister_cancel:([nonnull] __pthread_unwind_buf_t *buf);

%#ifdef __USE_GNU
%/* Install a cleanup handler as pthread_cleanup_push does, but also
% * saves the current cancellation type and sets it to deferred
% * cancellation. */
%#define pthread_cleanup_push_defer_np(routine, arg)                                                           \
%	do {                                                                                                      \
%		__pthread_unwind_buf_t __cancel_buf;                                                                  \
%		void (__LIBCCALL *__cancel_routine)(void *) = (routine);                                              \
%		void *__cancel_arg               = (arg);                                                             \
%		int __not_first_call = __sigsetjmp((struct __jmp_buf_tag *)(void *)__cancel_buf.__cancel_jmp_buf, 0); \
%		if __unlikely(__not_first_call) {                                                                     \
%			__cancel_routine(__cancel_arg);                                                                   \
%			__pthread_unwind_next(&__cancel_buf);                                                             \
%			__builtin_unreachable();                                                                          \
%		}                                                                                                     \
%		__pthread_register_cancel_defer(&__cancel_buf);                                                       \
%		do {

[attribute(@__cleanup_fct_attribute@)]
__pthread_register_cancel_defer:([nonnull] __pthread_unwind_buf_t *buf);

%/* Remove a cleanup handler as pthread_cleanup_pop does, but also
% * restores the cancellation type that was in effect when the matching
% * pthread_cleanup_push_defer was called. */
%#define pthread_cleanup_pop_restore_np(execute)                                \
%			do {                                                               \
%			} __WHILE0; /* Empty to allow label before pthread_cleanup_pop. */ \
%		} __WHILE0;                                                            \
%		__pthread_unregister_cancel_restore(&__cancel_buf);                    \
%		if (execute)                                                           \
%			(*__cancel_routine)(__cancel_arg);                                 \
%	} __WHILE0

[attribute(@__cleanup_fct_attribute@)]
__pthread_unregister_cancel_restore:([nonnull] __pthread_unwind_buf_t *buf);
%#endif /* __USE_GNU */

@@Internal interface to initiate cleanup
[ATTR_WEAK] /* XXX:[if(!defined(SHARED)), ATTR_WEAK] */
[attribute(@__cleanup_fct_attribute@)][ATTR_NORETURN]
__pthread_unwind_next:([nonnull] __pthread_unwind_buf_t *buf);

%#endif /* !__GNUC__ || !__EXCEPTIONS */

%
%/* Function used in the macros. */
%struct __jmp_buf_tag;
[alias(sigsetjmp)][noexport][nouser]
__sigsetjmp:([nonnull] struct __jmp_buf_tag *env, int savemask) -> int;

%
%/* Mutex handling. */
%
@@Initialize a mutex
pthread_mutex_init:([nonnull] pthread_mutex_t *mutex, pthread_mutexattr_t const *mutexattr) -> int;

%
@@Destroy a mutex
[export_alias(mtx_destroy)]
pthread_mutex_destroy:([nonnull] pthread_mutex_t *mutex) -> int;

%
@@Try locking a mutex
pthread_mutex_trylock:([nonnull] pthread_mutex_t *mutex) -> int;

%
@@Lock a mutex
pthread_mutex_lock:([nonnull] pthread_mutex_t *mutex) -> int;

%
%#ifdef __USE_XOPEN2K
@@Wait until lock becomes available, or specified time passes
[if(defined(__USE_TIME_BITS64)), preferred_alias(pthread_mutex_timedlock64)]
[if(!defined(__USE_TIME_BITS64)), preferred_alias(pthread_mutex_timedlock)][cp]
[decl_include(<bits/pthreadtypes.h>)]
[decl_include(<bits/timespec.h>)]
[requires(defined(__CRT_HAVE_pthread_mutex_timedlock) || defined(__CRT_HAVE_pthread_mutex_timedlock64))]
pthread_mutex_timedlock:([nonnull] pthread_mutex_t *__restrict mutex,
                         [nonnull] struct timespec const *__restrict abstime) -> int {
#ifdef __CRT_HAVE_pthread_mutex_timedlock
	int result;
	struct timespec32 abstime32;
	abstime32.@tv_sec@  = (time32_t)abstime->@tv_sec@;
	abstime32.@tv_nsec@ = abstime->@tv_nsec@;
	result = pthread_mutex_timedlock32(mutex, &abstime32);
	return result;
#else /* __CRT_HAVE_pthread_mutex_timedlock */
	int result;
	struct timespec64 abstime64;
	abstime64.@tv_sec@  = (time64_t)abstime->@tv_sec@;
	abstime64.@tv_nsec@ = abstime->@tv_nsec@;
	result = pthread_mutex_timedlock64(mutex, &abstime64);
	return result;
#endif /* !__CRT_HAVE_pthread_mutex_timedlock */
}

%#ifdef __USE_TIME64
[ignore][cp][doc_alias(pthread_mutex_timedlock)]
[decl_include(<bits/pthreadtypes.h>)]
[decl_include(<bits/timespec.h>)]
pthread_mutex_timedlock32:([nonnull] pthread_mutex_t *__restrict mutex,
                           [nonnull] struct timespec const *__restrict abstime)
		-> int = pthread_mutex_timedlock?;

[doc_alias(pthread_mutex_timedlock)][cp]
[time64_variant_of(pthread_mutex_timedlock)]
[decl_include(<bits/pthreadtypes.h>)]
[decl_include(<bits/timespec.h>)]
[requires($has_function(pthread_mutex_timedlock32))]
pthread_mutex_timedlock64:([nonnull] pthread_mutex_t *__restrict mutex,
                           [nonnull] struct timespec64 const *__restrict abstime) -> int {
	int result;
	struct timespec32 abstime32;
	abstime32.@tv_sec@  = (time32_t)abstime->@tv_sec@;
	abstime32.@tv_nsec@ = abstime->@tv_nsec@;
	result = pthread_mutex_timedlock32(mutex, &abstime32);
	return result;
}

%#endif /* __USE_TIME64 */
%#endif /* __USE_XOPEN2K */

%
@@Unlock a mutex
pthread_mutex_unlock:([nonnull] pthread_mutex_t *mutex) -> int;

%
@@Get the priority ceiling of MUTEX
pthread_mutex_getprioceiling:([nonnull] pthread_mutex_t const *__restrict mutex,
                              [nonnull] int *__restrict prioceiling) -> int;

%
@@Set the priority ceiling of MUTEX to PRIOCEILING, return old
@@priority ceiling value in *OLD_CEILING
pthread_mutex_setprioceiling:([nonnull] pthread_mutex_t *__restrict mutex, int prioceiling,
                              [nonnull] int *__restrict old_ceiling) -> int;

%
%#ifdef __USE_XOPEN2K8
@@Declare the state protected by MUTEX as consistent
pthread_mutex_consistent:([nonnull] pthread_mutex_t *mutex) -> int;
%#ifdef __USE_GNU
pthread_mutex_consistent_np:([nonnull] pthread_mutex_t *mutex) -> int;
%#endif /* __USE_GNU */
%#endif /* __USE_XOPEN2K8 */

%
%/* Functions for handling mutex attributes. */


%
@@Initialize mutex attribute object ATTR with default attributes (kind is PTHREAD_MUTEX_TIMED_NP)
pthread_mutexattr_init:([nonnull] pthread_mutexattr_t *attr) -> int;


%
@@Destroy mutex attribute object ATTR
pthread_mutexattr_destroy:([nonnull] pthread_mutexattr_t *attr) -> int;

%
@@Get the process-shared flag of the mutex attribute ATTR
pthread_mutexattr_getpshared:([nonnull] pthread_mutexattr_t const *__restrict attr,
                              [nonnull] int *__restrict pshared) -> int;

%
@@Set the process-shared flag of the mutex attribute ATTR
pthread_mutexattr_setpshared:([nonnull] pthread_mutexattr_t *attr, int pshared) -> int;


%
%#if defined(__USE_UNIX98) || defined(__USE_XOPEN2K8)
@@Return in *KIND the mutex kind attribute in *ATTR
pthread_mutexattr_gettype:([nonnull] pthread_mutexattr_t const *__restrict attr,
                           [nonnull] int *__restrict kind) -> int;

%
@@Set the mutex kind attribute in *ATTR to KIND (either PTHREAD_MUTEX_NORMAL,
@@PTHREAD_MUTEX_RECURSIVE, PTHREAD_MUTEX_ERRORCHECK, or PTHREAD_MUTEX_DEFAULT)
pthread_mutexattr_settype:([nonnull] pthread_mutexattr_t *attr, int kind) -> int;
%#endif /* __USE_UNIX98 || __USE_XOPEN2K8 */

%
@@Return in *PROTOCOL the mutex protocol attribute in *ATTR
pthread_mutexattr_getprotocol:([nonnull] pthread_mutexattr_t const *__restrict attr,
                               [nonnull] int *__restrict protocol) -> int;

%
@@Set the mutex protocol attribute in *ATTR to PROTOCOL (either
@@PTHREAD_PRIO_NONE, PTHREAD_PRIO_INHERIT, or PTHREAD_PRIO_PROTECT)
pthread_mutexattr_setprotocol:([nonnull] pthread_mutexattr_t *attr, int protocol) -> int;

%
@@Return in *PRIOCEILING the mutex prioceiling attribute in *ATTR
pthread_mutexattr_getprioceiling:([nonnull] pthread_mutexattr_t const *__restrict attr,
                                  [nonnull] int *__restrict prioceiling) -> int;

%
@@Set the mutex prioceiling attribute in *ATTR to PRIOCEILING
pthread_mutexattr_setprioceiling:([nonnull] pthread_mutexattr_t *attr, int prioceiling) -> int;

%
%#ifdef __USE_XOPEN2K
@@Get the robustness flag of the mutex attribute ATTR
pthread_mutexattr_getrobust:([nonnull] pthread_mutexattr_t const *attr,
                             [nonnull] int *robustness) -> int;

%
@@Set the robustness flag of the mutex attribute ATTR
pthread_mutexattr_setrobust:([nonnull] pthread_mutexattr_t *attr, int robustness) -> int;

%
%#ifdef __USE_GNU
pthread_mutexattr_getrobust_np:([nonnull] pthread_mutexattr_t const *attr,
                                [nonnull] int *robustness) -> int;
pthread_mutexattr_setrobust_np:([nonnull] pthread_mutexattr_t *attr, int robustness) -> int;
%#endif /* __USE_GNU */
%#endif /* __USE_XOPEN2K */


%
%#if defined(__USE_UNIX98) || defined(__USE_XOPEN2K)
%/* Functions for handling read-write locks. */

%
@@Initialize read-write lock RWLOCK using attributes ATTR,
@@or use the default values if later is NULL
pthread_rwlock_init:([nonnull] pthread_rwlock_t *__restrict rwlock, pthread_rwlockattr_t const *__restrict attr) -> int;

%
@@Destroy read-write lock RWLOCK
pthread_rwlock_destroy:([nonnull] pthread_rwlock_t *rwlock) -> int;

%
@@Acquire read lock for RWLOCK
[cp] pthread_rwlock_rdlock:([nonnull] pthread_rwlock_t *rwlock) -> int;

%
@@Try to acquire read lock for RWLOCK
pthread_rwlock_tryrdlock:([nonnull] pthread_rwlock_t *rwlock) -> int;

%
%#ifdef __USE_XOPEN2K

@@Try to acquire read lock for RWLOCK or return after specfied time
[if(defined(__USE_TIME_BITS64)), preferred_alias(pthread_rwlock_timedrdlock64)]
[if(!defined(__USE_TIME_BITS64)), preferred_alias(pthread_rwlock_timedrdlock)]
[decl_include(<bits/pthreadtypes.h>)]
[decl_include(<bits/timespec.h>)][cp]
[requires(defined(__CRT_HAVE_pthread_rwlock_timedrdlock) || defined(__CRT_HAVE_pthread_rwlock_timedrdlock64))]
pthread_rwlock_timedrdlock:([nonnull] pthread_rwlock_t *__restrict rwlock,
                            [nonnull] struct timespec const *__restrict abstime) -> int {
#ifdef __CRT_HAVE_pthread_rwlock_timedrdlock
	int result;
	struct timespec32 abstime32;
	abstime32.@tv_sec@  = (time32_t)abstime->@tv_sec@;
	abstime32.@tv_nsec@ = abstime->@tv_nsec@;
	result = pthread_rwlock_timedrdlock32(rwlock, &abstime32);
	return result;
#else /* __CRT_HAVE_pthread_rwlock_timedrdlock */
	int result;
	struct timespec64 abstime64;
	abstime64.@tv_sec@  = (time64_t)abstime->@tv_sec@;
	abstime64.@tv_nsec@ = abstime->@tv_nsec@;
	result = pthread_rwlock_timedrdlock64(rwlock, &abstime32);
	return result;
#endif /* !__CRT_HAVE_pthread_rwlock_timedrdlock */
}

%#ifdef __USE_TIME64

[cp][doc_alias(pthread_rwlock_timedrdlock)]
[decl_include(<bits/pthreadtypes.h>)]
[decl_include(<bits/timespec.h>)]
pthread_rwlock_timedrdlock32:([nonnull] pthread_rwlock_t *__restrict rwlock,
                              [nonnull] struct timespec32 const *__restrict abstime)
		-> int = pthread_rwlock_timedrdlock?;

[cp][doc_alias(pthread_rwlock_timedrdlock)]
[time64_variant_of(pthread_rwlock_timedrdlock)]
[decl_include(<bits/pthreadtypes.h>)]
[decl_include(<bits/timespec.h>)]
[requires($has_function(pthread_rwlock_timedrdlock32))]
pthread_rwlock_timedrdlock64:([nonnull] pthread_rwlock_t *__restrict rwlock,
                              [nonnull] struct timespec64 const *__restrict abstime) -> int {
	int result;
	struct timespec32 abstime32;
	abstime32.@tv_sec@  = (time32_t)abstime->@tv_sec@;
	abstime32.@tv_nsec@ = abstime->@tv_nsec@;
	result = pthread_rwlock_timedrdlock32(rwlock, &abstime32);
	return result;
}


%#endif /* __USE_TIME64 */
%#endif /* __USE_XOPEN2K */

%
@@Acquire write lock for RWLOCK
[cp] pthread_rwlock_wrlock:([nonnull] pthread_rwlock_t *rwlock) -> int;

%
@@Try to acquire write lock for RWLOCK
pthread_rwlock_trywrlock:([nonnull] pthread_rwlock_t *rwlock) -> int;

%
%#ifdef __USE_XOPEN2K

@@Try to acquire write lock for RWLOCK or return after specfied time
[if(defined(__USE_TIME_BITS64)), preferred_alias(pthread_rwlock_timedwrlock64)]
[if(!defined(__USE_TIME_BITS64)), preferred_alias(pthread_rwlock_timedwrlock)]
[decl_include(<bits/pthreadtypes.h>)]
[decl_include(<bits/timespec.h>)][cp]
[requires(defined(__CRT_HAVE_pthread_rwlock_timedwrlock) || defined(__CRT_HAVE_pthread_rwlock_timedwrlock64))]
pthread_rwlock_timedwrlock:([nonnull] pthread_rwlock_t *__restrict rwlock,
                            [nonnull] struct timespec const *__restrict abstime) -> int {
#ifdef __CRT_HAVE_pthread_rwlock_timedwrlock
	int result;
	struct timespec32 abstime32;
	abstime32.@tv_sec@  = (time32_t)abstime->@tv_sec@;
	abstime32.@tv_nsec@ = abstime->@tv_nsec@;
	result = pthread_rwlock_timedwrlock32(rwlock, &abstime32);
	return result;
#else /* __CRT_HAVE_pthread_rwlock_timedwrlock */
	int result;
	struct timespec64 abstime64;
	abstime64.@tv_sec@  = (time64_t)abstime->@tv_sec@;
	abstime64.@tv_nsec@ = abstime->@tv_nsec@;
	result = pthread_rwlock_timedwrlock64(rwlock, &abstime32);
	return result;
#endif /* !__CRT_HAVE_pthread_rwlock_timedwrlock */
}

%#ifdef __USE_TIME64

[cp][doc_alias(pthread_rwlock_timedwrlock)]
[decl_include(<bits/pthreadtypes.h>)]
[decl_include(<bits/timespec.h>)]
pthread_rwlock_timedwrlock32:([nonnull] pthread_rwlock_t *__restrict rwlock,
                              [nonnull] struct timespec32 const *__restrict abstime)
		-> int = pthread_rwlock_timedwrlock?;

[cp][doc_alias(pthread_rwlock_timedwrlock)]
[time64_variant_of(pthread_rwlock_timedwrlock)]
[decl_include(<bits/pthreadtypes.h>)]
[decl_include(<bits/timespec.h>)]
[requires($has_function(pthread_rwlock_timedwrlock32))]
pthread_rwlock_timedwrlock64:([nonnull] pthread_rwlock_t *__restrict rwlock,
                              [nonnull] struct timespec64 const *__restrict abstime) -> int {
	int result;
	struct timespec32 abstime32;
	abstime32.@tv_sec@  = (time32_t)abstime->@tv_sec@;
	abstime32.@tv_nsec@ = abstime->@tv_nsec@;
	result = pthread_rwlock_timedwrlock32(rwlock, &abstime32);
	return result;
}
%#endif /* __USE_TIME64 */
%#endif /* __USE_XOPEN2K */

%
@@Unlock RWLOCK
pthread_rwlock_unlock:([nonnull] pthread_rwlock_t *rwlock) -> int;

%
%/* Functions for handling read-write lock attributes. */

%
@@Initialize attribute object ATTR with default values
pthread_rwlockattr_init:([nonnull] pthread_rwlockattr_t *attr) -> int;

%
@@Destroy attribute object ATTR
pthread_rwlockattr_destroy:([nonnull] pthread_rwlockattr_t *attr) -> int;

%
@@Return current setting of process-shared attribute of ATTR in PSHARED
pthread_rwlockattr_getpshared:([nonnull] pthread_rwlockattr_t const *__restrict attr,
                               [nonnull] int *__restrict pshared) -> int;

%
@@Set process-shared attribute of ATTR to PSHARED
pthread_rwlockattr_setpshared:([nonnull] pthread_rwlockattr_t *attr, int pshared) -> int;

%
@@Return current setting of reader/writer preference
pthread_rwlockattr_getkind_np:([nonnull] pthread_rwlockattr_t const *__restrict attr,
                               [nonnull] int *__restrict pref) -> int;

%
@@Set reader/write preference
pthread_rwlockattr_setkind_np:([nonnull] pthread_rwlockattr_t *attr, int pref) -> int;
%#endif /* __USE_UNIX98 || __USE_XOPEN2K */

%
%/* Functions for handling conditional variables. */

%
@@Initialize condition variable COND using attributes ATTR, or use
@@the default values if later is NULL
pthread_cond_init:([nonnull] pthread_cond_t *__restrict cond,
                   [nullable] pthread_condattr_t const *__restrict cond_attr) -> int;

%
@@Destroy condition variable COND
[export_alias(cnd_destroy)]
pthread_cond_destroy:([nonnull] pthread_cond_t *cond) -> int;

%
@@Wake up one thread waiting for condition variable COND
pthread_cond_signal:([nonnull] pthread_cond_t *cond) -> int;

%
@@Wake up all threads waiting for condition variables COND
pthread_cond_broadcast:([nonnull] pthread_cond_t *cond) -> int;

%
@@Wait for condition variable COND to be signaled or broadcast.
@@MUTEX is assumed to be locked before.
[cp] pthread_cond_wait:([nonnull] pthread_cond_t *__restrict cond,
                        [nonnull] pthread_mutex_t *__restrict mutex) -> int;

%
@@Wait for condition variable COND to be signaled or broadcast until
@@ABSTIME. MUTEX is assumed to be locked before. ABSTIME is an
@@absolute time specification; zero is the beginning of the epoch
@@(00:00:00 GMT, January 1, 1970).
[if(defined(__USE_TIME_BITS64)), preferred_alias(pthread_cond_timedwait64)]
[if(!defined(__USE_TIME_BITS64)), preferred_alias(pthread_cond_timedwait)][cp]
[decl_include(<bits/pthreadtypes.h>)]
[decl_include(<bits/timespec.h>)]
[requires(defined(__CRT_HAVE_pthread_cond_timedwait) || defined(__CRT_HAVE_pthread_cond_timedwait64))]
pthread_cond_timedwait:([nonnull] pthread_cond_t *__restrict cond,
                        [nonnull] pthread_mutex_t *__restrict mutex,
                        [nonnull] struct timespec const *__restrict abstime) -> int {
#ifdef __CRT_HAVE_pthread_cond_timedwait
	int result;
	struct timespec32 abstime32;
	abstime32.@tv_sec@  = (time32_t)abstime->@tv_sec@;
	abstime32.@tv_nsec@ = abstime->@tv_nsec@;
	result = pthread_cond_timedwait32(cond, mutex, &abstime32);
	return result;
#else /* __CRT_HAVE_pthread_cond_timedwait */
	int result;
	struct timespec64 abstime64;
	abstime64.@tv_sec@  = (time64_t)abstime->@tv_sec@;
	abstime64.@tv_nsec@ = abstime->@tv_nsec@;
	result = pthread_cond_timedwait64(cond, mutex, &abstime64);
	return result;
#endif /* !__CRT_HAVE_pthread_cond_timedwait */
}

%#ifdef __USE_TIME64
[ignore][cp]
[decl_include(<bits/pthreadtypes.h>)]
[decl_include(<bits/timespec.h>)]
pthread_cond_timedwait32:([nonnull] pthread_cond_t *__restrict cond,
                          [nonnull] pthread_mutex_t *__restrict mutex,
                          [nonnull] struct timespec32 const *__restrict abstime)
		-> int = pthread_cond_timedwait?;

[cp][doc_alias(pthread_cond_timedwait)][time64_variant_of(pthread_cond_timedwait)]
[decl_include(<bits/pthreadtypes.h>)]
[decl_include(<bits/timespec.h>)]
[requires($has_function(pthread_cond_timedwait32))]
pthread_cond_timedwait64:([nonnull] pthread_cond_t *__restrict cond,
                          [nonnull] pthread_mutex_t *__restrict mutex,
                          [nonnull] struct timespec64 const *__restrict abstime) -> int {
	int result;
	struct timespec32 abstime32;
	abstime32.@tv_sec@  = (time32_t)abstime->@tv_sec@;
	abstime32.@tv_nsec@ = abstime->@tv_nsec@;
	result = pthread_cond_timedwait32(cond, mutex, &abstime32);
	return result;
}

%#endif /* __USE_TIME64 */


%
%/* Functions for handling condition variable attributes. */

%
@@Initialize condition variable attribute ATTR
pthread_condattr_init:([nonnull] pthread_condattr_t *attr) -> int;

%
@@Destroy condition variable attribute ATTR
pthread_condattr_destroy:([nonnull] pthread_condattr_t *attr) -> int;

%
@@Get the process-shared flag of the condition variable attribute ATTR
pthread_condattr_getpshared:([nonnull] pthread_condattr_t const *__restrict attr,
                             [nonnull] int *__restrict pshared) -> int;

%
@@Set the process-shared flag of the condition variable attribute ATTR
pthread_condattr_setpshared:([nonnull] pthread_condattr_t *attr, int pshared) -> int;

%
%#ifdef __USE_XOPEN2K

@@Get the clock selected for the condition variable attribute ATTR
pthread_condattr_getclock:([nonnull] pthread_condattr_t const *__restrict attr,
                           [nonnull] $clockid_t *__restrict clock_id) -> int;

@@Set the clock selected for the condition variable attribute ATTR
pthread_condattr_setclock:([nonnull] pthread_condattr_t *attr, $clockid_t clock_id) -> int;
%#endif /* __USE_XOPEN2K */

%
%#ifdef __USE_XOPEN2K
%/* Functions to handle spinlocks. */


%
@@Initialize the spinlock LOCK. If PSHARED is nonzero the
@@spinlock can be shared between different processes
pthread_spin_init:([nonnull] pthread_spinlock_t *lock, int pshared) -> int;

%
@@Destroy the spinlock LOCK
pthread_spin_destroy:([nonnull] pthread_spinlock_t *lock) -> int;

%
@@Wait until spinlock LOCK is retrieved
pthread_spin_lock:([nonnull] pthread_spinlock_t *lock) -> int;

%
@@Try to lock spinlock LOCK
pthread_spin_trylock:([nonnull] pthread_spinlock_t *lock) -> int;

%
@@Release spinlock LOCK
pthread_spin_unlock:([nonnull] pthread_spinlock_t *lock) -> int;


%
%/* Functions to handle barriers. */

%
@@Initialize BARRIER with the attributes in ATTR.
@@The barrier is opened when COUNT waiters arrived
pthread_barrier_init:([nonnull] pthread_barrier_t *__restrict barrier,
                      [inp(count)] pthread_barrierattr_t const *__restrict attr,
                      unsigned int count) -> int;

%
@@Destroy a previously dynamically initialized barrier BARRIER
pthread_barrier_destroy:([nonnull] pthread_barrier_t *barrier) -> int;

%
@@Wait on barrier BARRIER
[cp] pthread_barrier_wait:([nonnull] pthread_barrier_t *barrier) -> int;

%
@@Initialize barrier attribute ATTR
pthread_barrierattr_init:([nonnull] pthread_barrierattr_t *attr) -> int;

%
@@Destroy previously dynamically initialized barrier attribute ATTR
pthread_barrierattr_destroy:([nonnull] pthread_barrierattr_t *attr) -> int;

%
@@Get the process-shared flag of the barrier attribute ATTR
pthread_barrierattr_getpshared:([nonnull] pthread_barrierattr_t const *__restrict attr,
                                [nonnull] int *__restrict pshared) -> int;

%
@@Set the process-shared flag of the barrier attribute ATTR
pthread_barrierattr_setpshared:([nonnull] pthread_barrierattr_t *attr, int pshared) -> int;

%#endif /* __USE_XOPEN2K */

%
%/* Functions for handling thread-specific data. */


%{
#ifndef ____pthread_destr_function_t_defined
#define ____pthread_destr_function_t_defined 1
typedef void (__LIBCCALL *__pthread_destr_function_t)(void *);
#endif /* !____pthread_destr_function_t_defined */
}


%
@@Create a key value identifying a location in the thread-specific
@@data area. Each thread maintains a distinct thread-specific data
@@area. DESTR_FUNCTION, if non-NULL, is called with the value
@@associated to that key when the key is destroyed.
@@DESTR_FUNCTION is not called if the value associated is NULL when
@@the key is destroyed
[decl_prefix(
#ifndef ____pthread_destr_function_t_defined
#define ____pthread_destr_function_t_defined 1
typedef void (__LIBCCALL *__pthread_destr_function_t)(void *);
#endif /* !____pthread_destr_function_t_defined */
)]
pthread_key_create:([nonnull] pthread_key_t *key,
                    [nullable] __pthread_destr_function_t destr_function) -> int;

%
@@Destroy KEY
[export_alias(tss_delete)]
pthread_key_delete:(pthread_key_t key) -> int;

%
@@Return current value of the thread-specific data slot identified by KEY
[export_alias(tss_get)] pthread_getspecific:(pthread_key_t key) -> void *;

%
@@Store POINTER in the thread-specific data slot identified by KEY
pthread_setspecific:(pthread_key_t key, void const *pointer) -> int;

%
%#ifdef __USE_XOPEN2K
@@Get ID of CPU-time clock for thread THREAD_ID
pthread_getcpuclockid:(pthread_t pthread_id, [nonnull] $clockid_t *clock_id) -> int;
%#endif /* __USE_XOPEN2K */

%
%typedef void (__LIBCCALL *__pthread_atfork_func_t)(void);

%
@@Install handlers to be called when a new process is created with FORK.
@@The PREPARE handler is called in the parent process just before performing
@@FORK. The PARENT handler is called in the parent process just after FORK.
@@The CHILD handler is called in the child process. Each of the three
@@handlers can be NULL, meaning that no handler needs to be called at that
@@point.
@@PTHREAD_ATFORK can be called several times, in which case the PREPARE
@@handlers are called in LIFO order (last added with PTHREAD_ATFORK,
@@first called before FORK), and the PARENT and CHILD handlers are called
@@in FIFO (first added, first called)
pthread_atfork:([nullable] __pthread_atfork_func_t prepare,
                [nullable] __pthread_atfork_func_t parent,
                [nullable] __pthread_atfork_func_t child) -> int;



%{
#endif /* __CC__ */

__SYSDECL_END

}