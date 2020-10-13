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
#ifndef GUARD_KERNEL_SRC_MEMORY_TRACE_MALLOC_C
#define GUARD_KERNEL_SRC_MEMORY_TRACE_MALLOC_C 1
#define __OMIT_KMALLOC_CONSTANT_P_WRAPPERS 1
#define __OMIT_HEAP_TRACED_CONSTANT_P_WRAPPERS 1
#define _KOS_SOURCE 1

#include <kernel/compiler.h>

#include <kernel/malloc-defs.h>

#if 0 /* Quick toggle to disable debug_malloc (and use the weak definitions from `malloc.c'). */
#undef CONFIG_DEBUG_MALLOC
#endif

#ifdef CONFIG_DEBUG_MALLOC
#include <kernel/malloc.h>
#ifdef CONFIG_USE_NEW_DEBUG_MALLOC
#include <debugger/config.h>
#include <debugger/hook.h>
#include <kernel/addr2line.h>
#include <kernel/except.h>
#include <kernel/heap.h>
#include <kernel/panic.h>
#include <kernel/printk.h>
#include <kernel/slab.h>
#include <kernel/syslog.h>
#include <sched/enum.h>
#include <sched/pid.h>
#include <sched/scheduler.h>
#include <sched/task.h>

#include <hybrid/align.h>
#include <hybrid/overflow.h>
#include <hybrid/sequence/atree.h>
#include <hybrid/sync/atomic-lock.h>

#include <kos/kernel/cpu-state-helpers.h>
#include <kos/kernel/cpu-state.h>
#include <sys/param.h>

#include <alloca.h>
#include <assert.h>
#include <format-printer.h>
#include <inttypes.h>
#include <stddef.h>
#include <string.h>

#include <libinstrlen/instrlen.h>
#include <libunwind/unwind.h>

#include "corebase.h"

/* The minimum amount of traceback entries that MALL
 * should attempt to include in debug information of
 * allocated pointers. */
#ifndef CONFIG_TRACE_MALLOC_MIN_TRACEBACK
#define CONFIG_TRACE_MALLOC_MIN_TRACEBACK 4
#endif /* !CONFIG_TRACE_MALLOC_MIN_TRACEBACK */


DECL_BEGIN

#define HINT_ADDR(x, y) x
#define HINT_MODE(x, y) y
#define HINT_GETADDR(x) HINT_ADDR x
#define HINT_GETMODE(x) HINT_MODE x


#ifndef CONFIG_MALL_HEAD_PATTERN
#define CONFIG_MALL_HEAD_PATTERN 0x33333333
#endif /* !CONFIG_MALL_HEAD_PATTERN */
#ifndef CONFIG_MALL_TAIL_PATTERN
#define CONFIG_MALL_TAIL_PATTERN 0x77777777
#endif /* !CONFIG_MALL_TAIL_PATTERN */
#ifndef CONFIG_MALL_HEAD_SIZE
#define CONFIG_MALL_HEAD_SIZE HEAP_ALIGNMENT
#endif /* !CONFIG_MALL_HEAD_SIZE */
#ifndef CONFIG_MALL_TAIL_SIZE
#define CONFIG_MALL_TAIL_SIZE HEAP_ALIGNMENT
#endif /* !CONFIG_MALL_TAIL_SIZE */


#define TRACE_NODE_KIND_MALL   0x00 /* Normal kmalloc() memory */
#define TRACE_NODE_KIND_USER   0x01 /* Custom, mall_trace()'d memory */
#define TRACE_NODE_KIND_BITSET 0x02 /* Custom, mall_trace()'d memory, but instead of having a vector
                                     * of traceback locations, this node has a bitset describing which
                                     * of its pointed-to memory is still being traced. This kind of
                                     * node is needed to represent the result of mall_trace(), followed
                                     * by mall_untrace_n(), where the range being untraced doesn't span
                                     * or touch any node borders, in which case a node would have to be
                                     * split into 2 parts. If at that point the system fails to allocate
                                     * a secondary node, the original node it transformed into this kind,
                                     * and given a bitset of POINTER-aligned memory locations that are
                                     * still considered as traced. */
#define TRACE_NODE_KIND_HAS_PADDING(kind)   ((kind) == TRACE_NODE_KIND_MALL)
#define TRACE_NODE_KIND_HAS_TRACEBACK(kind) ((kind) != TRACE_NODE_KIND_BITSET)

#define TRACE_NODE_FLAG_NOLEAK 0x10 /* FLAG: Don't consider this one a leak. */
#define TRACE_NODE_FLAG_NOWALK 0x20 /* FLAG: Don't search this one's contents for other reachable locations. */

#if ((GFP_NOLEAK >> 8) == TRACE_NODE_FLAG_NOLEAK && \
     (GFP_NOWALK >> 8) == TRACE_NODE_FLAG_NOWALK)
#define TRACE_NODE_FLAG_FROM_GFP(gfp) ((gfp & (GFP_NOLEAK | GFP_NOWALK)) >> 8)
#else /* ... */
#define TRACE_NODE_FLAG_FROM_GFP(gfp)                  \
	(((gfp)&GFP_NOLEAK ? TRACE_NODE_FLAG_NOLEAK : 0) | \
	 ((gfp)&GFP_NOWALK ? TRACE_NODE_FLAG_NOWALK : 0))
#endif /* !... */

struct trace_node {
	ATREE_NODE(struct trace_node, uintptr_t)
	            tn_link;  /* [const] Trace node link. */
	size_t      tn_size;  /* [const] Allocated heap size of this TRACE-node. */
	u8          tn_reach; /* Last leak-check iteration when this node was reached. */
	u8          tn_visit; /* Last leak-check iteration when this node was visited. */
	u8          tn_kind;  /* Node kind (one of `TRACE_NODE_KIND_*') */
	u8          tn_flags; /* Node flags (or'd with `__GFP_HEAPMASK') */
	u32         tn_tid;   /* [const] ROOT-namespace TID of the allocating thread. */
	COMPILER_FLEXIBLE_ARRAY(void *, tn_trace);
	                      /* [1..1][0..MALLNODE_TRACESZ(self)]
	                       * Traceback of where the pointer was originally allocated. */
};

/************************************************************************/
/* GENERIC HELPERS                                                      */
/************************************************************************/
#define trace_node_umin(self)   ((self)->tn_link.a_vmin)
#define trace_node_umax(self)   ((self)->tn_link.a_vmax)
#define trace_node_uaddr(self)  ((byte_t *)(self)->tn_link.a_vmin)       /* NOTE: This one's always pointer-aligned! */
#define trace_node_uend(self)   ((byte_t *)((self)->tn_link.a_vmax + 1)) /* NOTE: This one's always pointer-aligned! */
#define trace_node_usize(self)  (((self)->tn_link.a_vmax - (self)->tn_link.a_vmin) + 1)


/************************************************************************/
/* TRACEBACK HELPERS                                                    */
/************************************************************************/
/* Return the base address of a `void **'-vector of PC-locations. */
#define trace_node_traceback_vector(self) ((self)->tn_trace)
/* Return the upper limit of how many elements are in the traceback vector.
 * The vector may terminate prematurely when a NULL-entry is encountered. */
#define trace_node_traceback_count(self) (((self)->tn_size - offsetof(struct trace_node, tn_trace)) / sizeof(void *))


/************************************************************************/
/* HELPERS FOR `TRACE_NODE_KIND_BITSET'                                 */
/************************************************************************/
typedef uintptr_t trace_node_bitset_t;

/* Return the address of the is-traced bitset */
#define trace_node_bitset_words(self) ((trace_node_bitset_t *)(self)->tn_trace)

/* Return the # of bits within the is-traced bitset. (s.a. `trace_node_bitset_words()') */
#define trace_node_bitset_count(self) (trace_node_usize(self) / __SIZEOF_POINTER__)

#define trace_node_bitset_words_word(i) ((i) / BITSOF(trace_node_bitset_t))
#define trace_node_bitset_words_mask(i) ((trace_node_bitset_t)1 << ((i) % BITSOF(trace_node_bitset_t)))

#define trace_node_bitset_bitget(self, i) (trace_node_bitset_words(self)[trace_node_bitset_words_word(i)] & trace_node_bitset_words_mask(i))
#define trace_node_bitset_biton(self, i)  (trace_node_bitset_words(self)[trace_node_bitset_words_word(i)] |= trace_node_bitset_words_mask(i))
#define trace_node_bitset_bitoff(self, i) (trace_node_bitset_words(self)[trace_node_bitset_words_word(i)] &= ~trace_node_bitset_words_mask(i))


/* Example code for enumerating traced words of a `TRACE_NODE_KIND_BITSET'-node:
 * >> if (node->tn_kind == TRACE_NODE_KIND_BITSET) {
 * >>     trace_node_bitset_t *words;
 * >>     uintptr_t *uaddr;
 * >>     size_t i, count;
 * >>     uaddr = trace_node_bitset_uaddr(node);
 * >>     words = trace_node_bitset_words(node);
 * >>     count = trace_node_bitset_count(node);
 * >>     for (i = 0; i < count; ++i) {
 * >>          trace_node_bitset_t word, mask;
 * >>          word = words[trace_node_bitset_words_word(i)];
 * >>          mask = trace_node_bitset_words_mask(i);
 * >>          if (word & mask) {
 * >>              HANDLE_TRACED_POINTER(&uaddr[i]);
 * >>          }
 * >>     }
 * >> }
 */


PRIVATE ATTR_PURE WUNUSED NONNULL((1)) size_t
NOTHROW(KCALL sizeof_trace_node_for_tb)(struct trace_node *__restrict self) {
	size_t result, count;
	if (!TRACE_NODE_KIND_HAS_TRACEBACK(self->tn_kind))
		return offsetof(struct trace_node, tn_trace);
	count = trace_node_bitset_count(self);
	for (result = 0; result < count; ++result) {
		if (!trace_node_traceback_vector(self)[result])
			break;
	}
	result *= sizeof(void *);
	result += offsetof(struct trace_node, tn_trace);
	return result;
}

PRIVATE ATTR_RETNONNULL WUNUSED NONNULL((1)) struct trace_node *
NOTHROW(KCALL copy_trace_node_for_tb)(struct trace_node *__restrict copy,
                                      struct trace_node *__restrict self,
                                      size_t sizeof_copy) {
	memcpy(copy, self, sizeof_copy);
	copy->tn_size = sizeof_copy;
	return copy;
}

/* Duplicate the given trace-node for the purpose of logging. */
#define trace_node_dupa_tb(self)                                                       \
	({                                                                                 \
		size_t _dup_siz = sizeof_trace_node_for_tb(self);                              \
		copy_trace_node_for_tb((struct trace_node *)alloca(_dup_siz), self, _dup_siz); \
	})


DECL_END

/* Define the ABI for the address tree used by trace nodes. */
#define ATREE(x)      trace_node_tree_##x
#define ATREE_CALL    KCALL
#define ATREE_NOTHROW NOTHROW
#define Tkey          uintptr_t
#define T             struct trace_node
#define N_NODEPATH    tn_link
#include <hybrid/sequence/atree-abi.h>

DECL_BEGIN

/* SMP-lock for accessing internal TRACE-malloc structures. */
#ifndef CONFIG_NO_SMP
PRIVATE ATTR_MALL_UNTRACKED struct atomic_lock smplock = ATOMIC_LOCK_INIT;
DEFINE_DBG_BZERO_OBJECT(smplock);
#define smplock_acquire_nopr() atomic_lock_acquire_nopr(&smplock)
#define smplock_release_nopr() atomic_lock_release(&smplock)
#else /* !CONFIG_NO_SMP */
#define smplock_acquire_nopr() (void)0
#define smplock_release_nopr() (void)0
#endif /* CONFIG_NO_SMP */

#define LOCK_PARAMS pflag_t _l_was
#define LOCK_ARGS   _l_was

#define lock_acquire()                         \
	do {                                       \
		pflag_t _l_was = PREEMPTION_PUSHOFF(); \
		smplock_acquire_nopr()
#define lock_break()             \
		(smplock_release_nopr(), \
		 PREEMPTION_POP(_l_was))
#define lock_regain()                   \
		(_l_was = PREEMPTION_PUSHOFF(), \
		 smplock_acquire_nopr())
#define lock_release()           \
		 smplock_release_nopr(); \
		 PREEMPTION_POP(_l_was); \
	} __WHILE0


/* [0..1] Tree of traced nodes */
PRIVATE ATTR_MALL_UNTRACKED ATREE_HEAD(struct trace_node) nodes = NULL;

/* Debug-heap used for allocating `struct trace_node' objects. */
INTERN struct heap trace_heap =
HEAP_INIT(PAGESIZE * 4,
          PAGESIZE * 16,
          HINT_GETADDR(KERNEL_VMHINT_DHEAP),
          HINT_GETMODE(KERNEL_VMHINT_DHEAP));
DEFINE_VALIDATABLE_HEAP(trace_heap);
DEFINE_DBG_BZERO_OBJECT(trace_heap.h_lock);
#define TRACE_HEAP_FLAGS (GFP_NORMAL | GFP_LOCKED | GFP_PREFLT)



PRIVATE NOBLOCK void
NOTHROW(KCALL trace_node_free)(struct trace_node *__restrict self) {
	heap_free_untraced(&trace_heap,
	                   self,
	                   self->tn_size,
	                   TRACE_HEAP_FLAGS);
}


/* Generator-compatible ("%[gen:c]") function to print the traceback of a given node. */
PRIVATE ATTR_COLDTEXT ssize_t KCALL
trace_node_print_traceback(struct trace_node *__restrict self,
                           pformatprinter printer, void *arg) {
	ssize_t temp, result = 0;
	size_t i, max_count;
	if unlikely(!TRACE_NODE_KIND_HAS_TRACEBACK(self->tn_kind))
		goto done;
	max_count = trace_node_traceback_count(self);
	for (i = 0; i < max_count; ++i) {
		void const *prev_pc, *pc;
		pc = trace_node_traceback_vector(self)[i];
		if (!pc)
			break;
		prev_pc = instruction_trypred(pc, INSTRLEN_ISA_DEFAULT);
		if (i) {
			temp = addr2line_printf(printer, arg, prev_pc, pc,
			                        "Called here");
		} else {
			temp = addr2line_printf(printer, arg, prev_pc, pc,
			                        "Allocated from here [tid=%" PRIu32 "]",
			                        self->tn_tid);
		}
		if unlikely(temp < 0)
			goto err;
		result += temp;
	}
done:
	return result;
err:
	return temp;
}



/* Find the trace-block that `ptr' is apart of and remove it.
 * If no such block exists, the kernel will panic.
 * For this purpose, `CEIL_ALIGN(ptr, sizeof(void *))' must
 * point somewhere into an address range that has previously
 * been registered by `kmalloc_trace()'.
 * NOTE: When `ptr' is `NULL', then this function does nothing. */
PUBLIC ATTR_NOINLINE NOBLOCK void
NOTHROW(KCALL kmalloc_untrace)(void *ptr) {
	struct trace_node *node;
	if unlikely(!ptr)
		return;
	ptr = (void *)CEIL_ALIGN((uintptr_t)ptr, sizeof(void *));
	lock_acquire();
	node = trace_node_tree_remove(&nodes, (uintptr_t)ptr);
	if unlikely(!node) {
		lock_break();
		kernel_panic_n(/* n_skip: */ 1,
		               "kmalloc_untrace(%p): No node at this location",
		               ptr);
		return;
	}
	switch (node->tn_kind) {

	case TRACE_NODE_KIND_MALL: {
		/* kmalloc_untrace() cannot be used to untrace internal kmalloc() tracings! */
		lock_break();
		kernel_panic_n(/* n_skip: */ 1,
		               "kmalloc_untrace(%p): Cannot untrace kmalloc()-pointer %p...%p\n"
		               "%[gen:c]",
		               ptr, trace_node_umin(node), trace_node_umax(node),
		               &trace_node_print_traceback, node);
		trace_node_free(node);
		return;
	}	break;

	case TRACE_NODE_KIND_BITSET: {
		size_t i, ptr_index, minbit, maxbit, limit;
		/* Special case: Partially untraced node. */
		ptr_index = (size_t)((byte_t *)ptr - trace_node_uaddr(node)) / sizeof(void *);
		limit     = trace_node_bitset_count(node);
		assert(limit);
		--limit;
		assert(ptr_index <= limit);
		if unlikely(!trace_node_bitset_bitget(node, ptr_index)) {
			lock_break();
			kernel_panic_n(/* n_skip: */ 1,
			               "kmalloc_untrace(%p): Node at %p...%p was already untraced",
			               ptr, trace_node_umin(node), trace_node_umax(node));
			return;
		}
		minbit = ptr_index;
		maxbit = ptr_index;
		while (minbit > 0 && trace_node_bitset_bitget(node, minbit - 1))
			--minbit;
		while (maxbit < limit && trace_node_bitset_bitget(node, maxbit + 1))
			++maxbit;
		if (minbit == 0 && maxbit == limit) {
			/* Completely remove this node. */
			lock_break();
			trace_node_free(node);
			return;
		}
		/* Mark all address locations in [minbit, maxbit] as untraced. */
		for (i = minbit; i <= maxbit; ++i)
			trace_node_bitset_bitoff(node, i);
		/* Re-insert the node into the tree */
		trace_node_tree_insert(&nodes, node);
		lock_break();
		return;
	}	break;

	default: break;
	}

	/* Since we're supposed to get rid of the whole node, we can simply
	 * release the trace-lock, and free the node, since we've already
	 * removed it from the tree. */
	lock_release();
	trace_node_free(node);
}



PRIVATE ATTR_NOINLINE NOBLOCK void
NOTHROW(KCALL kmalloc_untrace_n_impl)(void *base, size_t num_bytes,
                                      unsigned int n_skip) {
	uintptr_t endaddr;
	if unlikely(!IS_ALIGNED((uintptr_t)base, sizeof(void *))) {
		size_t offset = sizeof(void *) - ((uintptr_t)base & (sizeof(void *) - 1));
		if unlikely(offset >= num_bytes)
			return;
		base = (byte_t *)base + offset;
		num_bytes -= offset;
	}
	assert(((uintptr_t)base & (sizeof(void *) - 1)) == 0);
	num_bytes = FLOOR_ALIGN(num_bytes, sizeof(void *));
	endaddr   = (uintptr_t)base + num_bytes;
	while (num_bytes) {
		struct trace_node *node;
		lock_acquire();
		node = trace_node_tree_remove(&nodes, (uintptr_t)base);
		if unlikely(!node) {
			lock_break();
			kernel_panic_n(/* n_skip: */ n_skip + 1,
			               "kmalloc_untrace_n(%p...%p): No node at %p",
			               base, endaddr - 1, base);
			return;
		}
		if unlikely(node->tn_kind == TRACE_NODE_KIND_MALL) {
			lock_break();
			kernel_panic_n(/* n_skip: */ n_skip + 1,
			               "kmalloc_untrace_n(%p...%p): Cannot untrace kmalloc()-pointer %p...%p\n"
			               "%[gen:c]",
			               base, endaddr - 1, trace_node_umin(node), trace_node_umax(node),
			               &trace_node_print_traceback, node);
			trace_node_free(node);
			return;
		}
		assert((uintptr_t)base >= trace_node_umin(node));
		if (node->tn_kind == TRACE_NODE_KIND_BITSET) {
			/* Assert that every byte of the given range is still marked as traced. */
			size_t i, minbit, maxbit;
			minbit = ((uintptr_t)base - trace_node_umin(node)) / sizeof(void *);
			maxbit = trace_node_bitset_count(node);
			if (endaddr <= trace_node_umax(node)) {
				size_t new_maxbit;
				new_maxbit = ((endaddr - sizeof(void *)) - trace_node_umin(node)) / sizeof(void *);
				assert(new_maxbit < maxbit);
				maxbit = new_maxbit;
			}
			assert(minbit <= maxbit);
			for (i = minbit; i <= maxbit; ++i) {
				size_t untrace_min, untrace_max;
				if (trace_node_bitset_bitget(node, i))
					continue;
				/* oops... Found a bit that's already been untraced... */
				untrace_min = i;
				while (i < maxbit && !trace_node_bitset_bitget(node, i + 1))
					++i;
				untrace_max = i;
				lock_break();
				kernel_panic_n(/* n_skip: */ n_skip + 1,
				               "kmalloc_untrace_n(%p...%p): Data at %p...%p (of node %p...%p) was already untraced",
				               base, endaddr - 1,
				               trace_node_umin(node) + untrace_min * sizeof(void *),
				               (trace_node_umin(node) + (untrace_max + 1) * sizeof(void *)) - 1,
				               trace_node_umin(node), trace_node_umax(node));
				trace_node_free(node);
				return;
			}
		}
		if (endaddr > trace_node_umax(node)) {
			byte_t *uend;
			/* Untrace range overlaps with the end of this node.
			 * -> Can either remove fully, or at least truncate the end. */
			if ((uintptr_t)base <= trace_node_umin(node)) {
				/* Fully remove this node! */
				lock_break();
				base      = trace_node_uend(node);
				num_bytes = endaddr - (uintptr_t)base;
				trace_node_free(node);
				continue;
			}
			/* Truncate the node near its upper end. */
			uend                 = trace_node_uend(node);
			node->tn_link.a_vmax = (uintptr_t)base;
			base                 = uend;
			trace_node_tree_insert(&nodes, node);
			lock_break();
			num_bytes = endaddr - (uintptr_t)base;
			continue;
		}
		/* Check if we can truncate the node at its base */
		if ((uintptr_t)base <= trace_node_umin(node)) {
			assert((uintptr_t)base == trace_node_umin(node));
			/* Truncate the node near its lower end. */
			assert(endaddr > node->tn_link.a_vmin);
			if (node->tn_kind == TRACE_NODE_KIND_BITSET) {
				size_t bitset_offset, count, dst, src;
				/* Shift down the bits of the is-traced bitset down by `bitset_offset' */
				bitset_offset = (endaddr - node->tn_link.a_vmin) / sizeof(void *);
				assert(bitset_offset != 0);
				count = trace_node_bitset_count(node) - bitset_offset;
				assert(count != 0);
				dst   = 0;
				src   = bitset_offset;
				for (;;) {
					if (trace_node_bitset_bitget(node, src)) {
						trace_node_bitset_biton(node, dst);
					} else {
						trace_node_bitset_bitoff(node, dst);
					}
					if (!count)
						break;
					--count;
					++dst;
					++src;
				}
			}
			node->tn_link.a_vmin = endaddr;
			trace_node_tree_insert(&nodes, node);
			lock_break();
			break;
		}
		/* Must split the node (the caller wants us to cut a hole into the middle) */
		if (node->tn_kind == TRACE_NODE_KIND_BITSET) {
			/* Special case: This already is a bitset node, (and code above already
			 *               asserted that our range is still traced), so all we have
			 *               to do is clear some more bits corresponding to our range. */
			size_t i, minbit, maxbit;
			minbit = ((uintptr_t)base - trace_node_umin(node)) / sizeof(void *);
			maxbit = ((endaddr - sizeof(void *)) - trace_node_umin(node)) / sizeof(void *);
			assert(minbit <= maxbit);
			for (i = minbit; i <= maxbit; ++i) {
				assert(trace_node_bitset_bitget(node, i));
				trace_node_bitset_bitoff(node, i);
			}
			trace_node_tree_insert(&nodes, node);
			lock_break();
			break;
		}

#if 0
		/* TODO: Try to allocate a secondary node (but do this without holding the trace-lock) */
#endif

		/* Fallback: We've failed to allocate a secondary node, and so we have to convert the
		 *           node that we've already got into a `TRACE_NODE_KIND_BITSET'-kind node,
		 *           who's bitset is all ones, except for the bits from the range that our
		 *           caller wants us to mark as untraced.
		 * The only down-side is that in doing this, we loose the node's traceback */
		node->tn_kind = TRACE_NODE_KIND_BITSET;
		memset(trace_node_bitset_words(node), 0xff,
		       CEILDIV(trace_node_bitset_count(node), NBBY));
		{
			size_t i, minbit, maxbit;
			minbit = ((uintptr_t)base - trace_node_umin(node)) / sizeof(void *);
			maxbit = ((endaddr - sizeof(void *)) - trace_node_umin(node)) / sizeof(void *);
			assert(minbit <= maxbit);
			for (i = minbit; i <= maxbit; ++i)
				trace_node_bitset_bitoff(node, i);
		}
		trace_node_tree_insert(&nodes, node);
		lock_release();
		break;
	}
}

/* Similar to `kmalloc_untrace()', but explicitly untrace only the given address range.
 * Just like `kmalloc_trace()', this function will truncate the given address range
 * to have it start/end at a pointer-aligned byte boundary.
 * This function will then try to truncate the internal descriptor(s) used for the given
 * address range (which is allowed to span multiple prior invocations of `kmalloc_trace()',
 * so-long as no gaps exist between individually traced ranges), and if this fails (which
 * can happen when this function would have to carve out a chunk from the middle of some
 * pre-existing trace-node), that node will be changed such that the given range is marked
 * as untraced, which will prevent the kernel from accessing its contents during GC scans.
 * In practice though, you shouldn't need to concern yourself with this behavior. */
PUBLIC ATTR_NOINLINE NOBLOCK void
NOTHROW(KCALL kmalloc_untrace_n)(void *base, size_t num_bytes) {
	kmalloc_untrace_n_impl(base, num_bytes, 1);
}

/* Return the traceback stored inside of the debug descriptor of `ptr'.
 * When `ptr' is `NULL', or debug-malloc is disabled, then this function
 * will simply return with `0'. Otherwise, `CEIL_ALIGN(ptr, sizeof(void *))'
 * must point into a currently traced data-block, and if it doesn't, then
 * this function will trigger a kernel panic. (unless the kernel has already
 * been poisoned, in which case it'll simply return `0')
 * When `ptr' is a SLAB-pointer, this function will also always return `0'
 * @param: tb:     Buffer to-be filled with traceback PC-locations pointer.
 * @param: buflen: Available buffer length in `tb' (# of allocated entries; not bytes)
 * @param: p_alloc_roottid: When non-NULL, store the root-namespace TID of the thread
 *                          that originally allocated the block of `ptr' (when allocated
 *                          by a kernel thread, TID=0 will be returned)
 * @return: * :    The total number of traceback PC-locations available for `ptr'
 *                 When `> buflen', not all locations were written to `*tb', and
 *                 the caller should re-attempt the call with more space. */
PUBLIC ATTR_NOINLINE NOBLOCK size_t
NOTHROW(KCALL kmalloc_traceback)(void *ptr, /*out*/ void **tb, size_t buflen,
                                 pid_t *p_alloc_roottid) {
	size_t result;
	struct trace_node *node;
	if unlikely(!ptr)
		return 0;
#ifdef CONFIG_USE_SLAB_ALLOCATORS
	if (KERNEL_SLAB_CHECKPTR(ptr))
		return 0;
#endif /* CONFIG_USE_SLAB_ALLOCATORS */
	lock_acquire();
	node = trace_node_tree_locate(nodes, (uintptr_t)ptr);
	if unlikely(!node) {
		lock_break();
		kernel_panic_n(/* n_skip: */ 1,
		               "kmalloc_traceback(%p): No node at this address",
		               ptr);
		return 0;
	}
	/* Write-back the TID of the thread that originally allocated the node. */
	if (p_alloc_roottid)
		*p_alloc_roottid = node->tn_tid;
	/* Check if this node has a traceback */
	if (!TRACE_NODE_KIND_HAS_TRACEBACK(node->tn_kind)) {
		result = 0;
	} else {
		result = trace_node_traceback_count(node);
		/* Copy all traceback entries for which the caller has space. */
		memcpy(tb, trace_node_traceback_vector(node),
		       MIN(result, buflen), sizeof(void *));
	}
	lock_release();
	return result;
}

#if CONFIG_MALL_HEAD_SIZE != 0 || CONFIG_MALL_TAIL_SIZE != 0
#define HAVE_kmalloc_validate_node
PRIVATE ATTR_NOINLINE NOBLOCK bool
NOTHROW(KCALL kmalloc_validate_node)(unsigned int n_skip,
                                     struct trace_node *__restrict node) {
	unsigned int i;
	u32 *base;
	/* Check if the node has padding which we can validate. */
	if (!TRACE_NODE_KIND_HAS_PADDING(node->tn_kind))
		goto done;
	base = (u32 *)trace_node_uaddr(node);
#if CONFIG_MALL_HEAD_SIZE != 0
	for (i = sizeof(size_t) / 4; i < CONFIG_MALL_HEAD_SIZE / 4; ++i) {
		if (base[i] != CONFIG_MALL_HEAD_PATTERN) {
			u32 word;
			word = CONFIG_MALL_HEAD_PATTERN;
			base += i;
			while (*(u8 *)base == ((u8 *)&word)[(uintptr_t)base & 3])
				base = (u32 *)((byte_t *)base + 1);
			kernel_panic_n(n_skip + 1,
			               "Corrupted MALL header in at %p (offset %" PRIdSIZ " from %p...%p)\n"
			               "%$[hex]\n"
			               "%[gen:c]",
			               base, (uintptr_t)base - (trace_node_umin(node) + CONFIG_MALL_HEAD_SIZE),
			               trace_node_uaddr(node) + CONFIG_MALL_HEAD_SIZE,
			               trace_node_uend(node) - CONFIG_MALL_TAIL_SIZE,
			               /* %$[hex]  */ trace_node_usize(node), trace_node_uaddr(node),
			               /* %[gen:c] */ &trace_node_print_traceback, node);
			return false;
		}
	}
#endif /* CONFIG_MALL_HEAD_SIZE != 0 */
#if CONFIG_MALL_TAIL_SIZE != 0
	base = (u32 *)(trace_node_umax(node) + 1 - CONFIG_MALL_TAIL_SIZE);
	for (i = 0; i < CONFIG_MALL_TAIL_SIZE / 4; ++i) {
		if (base[i] != CONFIG_MALL_TAIL_PATTERN) {
			u32 word;
			word = CONFIG_MALL_TAIL_PATTERN;
			base += i;
			while (*(u8 *)base == ((u8 *)&word)[(uintptr_t)base & 3])
				base = (u32 *)((byte_t *)base + 1);
			kernel_panic_n(n_skip + 1,
			               "Corrupted MALL tail in at %p (offset %" PRIdSIZ " from %p...%p; "
			               "offset %" PRIuSIZ " from end of usable memory)\n"
			               "%$[hex]\n"
			               "%[gen:c]",
			               base, (uintptr_t)base - (trace_node_umin(node) + CONFIG_MALL_HEAD_SIZE),
			               trace_node_uaddr(node) + CONFIG_MALL_HEAD_SIZE,
			               trace_node_uend(node) - CONFIG_MALL_TAIL_SIZE,
			               (uintptr_t)base - (trace_node_umax(node) + 1 - CONFIG_MALL_TAIL_SIZE),
			               /* %$[hex]  */ trace_node_usize(node), trace_node_uaddr(node),
			               /* %[gen:c] */ &trace_node_print_traceback, node);
			return false;
		}
	}
#endif /* CONFIG_MALL_TAIL_SIZE != 0 */
done:
	return true;
}


PRIVATE ATTR_NOINLINE NOBLOCK void
NOTHROW(KCALL kmalloc_validate_walktree)(unsigned int n_skip,
                                         struct trace_node *__restrict node) {
again:
	kmalloc_validate_node(n_skip + 1, node);
	if (node->tn_link.a_min) {
		if (node->tn_link.a_max)
			kmalloc_validate_walktree(n_skip + 1, node->tn_link.a_max);
		node = node->tn_link.a_min;
		goto again;
	}
	if (node->tn_link.a_max) {
		node = node->tn_link.a_max;
		goto again;
	}
}
#endif /* CONFIG_MALL_HEAD_SIZE != 0 || CONFIG_MALL_TAIL_SIZE != 0 */

/* Validate that headers/footers of data blocks returned by kmalloc() havn't
 * been modified (which can accidentally happen as the result of programming
 * errors, such as array over-/under-runs)
 * If inconsistencies are found, the kernel will panic.
 * s.a. `heap_validate()' and `heap_validate_all()' */
PUBLIC ATTR_NOINLINE NOBLOCK void
NOTHROW(KCALL kmalloc_validate)(void) {
#if CONFIG_MALL_HEAD_SIZE != 0 || CONFIG_MALL_TAIL_SIZE != 0
	lock_acquire();
	if (nodes)
		kmalloc_validate_walktree(1, nodes);
	lock_release();
#else /* CONFIG_MALL_HEAD_SIZE != 0 || CONFIG_MALL_TAIL_SIZE != 0 */
	/* nothing... */
#endif /* CONFIG_MALL_HEAD_SIZE == 0 && CONFIG_MALL_TAIL_SIZE == 0 */
}


/************************************************************************/
/* GC MEMORY LEAK DETECTOR                                              */
/************************************************************************/

/* The current version number used when searching for memory leaks.
 * This value is used to identity pointers that haven't been reached/search yet. */
PRIVATE ATTR_MALL_UNTRACKED u8 gc_version = 0;


/* Mark a given `ptr' as reachable, returning the # of nodes reached from it. */
PRIVATE NOBLOCK ATTR_COLDTEXT size_t
NOTHROW(KCALL gc_reachable_pointer)(void *ptr);

/* Scan the given data-block for reachable pointers. */
PRIVATE NOBLOCK ATTR_COLDTEXT size_t
NOTHROW(KCALL gc_reachable_data)(void const *base, size_t num_bytes);


#ifdef CONFIG_USE_SLAB_ALLOCATORS
#define DECLARE_SLAB_DESCRIPTOR(size, _) \
	INTDEF struct slab_descriptor slab_desc##size;
SLAB_FOREACH_SIZE(DECLARE_SLAB_DESCRIPTOR, _)
#undef DECLARE_SLAB_DESCRIPTOR

/* Vector of pointers to all existing slab descriptors. */
PRIVATE ATTR_COLDRODATA struct slab_descriptor *const gc_slab_descs[] = {
#define DECLARE_SLAB_DESCRIPTOR(size, _) \
	&slab_desc##size,
	SLAB_FOREACH_SIZE(DECLARE_SLAB_DESCRIPTOR, _)
#undef DECLARE_SLAB_DESCRIPTOR
};

PRIVATE NOBLOCK ATTR_COLDTEXT NONNULL((1)) void
NOTHROW(KCALL gc_slab_reset_reach_slab)(struct slab *__restrict self) {
#if (SLAB_SEGMENT_STATUS_REACH & SLAB_SEGMENT_STATUS_ALLOC) != 0
	size_t i, count;
	uintptr_t *bits;
	bits = (uintptr_t *)(self + 1);
	bits  = (uintptr_t *)(self + 1);
	count = SLAB_LENGTHOF_BITSET(self->s_size);
	for (i = 0; i < count; ++i)
		bits[i] &= _SLAB_SEGMENT_STATUS_WORDMASK(SLAB_SEGMENT_STATUS_ALLOC);
#else /* (SLAB_SEGMENT_STATUS_REACH & SLAB_SEGMENT_STATUS_ALLOC) != 0 */
#error Not implemented
#endif /* (SLAB_SEGMENT_STATUS_REACH & SLAB_SEGMENT_STATUS_ALLOC) == 0 */
}

PRIVATE NOBLOCK ATTR_COLDTEXT void
NOTHROW(KCALL gc_slab_reset_reach_slab_chain)(struct slab *self) {
	for (; self; self = self->s_next)
		gc_slab_reset_reach_slab(self);
}

PRIVATE NOBLOCK ATTR_COLDTEXT NONNULL((1)) void
NOTHROW(KCALL gc_slab_reset_reach_desc)(struct slab_descriptor *__restrict self) {
	gc_slab_reset_reach_slab_chain(self->sd_free);
	gc_slab_reset_reach_slab_chain(self->sd_used);
}

PRIVATE NOBLOCK ATTR_COLDTEXT void
NOTHROW(KCALL gc_slab_reset_reach)(void) {
	unsigned int i;
	for (i = 0; i < COMPILER_LENOF(gc_slab_descs); ++i)
		gc_slab_reset_reach_desc(gc_slab_descs[i]);
}

PRIVATE ATTR_COLDBSS bool gc_slab_leak_did_notify_noslab_boot_option = false;

PRIVATE NOBLOCK ATTR_COLDTEXT NONNULL((1)) size_t
NOTHROW(KCALL gc_slab_unreachable_segment)(void *base, size_t num_bytes) {
	/* Slab allocations are way too light-weight to be able to support tracebacks.
	 * As such, we _are_ able to detect slab memory leaks, but we aren't able to
	 * tell the user where/how the leaking memory was allocated.
	 * However, since slab allocators are always allowed to either:
	 *
	 *   - Allocate regular kmalloc()-style memory (as a matter of fact, most calls
	 *     to slab allocators actually use compile-time dispatching from inside of
	 *     a FORCELOCAL kmalloc() wrapper)
	 *     Booting with `noslab' will modify the text of these functions to directly
	 *     call forward to the regular heap functions, which are then able to generate
	 *     proper tracebacks.
	 *
	 *   - Simply return `NULL'. This might come as a surprise, but because all kernel
	 *     slab memory _must_ be continuous, there is a chance that some conflicting
	 *     memory mapping will simply prevent us from expanding slab space any further.
	 *     Due to this case, pure slab allocators (i.e. slab allocator function that
	 *     will never return conventional heap memory) are always allowed to simply
	 *     return true, which is indicating of their inability to allocate additional
	 *     slab memory.
	 */
	if (!gc_slab_leak_did_notify_noslab_boot_option) {
		gc_slab_leak_did_notify_noslab_boot_option = true;
		printk(KERN_RAW "slab: Slab memory leaks don't include tracebacks.\n"
		                "slab: Consider rebooting with `noslab'\n");
	}
	/* Slabs are always small, to to help at least a little bit, also dump the
	 * contents of leaked blocks. - Those might still be (somewhat) useful... */
	printk(KERN_RAW "slab: Leaked %" PRIuSIZ " bytes of heap-memory at %p...%p\n"
	                "%$[hex]\n",
	       num_bytes, base, (byte_t *)base + num_bytes - 1,
	       num_bytes, base);
	return 1;
}

PRIVATE NOBLOCK ATTR_COLDTEXT NONNULL((1)) size_t
NOTHROW(KCALL gc_slab_unreachable_slab)(struct slab *__restrict self) {
	/* Search for slabs with a status of `SLAB_SEGMENT_STATUS_ALLOC' */
	uintptr_t *bits;
	size_t i, count;
	size_t result = 0;
	bits  = (uintptr_t *)(self + 1);
	count = SLAB_SEGMENT_COUNT(self->s_size);
	for (i = 0; i < count; ++i) {
		unsigned int status;
		status = (bits[_SLAB_SEGMENT_STATUS_WORD(i)] >>
		          _SLAB_SEGMENT_STATUS_SHFT(i)) &
		         SLAB_SEGMENT_STATUS_MASK;
		if likely(status != SLAB_SEGMENT_STATUS_ALLOC)
			continue;
		/* Well... It's a leak :( */
		result += gc_slab_unreachable_segment((byte_t *)self +
		                                   SLAB_SEGMENT_OFFSET(self->s_size) +
		                                   i * self->s_size,
		                                   self->s_size);
	}
	return result;
}

PRIVATE NOBLOCK ATTR_COLDTEXT size_t
NOTHROW(KCALL gc_slab_unreachable_slab_chain)(struct slab *self) {
	size_t result = 0;
	for (; self; self = self->s_next)
		result += gc_slab_unreachable_slab(self);
	return result;
}

PRIVATE NOBLOCK ATTR_COLDTEXT NONNULL((1)) size_t
NOTHROW(KCALL gc_slab_unreachable_desc)(struct slab_descriptor *__restrict self) {
	size_t result;
	result = gc_slab_unreachable_slab_chain(self->sd_free);
	result += gc_slab_unreachable_slab_chain(self->sd_used);
	return result;
}

PRIVATE NOBLOCK ATTR_COLDTEXT size_t
NOTHROW(KCALL gc_slab_unreachable)(void) {
	size_t result = 0;
	unsigned int i;
	for (i = 0; i < COMPILER_LENOF(gc_slab_descs); ++i)
		result += gc_slab_unreachable_desc(gc_slab_descs[i]);
	return result;
}

PRIVATE NOBLOCK ATTR_COLDTEXT size_t
NOTHROW(KCALL gc_reachable_slab_pointer)(void *ptr) {
	struct slab *s;
	byte_t *slab_segments_start;
	uintptr_t index, word, value;
	uintptr_t *bits;
	size_t result;
	unsigned int shift;
	s                   = SLAB_GET(ptr);
	slab_segments_start = (byte_t *)s + SLAB_SEGMENT_OFFSET(s->s_size);
	index               = (uintptr_t)((byte_t *)ptr - slab_segments_start) / s->s_size;
	if unlikely(index >= (size_t)SLAB_SEGMENT_COUNT(s->s_size))
		return 0; /* Not a valid slab user-segment pointer. */
	bits = (uintptr_t *)(s + 1);
	/* Check if `ptr' is actually allocated inside of `s', and wasn't already reached. */
	word  = _SLAB_SEGMENT_STATUS_WORD(index);
	shift = _SLAB_SEGMENT_STATUS_SHFT(index);
	value = bits[word];
	if (((value >> shift) & SLAB_SEGMENT_STATUS_MASK) != SLAB_SEGMENT_STATUS_ALLOC)
		return 0;
	/* Alright! So we've got a reachable slab-pointer!
	 * -> Mark it as such! */
#if (SLAB_SEGMENT_STATUS_REACH & SLAB_SEGMENT_STATUS_ALLOC) != 0
	bits[word] = value | ((uintptr_t)SLAB_SEGMENT_STATUS_REACH << shift);
#else /* (SLAB_SEGMENT_STATUS_REACH & SLAB_SEGMENT_STATUS_ALLOC) != 0 */
	bits[word] = (value & ~((uintptr_t)SLAB_SEGMENT_STATUS_ALLOC << shift)) |
	             /*     */ ((uintptr_t)SLAB_SEGMENT_STATUS_REACH << shift);
#endif /* (SLAB_SEGMENT_STATUS_REACH & SLAB_SEGMENT_STATUS_ALLOC) == 0 */
	/* Recursively scan the memory pointed-to by the segment. */
	result = gc_reachable_data(slab_segments_start +
	                           index * s->s_size,
	                           s->s_size);
	return result + 1;
}
#endif /* CONFIG_USE_SLAB_ALLOCATORS */





PRIVATE NOBLOCK ATTR_COLDTEXT size_t
NOTHROW(KCALL gc_reachable_pointer)(void *ptr) {
	/* Do a number of checks to filter out often-found pointers
	 * for which we know that they don't point to heap structures.
	 *
	 * By doing this here, we can cut down on a lot of boiler-plate
	 * code that's needed to verify heap pointers proper. */
	struct trace_node *node;

	/* Check if NULL is part of kernel-space. */
#if (defined(KERNELSPACE_LOWMEM) || \
     (defined(KERNELSPACE_HIGHMEM) && 0 >= KERNELSPACE_BASE))
	if (ptr == NULL)
		return 0; /* Optimization: NULL pointer */
#endif /* ADDR_ISKERN(NULL) */

	/* If the pointer doesn't belong to kernel-space, we ignore it. */
	if (!ADDR_ISKERN(ptr))
		return 0;

	/* Unaligned pointer -> not a heap pointer */
	if (((uintptr_t)ptr & (sizeof(void *) - 1)) != 0)
		return 0;

#if __SIZEOF_POINTER__ == 4
#ifdef CONFIG_DEBUG_HEAP
	if ((uintptr_t)ptr == DEBUGHEAP_NO_MANS_LAND)
		return 0; /* Optimization: No mans land */
	if ((uintptr_t)ptr == DEBUGHEAP_FRESH_MEMORY)
		return 0; /* Optimization: Fresh memory. */
#endif /* CONFIG_DEBUG_HEAP */
	if ((uintptr_t)ptr == 0xcccccccc)
		return 0; /* Optimization: Stack pre-initialization. */
#endif /* __SIZEOF_POINTER__ == 4 */

#ifdef CONFIG_USE_SLAB_ALLOCATORS
	/* Special handling for slab pointers. */
	if (KERNEL_SLAB_CHECKPTR(ptr))
		return gc_reachable_slab_pointer(ptr);
#endif /* CONFIG_USE_SLAB_ALLOCATORS */

	node = trace_node_tree_locate(nodes, (uintptr_t)ptr);
	if (!node)
		return 0;
	if (node->tn_reach == gc_version)
		return 0; /* Already reached. */
	if (node->tn_kind == TRACE_NODE_KIND_BITSET) {
		/* When node is `TRACE_NODE_KIND_BITSET', then only mark it
		 * as reachable when `ptr' points into its traced portion. */
		size_t index;
		/* HINT: We already know that `ptr' is POINTER-aligned (see the other check above) */
		index = ((uintptr_t)ptr - trace_node_umin(node)) / sizeof(void *);
		if (!trace_node_bitset_bitget(node, index))
			return 0; /* This specific pointer isn't actually traced. */
	}
	node->tn_reach = gc_version;
	return 1;
}


PRIVATE NOBLOCK ATTR_COLDTEXT size_t
NOTHROW(KCALL gc_reachable_data)(void const *base, size_t num_bytes) {
	size_t result = 0;
	if unlikely((uintptr_t)base & (sizeof(void *) - 1)) {
		/* Align the base pointer. */
		size_t offset;
		offset = sizeof(void *) - ((uintptr_t)base & (sizeof(void *) - 1));
		if unlikely(offset >= num_bytes)
			goto done;
		num_bytes -= offset;
		base = (byte_t *)base + offset;
	}
	while (num_bytes >= sizeof(void *)) {
		void *ptr;
		size_t page_bytes = PAGESIZE - ((uintptr_t)base & PAGEMASK);
		/* Only scan writable pages. */
		if (!pagedir_iswritable((void *)base)) {
			if (page_bytes >= num_bytes)
				break;
			base = (byte_t *)base + page_bytes;
			num_bytes -= page_bytes;
			continue;
		}
		if (page_bytes > num_bytes)
			page_bytes = num_bytes;
		num_bytes -= page_bytes;
		while (page_bytes >= sizeof(void *)) {
			COMPILER_READ_BARRIER();
			ptr = *(void **)base;
			COMPILER_READ_BARRIER();
			/* Check if this is a heap pointer, and if so: mark it as reachable. */
			result += gc_reachable_pointer(ptr);
			base = (byte_t *)base + sizeof(void *);
			page_bytes -= sizeof(void *);
		}
	}
done:
	return result;
}


PRIVATE NOBLOCK ATTR_COLDTEXT size_t
NOTHROW(KCALL gc_reachable_thread_scpustate)(struct task *__restrict thread,
                                             struct scpustate *__restrict context) {
	size_t result = 0;
	/* Search the registers of this thread. */
	if (scpustate_isuser(context)) {
		/* Thread is currently in user-space (meaning its kernel stack is unused) */
	} else {
		unsigned int i;
		void *sp, *stack_min, *stack_end;
		/* Search general-purpose registers. */
		for (i = 0; i < (sizeof(struct gpregs) / sizeof(void *)); ++i)
			result += gc_reachable_pointer(((void **)&context->scs_gpregs)[i]);
		stack_min = vm_node_getmin(&FORTASK(thread, this_kernel_stacknode));
		stack_end = vm_node_getend(&FORTASK(thread, this_kernel_stacknode));
#ifdef scpustate_getkernelpsp
		sp = (void *)scpustate_getkernelpsp(context);
#else /* scpustate_getkernelpsp */ 
		sp = (void *)scpustate_getsp(context);
#endif /* !scpustate_getkernelpsp */
		if (sp > stack_min && sp <= stack_end) {
			/* Search the used portion of the kernel stack. */
			result += gc_reachable_data((byte_t *)sp,
			                            (size_t)((byte_t *)stack_end -
			                                     (byte_t *)sp));
		} else {
			/* Stack pointer is out-of-bounds (no idea what this is
			 * about, but let's just assume the entire stack is allocated) */
			result += gc_reachable_data((byte_t *)stack_min,
			                            (size_t)((byte_t *)stack_end -
			                                     (byte_t *)stack_min));
		}
	}
	return result;
}


PRIVATE NOBLOCK ATTR_COLDTEXT size_t
NOTHROW(KCALL gc_reachable_this_thread)(void) {
	size_t result = 0;
	unsigned int i;
	void *sp, *stack_min, *stack_end;
	struct lcpustate context;
	struct vm_node const *my_stack;
	/* Search the registers of this thread. */
	lcpustate_current(&context);
	/* Search general-purpose registers. */
	for (i = 0; i < (sizeof(struct lcpustate) / sizeof(void *)); ++i)
		result += gc_reachable_pointer(((void **)&context)[i]);
	my_stack = THIS_KERNEL_STACK;
	sp       = (void *)lcpustate_getsp(&context);
	get_stack_for(&stack_min, &stack_end, sp);
	if (sp > stack_min && sp <= stack_end) {
		/* Search the used portion of the kernel stack. */
		result += gc_reachable_data((byte_t *)sp,
		                            (size_t)((byte_t *)stack_end -
		                                     (byte_t *)sp));
		/* If we're running from a custom stack, also search the original kernel stack! */
		if (stack_end != (byte_t *)vm_node_getend(my_stack))
			goto do_search_kernel_stack;
	} else {
		/* Stack pointer is out-of-bounds (no idea what this is
		 * about, but let's just assume the entire stack is allocated) */
do_search_kernel_stack:
		stack_min = vm_node_getmin(my_stack);
		stack_end = vm_node_getend(my_stack);
		result += gc_reachable_data((byte_t *)stack_min,
		                            (size_t)((byte_t *)stack_end -
		                                     (byte_t *)stack_min));
	}
	return result;
}


PRIVATE NOBLOCK ATTR_COLDTEXT size_t
NOTHROW(KCALL gc_reachable_recursion)(struct trace_node *__restrict node) {
	size_t result = 0;
again:
	if (node->tn_reach == gc_version &&
	    node->tn_visit != gc_version &&
	    !(node->tn_flags & TRACE_NODE_FLAG_NOWALK)) {
		byte_t *minaddr, *maxaddr;
		/* Scan the user-data block of this node. */
		minaddr = (byte_t *)trace_node_umin(node);
		maxaddr = (byte_t *)trace_node_umax(node);
		switch (node->tn_kind) {

		case TRACE_NODE_KIND_BITSET: {
			/* Only scan data that is still marked as being traced. */
			size_t i, start, count;
			count = trace_node_bitset_count(node);
			for (i = 0; i < count; ++i) {
				if (!trace_node_bitset_bitget(node, i))
					continue;
				/* Found a traced range (find its end) */
				start = i;
				for (;;) {
					++i;
					if (i >= count)
						break;
					if (!trace_node_bitset_bitget(node, i))
						break;
				}
				/* Scan the contents of this particular (still-traced) range. */
				result += gc_reachable_data(minaddr + start * sizeof(void *),
				                            (i - start) * sizeof(void *));
			}
		}	break;

		case TRACE_NODE_KIND_MALL:
			/* Don't scan the header- or tail-area! */
			minaddr += CONFIG_MALL_HEAD_SIZE;
			maxaddr -= CONFIG_MALL_TAIL_SIZE;
			ATTR_FALLTHROUGH
		default:
			assert(minaddr <= maxaddr);
			/* Do the recursive scan. */
			result += gc_reachable_data(minaddr, (size_t)(maxaddr - minaddr) + 1);
			break;
		}
		node->tn_visit = gc_version;
	}
	if (node->tn_link.a_min) {
		if (node->tn_link.a_max)
			result += gc_reachable_recursion(node->tn_link.a_max);
		node = node->tn_link.a_min;
		goto again;
	}
	if (node->tn_link.a_max) {
		node = node->tn_link.a_max;
		goto again;
	}
	return result;
}


INTDEF byte_t __debug_malloc_tracked_start[];
INTDEF byte_t __debug_malloc_tracked_end[];
INTDEF byte_t __debug_malloc_tracked_size[];

INTDEF atomic_ref<struct driver_state> current_driver_state ASMNAME("current_driver_state");

PRIVATE NOBLOCK ATTR_COLDTEXT size_t
NOTHROW(KCALL gc_reachable_corepage_chain)(struct vm_corepage *chain) {
	size_t result = 0;
	for (; chain; chain = chain->cp_ctrl.cpc_prev) {
		result += gc_reachable_data((byte_t *)chain->cp_parts,
		                            sizeof(chain->cp_parts));
	}
	return result;
}

PRIVATE NOBLOCK ATTR_COLDTEXT ssize_t
NOTHROW(KCALL gc_reachable_thread)(void *UNUSED(arg),
                                   struct task *thread,
                                   struct taskpid *UNUSED(pid)) {
	size_t result = 0;
	if (thread && thread != THIS_TASK)
		result += gc_reachable_thread_scpustate(thread, FORTASK(thread, this_sstate));
	return (ssize_t)result;
}

#if 0
#define PRINT_LEAKS_SEARCH_PHASE(...) printk(KERN_INFO __VA_ARGS__)
#else
#define PRINT_LEAKS_SEARCH_PHASE(...) (void)0
#endif


/* Scan the kernel for all reachable, traced pointers. */
PRIVATE NOBLOCK ATTR_COLDTEXT void
NOTHROW(KCALL gc_find_reachable)(void) {
	/* Search for memory leaks.
	 * The idea here is not to be able to find all memory blocks that were
	 * leaked, but rather to find anything that ~might~ be referenced.
	 * To do this, we search all places we can think of:
	 *    - Kernel .data & .bss
	 *    - Driver .data & .bss
	 *    - Stacks and general purpose registers of all other threads
	 *    - VMs reachable from those threads (XXX: maybe use kmalloc() for VMs in debug mode?)
	 * For this purpose, any properly aligned data word is considered a
	 * possible pointer and if directed at a known VM node, that node is
	 * saved as reachable.
	 * Following this first pass, we recursively analyze the user-data
	 * blocks of all heap pointers that were reached thus far, until we're
	 * no longer encountering any new ones.
	 * Anything that still hasn't been reached is then considered a leak. */
	PRINT_LEAKS_SEARCH_PHASE("Phase #1: Scan .data + .bss\n");
	gc_reachable_data((byte_t *)__debug_malloc_tracked_start,
	                  (size_t)__debug_malloc_tracked_size);

	/* Search all threads on all CPUs. */
	PRINT_LEAKS_SEARCH_PHASE("Phase #2: Scan running threads\n");
	task_enum_all_noipi_nb(&gc_reachable_thread, NULL);

	PRINT_LEAKS_SEARCH_PHASE("Phase #2.1: Scan the calling thread\n");
	gc_reachable_this_thread();

	/* Scan all allocated COREBASE pointers from the kernel VM.
	 * Since those are randomly sprinkled into the kernel VM tree,
	 * they normally wouldn't be able to forward contained data
	 * pointers, which would then result in us not realizing that
	 * any dynamic node pointed to by them is actually reachable.
	 * NOTE: COREBASE couldn't use `mall_trace()' because that
	 *       could cause infinite recursion when `mall_trace()'
	 *       tried to allocate a new recursion descriptor.
	 */
	PRINT_LEAKS_SEARCH_PHASE("Phase #3: Scan core base\n");
	gc_reachable_corepage_chain(vm_corepage_head);

	/* `vm_corepage_head' only chains core-base pages
	 * that contain at least one non-allocated page-slot!
	 *
	 * As such, we must also search a secondary chain of
	 * pages that is used to represent ones that are fully
	 * allocated. */
	gc_reachable_corepage_chain(vm_corepage_full);
	

	PRINT_LEAKS_SEARCH_PHASE("Phase #4: Scan loaded drivers\n");
	{
		struct driver_state *drivers;
		size_t i;
		drivers = current_driver_state.m_pointer;
		for (i = 0; i < drivers->ds_count; ++i) {
			uint16_t j;
			struct driver *drv = drivers->ds_drivers[i];
			if unlikely(wasdestroyed(drv))
				continue;
			/* Since we're in single-core mode, we know that when `drv' isn't
			 * destroyed at this point, it wont get destroyed before we're done! */
			for (j = 0; j < drv->d_phnum; ++j) {
				uintptr_t progaddr;
				size_t progsize;
				if (drv->d_phdr[j].p_type != PT_LOAD)
					continue;
				if (!(drv->d_phdr[j].p_flags & PF_W))
					continue;
				progaddr = (uintptr_t)(drv->d_loadaddr + drv->d_phdr[j].p_vaddr);
				progsize = drv->d_phdr[j].p_memsz;
				progsize += progaddr & PAGEMASK;
				progaddr &= ~PAGEMASK;
				gc_reachable_data((byte_t *)progaddr,
				                  CEIL_ALIGN(progsize, PAGESIZE));
			}
		}
	}

	PRINT_LEAKS_SEARCH_PHASE("Phase #5: Recursively scan reached pointers\n");
	if (nodes) {
		size_t num_found;
		/* With all data collected, recursively scan the data blocks of all reachable nodes.
		 * The recursion takes place because we keep scanning until nothing new shows up. */
		do {
			num_found = gc_reachable_recursion(nodes);
			PRINT_LEAKS_SEARCH_PHASE("Phase #5: Reached %" PRIuSIZ " pointers\n", num_found);
		} while (num_found);
	}
}

PRIVATE NOBLOCK ATTR_COLDTEXT void
NOTHROW(KCALL gc_print_unreachable_node)(struct trace_node *__restrict node) {
	size_t tracesize, i;
	void **traceback, *pc;
	traceback = trace_node_traceback_vector(node);
	tracesize = trace_node_traceback_count(node);
	pc        = tracesize ? traceback[0] : NULL;
	switch (node->tn_kind) {

	case TRACE_NODE_KIND_MALL:
		addr2line_printf(&syslog_printer, SYSLOG_LEVEL_RAW,
		                 instruction_trypred(pc, INSTRLEN_ISA_DEFAULT), pc,
		                 "Leaked %" PRIuSIZ " bytes of kmalloc-memory at %p...%p [tid=%" PRIu32 "]",
		                 trace_node_usize(node) - CONFIG_MALL_HEAD_SIZE + CONFIG_MALL_TAIL_SIZE,
		                 trace_node_umin(node) + CONFIG_MALL_HEAD_SIZE,
		                 trace_node_umax(node) - CONFIG_MALL_TAIL_SIZE,
		                 node->tn_tid);
		goto common_print_traceback_1plus;

	case TRACE_NODE_KIND_BITSET: {
		printk(KERN_RAW "Leaked %" PRIuSIZ " bytes of partially untraced memory between %p...%p [tid=%" PRIu32 "]\n",
		       trace_node_usize(node), trace_node_umin(node), trace_node_umax(node), node->tn_tid);
		/* TODO: Print one entry for every consecutive address range of still-traced words. */
	}	break;

	default:
		addr2line_printf(&syslog_printer, SYSLOG_LEVEL_RAW,
		                 instruction_trypred(pc, INSTRLEN_ISA_DEFAULT), pc,
		                 "Leaked %" PRIuSIZ " bytes of heap-memory at %p...%p [tid=%" PRIu32 "]",
		                 trace_node_usize(node),
		                 trace_node_umin(node), trace_node_umax(node),
		                 node->tn_tid);
common_print_traceback_1plus:
		for (i = 1; i < tracesize; ++i) {
			void *pc_ent;
			pc_ent = traceback[i];
			if (!pc_ent)
				break;
			addr2line_printf(&syslog_printer, SYSLOG_LEVEL_RAW,
			                 instruction_trypred(pc_ent, INSTRLEN_ISA_DEFAULT),
			                 pc_ent, "Called here");
		}
		break;
	}
	printk(KERN_RAW "\n");
}

PRIVATE NOBLOCK ATTR_COLDTEXT size_t
NOTHROW(KCALL gc_print_unreachable_nodes)(struct trace_node *__restrict node) {
	size_t result = 0;
again:
	if (node->tn_reach != gc_version) {
		/* This node wasn't reached. (but ignore if the node has the NOLEAK flag set) */
		if (!(node->tn_flags & TRACE_NODE_FLAG_NOLEAK)) {
			gc_print_unreachable_node(node);
			++result;
		}
	}
	if (node->tn_link.a_min) {
		if (node->tn_link.a_max)
			result += gc_print_unreachable_nodes(node->tn_link.a_max);
		node = node->tn_link.a_min;
		goto again;
	}
	if (node->tn_link.a_max) {
		node = node->tn_link.a_max;
		goto again;
	}
	return result;
}

PRIVATE NOBLOCK ATTR_COLDTEXT size_t
NOTHROW(KCALL gc_print_unreachables)(void) {
	size_t result;
#ifdef CONFIG_USE_SLAB_ALLOCATORS
	result = gc_slab_unreachable();
#else /* CONFIG_USE_SLAB_ALLOCATORS */
	result = 0;
#endif /* !CONFIG_USE_SLAB_ALLOCATORS */
	if (nodes)
		result += gc_print_unreachable_nodes(nodes);
	return result;
}


/* Called after having become a super-override */
PRIVATE size_t KCALL kmalloc_leaks_impl(void) {
	size_t result;

	/* Get a new GC version. */
	++gc_version;
	if unlikely(gc_version == 0)
		gc_version = 1; /* Don't recycle version #0 */

	/* Search for reachable data. */
	gc_find_reachable();

	/* TODO: Do some post-processing to form a dependency-tree between everything that got leaked:
	 *       Often times when a memory leak happens due to some reference-counted kernel object
	 *       not being decref'd when it should, the system log will be flooded by a wall of tracebacks.
	 *       This happens because any object pointed-to by that original object, as well as any heap
	 *       memory owned by it will also appear as a memory leak.
	 *
	 * By doing some post-processing, we could order memory leaks by severity, based on how often
	 * a leak is referenced by some other memory leak, as well as also include information about
	 * how individual heap-blocks are referencing each other. Leaks would then be printed in
	 * ascending order based on how often they are referenced by other leaks, as well as list the
	 * individual pointers and offsets by which different heap-blocks point into each other:
	 *
	 * ============= 3 Memory leaks
	 * Leaked 88 bytes of kmalloc-memory at E1A610F4...E1A61143 [tid=6] (Referenced 0 times)
	 * Traceback here....
	 *
	 * Leaked 68 bytes of heap-memory at E1A493D0...E1A49413 [tid=3] (Referenced 0 times)
	 * Traceback here....
	 *
	 * Leaked 60 bytes of kmalloc-memory at E1A4A044...E1A4A077 [tid=6] (Referenced 2 time)
	 *     Referenced at *E1A610F8=E1A4A044 by leak at E1A610F4...E1A61143
	 *     Referenced at *E1A493D4=E1A4A048[.+4] by leak at E1A493D0...E1A49413
	 * Traceback here....
	 *
	 * Where the `Referenced at'-line is formated as
	 *     Referenced at *ORIGIN_ADDR=TARGET_ADDR by leak at OTHER_LEAK_LO...OTHER_LEAK_HI
	 * ORIGIN_ADDR:   The address within some other leak that references our leak
	 * TARGET_ADDR:   The address within our leak referenced by the other leak, optionally
	 *                suffixed by `[.+N]' where N is the offset from our own base address.
	 * OTHER_LEAK_LO: Low address of the other leak (where that other leak can then also
	 *                be identified by simply copy+pasting this range and searching the
	 *                list of leaks for this same range)
	 * OTHER_LEAK_HI: High address of the other leak
	 *
	 * HINT: We can temporarily gain 1 additional pointer-sized word of writable memory for
	 *       every memory leak by removing all of them from the nodes-tree, chaining them
	 *       back together via one of the node-tree pointers and forming a singly linked
	 *       list, leaving the second pointer available for later use.
	 *       Cross-references between leaks can then be enumerated in O(N*(N-1)), by
	 *       enumerating all of the leaks, then scanning the bodies of all but the current
	 *       leak for additional references to the same leak.
	 * Also: by doing this we could off-load the process of printing the actual leaks onto
	 *       our caller, to-be done after they've released `sched_super_override_end()',
	 *       thus fixing the problem we're currently having where printing a traceback
	 *       could potentially access disk I/O, which in turn might require an async job,
	 *       which in turn requires that no schedule override be active in order to be
	 *       able to run.
	 * Only problem with that is: how would we dump leaks relating to the slab allocator.
	 * One solution would be to also ensure that `kernel_locked_heap' is available before
	 * acquiring a scheduler super-override, in which case we'd be allowed to allocate
	 * additional heap memory, which we could then use to store the addresses of all of
	 * the leaked slab pointers. */

	/* Print everything that could not be reached as leaks. */
	result = gc_print_unreachables();

#ifdef CONFIG_USE_SLAB_ALLOCATORS
	/* Clear the is-reachable bits from all of
	 * the different slabs that could be reached. */
	gc_slab_reset_reach();
#endif /* CONFIG_USE_SLAB_ALLOCATORS */

	return result;
}


/* Search for leaked heap memory, and return the total number of leaked blocks.
 * Note that to do what it does, this function has to temporarily elevate the
 * calling thread to super-override status (s.a. <sched/scheduler.h>) */
PUBLIC ATTR_NOINLINE size_t KCALL
kmalloc_leaks(void) THROWS(E_WOULDBLOCK) {
	size_t result;
again:
	/* Acquire a scheduler super-override, thus ensuring that we're
	 * the only thread running anywhere on the entire system.
	 *
	 * NOTE: We do this first, since a super-override should only
	 *       be acquired when not already holding any atomic locks,
	 *       since when holding such locks, there is a small chance
	 *       that other CPUs are currently trying to acquire them,
	 *       preventing us from reaching them.
	 * Technically, this shouldn't happen, since you shouldn't do a
	 * `while (!trylock()) task_pause();' loop (meaning that a cpu
	 * that is blocking-waiting for an atomic lock should also have
	 * preemption enabled), and where you are allowed to do this kind
	 * of loop, you're actually dealing with an SMP-lock, which also
	 * requires that preemption be disabled, where becoming a super
	 * override will implicitly cause one to acquire all SMP-locks,
	 * since a CPU that hold an SMP-lock must release it before re-
	 * enabling preemption, meaning that being able to send an IPI
	 * to every CPU, and having every cpu ACK that IPI also implies
	 * that all CPUs had preemption enabled, which then implies that
	 * no CPU was holding onto an SMP-lock.
	 *
	 * But despite all of that, it's better to be safe than sorry.
	 *
	 * HINT: `smplock' (see above) is (like the name says) an SMP-lock,
	 *       so we implicitly acquire it by being the super-override, so
	 *       we don't actually have to deal with that one at all! */
	sched_super_override_start();

	if (!vm_kernel_treelock_trywrite()) {
		sched_super_override_end();
		while (!sync_canwrite(&vm_kernel.v_treelock))
			task_yield();
		goto again;
	}

	/* Acquire a lock to the corepage system, thus
	 * ensuring that it's in a consistent state, too. */
	if (!sync_trywrite(&vm_corepage_lock)) {
		vm_kernel_treelock_endwrite();
		sched_super_override_end();
		while (!sync_canwrite(&vm_corepage_lock))
			task_yield();
		goto again;
	}

	/* At this point we're the only running thread, so there's really no
	 * point in still holding on to our lock to the kernel VM. - At this
	 * point we can do pretty much anything while disregarding any sort
	 * of locking! */
	sync_endwrite(&vm_corepage_lock);
	vm_kernel_treelock_endwrite();

	/* Actually search for memory leaks. */
	result = kmalloc_leaks_impl();

	/* Release the scheduler super-override, allowing normal
	 * system execution to resume. */
	sched_super_override_end();

	return result;
}



INTERN void /* Must be INTERN, because this function gets overwritten
             * when `nomall' is passed on the commandline */
NOTHROW(KCALL trace_malloc_generate_traceback)(void **__restrict buffer, size_t buflen,
                                               struct lcpustate *__restrict state,
                                               unsigned int n_skip) {
#if 1
	TRY {
		struct lcpustate oldstate;
		oldstate = *state;
		/* NOTE: Despite this being for debugging purposes, _DONT_ use `unwind_for_debug()' here!
		 *       That one has to make use of the kernel heap (and consequently us) in order
		 *       to be able to construct user-space program/section descriptors, etc, where-as
		 *       the normal unwind() function is specifically designed to _not_ make use of any
		 *       heap memory (which is also the reason why drivers pre-load their .eh_frame
		 *       sections during initialization, rather than loading it lazily)
		 * But the reason this is done isn't only for this, but rather to ensure that throwing
		 * exceptions and the like don't just randomly fail because the kernel couldn't find
		 * enough memory... */
		do {
			if (n_skip == 0) {
				*buffer++ = (void *)lcpustate_getpc(state);
				if (!--buflen)
					return;
			} else {
				--n_skip;
			}
			oldstate = *state;
		} while (unwind((void *)(lcpustate_getpc(&oldstate) - 1),
		                &unwind_getreg_lcpustate, &oldstate,
		                &unwind_setreg_lcpustate, state) == UNWIND_SUCCESS);
	} EXCEPT {
		if (!was_thrown(E_SEGFAULT) && !was_thrown(E_WOULDBLOCK))
			RETHROW(); /* This causes panic because we're NOTHROW */
	}
#endif
	/* Make sure that when there are unused entries, the chain is NULL-terminated */
	if (buflen)
		*buffer = NULL;
}

/* Begin tracing the given `node' */
PRIVATE ATTR_NOINLINE NOBLOCK_IF(gfp & GFP_ATOMIC) void FCALL
insert_trace_node(struct trace_node *__restrict node,
                  gfp_t gfp, unsigned int n_skip)
		THROWS(E_BADALLOC, E_WOULDBLOCK);
PRIVATE ATTR_NOINLINE NOBLOCK_IF(gfp & GFP_ATOMIC) bool
NOTHROW(FCALL insert_trace_node_nx)(struct trace_node *__restrict node,
                                    gfp_t gfp, unsigned int n_skip);

/* Resolve existing (possibly bitset-based) mappings between [umin, umax]
 * These functions must be called while already holding the lock.
 * NOTE: If these functions return an exception/false, the lock will have been released! */
PRIVATE ATTR_NOINLINE NOBLOCK_IF(gfp & GFP_ATOMIC) void FCALL
insert_trace_node_resolve(uintptr_t umin, uintptr_t umax,
                          gfp_t gfp, unsigned int n_skip,
                          LOCK_PARAMS)
		THROWS(E_BADALLOC, E_WOULDBLOCK);
PRIVATE ATTR_NOINLINE NOBLOCK_IF(gfp & GFP_ATOMIC) bool
NOTHROW(FCALL insert_trace_node_resolve_nx)(uintptr_t umin, uintptr_t umax,
                                            gfp_t gfp, unsigned int n_skip,
                                            LOCK_PARAMS);


/************************************************************************/
/* Auto-tracing heap functions                                          */
/************************************************************************/

PUBLIC ATTR_NOINLINE NOBLOCK void
NOTHROW(KCALL heap_free)(struct heap *__restrict self,
                         VIRT void *ptr, size_t num_bytes,
                         gfp_t flags) {
	kmalloc_untrace_n_impl(ptr, num_bytes, 1);
	heap_free_untraced(self, ptr, num_bytes, flags);
}

PUBLIC ATTR_NOINLINE WUNUSED NONNULL((1)) size_t
NOTHROW(KCALL heap_truncate)(struct heap *__restrict self, void *base,
                             size_t old_size, size_t new_size, gfp_t free_flags) {
	size_t free_bytes;
	assertf(!old_size || old_size >= HEAP_MINSIZE,
	        "Invalid heap_truncate(): Too few bytes (%" PRIuSIZ " < %" PRIuSIZ ")",
	        old_size, HEAP_MINSIZE);
	assertf(!old_size || IS_ALIGNED((uintptr_t)base, HEAP_ALIGNMENT),
	        "Invalid heap_truncate(): Unaligned base pointer %p",
	        base);
	assertf(IS_ALIGNED(old_size, HEAP_ALIGNMENT),
	        "Invalid heap_truncate(): Unaligned old_size size %" PRIuSIZ " (%#" PRIxSIZ ")",
	        old_size, old_size);
	if unlikely(OVERFLOW_UADD(new_size, (size_t)(HEAP_ALIGNMENT - 1), &new_size))
		goto return_old_size;
	new_size &= ~(HEAP_ALIGNMENT - 1);
	if unlikely(new_size < HEAP_MINSIZE)
		new_size = HEAP_MINSIZE;
	if unlikely(new_size >= old_size)
		goto return_old_size;
	/* Free trailing memory. */
	free_bytes = old_size - new_size;
	if (free_bytes >= HEAP_MINSIZE) {
		heap_free(self, (void *)((uintptr_t)base + new_size),
		          free_bytes, free_flags);
		return new_size;
	}
return_old_size:
	return old_size;
}

PUBLIC ATTR_NOINLINE NOBLOCK WUNUSED size_t
NOTHROW(KCALL kmalloc_usable_size)(VIRT void *ptr) {
	struct trace_node *node;
	size_t result;
	if (!ptr)
		return 0;
	assert(IS_ALIGNED((uintptr_t)ptr, HEAP_ALIGNMENT));
#ifdef CONFIG_USE_SLAB_ALLOCATORS
	if (KERNEL_SLAB_CHECKPTR(ptr))
		return SLAB_GET(ptr)->s_size;
#endif /* CONFIG_USE_SLAB_ALLOCATORS */
	lock_acquire();
	node = trace_node_tree_locate(nodes, (uintptr_t)ptr);
	if unlikely(!node) {
		lock_break();
		kernel_panic_n(/* n_skip: */ 1,
		               "kmalloc_usable_size(%p): No node at this address",
		               ptr);
		return 0;
	}
	if unlikely(node->tn_kind != TRACE_NODE_KIND_MALL) {
		node = trace_node_dupa_tb(node);
		lock_break();
		kernel_panic_n(/* n_skip: */ 1,
		               "kmalloc_usable_size(%p): Node at %p...%p wasn't created by kmalloc()\n"
		               "%[gen:c]",
		               ptr, trace_node_umin(node), trace_node_umax(node),
		               &trace_node_print_traceback, node);
		return 0;
	}
	if unlikely(ptr != trace_node_uaddr(node) + CONFIG_MALL_HEAD_SIZE) {
		node = trace_node_dupa_tb(node);
		lock_break();
		kernel_panic_n(/* n_skip: */ 1,
		               "kmalloc_usable_size(%p): Passed pointer does not "
		               "match start of containing node %p...%p (%p...%p)\n"
		               "%[gen:c]",
		               ptr,
		               trace_node_umin(node) + CONFIG_MALL_HEAD_SIZE,
		               trace_node_umax(node) - CONFIG_MALL_TAIL_SIZE,
		               trace_node_umin(node), trace_node_umax(node),
		               &trace_node_print_traceback, node);
		return 0;
	}

	/* Verify head/tail integrity of `node' */
#ifdef HAVE_kmalloc_validate_node
	if (!kmalloc_validate_node(1, node)) {
		lock_break();
		return 0;
	}
#endif /* HAVE_kmalloc_validate_node */

	/* Calculate the user-payload-size from the trace-node. */
	result = trace_node_usize(node) - (CONFIG_MALL_HEAD_SIZE +
	                                   CONFIG_MALL_TAIL_SIZE);
	lock_release();
	return result;
}

PUBLIC ATTR_NOINLINE NOBLOCK void
NOTHROW(KCALL kffree)(VIRT void *ptr, gfp_t flags) {
	struct heapptr user_area;
	struct trace_node *node;
	if (!ptr)
		return; /* Ignore NULL-pointers. */
#ifdef CONFIG_USE_SLAB_ALLOCATORS
	if (KERNEL_SLAB_CHECKPTR(ptr)) {
		slab_ffree(ptr, flags);
		return;
	}
#endif /* CONFIG_USE_SLAB_ALLOCATORS */
	lock_acquire();
	node = trace_node_tree_remove(&nodes, (uintptr_t)ptr);
	if unlikely(!node) {
		lock_break();
		kernel_panic_n(/* n_skip: */ 1,
		               "kffree(%p, %#x): No node at this address",
		               ptr, flags);
		return;
	}
	if unlikely(node->tn_kind != TRACE_NODE_KIND_MALL) {
		trace_node_tree_insert(&nodes, node);
		node = trace_node_dupa_tb(node);
		lock_break();
		kernel_panic_n(/* n_skip: */ 1,
		               "kffree(%p, %#x): Node at %p...%p wasn't created by kmalloc()\n"
		               "%[gen:c]",
		               ptr, flags, trace_node_umin(node), trace_node_umax(node),
		               &trace_node_print_traceback, node);
		return;
	}
	if unlikely(ptr != trace_node_uaddr(node) + CONFIG_MALL_HEAD_SIZE) {
		trace_node_tree_insert(&nodes, node);
		node = trace_node_dupa_tb(node);
		lock_break();
		kernel_panic_n(/* n_skip: */ 1,
		               "kffree(%p, %#x): Passed pointer does not match "
		               "start of containing node %p...%p (%p...%p)\n"
		               "%[gen:c]",
		               ptr, flags,
		               trace_node_umin(node) + CONFIG_MALL_HEAD_SIZE,
		               trace_node_umax(node) - CONFIG_MALL_TAIL_SIZE,
		               trace_node_umin(node), trace_node_umax(node),
		               &trace_node_print_traceback, node);
		return;
	}

	/* Validate the padding for a node before freeing it, thus ensuring
	 * that the user hasn't done anything to corrupt the heap block during
	 * the block's life-time. */
#ifdef HAVE_kmalloc_validate_node
	if (!kmalloc_validate_node(1, node)) {
		trace_node_tree_insert(&nodes, node);
		lock_break();
		return;
	}
#endif /* HAVE_kmalloc_validate_node */
	lock_release();

	/* All right! everything checks out, so we can actually move on to
	 * freeing the associated node, as well as the user-data block. */
	user_area.hp_ptr = trace_node_uaddr(node);
	user_area.hp_siz = trace_node_usize(node);

	/* Load heap-flag properties from the node itself. */
	flags = (flags & ~__GFP_HEAPMASK) | (node->tn_flags & __GFP_HEAPMASK);
	assert(user_area.hp_siz >=
	       MAX(HEAP_MINSIZE, CONFIG_MALL_HEAD_SIZE +
	                         CONFIG_MALL_TAIL_SIZE));

	/* When freeing zero-initialized memory,
	 * clean up the head & tail blocks first! */
	if (flags & GFP_CALLOC) {
		bzero(user_area.hp_ptr, CONFIG_MALL_HEAD_SIZE);
		bzero((byte_t *)user_area.hp_ptr +
		      (user_area.hp_siz - CONFIG_MALL_TAIL_SIZE),
		      CONFIG_MALL_TAIL_SIZE);
	}

	/* Finally, free the actual payload */
	heap_free_untraced(&kernel_heaps[flags & __GFP_HEAPMASK],
	                   user_area.hp_ptr,
	                   user_area.hp_siz,
	                   flags);
	trace_node_free(node);
}

PUBLIC ATTR_NOINLINE NOBLOCK void
NOTHROW(KCALL kfree)(VIRT void *ptr) {
	struct heapptr user_area;
	struct trace_node *node;
	gfp_t flags;
	if (!ptr)
		return; /* Ignore NULL-pointers. */
#ifdef CONFIG_USE_SLAB_ALLOCATORS
	if (KERNEL_SLAB_CHECKPTR(ptr)) {
		slab_free(ptr);
		return;
	}
#endif /* CONFIG_USE_SLAB_ALLOCATORS */
	lock_acquire();
	node = trace_node_tree_remove(&nodes, (uintptr_t)ptr);
	if unlikely(!node) {
		lock_break();
		kernel_panic_n(/* n_skip: */ 1,
		               "kfree(%p): No node at this address",
		               ptr);
		return;
	}
	if unlikely(node->tn_kind != TRACE_NODE_KIND_MALL) {
		trace_node_tree_insert(&nodes, node);
		node = trace_node_dupa_tb(node);
		lock_break();
		kernel_panic_n(/* n_skip: */ 1,
		               "kfree(%p): Node at %p...%p wasn't created by kmalloc()\n"
		               "%[gen:c]",
		               ptr, trace_node_umin(node), trace_node_umax(node),
		               &trace_node_print_traceback, node);
		return;
	}
	if unlikely(ptr != trace_node_uaddr(node) + CONFIG_MALL_HEAD_SIZE) {
		trace_node_tree_insert(&nodes, node);
		node = trace_node_dupa_tb(node);
		lock_break();
		kernel_panic_n(/* n_skip: */ 1,
		               "kfree(%p): Passed pointer does not match "
		               "start of containing node %p...%p (%p...%p)\n"
		               "%[gen:c]",
		               ptr,
		               trace_node_umin(node) + CONFIG_MALL_HEAD_SIZE,
		               trace_node_umax(node) - CONFIG_MALL_TAIL_SIZE,
		               trace_node_umin(node), trace_node_umax(node),
		               &trace_node_print_traceback, node);
		return;
	}

	/* Validate the padding for a node before freeing it, thus ensuring
	 * that the user hasn't done anything to corrupt the heap block during
	 * the block's life-time. */
#ifdef HAVE_kmalloc_validate_node
	if unlikely(!kmalloc_validate_node(1, node)) {
		trace_node_tree_insert(&nodes, node);
		lock_break();
		return;
	}
#endif /* HAVE_kmalloc_validate_node */
	lock_release();

	/* All right! everything checks out, so we can actually move on to
	 * freeing the associated node, as well as the user-data block. */
	user_area.hp_ptr = trace_node_uaddr(node);
	user_area.hp_siz = trace_node_usize(node);

	/* Load heap-flag properties from the node itself. */
	flags = node->tn_flags & __GFP_HEAPMASK;
	assert(user_area.hp_siz >=
	       MAX(HEAP_MINSIZE, CONFIG_MALL_HEAD_SIZE +
	                         CONFIG_MALL_TAIL_SIZE));

	/* Finally, free the actual payload */
	heap_free_untraced(&kernel_heaps[flags & __GFP_HEAPMASK],
	                   user_area.hp_ptr,
	                   user_area.hp_siz,
	                   flags);
	trace_node_free(node);
}



DECL_END

#ifndef __INTELLISENSE__
#define DEFINE_X_except
#include "trace-malloc-heap.c.inl"
#define DEFINE_X_noexcept
#include "trace-malloc-heap.c.inl"
#endif /* !__INTELLISENSE__ */

#endif /* CONFIG_USE_NEW_DEBUG_MALLOC */
#endif /* CONFIG_DEBUG_MALLOC */

#endif /* !GUARD_KERNEL_SRC_MEMORY_TRACE_MALLOC_C */
