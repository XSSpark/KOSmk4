/* Copyright (c) 2019 Griefer@Work                                            *
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
#ifndef GUARD_KERNEL_CORE_ARCH_I386_SCHED_FPU_C
#define GUARD_KERNEL_CORE_ARCH_I386_SCHED_FPU_C 1
#define _KOS_SOURCE 1

#include <kernel/compiler.h>

#include <kernel/cpuid.h>
#include <kernel/driver-param.h>
#include <kernel/except.h>
#include <kernel/fpu.h>
#include <kernel/heap.h>
#include <kernel/malloc.h>
#include <kernel/printk.h>
#include <kernel/types.h>
#include <sched/pid.h>
#include <sched/task.h>

#include <hybrid/atomic.h>

#include <asm/cpu-cpuid.h>
#include <asm/cpu-flags.h>
#include <asm/intrin-fpu.h>
#include <asm/intrin.h>
#include <kos/kernel/cpu-state-compat.h>
#include <kos/kernel/cpu-state.h>
#include <kos/kernel/fpu-state.h>
#include <kos/kernel/fpu-state32.h>

#include <assert.h>
#include <stddef.h>
#include <string.h>

DECL_BEGIN

#define FPU_GFP  (GFP_NORMAL)

#ifndef CONFIG_NO_FPU

/* [const] The type of FPU state used (One of `FPU_STATE_*') */
PUBLIC unsigned int x86_fpustate_variant_ ASMNAME("x86_fpustate_variant") = FPU_STATE_XSTATE;

/* [0..1] The task associated with the current FPU register contents, or NULL if none.
 * NOTE: When accessing this field, preemption must be disabled, as
 *       this field affects the behavior of task state switches. */
PUBLIC ATTR_PERCPU struct task *x86_fpu_current = NULL;

/* [0..1][owned] The per-task FPU state (lazily allocated) */
PUBLIC ATTR_PERTASK struct fpustate *_this_fpustate = NULL;

DEFINE_PERTASK_ONEXIT(pertask_cleanup_fpustate);
INTERN NOBLOCK void
NOTHROW(KCALL pertask_cleanup_fpustate)(void) {
	/* Unset the calling thread potentially holding the FPU state.
	 * Since the task will go away, we don't actually have to save it. */
	ATOMIC_CMPXCH(PERCPU(x86_fpu_current), THIS_TASK, NULL);
}

DEFINE_PERTASK_FINI(pertask_finalize_fpustate);
INTERN NOBLOCK void
NOTHROW(KCALL pertask_finalize_fpustate)(struct task *__restrict self) {
	fpustate_free(FORTASK(self, _this_fpustate));
}

#if 0 /* No need... */
DEFINE_PERTASK_CLONE(clone_fpustate);
PRIVATE ATTR_USED void KCALL
clone_fpustate(struct task *__restrict new_thread, uintptr_t UNUSED(flags)) {
	struct fpustate *fst;
	fst = PERTASK_GET(_this_fpustate);
	if (fst) {
		struct fpustate *copy;
		fpustate_save();
		copy = fpustate_alloc_noinit();
		memcpy(copy, fst, sizeof(struct fpustate));
	}
}
#endif


PRIVATE WUNUSED ATTR_RETNONNULL ATTR_MALLOC struct fpustate *KCALL
fpustate_alloc_noinit(void) {
	/* TODO: Slab-style cache */
	return (struct fpustate *)kmemalign(ALIGNOF_FPUSTATE,
	                                    SIZEOF_FPUSTATE,
	                                    FPU_GFP);
}

PUBLIC WUNUSED ATTR_RETNONNULL ATTR_MALLOC struct fpustate *KCALL
fpustate_alloc(void) {
	struct fpustate *result;
	/* TODO: Slab-style cache */
	result = (struct fpustate *)kmemalign(ALIGNOF_FPUSTATE,
	                                      SIZEOF_FPUSTATE,
	                                      FPU_GFP | GFP_CALLOC);
	x86_fpustate_init(result);
	return result;
}

PUBLIC WUNUSED ATTR_MALLOC struct fpustate *
NOTHROW(KCALL fpustate_alloc_nx)(void) {
	struct fpustate *result;
	/* TODO: Slab-style cache */
	result = (struct fpustate *)kmemalign_nx(ALIGNOF_FPUSTATE,
	                                         SIZEOF_FPUSTATE,
	                                         FPU_GFP | GFP_CALLOC);
	if likely(result)
		x86_fpustate_init(result);
	return result;
}
PUBLIC NOBLOCK void
NOTHROW(KCALL fpustate_free)(struct fpustate *__restrict self) {
	/* TODO: Slab-style cache */
	kfree(self);
}



/* Ensure that `_this_fpustate' has been allocated, allocating
 * and initializing it now if it hasn't already. */
PUBLIC void KCALL fpustate_init(void) THROWS(E_BADALLOC) {
	if (PERTASK_GET(_this_fpustate))
		return;
	PERTASK_SET(_this_fpustate, fpustate_alloc());
}

PUBLIC WUNUSED bool NOTHROW(KCALL fpustate_init_nx)(void) {
	struct fpustate *state;
	if (PERTASK_GET(_this_fpustate))
		return true;
	state = fpustate_alloc_nx();
	if unlikely(!state)
		return false;
	PERTASK_SET(_this_fpustate, state);
	return true;
}

/* Save the FPU context of the calling thread.
 * This functions are no-ops if the calling thread wasn't the
 * last one to use the FPU, or has never used the FPU at all. */
PUBLIC NOBLOCK void NOTHROW(KCALL fpustate_save)(void) {
	pflag_t was;
	if (PERCPU(x86_fpu_current) != THIS_TASK)
		return;
	was = PREEMPTION_PUSHOFF();
	COMPILER_READ_BARRIER();
	if (PERCPU(x86_fpu_current) == THIS_TASK) {
		__clts();
		/* The FPU state was changed. */
		assert(PERTASK_GET(_this_fpustate));
		x86_fpustate_save(PERTASK_GET(_this_fpustate));
		__wrcr0(__rdcr0() | CR0_TS);
		PERCPU(x86_fpu_current) = NULL;
	}
	PREEMPTION_POP(was);
}

/* Similar to `fpustate_save()', but save the state of whichever thread is
 * currently holding the active FPU context. - When called, this function
 * will ensure that the most-up-to-date FPU context is written to memory,
 * and will be read back from memory the next time that thread (if any)
 * performs another FPU operation.
 * The main purpose of this function is to aid in implementing FPU support
 * in debuggers, where this function is called when suspending execution of
 * the associated CPU, after which the debugger can read/write FPU information
 * for any thread by simply looking at `PERTASK(thread, _this_fpustate)' */
PUBLIC NOBLOCK void NOTHROW(KCALL fpustate_savecpu)(void) {
	pflag_t was;
	struct task *holder;
	was = PREEMPTION_PUSHOFF();
	holder = PERCPU(x86_fpu_current);
	if (holder) {
		assert(!wasdestroyed(holder));
		__clts();
		/* The FPU state was changed. */
		assertf(FORTASK(holder, _this_fpustate),
		        "This should have been allocated the first time thread %p used the FPU",
		        holder);
		/* Save the context */
		x86_fpustate_save(FORTASK(holder, _this_fpustate));
		/* Disable FPU access (so-as to lazily re-load it
		 * the next time an access is made by `holder') */
		__wrcr0(__rdcr0() | CR0_TS);
		PERCPU(x86_fpu_current) = NULL;
	}
	PREEMPTION_POP(was);
}




/* Load / Save the FPU context of the calling thread to/from the given `state'
 * If no FPU state had yet to be allocated when `fpustate_loadfrom()' is called,
 * a new state will be allocated before returning. */
PUBLIC NOBLOCK void KCALL
fpustate_loadfrom(USER CHECKED struct fpustate const *state)
		THROWS(E_SEGFAULT, E_BADALLOC) {
	struct fpustate *mystate;
	mystate = PERTASK_GET(_this_fpustate);
	if (mystate) {
		pflag_t was;
		was = PREEMPTION_PUSHOFF();
		/* Make sure that the calling thread isn't the current FPU user. */
		if (PERCPU(x86_fpu_current) == THIS_TASK) {
			PERCPU(x86_fpu_current) = NULL;
			__wrcr0(__rdcr0() | CR0_TS);
		}
		PREEMPTION_POP(was);
		/* Copy the given state to set it as the new FPU state. */
		memcpy(mystate, state, SIZEOF_FPUSTATE);
	} else {
		mystate = fpustate_alloc_noinit();
		TRY {
			memcpy(mystate, state, SIZEOF_FPUSTATE);
		} EXCEPT {
			fpustate_free(mystate);
			RETHROW();
		}
		/* Save the newly allocated FPU state. */
		PERTASK_SET(_this_fpustate, mystate);
	}
}

PUBLIC NOBLOCK void KCALL
fpustate_saveinto(USER CHECKED struct fpustate *state)
		THROWS(E_SEGFAULT) {
	if (!PERTASK_GET(_this_fpustate)) {
		memset(state, 0, SIZEOF_FPUSTATE);
		x86_fpustate_init(state);
		return;
	}
	/* Make sure that the calling thread's FPU state has been saved. */
	fpustate_save();
	memcpy(state, PERTASK_GET(_this_fpustate), SIZEOF_FPUSTATE);
}



INTERN struct icpustate *FCALL /* #NM */
x86_handle_device_not_available(struct icpustate *__restrict state) {
	struct task *old_task, *new_task;
	old_task = PERCPU(x86_fpu_current);
	new_task = THIS_TASK;
	COMPILER_BARRIER();
	if (new_task == old_task) {
		/* No other thread used the FPU in the mean time.
		 * We get here when preemption disabled the FPU, only for the original
		 * thread to be the only one to access the FPU once it loops back around. */
		__clts();
	} else {
		struct fpustate *mystate;
		printk(KERN_INFO "Switch FPU context from task %p[pid=%u] to %p[pid=%u] [pc=%p]\n",
		       old_task, old_task ? task_getroottid_of_s(old_task) : 0,
		       new_task, task_getroottid_of_s(new_task),
		       state->ics_irregs.ir_pip);
		mystate = PERTASK_GET(_this_fpustate);
		if (!mystate) {
			/* Try to have interrupts enabled when allocating memory.
			 * By doing this, we prevent the allocation failing with `E_WOULDBLOCK'
			 * if at some point it needs to do something that may block. */
			if (state->ics_irregs.ir_pflags & EFLAGS_IF)
				__sti();
			/* Lazily allocate a new state upon first access.
			 * NOTE: If this causes an exception, that exception
			 *       will be propagated to user-space. */
			mystate = fpustate_alloc();
			__cli();
			PERTASK_SET(_this_fpustate, mystate);
			/* Because we (may) had interrupts enabled again, we must re-read
			 * the currently active FPU task again, as it may have changed in
			 * the mean time. */
			COMPILER_READ_BARRIER();
			old_task = PERCPU(x86_fpu_current);
		}
		__clts();
		if (old_task) {
			/* Save the current state within the old context holder. */
			assert(FORTASK(old_task, _this_fpustate));
			x86_fpustate_save(FORTASK(old_task, _this_fpustate));
		}
		/* Load the new FPU state */
		TRY {
			x86_fpustate_load(mystate);
		} EXCEPT {
			/* Because we've saved the old task's state, a failure to load the
			 * new task's state will may have left the FPU in an undefined state.
			 * -> Because of this, we must indicate that no one is holding the
			 *    active FPU context at this point. */
			PERCPU(x86_fpu_current) = NULL;
			RETHROW();
		}
		PERCPU(x86_fpu_current) = THIS_TASK;
	}
	return state;
}


/* TODO: FPU Exception handlers. */
//INTDEF struct icpustate *FCALL x86_handle_coprocessor_fault(struct icpustate *__restrict state);
//INTDEF struct icpustate *FCALL x86_handle_fpu_x87(struct icpustate *__restrict state);
//INTDEF struct icpustate *FCALL x86_handle_fpu_simd(struct icpustate *__restrict state);



#define CPUID_FEATURES_WRITABLE  (*(struct cpuinfo *)&CPUID_FEATURES)


DATDEF u32 x86_fxsave_mxcsr_mask_ ASMNAME("x86_fxsave_mxcsr_mask");

INTDEF byte_t x86_fxsave_section_stmxcsr[];
INTDEF byte_t x86_fxsave_section_savexmm[];
INTDEF byte_t x86_fxrstor_section_ldmxcsr[];
INTDEF byte_t x86_fxrstor_section_loadxmm[];
INTDEF byte_t x86_fpustate_init_mxcsr[];
INTDEF byte_t x86_fpustate_init_ftw_offset_byte;
INTDEF u32 x86_fpustate_init_mxcsr_value;
INTDEF u16 x86_fpustate_init_ftw_value;


#define ENCODE32(x) (x) & 0xff, ((x) & 0xff00) >> 8, ((x) & 0xff0000) >> 16, ((x) & 0xff000000) >> 24
PRIVATE ATTR_FREERODATA byte_t const savexmm_nosse[] = {
	0x8d, 0xb9, ENCODE32(OFFSET_XFPUSTATE_XMM(0)), /* leal   OFFSET_XFPUSTATE_XMM(0)(%ecx), %edi */
	0xb9,       ENCODE32(8 * (16 / 4)),            /* movl   $(8 * (16 / 4)), %ecx               */
	0xf3, 0xab,                                    /* rep    stosl                               */
	0x5e,                                          /* pop    %esi                                */
	0x5f,                                          /* pop    %edi                                */
	0xc3                                           /* ret                                        */
};

FORCELOCAL ATTR_FREETEXT void KCALL inject_jmp(void *addr, void const *dst) {
	*(byte_t *)addr = 0xe9;
	*((s32 *)((byte_t *)addr + 1)) = (s32)((intptr_t)dst - ((intptr_t)addr + 5));
}

INTERN ATTR_FREEBSS bool x86_config_nofpu = false;
DEFINE_VERY_EARLY_KERNEL_COMMANDLINE_OPTION(x86_config_nofpu,
                                            KERNEL_COMMANDLINE_OPTION_TYPE_BOOL,
                                            "nofpu");

PRIVATE ATTR_FREETEXT void
NOTHROW(KCALL test_fpu64)(void) {
#define VAL1  __UINT64_C(0x0123456789abcdef)
#define VAL2  __UINT64_C(0xfedcba9876543210)
	volatile u64 value = VAL1;
	u64 v;
	v = ATOMIC_READ(value);
	assertf(v == VAL1, "v = %#I64x", v);
	ATOMIC_WRITE(value, VAL2);
	v = ATOMIC_READ(value);
	assertf(v == VAL2, "v = %#I64x", v);
}

/* Convert sfpustate's FTW to xfpustate's */
INTERN NOBLOCK u8
NOTHROW(FCALL x86_fxsave_compress_ftw)(struct sfpustate const *__restrict self) {
	unsigned int i;
	u16 ftw = self->fs_ftw;
	u8 res = 0;
	for (i = 0; i < 8; ++i) {
		if ((ftw & FTW_MASK(i)) != FTW_EMPTY(i))
			res |= 1 << i;
	}
	return res;
}

/* Convert xfpustate's FTW to sfpustate's
 * NOTE: Return value is actually a `u16', but use `u32'
 *       so assembly doesn't have to movzwl the value! */
INTERN NOBLOCK u32
NOTHROW(FCALL x86_fxsave_decompress_ftw)(struct xfpustate const *__restrict self) {
	unsigned int i;
	u8 ftw = self->fs_ftw;
	u32 res = 0;
	for (i = 0; i < 8; ++i) {
		if (ftw & (1 << i)) {
			/* s.a. `Table 3-45' in the Intel developer manual */
			if (self->fs_regs[i].ieee_nan.exponent == 0x7fff) {
				/* Special */
				res |= FTW_SPEC(i);
			} else if (self->fs_regs[i].ieee_nan.exponent == 0) {
				if (self->fs_regs[i].ieee_nan.mantissa1 == 0 &&
				    self->fs_regs[i].ieee_nan.mantissa0 == 0 &&
					self->fs_regs[i].ieee_nan.one == 0)
					res |= FTW_ZERO(i); /* Fraction all 0's (and j == 0) */
				else {
					res |= FTW_SPEC(i);
				}
			} else if (self->fs_regs[i].ieee_nan.one == 0) {
				res |= FTW_SPEC(i);
			} else {
				res |= FTW_VALID(i);
			}
		} else {
			res |= FTW_EMPTY(i);
		}
	}
	return res;
}


#define HAVE_FPU  (CPUID_FEATURES.ci_1d & CPUID_1D_FPU)
#define HAVE_FXSR (CPUID_FEATURES.ci_1d & CPUID_1D_FXSR)
#define HAVE_SSE  (CPUID_FEATURES.ci_1d & CPUID_1D_SSE)


INTERN ATTR_FREETEXT void NOTHROW(KCALL x86_initialize_fpu)(void) {
	u32 cr0;
	if (x86_config_nofpu) {
		printk(FREESTR(KERN_INFO "[fpu] Disable fpu support\n"));
		__wrcr0((__rdcr0() & ~(CR0_MP)) | (CR0_EM | CR0_TS));
setup_fpu_emulation:
		/* TODO: Add an FPU emulation library
		 *       Currently, enabling `nofpu' will cause a DOUBLE_FAULT the first
		 *       time we get into the #NM interrupt handler, because it will
		 *       recurse into itself... */
		return;
	}
	/* Check for the existence of an FPU. */
	cr0 = __rdcr0() & ~(CR0_EM | CR0_TS);
	if (!HAVE_FPU) {
		u16 testword = 0x55aa;
		__wrcr0(cr0);
		__fninit();
		__asm__ __volatile__("fnstsw %0" : "+m" (testword));
		if (testword != 0) {
			printk(FREESTR(KERN_INFO "[fpu] No co-processor detected\n"));
			cr0 &= ~CR0_MP; /* Disable for emulated FPU */
			cr0 |= CR0_EM | CR0_TS;
			__wrcr0(cr0);
			goto setup_fpu_emulation;
		}
		CPUID_FEATURES_WRITABLE.ci_1d |= CPUID_1D_FPU;
		CPUID_FEATURES_WRITABLE.ci_80000001d |= CPUID_80000001D_FPU;
	}
	printk(FREESTR(KERN_INFO "[fpu] Initializing co-processor\n"));
	if (!(cr0 & CR0_ET)) {
		/* TODO: Only set `CR0_ET' if we're connected to a 387 (as opposed to a 287) */
		cr0 |= CR0_ET;
	}
	if (PERCPU(cpu_features) & CPU_FEATURE_FI486)
		cr0 |= CR0_NE; /* Native exceptions enable */
	cr0 |= CR0_MP; /* Enable for native FPU */
	__wrcr0(cr0);

	/* Re-write `x86_fxsave()' and `x86_fxrstor()' based on processor capabilities. */
	if (HAVE_FXSR) {
		u32 cr4;
		cr4 = __rdcr4();
		if (HAVE_SSE) {
			printk(FREESTR(KERN_INFO "[fpu] Enable SSE & #XF exception support\n"));
			cr4 |= (CR4_OSFXSR/* | CR4_OSXMMEXCPT*/);
		}
		__wrcr4(cr4);
		if (!HAVE_SSE) {
			/* No SSE support */
			x86_fxsave_mxcsr_mask_ = 0;
		} else {
			struct xfpustate32 fst;
			fst.fs_mxcsr_mask = 0;
			__fninit();
			__fxsave(&fst);
			if (!fst.fs_mxcsr_mask) /* Intel says that zero should equate `0xffbf' */
				fst.fs_mxcsr_mask = 0x0000ffbf;
			x86_fxsave_mxcsr_mask_ = fst.fs_mxcsr_mask;
		}
		/* Mask the default MXCSR value with what is actually available */
		x86_fpustate_init_mxcsr_value &= x86_fxsave_mxcsr_mask_;
		printk(FREESTR(KERN_INFO "[fpu] Enable native fxsave/fxrstor support [mxcsr_mask=%#I32x]\n"),
		       x86_fxsave_mxcsr_mask_);
		/* The FXSAVE variants version of FTW works differently: a 0-bit means an empty register,
		 * and a 1-bit means a used register, whereas FSAVE assigns 2 bits per register, with 3
		 * meaning empty, and 0-2 meaning used, potentially with special values. */
		x86_fpustate_init_ftw_value = 0;

		((byte_t *)&x86_fxsave)[0] = 0x0f; /* fxsave (%ecx) */
		((byte_t *)&x86_fxsave)[1] = 0xae;
		((byte_t *)&x86_fxsave)[2] = 0x01;
		((byte_t *)&x86_fxsave)[3] = 0xc3; /* ret */

		((byte_t *)&x86_fxrstor)[0] = 0x0f; /* fxrstor (%ecx) */
		((byte_t *)&x86_fxrstor)[1] = 0xae;
		((byte_t *)&x86_fxrstor)[2] = 0x09;
		((byte_t *)&x86_fxrstor)[3] = 0xc3; /* ret */
	} else {
		if (HAVE_SSE) {
			/* x86_fxsave_mxcsr_mask_ = 0x0000ffbf; // Already the default */
			inject_jmp((void *)&x86_fpustate_load, (void const *)&x86_fxrstor);
			inject_jmp((void *)&x86_fpustate_save, (void const *)&x86_fxsave);
			inject_jmp((void *)&x86_fpustate_save_noreset, (void const *)&x86_fxsave);
			printk(FREESTR(KERN_INFO "[fpu] Enable #XF exception support\n"));
			__wrcr4(__rdcr4() | CR4_OSXMMEXCPT);
		} else {
			/* None of the MXCSR features are available. */
			x86_fxsave_mxcsr_mask_ = 0;

			/* Disable SSE support in `x86_fxsave()' and `x86_fxrstor()' */
			/* >> movl    %eax, OFFSET_XFPUSTATE_MXCSR(%ecx) */
			x86_fxsave_section_stmxcsr[0] = 0x89;
			x86_fxsave_section_stmxcsr[1] = 0x41;
			x86_fxsave_section_stmxcsr[2] = OFFSET_XFPUSTATE_MXCSR;
			x86_fxsave_section_stmxcsr[3] = 0x90; /* nop */
			/* >> leal   OFFSET_XFPUSTATE_XMM(0)(%ecx), %edi
			 * >> movl   $(8 * (16 / 4)), %ecx
			 * >> rep    stosl */
			memcpy(x86_fxsave_section_savexmm, savexmm_nosse, sizeof(savexmm_nosse));
			x86_fxrstor_section_ldmxcsr[0] = 0xc3; /* ret */
			x86_fxrstor_section_loadxmm[0] = 0xc3; /* ret */

			x86_fpustate_variant_ = FPU_STATE_SSTATE;

			((byte_t *)&x86_fpustate_load)[0] = 0xdd; /* frstor (%ecx) */
			((byte_t *)&x86_fpustate_load)[1] = 0x21;
			((byte_t *)&x86_fpustate_load)[2] = 0xc3; /* ret */

			((byte_t *)&x86_fpustate_save)[0] = 0xdd; /* fnsave (%ecx) */
			((byte_t *)&x86_fpustate_save)[1] = 0x31;
			((byte_t *)&x86_fpustate_save)[2] = 0xc3; /* ret */

			((byte_t *)&x86_fpustate_save_noreset)[0] = 0xdd; /* fnsave (%ecx) */
			((byte_t *)&x86_fpustate_save_noreset)[1] = 0x31;
			((byte_t *)&x86_fpustate_save_noreset)[2] = 0xdd; /* frstor (%ecx) */
			((byte_t *)&x86_fpustate_save_noreset)[3] = 0x21;
			((byte_t *)&x86_fpustate_save_noreset)[4] = 0xc3; /* ret */

			x86_fpustate_init_ftw_offset_byte = OFFSET_SFPUSTATE_FTW;
			/* Don't initialize the MXCSR dword, which doesn't exist without SSE */
			x86_fpustate_init_mxcsr[0] = 0xc3; /* ret */
		}
	}
	/* Set the TS bit because while the FPU is now initialized, `x86_fpu_current'
	 * isn't actually set to anything, much less the calling thread. */
	__wrcr0(cr0 | CR0_TS);
	test_fpu64();
}

#endif /* !CONFIG_NO_FPU */

DECL_END

#endif /* !GUARD_KERNEL_CORE_ARCH_I386_SCHED_FPU_C */
