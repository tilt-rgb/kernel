/* SPDX-License-Identifier: GPL-2.0 */
#ifndef PAGE_FLAGS_LAYOUT_H
#define PAGE_FLAGS_LAYOUT_H

#include <linux/numa.h>
#include <generated/bounds.h>

/*
 * When a memory allocation must conform to specific limitations (such
 * as being suitable for DMA) the caller will pass in hints to the
 * allocator in the gfp_mask, in the zone modifier bits.  These bits
 * are used to select a priority ordered list of memory zones which
 * match the requested limits. See gfp_zone() in include/linux/gfp.h
 */
#if MAX_NR_ZONES < 2
#define ZONES_SHIFT 0
#elif MAX_NR_ZONES <= 2
#define ZONES_SHIFT 1
#elif MAX_NR_ZONES <= 4
#define ZONES_SHIFT 2
#elif MAX_NR_ZONES <= 8
#define ZONES_SHIFT 3
#else
#error ZONES_SHIFT "Too many zones configured"
#endif

#define ZONES_WIDTH		ZONES_SHIFT

#ifdef CONFIG_SPARSEMEM
#include <asm/sparsemem.h>
#define SECTIONS_SHIFT	(MAX_PHYSMEM_BITS - SECTION_SIZE_BITS)
#else
#define SECTIONS_SHIFT	0
#endif

/*
 * page->flags layout:
 *
 * There are five possibilities for how page->flags get laid out.  The first
 * pair is for the normal case without sparsemem. The second pair is for
 * sparsemem when there is plenty of space for node and section information.
 * The last is when there is insufficient space in page->flags and a separate
 * lookup is necessary.
 *
 * No sparsemem or sparsemem vmemmap: |       NODE     | ZONE |             ... | FLAGS |
 *      " plus space for last_cpupid: |       NODE     | ZONE | LAST_CPUPID ... | FLAGS |
 * classic sparse with space for node:| SECTION | NODE | ZONE |             ... | FLAGS |
 *      " plus space for last_cpupid: | SECTION | NODE | ZONE | LAST_CPUPID ... | FLAGS |
 * classic sparse no space for node:  | SECTION |     ZONE    | ... | FLAGS |
 */
#if defined(CONFIG_SPARSEMEM) && !defined(CONFIG_SPARSEMEM_VMEMMAP)
#define SECTIONS_WIDTH		SECTIONS_SHIFT
#else
#define SECTIONS_WIDTH		0
#endif

#if ZONES_WIDTH + LRU_GEN_WIDTH + SECTIONS_WIDTH + NODES_SHIFT \
	<= BITS_PER_LONG - NR_PAGEFLAGS
#define NODES_WIDTH		NODES_SHIFT
#elif defined(CONFIG_SPARSEMEM_VMEMMAP)
#error "Vmemmap: No space for nodes field in page flags"
#else
#define NODES_WIDTH		0
#endif

/*
 * Note that this #define MUST have a value so that it can be tested with
 * the IS_ENABLED() macro.
 */
#if NODES_SHIFT != 0 && NODES_WIDTH == 0
#define NODE_NOT_IN_PAGE_FLAGS	1
#endif

#ifdef CONFIG_NUMA_BALANCING
#define LAST__PID_SHIFT 8
#define LAST__PID_MASK  ((1 << LAST__PID_SHIFT)-1)

#define LAST__CPU_SHIFT NR_CPUS_BITS
#define LAST__CPU_MASK  ((1 << LAST__CPU_SHIFT)-1)

#define LAST_CPUPID_SHIFT (LAST__PID_SHIFT+LAST__CPU_SHIFT)
#else
#define LAST_CPUPID_SHIFT 0
#endif

#if SECTIONS_WIDTH+ZONES_WIDTH+NODES_SHIFT+LAST_CPUPID_SHIFT <= BITS_PER_LONG - NR_PAGEFLAGS
#ifdef CONFIG_KASAN_SW_TAGS
#define KASAN_TAG_WIDTH 8
#else
#define KASAN_TAG_WIDTH 0
#endif

#if SECTIONS_WIDTH+ZONES_WIDTH+NODES_WIDTH+LAST_CPUPID_SHIFT+KASAN_TAG_WIDTH \
#if ZONES_WIDTH + SECTIONS_WIDTH + NODES_WIDTH + KASAN_TAG_WIDTH + LAST_CPUPID_SHIFT \
	<= BITS_PER_LONG - NR_PAGEFLAGS
#if ZONES_WIDTH + LRU_GEN_WIDTH + LRU_REFS_WIDTH + SECTIONS_WIDTH + NODES_WIDTH + \
#if ZONES_WIDTH + LRU_GEN_WIDTH + SECTIONS_WIDTH + NODES_WIDTH + \
<<<<<<< HEAD
	KASAN_TAG_WIDTH + LAST_CPUPID_SHIFT <= BITS_PER_LONG - NR_PAGEFLAGS

=======
	LAST_CPUPID_SHIFT <= BITS_PER_LONG - NR_PAGEFLAGS
>>>>>>> c9f6e71c0179 (mm: mglru fix on qcom baseline)
#define LAST_CPUPID_WIDTH LAST_CPUPID_SHIFT
#else
#define LAST_CPUPID_WIDTH 0
#endif

#ifdef CONFIG_KASAN_SW_TAGS
#define KASAN_TAG_WIDTH 8
#if SECTIONS_WIDTH+NODES_WIDTH+ZONES_WIDTH+LAST_CPUPID_WIDTH+KASAN_TAG_WIDTH \
=======
#if LAST_CPUPID_SHIFT != 0 && LAST_CPUPID_WIDTH == 0
#define LAST_CPUPID_NOT_IN_PAGE_FLAGS
#endif

<<<<<<< HEAD
<<<<<<< HEAD
<<<<<<< HEAD
#if ZONES_WIDTH + SECTIONS_WIDTH + NODES_WIDTH + KASAN_TAG_WIDTH + LAST_CPUPID_WIDTH \
>>>>>>> edfc27d8d3bb (include/linux/page-flags-layout.h: cleanups)
	> BITS_PER_LONG - NR_PAGEFLAGS
#error "KASAN: not enough bits in page flags for tag"
#endif
#else
#define KASAN_TAG_WIDTH 0
=======
#if ZONES_WIDTH + LRU_GEN_WIDTH + LRU_REFS_WIDTH + SECTIONS_WIDTH + NODES_WIDTH + \
=======
=======
#ifdef CONFIG_KASAN_SW_TAGS
#define KASAN_TAG_WIDTH 8
#if ZONES_WIDTH + SECTIONS_WIDTH + NODES_WIDTH + \
	KASAN_TAG_WIDTH + LAST_CPUPID_WIDTH > BITS_PER_LONG - NR_PAGEFLAGS
#error "Not enough bits in page flags"
#endif

#else
#define KASAN_TAG_WIDTH 0
#endif

>>>>>>> c9f6e71c0179 (mm: mglru fix on qcom baseline)
#if ZONES_WIDTH + LRU_GEN_WIDTH + SECTIONS_WIDTH + NODES_WIDTH + \
>>>>>>> a21e91db5a18 (mm: multi-gen LRU: merge v11 patchset)
	KASAN_TAG_WIDTH + LAST_CPUPID_WIDTH > BITS_PER_LONG - NR_PAGEFLAGS
#error "Not enough bits in page flags"
>>>>>>> 537705ac0fee (FROMLIST: mm: multi-gen LRU: groundwork)
#endif

<<<<<<< HEAD
<<<<<<< HEAD
/*
 * We are going to use the flags for the page to node mapping if its in
 * there.  This includes the case where there is no node, so it is implicit.
 */
#if !(NODES_WIDTH > 0 || NODES_SHIFT == 0)
#define NODE_NOT_IN_PAGE_FLAGS
=======
/* see the comment on MAX_NR_TIERS */
#define LRU_REFS_WIDTH	min(__LRU_REFS_WIDTH, BITS_PER_LONG - NR_PAGEFLAGS - \
			    ZONES_WIDTH - LRU_GEN_WIDTH - SECTIONS_WIDTH - \
			    NODES_WIDTH - KASAN_TAG_WIDTH - LAST_CPUPID_WIDTH)

>>>>>>> a21e91db5a18 (mm: multi-gen LRU: merge v11 patchset)
#endif

#if defined(CONFIG_NUMA_BALANCING) && LAST_CPUPID_WIDTH == 0
#define LAST_CPUPID_NOT_IN_PAGE_FLAGS
#endif

=======
#endif
>>>>>>> edfc27d8d3bb (include/linux/page-flags-layout.h: cleanups)
#endif /* _LINUX_PAGE_FLAGS_LAYOUT */
