/* SPDX-License-Identifier: GPL-2.0-only */
#ifndef _SIMPLE_LMK_COMPAT_H
#define _SIMPLE_LMK_COMPAT_H

#include <linux/version.h>
#include <linux/oom.h>
#include <linux/printk.h>

#if LINUX_VERSION_CODE < KERNEL_VERSION(4, 19, 0)

#ifndef PIDTYPE_TGID
#define PIDTYPE_TGID PIDTYPE_PID
#endif


static inline bool __oom_reap_task_mm_4_14(struct mm_struct *mm)
{
    __oom_reap_task_mm(mm);
    return test_bit(MMF_OOM_SKIP, &mm->flags);
}
#define __oom_reap_task_mm(mm) __oom_reap_task_mm_4_14(mm)

#endif /* < 4.19 */

#endif /* _SIMPLE_LMK_COMPAT_H */