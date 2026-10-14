/* SPDX-License-Identifier: GPL-2.0 */
#undef TRACE_SYSTEM
#define TRACE_SYSTEM midas_dev
#undef TRACE_INCLUDE_PATH
#define TRACE_INCLUDE_PATH trace/hooks
#if !defined(_TRACE_HOOK_MIDAS_DEV_H) || defined(TRACE_HEADER_MULTI_READ)
#define _TRACE_HOOK_MIDAS_DEV_H
#include <linux/tracepoint.h>
#include <trace/hooks/vendor_hooks.h>

struct task_struct;

DECLARE_HOOK(android_vh_midas_record_task_times,
	TP_PROTO(u64 cputime, struct task_struct *p, unsigned int state),
	TP_ARGS(cputime, p, state));

#endif /* _TRACE_HOOK_MIDAS_DEV_H */
/* This part must be outside protection */
#include <trace/define_trace.h>
