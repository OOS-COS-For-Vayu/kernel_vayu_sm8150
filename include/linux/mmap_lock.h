/* SPDX-License-Identifier: GPL-2.0 */
#ifndef _LINUX_MMAP_LOCK_H
#define _LINUX_MMAP_LOCK_H

/*
 * Subset of the 5.8 mmap locking API as wrappers around 4.14's mm->mmap_sem,
 * for code backported from newer kernels. Include it explicitly; it is not
 * pulled in by <linux/mm.h>.
 */

#include <linux/mm_types.h>
#include <linux/rwsem.h>

static inline void mmap_read_lock(struct mm_struct *mm)
{
	down_read(&mm->mmap_sem);
}

static inline int mmap_read_lock_killable(struct mm_struct *mm)
{
	return down_read_killable(&mm->mmap_sem);
}

static inline bool mmap_read_trylock(struct mm_struct *mm)
{
	return down_read_trylock(&mm->mmap_sem) != 0;
}

static inline void mmap_read_unlock(struct mm_struct *mm)
{
	up_read(&mm->mmap_sem);
}

static inline int mmap_lock_is_contended(struct mm_struct *mm)
{
	return rwsem_is_contended(&mm->mmap_sem);
}

#endif /* _LINUX_MMAP_LOCK_H */
