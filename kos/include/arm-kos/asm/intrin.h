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
#ifndef _ARM_KOS_ASM_INTRIN_H
#define _ARM_KOS_ASM_INTRIN_H 1

#include <__stdinc.h>

#include <hybrid/typecore.h>

#ifdef __CC__
__DECL_BEGIN

#ifdef __COMPILER_HAVE_GCC_ASM

/* Get/set ApplicationProgramStatusRegister (only status flags are writable) */
__FORCELOCAL __ATTR_WUNUSED __UINT32_TYPE__ (__rdapsr)(void) { __UINT32_TYPE__ __apsr; __asm__ __volatile__("mrs %0, APSR" : "=r" (__apsr)); return __apsr; }
__FORCELOCAL void (__wrapsr)(__UINT32_TYPE__ __apsr) { __asm__ __volatile__("msr APSR_nzcvq, %0" : : "Ir" (__apsr)); }

/* Get/set CurrentProgramStatusRegister (super-set of APSR; only accessible to kernel) */
__FORCELOCAL __ATTR_WUNUSED __UINT32_TYPE__ (__rdcpsr)(void) { __UINT32_TYPE__ __cpsr; __asm__ __volatile__("mrs %0, CPSR" : "=r" (__cpsr)); return __cpsr; }
__FORCELOCAL void (__wrcpsr)(__UINT32_TYPE__ __cpsr) { __asm__ __volatile__("msr CPSR_fcxs, %0" : : "Ir" (__cpsr)); }

/* Get/set SavedProgramStatusRegister (in kernel: user-space program status register) */
__FORCELOCAL __ATTR_WUNUSED __UINT32_TYPE__ (__rdspsr)(void) { __UINT32_TYPE__ __cpsr; __asm__ __volatile__("mrs %0, SPSR" : "=r" (__cpsr)); return __cpsr; }
__FORCELOCAL void (__wrspsr)(__UINT32_TYPE__ __cpsr) { __asm__ __volatile__("msr SPSR_fcxs, %0" : : "Ir" (__cpsr)); }


/* General purpose registers */
#ifdef __COMPILER_HAVE_REGISTER_VARS
__FORCELOCAL __ATTR_WUNUSED __UINTPTR_TYPE__ (__rdr0)(void) { __register_var(__UINTPTR_TYPE__, __r0, "r0"); return __r0; }
__FORCELOCAL __ATTR_WUNUSED __UINTPTR_TYPE__ (__rdr1)(void) { __register_var(__UINTPTR_TYPE__, __r1, "r1"); return __r1; }
__FORCELOCAL __ATTR_WUNUSED __UINTPTR_TYPE__ (__rdr2)(void) { __register_var(__UINTPTR_TYPE__, __r2, "r2"); return __r2; }
__FORCELOCAL __ATTR_WUNUSED __UINTPTR_TYPE__ (__rdr3)(void) { __register_var(__UINTPTR_TYPE__, __r3, "r3"); return __r3; }
__FORCELOCAL __ATTR_WUNUSED __UINTPTR_TYPE__ (__rdr4)(void) { __register_var(__UINTPTR_TYPE__, __r4, "r4"); return __r4; }
__FORCELOCAL __ATTR_WUNUSED __UINTPTR_TYPE__ (__rdr5)(void) { __register_var(__UINTPTR_TYPE__, __r5, "r5"); return __r5; }
__FORCELOCAL __ATTR_WUNUSED __UINTPTR_TYPE__ (__rdr6)(void) { __register_var(__UINTPTR_TYPE__, __r6, "r6"); return __r6; }
__FORCELOCAL __ATTR_WUNUSED __UINTPTR_TYPE__ (__rdr7)(void) { __register_var(__UINTPTR_TYPE__, __r7, "r7"); return __r7; }
__FORCELOCAL __ATTR_WUNUSED __UINTPTR_TYPE__ (__rdr8)(void) { __register_var(__UINTPTR_TYPE__, __r8, "r8"); return __r8; }
__FORCELOCAL __ATTR_WUNUSED __UINTPTR_TYPE__ (__rdr9)(void) { __register_var(__UINTPTR_TYPE__, __r9, "r9"); return __r9; }
__FORCELOCAL __ATTR_WUNUSED __UINTPTR_TYPE__ (__rdr10)(void) { __register_var(__UINTPTR_TYPE__, __r10, "r10"); return __r10; }
__FORCELOCAL __ATTR_WUNUSED __UINTPTR_TYPE__ (__rdr11)(void) { __register_var(__UINTPTR_TYPE__, __r11, "r11"); return __r11; }
__FORCELOCAL __ATTR_WUNUSED __UINTPTR_TYPE__ (__rdr12)(void) { __register_var(__UINTPTR_TYPE__, __r12, "r12"); return __r12; }
__FORCELOCAL __ATTR_WUNUSED void *(__rdsp)(void) { __register_var(void *, __sp, "sp"); return __sp; }
__FORCELOCAL __ATTR_WUNUSED void *(__rdlr)(void) { __register_var(void *, __lr, "lr"); return __lr; }
__FORCELOCAL __ATTR_WUNUSED void *(__rdpc)(void) { __register_var(void *, __pc, "pc"); return __pc; }
__FORCELOCAL void (__wrr0)(__UINTPTR_TYPE__ __val) { __register_var(__UINTPTR_TYPE__, __r0, "r0"); __r0 = __val; }
__FORCELOCAL void (__wrr1)(__UINTPTR_TYPE__ __val) { __register_var(__UINTPTR_TYPE__, __r1, "r1"); __r1 = __val; }
__FORCELOCAL void (__wrr2)(__UINTPTR_TYPE__ __val) { __register_var(__UINTPTR_TYPE__, __r2, "r2"); __r2 = __val; }
__FORCELOCAL void (__wrr3)(__UINTPTR_TYPE__ __val) { __register_var(__UINTPTR_TYPE__, __r3, "r3"); __r3 = __val; }
__FORCELOCAL void (__wrr4)(__UINTPTR_TYPE__ __val) { __register_var(__UINTPTR_TYPE__, __r4, "r4"); __r4 = __val; }
__FORCELOCAL void (__wrr5)(__UINTPTR_TYPE__ __val) { __register_var(__UINTPTR_TYPE__, __r5, "r5"); __r5 = __val; }
__FORCELOCAL void (__wrr6)(__UINTPTR_TYPE__ __val) { __register_var(__UINTPTR_TYPE__, __r6, "r6"); __r6 = __val; }
__FORCELOCAL void (__wrr7)(__UINTPTR_TYPE__ __val) { __register_var(__UINTPTR_TYPE__, __r7, "r7"); __r7 = __val; }
__FORCELOCAL void (__wrr8)(__UINTPTR_TYPE__ __val) { __register_var(__UINTPTR_TYPE__, __r8, "r8"); __r8 = __val; }
__FORCELOCAL void (__wrr9)(__UINTPTR_TYPE__ __val) { __register_var(__UINTPTR_TYPE__, __r9, "r9"); __r9 = __val; }
__FORCELOCAL void (__wrr10)(__UINTPTR_TYPE__ __val) { __register_var(__UINTPTR_TYPE__, __r10, "r10"); __r10 = __val; }
__FORCELOCAL void (__wrr11)(__UINTPTR_TYPE__ __val) { __register_var(__UINTPTR_TYPE__, __r11, "r11"); __r11 = __val; }
__FORCELOCAL void (__wrr12)(__UINTPTR_TYPE__ __val) { __register_var(__UINTPTR_TYPE__, __r12, "r12"); __r12 = __val; }
__FORCELOCAL void (__wrsp)(void *__val) { __register_var(void *, __sp, "sp"); __sp = __val; }
__FORCELOCAL void (__wrlr)(void *__val) { __register_var(void *, __lr, "lr"); __lr = __val; }
__FORCELOCAL __ATTR_NORETURN void (__wrpc)(void *__val) { __register_var(void *, __pc, "pc"); __pc = __val; }
#else /* __COMPILER_HAVE_REGISTER_VARS */
__FORCELOCAL __ATTR_WUNUSED __UINTPTR_TYPE__ (__rdr0)(void) { __UINTPTR_TYPE__ __res; __asm__ __volatile__("mov %0, r0" : "=r" (__res)); return __res; }
__FORCELOCAL __ATTR_WUNUSED __UINTPTR_TYPE__ (__rdr1)(void) { __UINTPTR_TYPE__ __res; __asm__ __volatile__("mov %0, r1" : "=r" (__res)); return __res; }
__FORCELOCAL __ATTR_WUNUSED __UINTPTR_TYPE__ (__rdr2)(void) { __UINTPTR_TYPE__ __res; __asm__ __volatile__("mov %0, r2" : "=r" (__res)); return __res; }
__FORCELOCAL __ATTR_WUNUSED __UINTPTR_TYPE__ (__rdr3)(void) { __UINTPTR_TYPE__ __res; __asm__ __volatile__("mov %0, r3" : "=r" (__res)); return __res; }
__FORCELOCAL __ATTR_WUNUSED __UINTPTR_TYPE__ (__rdr4)(void) { __UINTPTR_TYPE__ __res; __asm__ __volatile__("mov %0, r4" : "=r" (__res)); return __res; }
__FORCELOCAL __ATTR_WUNUSED __UINTPTR_TYPE__ (__rdr5)(void) { __UINTPTR_TYPE__ __res; __asm__ __volatile__("mov %0, r5" : "=r" (__res)); return __res; }
__FORCELOCAL __ATTR_WUNUSED __UINTPTR_TYPE__ (__rdr6)(void) { __UINTPTR_TYPE__ __res; __asm__ __volatile__("mov %0, r6" : "=r" (__res)); return __res; }
__FORCELOCAL __ATTR_WUNUSED __UINTPTR_TYPE__ (__rdr7)(void) { __UINTPTR_TYPE__ __res; __asm__ __volatile__("mov %0, r7" : "=r" (__res)); return __res; }
__FORCELOCAL __ATTR_WUNUSED __UINTPTR_TYPE__ (__rdr8)(void) { __UINTPTR_TYPE__ __res; __asm__ __volatile__("mov %0, r8" : "=r" (__res)); return __res; }
__FORCELOCAL __ATTR_WUNUSED __UINTPTR_TYPE__ (__rdr9)(void) { __UINTPTR_TYPE__ __res; __asm__ __volatile__("mov %0, r9" : "=r" (__res)); return __res; }
__FORCELOCAL __ATTR_WUNUSED __UINTPTR_TYPE__ (__rdr10)(void) { __UINTPTR_TYPE__ __res; __asm__ __volatile__("mov %0, r10" : "=r" (__res)); return __res; }
__FORCELOCAL __ATTR_WUNUSED __UINTPTR_TYPE__ (__rdr11)(void) { __UINTPTR_TYPE__ __res; __asm__ __volatile__("mov %0, r11" : "=r" (__res)); return __res; }
__FORCELOCAL __ATTR_WUNUSED __UINTPTR_TYPE__ (__rdr12)(void) { __UINTPTR_TYPE__ __res; __asm__ __volatile__("mov %0, r12" : "=r" (__res)); return __res; }
__FORCELOCAL __ATTR_WUNUSED void *(__rdsp)(void) { void *__res; __asm__ __volatile__("mov %0, sp" : "=r" (__res)); return __res; }
__FORCELOCAL __ATTR_WUNUSED void *(__rdlr)(void) { void *__res; __asm__ __volatile__("mov %0, lr" : "=r" (__res)); return __res; }
__FORCELOCAL __ATTR_WUNUSED void *(__rdpc)(void) { void *__res; __asm__ __volatile__("mov %0, pc" : "=r" (__res)); return __res; }
__FORCELOCAL void (__wrr0)(__UINTPTR_TYPE__ __val) { __asm__ __volatile__("mov r0, %0" : : "r" (__val) : "r0"); }
__FORCELOCAL void (__wrr1)(__UINTPTR_TYPE__ __val) { __asm__ __volatile__("mov r1, %0" : : "r" (__val) : "r1"); }
__FORCELOCAL void (__wrr2)(__UINTPTR_TYPE__ __val) { __asm__ __volatile__("mov r2, %0" : : "r" (__val) : "r2"); }
__FORCELOCAL void (__wrr3)(__UINTPTR_TYPE__ __val) { __asm__ __volatile__("mov r3, %0" : : "r" (__val) : "r3"); }
__FORCELOCAL void (__wrr4)(__UINTPTR_TYPE__ __val) { __asm__ __volatile__("mov r4, %0" : : "r" (__val) : "r4"); }
__FORCELOCAL void (__wrr5)(__UINTPTR_TYPE__ __val) { __asm__ __volatile__("mov r5, %0" : : "r" (__val) : "r5"); }
__FORCELOCAL void (__wrr6)(__UINTPTR_TYPE__ __val) { __asm__ __volatile__("mov r6, %0" : : "r" (__val) : "r6"); }
__FORCELOCAL void (__wrr7)(__UINTPTR_TYPE__ __val) { __asm__ __volatile__("mov r7, %0" : : "r" (__val) : "r7"); }
__FORCELOCAL void (__wrr8)(__UINTPTR_TYPE__ __val) { __asm__ __volatile__("mov r8, %0" : : "r" (__val) : "r8"); }
__FORCELOCAL void (__wrr9)(__UINTPTR_TYPE__ __val) { __asm__ __volatile__("mov r9, %0" : : "r" (__val) : "r9"); }
__FORCELOCAL void (__wrr10)(__UINTPTR_TYPE__ __val) { __asm__ __volatile__("mov r10, %0" : : "r" (__val) : "r10"); }
__FORCELOCAL void (__wrr11)(__UINTPTR_TYPE__ __val) { __asm__ __volatile__("mov r11, %0" : : "r" (__val) : "r11"); }
__FORCELOCAL void (__wrr12)(__UINTPTR_TYPE__ __val) { __asm__ __volatile__("mov r12, %0" : : "r" (__val) : "r12"); }
__FORCELOCAL void (__wrsp)(void *__val) { __asm__ __volatile__("mov sp, %0" : : "r" (__val) : "sp"); }
__FORCELOCAL void (__wrlr)(void *__val) { __asm__ __volatile__("mov lr, %0" : : "r" (__val) : "lr"); }
__FORCELOCAL __ATTR_NORETURN void (__wrpc)(void *__val) { __asm__ __volatile__("mov pc, %0" : : "r" (__val) : "pc"); }
#endif /* !__COMPILER_HAVE_REGISTER_VARS */


#endif /* __COMPILER_HAVE_GCC_ASM */

__DECL_END
#endif /* __CC__ */


#endif /* !_ARM_KOS_ASM_INTRIN_H */