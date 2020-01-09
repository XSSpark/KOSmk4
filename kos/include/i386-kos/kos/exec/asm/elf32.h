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
#ifndef _I386_KOS_KOS_EXEC_ASM_ELF32_H
#define _I386_KOS_KOS_EXEC_ASM_ELF32_H 1

#include <__stdinc.h>
#include <features.h>

#include <hybrid/host.h>

#include <bits/types.h>

#include <elf.h>

#include <libc/string.h>

#ifdef __KERNEL__
#if defined(__x86_64__) || defined(__i386__)
#include <kernel/compiler.h>

#include <fs/node.h>
#include <fs/vfs.h>
#include <kernel/gdt.h>
#include <kernel/types.h>
#include <sched/arch/posix-signal.h>

#include <kos/kernel/cpu-state-helpers.h>
#include <kos/kernel/cpu-state.h>
#ifdef __x86_64__
#include <kos/kernel/gdt.h>
#endif /* __x86_64__ */
#endif /* __x86_64__ || __i386__ */
#endif /* __KERNEL__ */


#if !defined(__x86_64__) && defined(__i386__)
#define ELF_ARCH_CLASS    ELF_ARCH386_CLASS
#define ELF_ARCH_MACHINE  ELF_ARCH386_MACHINE
#define ELF_ARCH_DATA     ELF_ARCH386_DATA
#define ELF_ARCH_USESRELA ELF_ARCH386_USESRELA
#define ELF_ARCH_LAZYINDX ELF_ARCH386_LAZYINDX
#define __OFFSET_PROCESS_RTLD_ELF_RTLDADDR  __OFFSET_PROCESS_RTLD_ELF32_RTLDADDR
#define __OFFSET_PROCESS_RTLD_ELF_NUM       __OFFSET_PROCESS_RTLD_ELF32_NUM
#define __OFFSET_PROCESS_RTLD_ELF_PHDR      __OFFSET_PROCESS_RTLD_ELF32_PHDR
#define __elfexec_info_defined   1
#define elfexec_info32           elfexec_info
#define elfexec_info_getfilename elfexec_info32_getfilename
#define elfexec_info_getentry    elfexec_info32_getentry
#ifdef __KERNEL__
#if defined(__x86_64__) || defined(__i386__)
#define elfexec_init_entry elfexec_init_entry32
#define elfexec_init_rtld  elfexec_init_rtld32
#endif /* __x86_64__ || __i386__ */
#endif /* __KERNEL__ */
#endif /* !__x86_64__ && __i386__ */


__DECL_BEGIN

#define ELF_ARCH386_CLASS    ELFCLASS32
#define ELF_ARCH386_MACHINE  EM_386
#define ELF_ARCH386_DATA     ELFDATA2LSB
#define ELF_ARCH386_USESRELA 0 /* i386 doesn't use addend-relocations */
#define ELF_ARCH386_LAZYINDX 0 /* Lazy relocations use offsets, rather than indices */

/* The user-space register holding a pointer to the ProcessEnvironmentBlock
 * immediately following the initial transition to user-space after a call to exec()
 * NOTE: This register points to a `struct process_peb' structure. */
#define ELF_ARCH386_PEB_REGISTER ebp

/* When a dynamic linker is loaded, additional information is passed
 * via user-space registers containing information about how the program
 * must be linked before it can actually be executed properly.
 * The exact structures pointed to by `ELF_ARCH386_DL_RTLDDATA_REGISTER'
 * depend on which DL loader was chosen:
 *   - ELF: `struct process_rtld_elfdata' */
#define ELF_ARCH386_DL_RTLDDATA_REGISTER ecx /* Pointer to a RTLD-specific initialization data. */
#define ELF_ARCH386_DL_LOADADDR_REGISTER edx /* The kernel-chose base-address of the application being loaded. */

#define __OFFSET_PROCESS_RTLD_ELF32_RTLDADDR 0
#define __OFFSET_PROCESS_RTLD_ELF32_PNUM     4
#define __OFFSET_PROCESS_RTLD_ELF32_PHDR     8

#ifdef __CC__
struct elfexec_info32 /*[PREFIX(ei_)]*/ {
	__uint32_t ei_rtldaddr;    /* Load address of the RTLD itself. */
	Elf32_Half ei_pnum;        /* Amount of ELF program headers. */
	Elf32_Half ei_ppad;        /* ... */
	Elf32_Phdr ei_phdr[1024];  /* [ei_pnum] Vector of ELF program headers.
	                            * All of these have already been loaded into memory. */
//	char       ei_filename[*]; /* NUL-terminated filename of the loaded binary. */
//	char       ei_align[*];    /* Align to next 4 byte boundary. */
//	__uint32_t ei_entry;       /* The final entry point for the program. */
//	byte_t     ei_entry_sp[];  /* Entry stack address (set the address of this field as
//	                            * stack-pointer when handing control to the hosted application) */
};
#define elfexec_info32_getfilename(x) \
	((char *)&(x)->ei_phdr[(x)->ei_pnum])
#define elfexec_info32_getentry(x) \
	(*(__uint32_t *)((__uint32_t)(__libc_strend(elfexec_info_getfilename(x)) + 1 + 3) & ~3))


#ifdef __KERNEL__
#if defined(__x86_64__) || defined(__i386__)
LOCAL struct icpustate *KCALL
elfexec_init_entry32(struct icpustate *__restrict user_state,
                     USER void *peb_address, USER void *ustack_base,
                     __size_t ustack_size, USER void *entry_pc) {
	set_user_fsbase(x86_get_random_userkern_address()); /* re-roll the ukern address. */
	gpregs_setpbp(&user_state->ics_gpregs, (__uintptr_t)peb_address); /* ELF_ARCH386_PEB_REGISTER */
	icpustate_setpc(user_state, (__uintptr_t)entry_pc);
	icpustate_setuserpsp(user_state, (__uintptr_t)ustack_base + ustack_size);
	{
		union x86_user_eflags_mask mask;
		mask.uem_word = atomic64_read(&x86_exec_eflags_mask);
		/* Mask eflags for exec() */
		icpustate_mskpflags(user_state, mask.uem_mask, mask.uem_flag);
	}
#ifdef __x86_64__
	icpustate_setcs(user_state, SEGMENT_USER_CODE32_RPL);
	icpustate_setss(user_state, SEGMENT_USER_DATA32_RPL);
#endif /* __x86_64__ */
	return user_state;
}

LOCAL struct icpustate *KCALL
elfexec_init_rtld32(struct icpustate *__restrict user_state,
                    struct path *__restrict exec_path,
                    struct directory_entry *__restrict exec_dentry,
                    struct regular_node *__restrict UNUSED(exec_node),
                    void *application_loadaddr,
                    void *linker_loadaddr,
                    KERNEL Elf32_Phdr const *__restrict phdr_vec,
                    Elf32_Half phdr_cnt) {
	/* The application-level entry point is stored
	 * stored at the base of the user-space stack. */
	__uintptr_t user_state_sp;
	__size_t buflen;
	USER struct elfexec_info32 *dl_data;
	user_state_sp = icpustate_getuserpsp(user_state);
	user_state_sp -= 4;
	*(__uint32_t *)user_state_sp = (__uint32_t)icpustate_getpc(user_state); /* dl_data->ei_entry */
	/* Print the application filename to the user-space stack. */
	buflen = 0;
	for (;;) {
		__size_t reqlen;
		reqlen = path_sprintent((USER char *)user_state_sp - buflen,
		                        buflen,
		                        exec_path,
		                        exec_dentry);
		if (reqlen <= buflen)
			break;
		buflen = (reqlen + 3) & ~3;
	}
	user_state_sp -= buflen;
	dl_data = (USER struct elfexec_info32 *)(user_state_sp -
	                                         ((phdr_cnt * sizeof(Elf32_Phdr)) +
	                                          offsetof(struct elfexec_info32, ei_phdr)));
	__libc_memcpyc(dl_data->ei_phdr, phdr_vec, phdr_cnt, sizeof(Elf32_Phdr));
	dl_data->ei_rtldaddr = (__uint32_t)(__uintptr_t)linker_loadaddr;
	dl_data->ei_pnum     = phdr_cnt;
	gpregs_setpcx(&user_state->ics_gpregs, (__uintptr_t)dl_data);              /* ELF_ARCH386_DL_RTLDDATA_REGISTER */
	gpregs_setpdx(&user_state->ics_gpregs, (__uintptr_t)application_loadaddr); /* ELF_ARCH386_DL_LOADADDR_REGISTER */
	icpustate_setuserpsp(user_state, (__uintptr_t)dl_data);
	icpustate_setpc(user_state, (__uintptr_t)linker_loadaddr); /* Entry point is at offset=0 */
	return user_state;
}
#endif /* __x86_64__ || __i386__ */
#endif /* __KERNEL__ */
#endif /* __CC__ */

__DECL_END

#endif /* !_I386_KOS_KOS_EXEC_ASM_ELF32_H */
