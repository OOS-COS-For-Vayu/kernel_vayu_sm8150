/* SPDX-License-Identifier: GPL-2.0 */
#ifndef _SA_COMMON_H_
#define _SA_COMMON_H_

#include <linux/sched.h>
#if __has_include(<linux/oplus_sched.h>)
#include <linux/oplus_sched.h>
#endif

struct task_struct;
struct rq;

#define ots_to_ts(ots)	((ots)->task)

#define IM_FLAG_NONE			0
#define IM_FLAG_SURFACEFLINGER		1
#define IM_FLAG_RENDERENGINE		2
#define IM_FLAG_RENDER_THREAD		3

typedef bool (*migrate_task_callback_t)(struct task_struct *p, int src_cpu, int dst_cpu);
typedef void (*android_rvh_schedule_handler_t)(struct task_struct *prev, struct task_struct *next, struct rq *rq);

extern migrate_task_callback_t fbg_migrate_task_callback;
extern android_rvh_schedule_handler_t fbg_android_rvh_schedule_callback;
#include <linux/arch_topology.h>
#include <asm/topology.h>
#undef arch_scale_cpu_capacity
#define arch_scale_cpu_capacity(cpu) topology_get_cpu_scale(NULL, (cpu))

struct oplus_rq;
#define SCHED_ASSIST_UX_MASK (0)
static inline struct oplus_rq *get_oplus_rq(struct rq *rq) { return NULL; }
static inline int oplus_get_ux_state(struct task_struct *t) { return 0; }
static inline bool orq_has_ux_tasks(struct oplus_rq *orq) { return false; }

#endif /* _SA_COMMON_H_ */
