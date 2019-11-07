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
#ifndef GUARD_KERNEL_SRC_MISC_PANIC_C
#define GUARD_KERNEL_SRC_MISC_PANIC_C 1

#include <kernel/compiler.h>

#include <kernel/addr2line.h>
#include <kernel/debugger.h>
#include <kernel/debugtrap.h>
#include <kernel/memory.h>
#include <kernel/printk.h>
#include <kernel/vm.h>
#include <sched/task.h>

#include <hybrid/align.h>
#include <hybrid/host.h>

#include <asm/intrin.h>
#include <kos/kernel/cpu-state-helpers.h>

#include <format-printer.h>
#include <signal.h>
#include <stdarg.h>
#include <stddef.h>
#include <string.h>

#include <libinstrlen/instrlen.h>
#include <libunwind/unwind.h>

#include "../../../libc/libc/assert.h"

DECL_BEGIN

#define LOG_STACK_REMAINDER 1


PRIVATE ATTR_COLDTEXT NOBLOCK bool
NOTHROW(KCALL is_pc)(uintptr_t pc) {
	struct vm_node *node;
	if (pc < KERNEL_BASE)
		return false;
	node = vm_getnodeof(&vm_kernel, VM_ADDR2PAGE((vm_virt_t)pc));
	if (!node)
		return false;
	return (node->vn_prot & VM_PROT_EXEC) != 0;
}

INTERN ATTR_COLDTEXT void KCALL
kernel_halt_dump_traceback(pformatprinter printer, void *arg,
                           struct ucpustate const *__restrict state) {
	unsigned int error;
	struct ucpustate mystate = *state;
#ifdef LOG_STACK_REMAINDER
	uintptr_t last_good_sp = ucpustate_getsp(&mystate);
#endif /* LOG_STACK_REMAINDER */
	addr2line_printf(printer, arg,
	                 (uintptr_t)instruction_trypred((void const *)ucpustate_getpc(&mystate)),
	                 ucpustate_getpc(&mystate),
	                 "Caused here [sp=%p]",
	                 (void *)ucpustate_getsp(&mystate));
	for (;;) {
		struct ucpustate old_state;
		old_state = mystate;
		error = unwind((void *)(ucpustate_getpc(&old_state) - 1),
		               &unwind_getreg_ucpustate, &old_state,
		               &unwind_setreg_ucpustate, &mystate);
		if (error != UNWIND_SUCCESS)
			break;
		addr2line_printf(printer, arg,
		                 (uintptr_t)instruction_trypred((void const *)ucpustate_getpc(&mystate)),
		                 ucpustate_getpc(&mystate),
		                 "Called here [sp=%p]",
		                 (void *)ucpustate_getsp(&mystate));
#ifdef LOG_STACK_REMAINDER
		last_good_sp = ucpustate_getsp(&mystate);
#endif /* LOG_STACK_REMAINDER */
	}
	if (error != UNWIND_NO_FRAME)
		format_printf(printer, arg, "Unwind failure: %u\n", error);
#ifdef LOG_STACK_REMAINDER
	{
		struct vm_node const *stack = stack_current();
		if ((vm_virt_t)last_good_sp >= VM_NODE_MINADDR(stack) &&
		    (vm_virt_t)last_good_sp <= VM_NODE_MAXADDR(stack)) {
			bool is_first = true;
#ifdef __ARCH_STACK_GROWS_DOWNWARDS
			uintptr_t iter, end;
			end  = (uintptr_t)VM_NODE_ENDADDR(stack);
			iter = CEIL_ALIGN(last_good_sp, sizeof(void *));
			for (; iter < end; iter += sizeof(void *))
#else /* __ARCH_STACK_GROWS_DOWNWARDS */
			uintptr_t iter, begin;
			iter  = CEIL_ALIGN(last_good_sp, sizeof(void *));
			begin = (uintptr_t)VM_NODE_MINADDR(stack);
			while (iter > begin)
#endif /* !__ARCH_STACK_GROWS_DOWNWARDS */
			{
				void *pc;
#ifndef __ARCH_STACK_GROWS_DOWNWARDS
				iter -= sizeof(void *);
#endif /* !__ARCH_STACK_GROWS_DOWNWARDS */
				pc = *(void **)iter;
				if (!is_pc((uintptr_t)pc))
					continue;
				if (is_first) {
					format_printf(printer, arg, "Analyzing remainder of stack\n");
					is_first = false;
				}
				addr2line_printf(printer, arg,
				                 (uintptr_t)instruction_trypred(pc),
				                 (uintptr_t)pc,
				                 "Return address from [sp=%p]",
				                 iter
#ifdef __ARCH_STACK_GROWS_DOWNWARDS
				                 - sizeof(void *)
#endif /* __ARCH_STACK_GROWS_DOWNWARDS */
				                 );
			}
		}
	}
#endif /* LOG_STACK_REMAINDER */
}


#ifndef CONFIG_NO_DEBUGGER
PRIVATE ATTR_COLDTEXT void KCALL
panic_assert_dbg_main(void *arg) {
	struct assert_args *args;
	args = (struct assert_args *)arg;
	dbg_printf(DF_SETCOLOR(DBG_COLOR_WHITE, DBG_COLOR_MAROON) "Assertion failure" DF_DEFCOLOR "\n"
	           "expr: " DF_SETFGCOLOR(DBG_COLOR_WHITE) "%s" DF_DEFFGCOLOR "\n"
	           "file: " DF_SETFGCOLOR(DBG_COLOR_WHITE) "%s" DF_DEFFGCOLOR
	           " (line " DF_SETFGCOLOR(DBG_COLOR_WHITE) "%u" DF_DEFFGCOLOR ")\n",
	           args->aa_expr,
	           args->aa_file,
	           args->aa_line);
	if (args->aa_func)
		dbg_printf("func: " DF_SETFGCOLOR(DBG_COLOR_WHITE) "%s" DF_DEFFGCOLOR "\n", args->aa_func);
	if (args->aa_format) {
		dbg_print("mesg: " DF_SETCOLOR(DBG_COLOR_TEAL, DBG_COLOR_BLACK));
		dbg_indent += 6;
		format_vprintf(&dbg_printer, NULL, args->aa_format, args->aa_args);
		dbg_indent -= 6;
		dbg_print(DF_DEFCOLOR "\n");
	}
	dbg_printf("addr: " DF_SETFGCOLOR(DBG_COLOR_WHITE) "%p" DF_DEFFGCOLOR "\n",
	           kcpustate_getpc(&args->aa_state));
	dbg_main(0);
}
#endif /* !CONFIG_NO_DEBUGGER */


INTERN ATTR_COLDTEXT ATTR_NOINLINE ATTR_NORETURN void FCALL
libc_assertion_failure_core(struct assert_args *__restrict args) {
	PREEMPTION_DISABLE();
	printk(KERN_RAW "\n\n\n");
	printk(KERN_EMERG "Assertion Failure [pc=%p]\n",
	       kcpustate_getpc(&args->aa_state));
	printk(KERN_RAW "%s(%d) : %s%s%s\n",
	       args->aa_file, args->aa_line,
	       args->aa_func ? args->aa_func : "",
	       args->aa_func ? " : " : "",
	       args->aa_expr);
	if (args->aa_format) {
		va_list vargs;
		va_copy(vargs, args->aa_args);
		format_vprintf(&kprinter, (void *)KERN_RAW, args->aa_format, vargs);
		va_end(vargs);
		printk(KERN_RAW "\n");
	}
	{
		struct ucpustate temp;
		kcpustate_to_ucpustate(&args->aa_state, &temp);
		kernel_halt_dump_traceback(&kprinter, (void *)KERN_RAW, &temp);
	}
	/* Try to trigger a debugger trap (if enabled) */
	if (kernel_debugtrap_enabled())
		kernel_debugtrap(&args->aa_state, SIGTRAP);
#ifndef CONFIG_NO_DEBUGGER
	/* Enter the debugger */
	dbg_enter(&args->aa_state, &panic_assert_dbg_main, args);
#else
	PREEMPTION_HALT();
#endif
}




#ifndef CONFIG_NO_DEBUGGER
PRIVATE ATTR_COLDTEXT void KCALL
panic_assert_chk_print_message(void *arg) {
	struct assert_args *args;
	args = (struct assert_args *)arg;
	dbg_printf("expr: " DF_SETFGCOLOR(DBG_COLOR_WHITE) "%s" DF_DEFFGCOLOR "\n"
	           "file: " DF_SETFGCOLOR(DBG_COLOR_WHITE) "%s" DF_DEFFGCOLOR
	           " (line " DF_SETFGCOLOR(DBG_COLOR_WHITE) "%u" DF_DEFFGCOLOR ")\n",
	           args->aa_expr,
	           args->aa_file,
	           args->aa_line);
	if (args->aa_func)
		dbg_printf("func: " DF_SETFGCOLOR(DBG_COLOR_WHITE) "%s" DF_DEFFGCOLOR "\n", args->aa_func);
	if (args->aa_format) {
		dbg_print("mesg: " DF_SETCOLOR(DBG_COLOR_TEAL, DBG_COLOR_BLACK));
		dbg_indent += 6;
		format_vprintf(&dbg_printer, NULL, args->aa_format, args->aa_args);
		dbg_indent -= 6;
		dbg_print(DF_DEFCOLOR "\n");
	}
	dbg_printf("addr: " DF_SETFGCOLOR(DBG_COLOR_WHITE) "%p" DF_DEFFGCOLOR "\n",
	           kcpustate_getpc(&args->aa_state));
}


#define ASSERTION_OPTION_RETRY          0
#define ASSERTION_OPTION_DEBUG          1
#define ASSERTION_OPTION_IGNORE         2
#define ASSERTION_OPTION_IGNORE_ALWAYS  3

PRIVATE ATTR_COLDRODATA char const *const assert_chk_options[] = {
	[ASSERTION_OPTION_RETRY]         = "retry",
	[ASSERTION_OPTION_DEBUG]         = "debug",
	[ASSERTION_OPTION_IGNORE]        = "ignore",
	[ASSERTION_OPTION_IGNORE_ALWAYS] = "ignore (always)",
	NULL
};
PRIVATE ATTR_COLDBSS uintptr_t always_ignored_assertions[64] = { 0 };


PRIVATE ATTR_COLDTEXT void KCALL
panic_assert_chk_dbg_main(void *arg) {
	struct assert_args *args;
	unsigned int i, option;
	args = (struct assert_args *)arg;
	for (i = 0; i < COMPILER_LENOF(always_ignored_assertions); ++i) {
		if (fcpustate_getpc(&dbg_exitstate) == always_ignored_assertions[i]) {
			option = ASSERTION_OPTION_IGNORE;
			goto handle_retry_or_ignore;
		}
	}

	/* Display a menu to let the user select what to do about the assertion... */
	option = dbg_menuex(DF_SETCOLOR(DBG_COLOR_WHITE, DBG_COLOR_MAROON)
	                    "Assertion failure" DF_DEFCOLOR,
	                    assert_chk_options,
	                    0,
	                    &panic_assert_chk_print_message,
	                    args);
	if (option == ASSERTION_OPTION_IGNORE_ALWAYS) {
		unsigned int i;
		for (i = 0; i < COMPILER_LENOF(always_ignored_assertions); ++i) {
			if (always_ignored_assertions[i])
				continue;
			always_ignored_assertions[i] = fcpustate_getpc(&dbg_exitstate);
			break;
		}
		option = ASSERTION_OPTION_IGNORE;
	}
	if (option == ASSERTION_OPTION_RETRY ||
	    option == ASSERTION_OPTION_IGNORE) {
		uintptr_t acheck_result;
handle_retry_or_ignore:
		acheck_result = option == ASSERTION_OPTION_RETRY ? 1 : 0;
		/* TODO: Make this part arch-independent */
#ifdef __x86_64__
		dbg_exitstate.fcs_gpregs.gp_rax = acheck_result;
#elif defined(__i386__)
		dbg_exitstate.fcs_gpregs.gp_eax = acheck_result;
#else
#error Unsupported arch
#endif
		return;
	}
	/* Enter debugger mode. */
	dbg_fillscreen(' ');
	dbg_setcur(0, 0);
	panic_assert_dbg_main(arg);
}
#endif /* !CONFIG_NO_DEBUGGER */

INTERN ATTR_COLDTEXT ATTR_NOINLINE struct kcpustate *FCALL
libc_assertion_check_core(struct assert_args *__restrict args) {
	/* TODO: Check if assertion failures at `kcpustate_getpc(&args->aa_state)' should be ignored. */
	PREEMPTION_DISABLE();
	printk(KERN_RAW "\n\n\n");
	printk(KERN_EMERG "Assertion Failure [pc=%p]\n",
	       kcpustate_getpc(&args->aa_state));
	printk(KERN_RAW "%s(%d) : %s%s%s\n",
	       args->aa_file, args->aa_line,
	       args->aa_func ? args->aa_func : "",
	       args->aa_func ? " : " : "",
	       args->aa_expr);
	if (args->aa_format) {
		va_list vargs;
		va_copy(vargs, args->aa_args);
		format_vprintf(&kprinter, (void *)KERN_RAW, args->aa_format, vargs);
		va_end(vargs);
		printk(KERN_RAW "\n");
	}
	{
		struct ucpustate temp;
		kcpustate_to_ucpustate(&args->aa_state, &temp);
		kernel_halt_dump_traceback(&kprinter, (void *)KERN_RAW, &temp);
	}
	/* Try to trigger a debugger trap (if enabled) */
	if (kernel_debugtrap_enabled())
		kernel_debugtrap(&args->aa_state, SIGTRAP);
#ifndef CONFIG_NO_DEBUGGER
	/* Enter the debugger */
	dbg_enter(&args->aa_state, &panic_assert_chk_dbg_main, args);
#else
	PREEMPTION_HALT();
#endif
}



#ifndef CONFIG_NO_DEBUGGER
PRIVATE ATTR_COLDTEXT void KCALL
panic_genfail_dbg_main(void *arg) {
	uintptr_t prev_pc;
	prev_pc = (uintptr_t)instruction_trypred((void const *)fcpustate_getpc(&dbg_exitstate));
	dbg_printf(DF_SETCOLOR(DBG_COLOR_WHITE, DBG_COLOR_MAROON) "%s" DF_DEFCOLOR "%[vinfo:"
	           "file: " DF_WHITE("%f") " (line " DF_WHITE("%l") ", column " DF_WHITE("%c") ")\n"
	           "func: " DF_WHITE("%n") "\n]"
	           "addr: " DF_WHITE("%p") "+" DF_WHITE("%Iu") "\n",
	           arg, prev_pc, prev_pc,
	           (size_t)(fcpustate_getpc(&dbg_exitstate) - prev_pc));
	dbg_main(0);
}
#endif


/* The `__stack_chk_guard' global is read _very_ often,
 * so place in in the COMPACT_DATA segment. */
#ifndef NDEBUG
PUBLIC ATTR_READMOSTLY uintptr_t __stack_chk_guard = 0x123baf37;
#else /* !NDEBUG */
PUBLIC ATTR_COLDDATA uintptr_t __stack_chk_guard = 0x123baf37;
#endif /* NDEBUG */

INTERN ATTR_COLDTEXT ATTR_COLD ATTR_NORETURN void FCALL
libc_stack_failure_core(struct kcpustate *__restrict state) {
	struct ucpustate ustate;
	PREEMPTION_DISABLE();
	printk(KERN_RAW "\n\n\n");
	printk(KERN_EMERG "Stack check failure [pc=%p]\n", kcpustate_getpc(state));
	kcpustate_to_ucpustate(state, &ustate);
	kernel_halt_dump_traceback(&kprinter, (void *)KERN_RAW, &ustate);
	/* Try to trigger a debugger trap (if enabled) */
	if (kernel_debugtrap_enabled())
		kernel_debugtrap(state, SIGSEGV);
#ifndef CONFIG_NO_DEBUGGER
	/* Enter the debugger */
	dbg_enter(state, &panic_genfail_dbg_main,
	          (void *)"Stack check failure (corrupted cookie)\n");
#else /* !CONFIG_NO_DEBUGGER */
	PREEMPTION_HALT();
#endif /* CONFIG_NO_DEBUGGER */
}

INTERN ATTR_COLDTEXT ATTR_COLD ATTR_NORETURN void FCALL
libc_abort_failure_core(struct kcpustate *__restrict state) {
	struct ucpustate ustate;
	PREEMPTION_DISABLE();
	printk(KERN_RAW "\n\n\n");
	printk(KERN_EMERG "Kernel aborted [pc=%p]\n", kcpustate_getpc(state));
	kcpustate_to_ucpustate(state, &ustate);
	kernel_halt_dump_traceback(&kprinter, (void *)KERN_RAW, &ustate);
	/* Try to trigger a debugger trap (if enabled) */
	if (kernel_debugtrap_enabled())
		kernel_debugtrap(state, SIGABRT);
#ifndef CONFIG_NO_DEBUGGER
	/* Enter the debugger */
	dbg_enter(state, &panic_genfail_dbg_main,
	          (void *)"Kernel called abort()\n");
#else /* !CONFIG_NO_DEBUGGER */
	PREEMPTION_HALT();
#endif /* CONFIG_NO_DEBUGGER */
}


#ifndef CONFIG_NO_DEBUGGER
struct panic_args {
	char const *format;
	va_list     args;
};
PRIVATE ATTR_COLDTEXT void KCALL
panic_kernel_dbg_main(void *arg) {
	struct panic_args *args;
	uintptr_t prev_pc;
	prev_pc = (uintptr_t)instruction_trypred((void const *)fcpustate_getpc(&dbg_exitstate));
	args    = (struct panic_args *)arg;
	dbg_printf("Kernel Panic\n"
	           "%[vinfo:" "file: " DF_WHITE("%f") " (line " DF_WHITE("%l") ", column " DF_WHITE("%c") ")\n"
	                      "func: " DF_WHITE("%n") "\n"
	           "]",
	           prev_pc);
	if (args->format) {
		dbg_print("mesg: " DF_SETCOLOR(DBG_COLOR_TEAL, DBG_COLOR_BLACK));
		dbg_indent += 6;
		format_vprintf(&dbg_printer, NULL, args->format, args->args);
		dbg_indent -= 6;
		dbg_print(DF_DEFCOLOR "\n");
	}
	dbg_printf("addr: " DF_WHITE("%p") "+" DF_WHITE("%Iu") "\n",
	           prev_pc, (size_t)(fcpustate_getpc(&dbg_exitstate) - prev_pc));
	dbg_main(0);
}
#endif /* !CONFIG_NO_DEBUGGER */

FUNDEF ATTR_NORETURN ATTR_COLD void FCALL
kernel_vpanic_ucpustate(struct ucpustate *__restrict state,
                        char const *format, va_list args) {
	PREEMPTION_DISABLE();
	printk(KERN_RAW "\n\n\n");
	printk(KERN_EMERG "Kernel Panic [pc=%p]\n",
	       ucpustate_getpc(state));
	if (format) {
		va_list cargs;
		va_copy(cargs, args);
		format_vprintf(&kprinter, (void *)KERN_RAW, format, cargs);
		va_end(cargs);
		printk(KERN_EMERG "\n");
	}
	kernel_halt_dump_traceback(&kprinter, (void *)KERN_RAW, state);
	/* Try to trigger a debugger trap (if enabled) */
	if (kernel_debugtrap_enabled())
		kernel_debugtrap(state, SIGABRT);
#ifndef CONFIG_NO_DEBUGGER
	/* Enter the debugger */
	{
		struct panic_args pargs;
		if (format)
			va_copy(pargs.args, args);
		pargs.format = format;
		dbg_enter(state, &panic_kernel_dbg_main, &pargs);
	}
#else /* !CONFIG_NO_DEBUGGER */
	PREEMPTION_HALT();
#endif /* CONFIG_NO_DEBUGGER */
}

FUNDEF ATTR_NORETURN ATTR_COLD void FCALL
kernel_vpanic_lcpustate(struct lcpustate *__restrict state,
                        char const *format, va_list args) {
	struct ucpustate ustate;
	lcpustate_to_ucpustate(state, &ustate);
	kernel_vpanic_ucpustate(&ustate, format, args);
}

FUNDEF ATTR_NORETURN ATTR_COLD void FCALL
kernel_vpanic_kcpustate(struct kcpustate *__restrict state,
                        char const *format, va_list args) {
	struct ucpustate ustate;
	kcpustate_to_ucpustate(state, &ustate);
	kernel_vpanic_ucpustate(&ustate, format, args);
}

FUNDEF ATTR_NORETURN ATTR_COLD void FCALL
kernel_vpanic_icpustate(struct icpustate *__restrict state,
                        char const *format, va_list args) {
	struct ucpustate ustate;
	icpustate_to_ucpustate(state, &ustate);
	kernel_vpanic_ucpustate(&ustate, format, args);
}

FUNDEF ATTR_NORETURN ATTR_COLD void FCALL
kernel_vpanic_scpustate(struct scpustate *__restrict state,
                        char const *format, va_list args) {
	struct ucpustate ustate;
	scpustate_to_ucpustate(state, &ustate);
	kernel_vpanic_ucpustate(&ustate, format, args);
}



FUNDEF ATTR_NORETURN ATTR_COLD void VCALL
kernel_panic_ucpustate(struct ucpustate *__restrict state,
                       char const *format, ...) {
	va_list args;
	va_start(args, format);
	kernel_vpanic_ucpustate(state, format, args);
}

FUNDEF ATTR_NORETURN ATTR_COLD void VCALL
kernel_panic_lcpustate(struct lcpustate *__restrict state,
                       char const *format, ...) {
	va_list args;
	va_start(args, format);
	kernel_vpanic_lcpustate(state, format, args);
}

FUNDEF ATTR_NORETURN ATTR_COLD void VCALL
kernel_panic_kcpustate(struct kcpustate *__restrict state,
                       char const *format, ...) {
	va_list args;
	va_start(args, format);
	kernel_vpanic_kcpustate(state, format, args);
}

FUNDEF ATTR_NORETURN ATTR_COLD void VCALL
kernel_panic_icpustate(struct icpustate *__restrict state,
                       char const *format, ...) {
	va_list args;
	va_start(args, format);
	kernel_vpanic_icpustate(state, format, args);
}

FUNDEF ATTR_NORETURN ATTR_COLD void VCALL
kernel_panic_scpustate(struct scpustate *__restrict state,
                       char const *format, ...) {
	va_list args;
	va_start(args, format);
	kernel_vpanic_scpustate(state, format, args);
}



DECL_END

#endif /* !GUARD_KERNEL_SRC_MISC_PANIC_C */
