/* SPDX-License-Identifier: GPL-2.0 */
#ifndef _LINUX_OPLUS_SCHED_H
#define _LINUX_OPLUS_SCHED_H

#include <linux/sched.h>
#include <linux/types.h>
#include <linux/list.h>
#include <linux/spinlock.h>

struct binder_thread;

struct oplus_task_struct {
	bool binder_async_ux_sts;
	int binder_async_ux_enable;
	int ux_state;
	int ux_depth;
	atomic64_t inherit_ux;
	unsigned long im_flag;
	int preferred_cluster_id;
	struct binder_thread *binder_thread_node;
	struct task_struct *task;
	u64 total_exec;
	u64 block_start_time;
	u64 amu_instruct;
	u64 amu_cycle;
	void *uid_struct;
	u64 enqueue_time;

	/* frame boost / fbg */
	int fbg_state;
	int fbg_depth;
	int fbg_cur_group;
	bool fbg_running;
	u64 last_wake_ts;
	struct list_head fbg_list;
	raw_spinlock_t fbg_list_entry_lock;
};

static inline struct oplus_task_struct *get_oplus_task_struct(struct task_struct *t)
{
	return NULL;
}

#endif /* _LINUX_OPLUS_SCHED_H */
