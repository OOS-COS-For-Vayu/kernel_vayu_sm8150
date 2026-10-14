/* SPDX-License-Identifier: GPL-2.0 */
#undef TRACE_SYSTEM
#define TRACE_SYSTEM binder
#undef TRACE_INCLUDE_PATH
#define TRACE_INCLUDE_PATH trace/hooks
#if !defined(_TRACE_HOOK_BINDER_H) || defined(TRACE_HEADER_MULTI_READ)
#define _TRACE_HOOK_BINDER_H
#include <linux/tracepoint.h>
#include <trace/hooks/vendor_hooks.h>
#include <linux/sched.h>
#include <linux/seq_file.h>
#include <linux/rbtree.h>
#include <linux/mutex.h>
#include <uapi/linux/android/binder.h>
#include <linux/oplus_sched.h>

#if __has_include(<../drivers/android/binder_alloc.h>)
#include <../drivers/android/binder_alloc.h>
#elif __has_include(<drivers/android/binder_alloc.h>)
#include <drivers/android/binder_alloc.h>
#elif __has_include(<binder_alloc.h>)
#include <binder_alloc.h>
#endif

#if __has_include(<../drivers/android/binder_internal.h>)
#include <../drivers/android/binder_internal.h>
#elif __has_include(<drivers/android/binder_internal.h>)
#include <drivers/android/binder_internal.h>
#elif __has_include(<binder_internal.h>)
#include <binder_internal.h>
#endif

#ifndef _TRACE_HOOK_BINDER_STRUCTS_H
#define _TRACE_HOOK_BINDER_STRUCTS_H

struct binder_priority {
	unsigned int sched_policy;
	int prio;
};

enum binder_work_type {
	BINDER_WORK_TRANSACTION = 1,
	BINDER_WORK_TRANSACTION_COMPLETE,
	BINDER_WORK_RETURN_ERROR,
	BINDER_WORK_NODE,
	BINDER_WORK_DEAD_BINDER,
	BINDER_WORK_DEAD_BINDER_AND_CLEAR,
	BINDER_WORK_CLEAR_DEATH_NOTIFICATION,
};

struct binder_work {
	struct list_head entry;
	enum binder_work_type type;
};

struct binder_node {
	int debug_id;
	bool has_async_transaction;
};

struct binder_proc {
	struct hlist_node proc_node;
	struct rb_root threads;
	struct rb_root nodes;
	struct rb_root refs_by_desc;
	struct rb_root refs_by_node;
	struct list_head waiting_threads;
	int pid;
	struct task_struct *tsk;
	struct files_struct *files;
	struct mutex files_lock;
	struct hlist_node deferred_work_node;
	int deferred_work;
	int outstanding_txns;
	bool is_dead;
	bool is_frozen;
	bool sync_recv;
	bool async_recv;
	wait_queue_head_t freeze_wait;

	struct list_head todo;
	struct list_head delivered_death;
	int max_threads;
	int requested_threads;
	int requested_threads_started;
	atomic_t tmp_ref;
	struct binder_priority default_priority;
	struct dentry *debugfs_entry;
	struct binder_alloc alloc;
	struct binder_context *context;
	spinlock_t inner_lock;
	spinlock_t outer_lock;
	struct dentry *binderfs_entry;
};

struct binder_thread {
	struct binder_proc *proc;
	struct rb_node rb_node;
	struct list_head waiting_thread_node;
	int pid;
	int looper;
	bool looper_need_return;
	struct binder_transaction *transaction_stack;
	struct list_head todo;
	bool process_todo;
	wait_queue_head_t wait;
	atomic_t tmp_ref;
	bool is_dead;
	struct task_struct *task;
	int debug_id;
	bool has_async_transaction;
};

struct binder_transaction {
	int debug_id;
	struct binder_work work;
	struct binder_thread *from;
	struct binder_transaction *from_parent;
	struct binder_proc *to_proc;
	struct binder_thread *to_thread;
	struct binder_transaction *to_parent;
	unsigned need_reply:1;
	struct binder_buffer *buffer;
	unsigned int code;
	unsigned int flags;
	struct binder_priority saved_priority;
	bool set_priority_called;
	kuid_t sender_euid;
	spinlock_t lock;
	u64 android_vendor_data1;
};

#endif /* _TRACE_HOOK_BINDER_STRUCTS_H */

DECLARE_HOOK(android_vh_binder_transaction_init,
	TP_PROTO(struct binder_transaction *t),
	TP_ARGS(t));
DECLARE_HOOK(android_vh_binder_priority_skip,
	TP_PROTO(struct task_struct *task, bool *skip),
	TP_ARGS(task, skip));
DECLARE_HOOK(android_vh_binder_set_priority,
	TP_PROTO(struct binder_transaction *t, struct task_struct *task),
	TP_ARGS(t, task));
DECLARE_HOOK(android_vh_binder_restore_priority,
	TP_PROTO(struct binder_transaction *t, struct task_struct *task),
	TP_ARGS(t, task));
DECLARE_HOOK(android_vh_binder_wakeup_ilocked,
	TP_PROTO(struct task_struct *task, bool sync, struct binder_proc *proc),
	TP_ARGS(task, sync, proc));
DECLARE_HOOK(android_vh_binder_wait_for_work,
	TP_PROTO(bool do_proc_work, struct binder_thread *tsk, struct binder_proc *proc),
	TP_ARGS(do_proc_work, tsk, proc));
DECLARE_HOOK(android_vh_sync_txn_recvd,
	TP_PROTO(struct task_struct *tsk, struct task_struct *from),
	TP_ARGS(tsk, from));
DECLARE_HOOK(android_vh_binder_alloc_new_buf_locked,
	TP_PROTO(size_t size, struct binder_alloc *alloc, int is_async),
	TP_ARGS(size, alloc, is_async));
DECLARE_HOOK(android_vh_binder_reply,
	TP_PROTO(struct binder_proc *target_proc, struct binder_proc *proc,
		struct binder_thread *thread, struct binder_transaction_data *tr),
	TP_ARGS(target_proc, proc, thread, tr));
DECLARE_HOOK(android_vh_binder_trans,
	TP_PROTO(struct binder_proc *target_proc, struct binder_proc *proc,
		struct binder_thread *thread, struct binder_transaction_data *tr),
	TP_ARGS(target_proc, proc, thread, tr));
DECLARE_HOOK(android_vh_binder_preset,
	TP_PROTO(struct hlist_head *hhead, struct mutex *lock),
	TP_ARGS(hhead, lock));
DECLARE_HOOK(android_vh_binder_proc_transaction,
	TP_PROTO(struct task_struct *caller_task, struct task_struct *binder_proc_task,
		struct task_struct *binder_th_task, int node_debug_id,
		unsigned int code, bool pending_async),
	TP_ARGS(caller_task, binder_proc_task, binder_th_task, node_debug_id, code, pending_async));
DECLARE_HOOK(android_vh_binder_proc_transaction_end,
	TP_PROTO(struct task_struct *caller_task, struct task_struct *binder_proc_task,
		struct task_struct *binder_th_task, unsigned int code,
		bool pending_async, bool sync),
	TP_ARGS(caller_task, binder_proc_task, binder_th_task, code, pending_async, sync));
DECLARE_HOOK(android_vh_binder_new_ref,
	TP_PROTO(struct task_struct *proc, uint32_t ref_desc, int node_debug_id),
	TP_ARGS(proc, ref_desc, node_debug_id));
DECLARE_HOOK(android_vh_binder_del_ref,
	TP_PROTO(struct task_struct *proc, uint32_t ref_desc),
	TP_ARGS(proc, ref_desc));
DECLARE_HOOK(android_vh_binder_print_transaction_info,
	TP_PROTO(struct seq_file *m, struct binder_proc *proc,
		 const char *prefix, struct binder_transaction *t),
	TP_ARGS(m, proc, prefix, t));
DECLARE_HOOK(android_vh_binder_looper_state_registered,
	TP_PROTO(struct binder_thread *thread, struct binder_proc *proc),
	TP_ARGS(thread, proc));
DECLARE_HOOK(android_vh_binder_thread_read,
	TP_PROTO(struct list_head **list, struct binder_proc *proc,
		struct binder_thread *thread),
	TP_ARGS(list, proc, thread));
DECLARE_HOOK(android_vh_binder_free_proc,
	TP_PROTO(struct binder_proc *proc),
	TP_ARGS(proc));
DECLARE_HOOK(android_vh_binder_thread_release,
	TP_PROTO(struct binder_proc *proc, struct binder_thread *thread),
	TP_ARGS(proc, thread));
DECLARE_HOOK(android_vh_binder_read_done,
	TP_PROTO(struct binder_proc *proc, struct binder_thread *thread),
	TP_ARGS(proc, thread));
DECLARE_HOOK(android_vh_binder_has_work_ilocked,
	TP_PROTO(struct binder_thread *thread, bool do_proc_work, int *ret),
	TP_ARGS(thread, do_proc_work, ret));
DECLARE_HOOK(android_vh_binder_proc_transaction_finish,
	TP_PROTO(struct binder_proc *proc, struct binder_transaction *t,
		struct task_struct *binder_th_task, bool pending_async, bool sync),
	TP_ARGS(proc, t, binder_th_task, pending_async, sync));
DECLARE_HOOK(android_vh_alloc_oem_binder_struct,
	TP_PROTO(struct binder_transaction_data *tr, struct binder_transaction *t,
		struct binder_proc *proc),
	TP_ARGS(tr, t, proc));
DECLARE_HOOK(android_vh_binder_transaction_received,
	TP_PROTO(struct binder_transaction *t, struct binder_proc *proc,
		struct binder_thread *thread, uint32_t cmd),
	TP_ARGS(t, proc, thread, cmd));
DECLARE_HOOK(android_vh_free_oem_binder_struct,
	TP_PROTO(struct binder_transaction *t),
	TP_ARGS(t));
DECLARE_HOOK(android_vh_binder_special_task,
	TP_PROTO(struct binder_transaction *t, struct binder_proc *proc,
		struct binder_thread *thread, struct binder_work *w,
		struct list_head *head, bool sync, bool *special_task),
	TP_ARGS(t, proc, thread, w, head, sync, special_task));
DECLARE_HOOK(android_vh_binder_free_buf,
	TP_PROTO(struct binder_proc *proc, struct binder_thread *thread,
		struct binder_buffer *buffer),
	TP_ARGS(proc, thread, buffer));
DECLARE_HOOK(android_vh_binder_buffer_release,
	TP_PROTO(struct binder_proc *proc, struct binder_thread *thread,
		struct binder_buffer *buffer, bool has_transaction),
	TP_ARGS(proc, thread, buffer, has_transaction));
DECLARE_HOOK(android_vh_binder_find_desc,
	TP_PROTO(struct binder_proc *proc, uint32_t *desc),
	TP_ARGS(proc, desc));
DECLARE_HOOK(android_vh_binder_set_desc_bit,
	TP_PROTO(struct binder_proc *proc, uint32_t desc),
	TP_ARGS(proc, desc));
DECLARE_HOOK(android_vh_binder_desc_init,
	TP_PROTO(struct binder_proc *proc),
	TP_ARGS(proc));

#endif /* _TRACE_HOOK_BINDER_H */
/* This part must be outside protection */
#include <trace/define_trace.h>
