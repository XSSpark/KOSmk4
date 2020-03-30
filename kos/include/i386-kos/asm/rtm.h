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
 *    in a product, an acknowledgement (see the following) in the product     *
 *    documentation is required:                                              *
 *    Portions Copyright (c) 2019-2020 Griefer@Work                           *
 * 2. Altered source versions must be plainly marked as such, and must not be *
 *    misrepresented as being the original software.                          *
 * 3. This notice may not be removed or altered from any source distribution. *
 */
#ifndef _I386_KOS_ASM_RTM_H
#define _I386_KOS_ASM_RTM_H 1

#include <__stdinc.h>

#include <hybrid/typecore.h>

/* Hardware defined RTM transaction status codes.
 * NOTE: A value of `0' is possible, and indicates that the transaction
 *       cannot be performed atomically (but that the fallback-path must
 *       be executed instead) */
#define _XABORT_FAILED   __UINT32_C(0x00000000) /*  */
#define _XABORT_EXPLICIT __UINT32_C(0x00000001) /* Abort was caused by `xabort' */
#define _XABORT_RETRY    __UINT32_C(0x00000002) /* The transaction may succeed if re-attempted */
#define _XABORT_CONFLICT __UINT32_C(0x00000004) /* Transaction failed due to an address-conflict with another processor */
#define _XABORT_CAPACITY __UINT32_C(0x00000008) /* The internal buffer to track transactions overflowed */
#define _XABORT_DEBUG    __UINT32_C(0x00000010) /* A #DB (%drN) or #BP (int3) was triggered */
#define _XABORT_NESTED   __UINT32_C(0x00000020) /* A nested transaction failed */
#define _XABORT_CODE_M   __UINT32_C(0xff000000) /*  */
#define _XABORT_CODE_S   24
#define _XABORT_CODE(x)  (((x) & _XABORT_CODE_M) >> _XABORT_CODE_S)

/* Returned by `__xbegin()' when RTM was entered successfully. */
#define _XBEGIN_STARTED __UINT32_C(0xffffffff)

#ifdef __CC__
__DECL_BEGIN

/* Start a transaction
 * @return: _XBEGIN_STARTED: Transaction started.
 * @return: _XABORT_* :      Transaction failed. */
#if __has_builtin(__builtin_ia32_xbegin)
__FORCELOCAL __UINT32_TYPE__(__xbegin)(void) {
	return (__UINT32_TYPE__)__builtin_ia32_xbegin();
}
#else /* __has_builtin(__builtin_ia32_xbegin) */
__FORCELOCAL __UINT32_TYPE__(__xbegin)(void) {
	__register __UINT32_TYPE__ __result;
	__asm__ __volatile__("xbegin 991f\n\t"
	                     "movl $-1, %%eax\n\t"
	                     "991:"
	                     : "=a" (__result));
	return __result;
}
#endif /* !__has_builtin(__builtin_ia32_xbegin) */


/* End a transaction
 * If the transaction was successful, return normally.
 * If the transaction failed, `__xbegin()' returns `_XABORT_*'
 * If no transaction was in progress, trigger #GP(0) */
#if __has_builtin(__builtin_ia32_xend)
__FORCELOCAL void (__xend)(void) {
	__builtin_ia32_xend();
}
#else /* __has_builtin(__builtin_ia32_xend) */
__FORCELOCAL void (__xend)(void) {
	__asm__ __volatile__("xend");
}
#endif /* !__has_builtin(__builtin_ia32_xend) */


/* Abort the current transaction by having `__xbegin()'
 * return with `_XABORT_EXPLICIT | ((code & 0xff) << _XABORT_CODE_S)'
 * If no transaction was in progress, behave as a no-op */
#if __has_builtin(__builtin_ia32_xend)
#define __xabort(code) __builtin_ia32_xabort(code)
#elif !defined(__NO_XBLOCK)
#define __xabort(code) __XBLOCK({ __asm__ __volatile__("xabort %0" : : "N" (code)); (void)0; })
#else /* ... */
#define __xabort(code) __asm__ __volatile__("xabort %0" : : "N" (code))
#endif /* !... */


/* Check if a transaction is currently in progress */
#if __has_builtin(__builtin_ia32_xtest)
__FORCELOCAL __BOOL (__xtest)(void) {
	return __builtin_ia32_xtest();
}
#else /* __has_builtin(__builtin_ia32_xtest) */
__FORCELOCAL __BOOL (__xtest)(void) {
	__BOOL __result;
	__asm__ __volatile__("xtest" : "=@ccnz" (__result));
	return __result;
}
#endif /* !__has_builtin(__builtin_ia32_xtest) */

__DECL_END
#endif /* __CC__ */


#endif /* !_I386_KOS_ASM_RTM_H */
