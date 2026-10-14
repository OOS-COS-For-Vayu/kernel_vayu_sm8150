// SPDX-License-Identifier: GPL-2.0-only
/* vendor_hook.c
 *
 * Android Vendor Hook Support
 *
 * Copyright 2020 Google LLC
 */

#define CREATE_TRACE_POINTS
#ifdef CONFIG_TRACEPOINTS
#include <trace/hooks/vendor_hooks.h>
#else
#include <trace/hooks/vendor_hooks_stub.h>
#endif
#include <trace/hooks/memory.h>
#include <trace/hooks/syscall_check.h>
#include <trace/hooks/binder.h>
#include <trace/hooks/signal.h>
#include <trace/hooks/sched.h>
#include <trace/hooks/midas_dev.h>

/*
 * Export tracepoints that act as a bare tracehook (ie: have no trace event
 * associated with them) to allow external modules to probe them.
 */
EXPORT_TRACEPOINT_SYMBOL_GPL(android_vh_set_memory_x);
EXPORT_TRACEPOINT_SYMBOL_GPL(android_vh_set_memory_nx);
EXPORT_TRACEPOINT_SYMBOL_GPL(android_vh_set_memory_ro);
EXPORT_TRACEPOINT_SYMBOL_GPL(android_vh_set_memory_rw);
EXPORT_TRACEPOINT_SYMBOL_GPL(android_vh_check_mmap_file);
EXPORT_TRACEPOINT_SYMBOL_GPL(android_vh_check_file_open);
EXPORT_TRACEPOINT_SYMBOL_GPL(android_vh_check_bpf_syscall);

/* Binder hooks */
EXPORT_TRACEPOINT_SYMBOL_GPL(android_vh_binder_transaction_init);
EXPORT_TRACEPOINT_SYMBOL_GPL(android_vh_binder_priority_skip);
EXPORT_TRACEPOINT_SYMBOL_GPL(android_vh_binder_set_priority);
EXPORT_TRACEPOINT_SYMBOL_GPL(android_vh_binder_restore_priority);
EXPORT_TRACEPOINT_SYMBOL_GPL(android_vh_binder_wakeup_ilocked);
EXPORT_TRACEPOINT_SYMBOL_GPL(android_vh_binder_wait_for_work);
EXPORT_TRACEPOINT_SYMBOL_GPL(android_vh_sync_txn_recvd);
EXPORT_TRACEPOINT_SYMBOL_GPL(android_vh_binder_alloc_new_buf_locked);
EXPORT_TRACEPOINT_SYMBOL_GPL(android_vh_binder_reply);
EXPORT_TRACEPOINT_SYMBOL_GPL(android_vh_binder_trans);
EXPORT_TRACEPOINT_SYMBOL_GPL(android_vh_binder_preset);
EXPORT_TRACEPOINT_SYMBOL_GPL(android_vh_binder_proc_transaction);
EXPORT_TRACEPOINT_SYMBOL_GPL(android_vh_binder_proc_transaction_end);
EXPORT_TRACEPOINT_SYMBOL_GPL(android_vh_binder_new_ref);
EXPORT_TRACEPOINT_SYMBOL_GPL(android_vh_binder_del_ref);
EXPORT_TRACEPOINT_SYMBOL_GPL(android_vh_binder_print_transaction_info);
EXPORT_TRACEPOINT_SYMBOL_GPL(android_vh_binder_free_proc);
EXPORT_TRACEPOINT_SYMBOL_GPL(android_vh_binder_proc_transaction_finish);
EXPORT_TRACEPOINT_SYMBOL_GPL(android_vh_alloc_oem_binder_struct);
EXPORT_TRACEPOINT_SYMBOL_GPL(android_vh_binder_transaction_received);
EXPORT_TRACEPOINT_SYMBOL_GPL(android_vh_free_oem_binder_struct);
EXPORT_TRACEPOINT_SYMBOL_GPL(android_vh_binder_special_task);
EXPORT_TRACEPOINT_SYMBOL_GPL(android_vh_binder_buffer_release);
EXPORT_TRACEPOINT_SYMBOL_GPL(android_vh_binder_find_desc);
EXPORT_TRACEPOINT_SYMBOL_GPL(android_vh_binder_set_desc_bit);
EXPORT_TRACEPOINT_SYMBOL_GPL(android_vh_binder_desc_init);

/* Signal hooks */
EXPORT_TRACEPOINT_SYMBOL_GPL(android_vh_do_send_sig_info);
EXPORT_TRACEPOINT_SYMBOL_GPL(android_vh_process_killed);
EXPORT_TRACEPOINT_SYMBOL_GPL(android_vh_killed_process);

/* Sched hooks */
EXPORT_TRACEPOINT_SYMBOL_GPL(android_vh_sched_stat_runtime_rt);
EXPORT_TRACEPOINT_SYMBOL_GPL(android_vh_dup_task_struct);
EXPORT_TRACEPOINT_SYMBOL_GPL(android_vh_free_task);
EXPORT_TRACEPOINT_SYMBOL_GPL(android_vh_account_task_time);
EXPORT_TRACEPOINT_SYMBOL_GPL(android_vh_irqtime_account_process_tick);
EXPORT_TRACEPOINT_SYMBOL_GPL(android_vh_sched_setaffinity_early);
EXPORT_TRACEPOINT_SYMBOL_GPL(android_vh_prepare_update_load_avg_se);
EXPORT_TRACEPOINT_SYMBOL_GPL(android_vh_finish_update_load_avg_se);
EXPORT_TRACEPOINT_SYMBOL_GPL(android_vh_map_util_freq);
EXPORT_TRACEPOINT_SYMBOL_GPL(android_vh_build_sched_domains);
EXPORT_TRACEPOINT_SYMBOL_GPL(android_rvh_flush_task);
EXPORT_TRACEPOINT_SYMBOL_GPL(android_rvh_sched_fork);
EXPORT_TRACEPOINT_SYMBOL_GPL(android_rvh_try_to_wake_up);
EXPORT_TRACEPOINT_SYMBOL_GPL(android_rvh_new_task_stats);

/* Midas hooks */
EXPORT_TRACEPOINT_SYMBOL_GPL(android_vh_midas_record_task_times);
