/* SPDX-License-Identifier: GPL-2.0-only */
/*
 * Copyright (C) 2022 Oplus. All rights reserved.
 */

#ifndef __OPLUS_CPU_JANK_BASE_H__
#define __OPLUS_CPU_JANK_BASE_H__


#if IS_ENABLED(CONFIG_SCHED_WALT)
/* 4.19: walt.h needs sched.h for struct rq, walt_sched_stats etc. */
#include <../../../kernel/sched/sched.h>
#include <../../../kernel/sched/walt.h>
#else
#include <kernel/sched/sched.h>
#include <fs/proc/internal.h>
#endif

#if IS_ENABLED(CONFIG_JANK_DEBUG)
#define JANK_DEBUG
#define JANK_SYSTRACE_DEBUG
#endif

#ifdef JANK_DEBUG
#define jank_dbg(fmt, args...)  pr_info("[BYHP:%s:%d] " fmt, __func__, __LINE__, ##args)
#else
#define jank_dbg(fmt, args...)
#endif

/* 4.19 compat: sched_assist not ported — stub sa_common.h */
#if __has_include("../sched_assist/sa_common.h")
#include "../sched_assist/sa_common.h"
#endif

#include <trace/hooks/sched.h>
/* trace/hooks/dtask.h, rwsem.h, topology.h, futex.h, cpufreq.h not in 4.19 — guard */
#if __has_include(<trace/hooks/dtask.h>)
#include <trace/hooks/dtask.h>
#endif
#if __has_include(<trace/hooks/rwsem.h>)
#include <trace/hooks/rwsem.h>
#endif
#if __has_include(<trace/hooks/topology.h>)
#include <trace/hooks/topology.h>
#endif
#if __has_include(<trace/hooks/binder.h>)
#include <trace/hooks/binder.h>
#endif
#if __has_include(<trace/hooks/futex.h>)
#include <trace/hooks/futex.h>
#endif
#if __has_include(<trace/hooks/cpufreq.h>)
#include <trace/hooks/cpufreq.h>
#endif
#if __has_include(<trace/events/task.h>)
#include <trace/events/task.h>
#endif
#include <trace/events/sched.h>
#include <linux/version.h>

#ifndef PROC_NUMBUF
#define PROC_NUMBUF 13
#endif

#if (LINUX_VERSION_CODE < KERNEL_VERSION(5, 6, 0))
#define proc_ops file_operations
#define proc_open open
#define proc_read read
#define proc_write write
#define proc_lseek llseek
#define proc_release release
#define proc_poll poll
#define proc_ioctl unlocked_ioctl
#define proc_mmap mmap
#define proc_get_unmapped_area get_unmapped_area
#endif

#ifndef IM_FLAG_NONE
enum IM_FLAG_TYPE {
	IM_FLAG_NONE = 0,
	IM_FLAG_SURFACEFLINGER,
	IM_FLAG_HWC,
	IM_FLAG_RENDERENGINE,
	IM_FLAG_WEBVIEW,
	IM_FLAG_CAMERA_HAL,
	IM_FLAG_AUDIO,
	IM_FLAG_HWBINDER,
	IM_FLAG_LAUNCHER,
	IM_FLAG_LAUNCHER_NON_UX_RENDER,
	IM_FLAG_SS_LOCK_OWNER,
	IM_FLAG_FORBID_SET_CPU_AFFINITY = 11,
	IM_FLAG_SYSTEMSERVER_PID,
	IM_FLAG_MIDASD,
	IM_FLAG_AUDIO_CAMERA_HAL,
	IM_FLAG_AFFINITY_THREAD,
	IM_FLAG_TPD_SET_CPU_AFFINITY = 16,
	IM_FLAG_COMPRESS_THREAD = 17,
	IM_FLAG_RENDER_THREAD = 18,
	IM_FLAG_CAMERAHAL_THREAD = 20,
	MAX_IM_FLAG_TYPE,
};
#endif

#define nr_perf_states nr_cap_states

static inline unsigned long oplus_get_im_flag(struct task_struct *t)
{
	struct oplus_task_struct *ots = get_oplus_task_struct(t);

	if (IS_ERR_OR_NULL(ots))
		return 0;
	return ots->im_flag;
}

#define REGISTER_TRACE_VH(vender_hook, handler) { \
		ret = register_trace_##vender_hook(handler, NULL); \
		if (ret) { \
			pr_err("failed to register_trace_"#vender_hook", ret=%d\n", ret); \
			return ret; \
		} \
	}

#define UNREGISTER_TRACE_VH(vender_hook, handler) { \
		unregister_trace_##vender_hook(handler, NULL); \
	}

#define REGISTER_TRACE_RVH		REGISTER_TRACE_VH


#ifdef VENDOR_DEBUG
#define UNREGISTER_TRACE_RVH	UNREGISTER_TRACE_VH
#else
#define UNREGISTER_TRACE_RVH(vender_hook, handler)
#endif


#define CPU_NUMS						8
#define INVALID_CPUID					(-1)


/* 1024*1024 ns in a ms */
#define JANK_NS_PER_MS_SHIFT			20
#define JANK_NS_PER_MS_MASK				((1 << JANK_NS_PER_MS_SHIFT)-1)


/* window size : 128ms */
#define JANK_WIN_SIZE_SHIFT				7
#define JANK_WIN_SIZE					(1 << JANK_WIN_SIZE_SHIFT)
#define JANK_WIN_SIZE_MASK				((1 << JANK_WIN_SIZE_SHIFT)-1)

#define JANK_WIN_SIZE_SHIFT_IN_NS \
				(JANK_WIN_SIZE_SHIFT + JANK_NS_PER_MS_SHIFT)
#define JANK_WIN_SIZE_IN_NS				(1 << JANK_WIN_SIZE_SHIFT_IN_NS)
#define JANK_WIN_SIZE_IN_NS_MASK \
				((1 << JANK_WIN_SIZE_SHIFT_IN_NS)-1)


/* Half of the window */
#define JANK_WIN_TICKS					(JANK_WIN_SIZE_IN_NS/TICK_NSEC)
#define JANK_RECORD_THRESHOLD			(JANK_WIN_TICKS >> 1)

/* thresholdis 1/2 */
#define JANK_WIN_SIZE_THRESHOLD_SHIFT	(1)
#define JANK_WIN_SIZE_THRESHOLD \
			(JANK_WIN_SIZE_IN_NS >> JANK_WIN_SIZE_THRESHOLD_SHIFT)


/* window count : 64 */
#define JANK_WIN_CNT_SHIFT				6
#define JANK_WIN_CNT					(1 << JANK_WIN_CNT_SHIFT)
#define JANK_WIN_CNT_MASK				((1 << JANK_WIN_CNT_SHIFT)-1)


#define time2idx(timestamp) \
			(((timestamp) >> JANK_WIN_SIZE_SHIFT_IN_NS) + \
			!!((timestamp) & JANK_WIN_SIZE_IN_NS_MASK))
#define time2winidx(timestamp) \
			((time2idx(timestamp)) & JANK_WIN_CNT_MASK)
#define time2wincnt(now, last) \
			(min_t(u32, (time2idx(now) - time2idx(last)), JANK_WIN_CNT))

#define winidx_add(idx, n) \
			((idx + n) & JANK_WIN_CNT_MASK)
#define winidx_sub(idx, n) \
			(((idx) + JANK_WIN_CNT - (n)) & JANK_WIN_CNT_MASK)



#define get_record_winidx(timestamp) \
		(((timestamp) >> JANK_WIN_SIZE_SHIFT_IN_NS) & RECOED_WINIDX_MASK)



#ifdef JANK_SYSTRACE_DEBUG
#define JANK_SYSTRACE_PID				12345
#define JANK_SYSTRACE_TASKTRACK			99999
#define JANK_SYSTRACE_CPUFREQ			88888
void jank_systrace_print(u32 pid, char *tag, u64 val);
void jank_systrace_print_idx(u32 pid,
			char *tag, u32 idx, u64 val);
#else
#define jank_systrace_print(pid, tag, val)
#define jank_systrace_print_idx(pid, tag, idx, val)
#endif

#define osi_debug(fmt, args...)					\
do {								            \
	if (g_osi_debug >= 1) {	                    \
		 printk(KERN_INFO "[OSI_DEBUG]"fmt, ##args);			\
	}							                \
} while (0)

#define osi_err(fmt, args...)					\
do {								            \
	if (g_osi_debug >= 1) {	                    \
		 printk(KERN_ERR "[OSI_ERR]"fmt, ##args);			\
	}							                \
} while (0)

#define osi_debug_deferred(fmt, args...)					\
do {								            \
	if (g_osi_debug >= 1) {	                    \
		 printk_deferred(KERN_INFO "[OSI_DEBUG]"fmt, ##args);			\
	}							                \
} while (0)

#define osi_err_deferred(fmt, args...)					\
do {								            \
	if (g_osi_debug >= 1) {	                    \
		 printk_deferred(KERN_ERR "[OSI_ERR]"fmt, ##args);			\
	}							                \
} while (0)

enum {
	CGROUP_RESV = 0,
	CGROUP_DEFAULT = 1,			/* sys */
	CGROUP_FOREGROUND,
	CGROUP_BACKGROUND,
	CGROUP_TOP_APP,

	CGROUP_NRS,
};
extern int g_osi_debug;
void osi_base_proc_init(struct proc_dir_entry *pde);
void osi_base_proc_deinit(struct proc_dir_entry *pde);
int task_cgroup_id(struct task_struct *p);
u32 split_window(u64 now, u64 delta,
			u64 *win_a, u64 *win_cnt, u64 *win_b);
bool is_same_idx(u64 timestamp, u64 now);
bool is_same_window(u64 timestamp, u64 now);
bool timestamp_is_valid(u64 timestamp, u64 now);

#endif  /* endif */
