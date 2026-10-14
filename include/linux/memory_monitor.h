/* SPDX-License-Identifier: GPL-2.0 */
#ifndef _LINUX_MEMORY_MONITOR_H
#define _LINUX_MEMORY_MONITOR_H

#include <linux/sched.h>

struct process_mem {
	int pid;
	int uid;
	unsigned long rss;
	int oom_score_adj;
	unsigned long gl_dev;
	char comm[TASK_COMM_LEN];
};

static inline void mm_get_all_pmem(int *cnt, struct process_mem *pmem)
{
	*cnt = 0;
}

#endif /* _LINUX_MEMORY_MONITOR_H */
