/* SPDX-License-Identifier: GPL-2.0 */
#undef TRACE_SYSTEM
#define TRACE_SYSTEM sched
#undef TRACE_INCLUDE_PATH
#define TRACE_INCLUDE_PATH trace/hooks
#if !defined(_TRACE_HOOK_SCHED_H) || defined(TRACE_HEADER_MULTI_READ)
#define _TRACE_HOOK_SCHED_H
#include <linux/tracepoint.h>
#include <trace/hooks/vendor_hooks.h>

struct task_struct;
struct rq;
struct cpumask;
struct sched_entity;
struct cfs_rq;
struct cpufreq_policy;

DECLARE_HOOK(android_vh_sched_stat_runtime_rt,
	TP_PROTO(struct task_struct *tsk, u64 delta),
	TP_ARGS(tsk, delta));

DECLARE_HOOK(android_vh_dup_task_struct,
	TP_PROTO(struct task_struct *tsk, struct task_struct *orig),
	TP_ARGS(tsk, orig));

DECLARE_HOOK(android_vh_free_task,
	TP_PROTO(struct task_struct *p),
	TP_ARGS(p));

DECLARE_HOOK(android_vh_account_task_time,
	TP_PROTO(struct task_struct *p, struct rq *rq, int user_tick),
	TP_ARGS(p, rq, user_tick));

DECLARE_HOOK(android_vh_irqtime_account_process_tick,
	TP_PROTO(struct task_struct *p, struct rq *rq, int user_tick, int ticks),
	TP_ARGS(p, rq, user_tick, ticks));

DECLARE_HOOK(android_vh_sched_setaffinity_early,
	TP_PROTO(struct task_struct *p, const struct cpumask *new_mask, int *retval),
	TP_ARGS(p, new_mask, retval));

DECLARE_HOOK(android_vh_prepare_update_load_avg_se,
	TP_PROTO(struct sched_entity *se, int flags),
	TP_ARGS(se, flags));

DECLARE_HOOK(android_vh_finish_update_load_avg_se,
	TP_PROTO(struct sched_entity *se, int flags),
	TP_ARGS(se, flags));

DECLARE_HOOK(android_vh_map_util_freq,
	TP_PROTO(unsigned long util, unsigned long freq,
		unsigned long cap, unsigned long *next_freq, struct cpufreq_policy *policy,
		bool *need_freq_update),
	TP_ARGS(util, freq, cap, next_freq, policy, need_freq_update));

DECLARE_HOOK(android_vh_build_sched_domains,
	TP_PROTO(bool has_asym),
	TP_ARGS(has_asym));

DECLARE_RESTRICTED_HOOK(android_rvh_flush_task,
	TP_PROTO(struct task_struct *p),
	TP_ARGS(p), 1);

DECLARE_RESTRICTED_HOOK(android_rvh_sched_fork,
	TP_PROTO(struct task_struct *p),
	TP_ARGS(p), 1);

DECLARE_RESTRICTED_HOOK(android_rvh_try_to_wake_up,
	TP_PROTO(struct task_struct *p),
	TP_ARGS(p), 1);

DECLARE_RESTRICTED_HOOK(android_rvh_new_task_stats,
	TP_PROTO(struct task_struct *p),
	TP_ARGS(p), 1);

#endif /* _TRACE_HOOK_SCHED_H */
/* This part must be outside protection */
#include <trace/define_trace.h>
