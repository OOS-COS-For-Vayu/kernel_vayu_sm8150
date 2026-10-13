// SPDX-License-Identifier: GPL-2.0-only
/*
 * Copyright (C) 2020 Oplus. All rights reserved.
 */

#include <linux/sched.h>
#include <linux/sched/task.h>
#include <linux/proc_fs.h>
#include <linux/uaccess.h>
#include <linux/seq_file.h>
#include <linux/slab.h>
#include <linux/compat.h>

#include "binder_sched.h"
#include "binder_sysfs.h"

#define OPLUS_BINDER_PROC_DIR	"oplus_binder"

static struct proc_dir_entry *d_oplus_binder;

enum {
	BINDER_OPT_STR_TYPE = 0,
	BINDER_OPT_STR_PID,
	BINDER_OPT_STR_VAL,
	BINDER_OPT_STR_MAX = 3,
};

static ssize_t proc_async_ux_enable_write(struct file *file, const char __user *buf,
		size_t count, loff_t *ppos)
{
	char buffer[8];
	int err, val;

	memset(buffer, 0, sizeof(buffer));

	if (count > sizeof(buffer) - 1)
		count = sizeof(buffer) - 1;

	if (copy_from_user(buffer, buf, count))
		return -EFAULT;

	buffer[count] = '\0';
	err = kstrtoint(strstrip(buffer), 10, &val);
	if (err)
		return err;

	g_async_ux_enable = val;

	return count;
}

static ssize_t proc_async_ux_enable_read(struct file *file, char __user *buf,
		size_t count, loff_t *ppos)
{
	char buffer[20];
	size_t len = 0;

	len = snprintf(buffer, sizeof(buffer), "%d\n", g_async_ux_enable);

	return simple_read_from_buffer(buf, count, ppos, buffer, len);
}

static ssize_t proc_set_last_async_ux_write(struct file *file, const char __user *buf,
		size_t count, loff_t *ppos)
{
	char buffer[8];
	int err, val;

	memset(buffer, 0, sizeof(buffer));

	if (count > sizeof(buffer) - 1)
		count = sizeof(buffer) - 1;

	if (copy_from_user(buffer, buf, count))
		return -EFAULT;

	buffer[count] = '\0';
	err = kstrtoint(strstrip(buffer), 10, &val);
	if (err)
		return err;

	g_set_last_async_ux = val;

	return count;
}

static ssize_t proc_set_last_async_ux_read(struct file *file, char __user *buf,
		size_t count, loff_t *ppos)
{
	char buffer[20];
	size_t len = 0;

	len = snprintf(buffer, sizeof(buffer), "%d\n", g_set_last_async_ux);

	return simple_read_from_buffer(buf, count, ppos, buffer, len);
}


static ssize_t proc_set_async_ux_after_pending_write(struct file *file, const char __user *buf,
		size_t count, loff_t *ppos)
{
	char buffer[8];
	int err, val;
	memset(buffer, 0, sizeof(buffer));
	if (count > sizeof(buffer) - 1)
		count = sizeof(buffer) - 1;
	if (copy_from_user(buffer, buf, count))
		return -EFAULT;
	buffer[count] = '\0';
	err = kstrtoint(strstrip(buffer), 10, &val);
	if (err)
		return err;
	g_set_async_ux_after_pending = val;
	return count;
}
static ssize_t proc_set_async_ux_after_pending_read(struct file *file, char __user *buf,
		size_t count, loff_t *ppos)
{
	char buffer[20];
	size_t len = 0;
	len = snprintf(buffer, sizeof(buffer), "%d\n", g_set_async_ux_after_pending);
	return simple_read_from_buffer(buf, count, ppos, buffer, len);
}
static long proc_async_ux_flag_ioctl(struct file *file, unsigned int cmd, unsigned long arg)
{
	void __user *uarg = (void __user *)arg;
	int pid;
	int ux_flag;

	if (cmd == CMD_SET_ASYNC_BINDER_UX) {
		int write_param[2];
		if (copy_from_user(write_param, uarg, sizeof(write_param))) {
			return -EFAULT;
		}
		pid = write_param[0];
		ux_flag = write_param[1];
		set_task_async_ux_enable(pid, ux_flag);
		return 0;
	}
	return -ENOTTY;
}

#ifdef CONFIG_COMPAT
static long proc_async_ux_flag_compat_ioctl(struct file *file, unsigned int cmd, unsigned long arg)
{
	return proc_async_ux_flag_ioctl(file, cmd, (unsigned long)(compat_ptr(arg)));
}
#endif
static ssize_t proc_async_ux_flag_write(struct file *file, const char __user *buf,
		size_t count, loff_t *ppos)
{
	char buffer[64];
	char *str, *token;
	char opt_str[BINDER_OPT_STR_MAX][8];
	int cnt = 0;
	int pid = 0;
	int ux_flag = 0;
	int err = 0;

	if (unlikely(!g_async_ux_enable)) {
		return count;
	}
	memset(buffer, 0, sizeof(buffer));

	if (count > sizeof(buffer) - 1)
		count = sizeof(buffer) - 1;

	if (copy_from_user(buffer, buf, count))
		return -EFAULT;

	buffer[count] = '\0';
	str = strstrip(buffer);
	while ((token = strsep(&str, " ")) && *token && (cnt < BINDER_OPT_STR_MAX)) {
		strlcpy(opt_str[cnt], token, sizeof(opt_str[cnt]));
		cnt += 1;
	}

	if (cnt != BINDER_OPT_STR_MAX) {
		if (cnt == (BINDER_OPT_STR_MAX - 1) && !strncmp(opt_str[BINDER_OPT_STR_TYPE], "r", 1)) {
			err = kstrtoint(strstrip(opt_str[BINDER_OPT_STR_PID]), 10, &pid);
			if (err)
				return err;

			return count;
		} else {
			return -EFAULT;
		}
	}

	err = kstrtoint(strstrip(opt_str[BINDER_OPT_STR_PID]), 10, &pid);
	if (err)
		return err;

	err = kstrtoint(strstrip(opt_str[BINDER_OPT_STR_VAL]), 10, &ux_flag);
	if (err)
		return err;

	if (!strncmp(opt_str[BINDER_OPT_STR_TYPE], "p", 1)) {
		set_task_async_ux_enable(pid, ux_flag);
	}

	return count;
}

static ssize_t proc_async_ux_flag_read(struct file *file, char __user *buf,
		size_t count, loff_t *ppos)
{
	char buffer[256];
	size_t len = 0;
	struct task_struct *task = current;

	len = snprintf(buffer, sizeof(buffer), "comm=%s pid=%d tgid=%d enable=%d\n",
		task->comm, task->pid, task->tgid, get_task_async_ux_enable(CURRENT_TASK_PID));

	return simple_read_from_buffer(buf, count, ppos, buffer, len);
}

static ssize_t proc_binder_all_tasks_async_ux_read(struct file *file, char __user *buf,
		size_t count, loff_t *ppos)
{
	char buffer[256];
	size_t len = 0;

	get_all_tasks_async_ux_enable();
	len = snprintf(buffer, sizeof(buffer), "pls check dmesg [async_ux_tasks]\n");

	return simple_read_from_buffer(buf, count, ppos, buffer, len);
}

static ssize_t proc_sched_debug_enable_write(struct file *file, const char __user *buf,
		size_t count, loff_t *ppos)
{
	char buffer[16];
	unsigned long long val = 0;
	int err;
	memset(buffer, 0, sizeof(buffer));
	if (count > sizeof(buffer) - 1)
		count = sizeof(buffer) - 1;
	if (copy_from_user(buffer, buf, count))
		return -EFAULT;
	buffer[count] = '\0';
	err = kstrtou64(strstrip(buffer), 10, &val);
	if (err)
		return err;
	if (val)
		g_sched_debug |= val;
	else
		g_sched_debug = val;
	return count;
}

static ssize_t proc_sched_debug_enable_read(struct file *file, char __user *buf,
		size_t count, loff_t *ppos)
{
	char buffer[20];
	size_t len = 0;
	len = snprintf(buffer, sizeof(buffer), "%lld\n", g_sched_debug);
	return simple_read_from_buffer(buf, count, ppos, buffer, len);
}

static ssize_t proc_use_t_vendordata_write(struct file *file, const char __user *buf,
		size_t count, loff_t *ppos)
{
	char buffer[8];
	int err, val;

	memset(buffer, 0, sizeof(buffer));

	if (count > sizeof(buffer) - 1)
		count = sizeof(buffer) - 1;

	if (copy_from_user(buffer, buf, count))
		return -EFAULT;

	buffer[count] = '\0';
	err = kstrtoint(strstrip(buffer), 10, &val);
	if (err)
		return err;

	sync_use_t_vendordata = val;

	return count;
}

static ssize_t proc_use_t_vendordata_read(struct file *file, char __user *buf,
		size_t count, loff_t *ppos)
{
	char buffer[20];
	size_t len = 0;

	len = snprintf(buffer, sizeof(buffer), "%d\n", sync_use_t_vendordata);

	return simple_read_from_buffer(buf, count, ppos, buffer, len);
}

static ssize_t proc_unset_async_ux_inrestore_write(struct file *file, const char __user *buf,
		size_t count, loff_t *ppos)
{
	char buffer[8];
	int err, val;

	memset(buffer, 0, sizeof(buffer));

	if (count > sizeof(buffer) - 1)
		count = sizeof(buffer) - 1;

	if (copy_from_user(buffer, buf, count))
		return -EFAULT;

	buffer[count] = '\0';
	err = kstrtoint(strstrip(buffer), 10, &val);
	if (err)
		return err;

	unset_async_ux_inrestore = val;

	return count;
}

static ssize_t proc_unset_async_ux_inrestore_read(struct file *file, char __user *buf,
		size_t count, loff_t *ppos)
{
	char buffer[20];
	size_t len = 0;

	len = snprintf(buffer, sizeof(buffer), "%d\n", unset_async_ux_inrestore);

	return simple_read_from_buffer(buf, count, ppos, buffer, len);
}

static ssize_t proc_get_random_binder_task_write(struct file *file, const char __user *buf,
		size_t count, loff_t *ppos)
{
	char buffer[8];
	int err, val;

	memset(buffer, 0, sizeof(buffer));

	if (count > sizeof(buffer) - 1)
		count = sizeof(buffer) - 1;

	if (copy_from_user(buffer, buf, count))
		return -EFAULT;

	buffer[count] = '\0';
	err = kstrtoint(strstrip(buffer), 10, &val);
	if (err)
		return err;

	get_random_binder_task = val;

	return count;
}

static ssize_t proc_get_random_binder_task_read(struct file *file, char __user *buf,
		size_t count, loff_t *ppos)
{
	char buffer[20];
	size_t len = 0;

	len = snprintf(buffer, sizeof(buffer), "%d\n", get_random_binder_task);

	return simple_read_from_buffer(buf, count, ppos, buffer, len);
}


/* 4.19 compat: proc_ops doesn't exist, use file_operations */
static const struct file_operations proc_async_ux_enable_fops = {
	.write		= proc_async_ux_enable_write,
	.read		= proc_async_ux_enable_read,
	.llseek		= default_llseek,
};

static const struct file_operations proc_set_last_async_ux_fops = {
	.write		= proc_set_last_async_ux_write,
	.read		= proc_set_last_async_ux_read,
	.llseek		= default_llseek,
};

static const struct file_operations proc_set_async_ux_after_pending_fops = {
	.write		= proc_set_async_ux_after_pending_write,
	.read		= proc_set_async_ux_after_pending_read,
	.llseek		= default_llseek,
};
static const struct file_operations proc_binder_async_ux_flag_fops = {
	.write		= proc_async_ux_flag_write,
	.read		= proc_async_ux_flag_read,
	.llseek		= default_llseek,
	.unlocked_ioctl	= proc_async_ux_flag_ioctl,
#ifdef CONFIG_COMPAT
	.compat_ioctl	= proc_async_ux_flag_compat_ioctl,
#endif
};

static const struct file_operations proc_binder_all_tasks_ux_sts_fops = {
	.write		= NULL,
	.read		= proc_binder_all_tasks_async_ux_read,
	.llseek		= default_llseek,
};

static const struct file_operations proc_binder_sched_debug_enable_fops = {
	.write		= proc_sched_debug_enable_write,
	.read		= proc_sched_debug_enable_read,
	.llseek		= default_llseek,
};

static const struct file_operations proc_use_t_vendordata_fops = {
	.write		= proc_use_t_vendordata_write,
	.read		= proc_use_t_vendordata_read,
	.llseek		= default_llseek,
};

static const struct file_operations proc_unset_async_ux_inrestore_fops = {
	.write		= proc_unset_async_ux_inrestore_write,
	.read		= proc_unset_async_ux_inrestore_read,
	.llseek		= default_llseek,
};

static const struct file_operations proc_get_random_binder_task_fops = {
	.write		= proc_get_random_binder_task_write,
	.read		= proc_get_random_binder_task_read,
	.llseek		= default_llseek,
};

int oplus_binder_sysfs_init(void)
{
	struct proc_dir_entry *proc_node;

	d_oplus_binder = proc_mkdir(OPLUS_BINDER_PROC_DIR, NULL);
	if (!d_oplus_binder) {
		pr_err("failed to create proc dir d_oplus_binder\n");
		goto err_create_d_oplus_binder;
	}

	proc_node = proc_create("async_ux_enable", 0666, d_oplus_binder, &proc_async_ux_enable_fops);
	if (!proc_node) {
		pr_err("failed to create proc node proc_async_ux_enable_fops\n");
		goto err_create_async_ux_enable;
	}

	proc_node = proc_create("set_last_async_ux", 0666, d_oplus_binder, &proc_set_last_async_ux_fops);
	if (!proc_node) {
		pr_err("failed to create proc node proc_set_last_async_ux_fops\n");
		goto err_create_set_last_async_ux;
	}
	proc_node = proc_create("set_async_ux_after_pending", 0666, d_oplus_binder, &proc_set_async_ux_after_pending_fops);
	if (!proc_node) {
		pr_err("failed to create proc node proc_set_async_ux_after_pending_fops\n");
		goto err_create_set_async_ux_after_pending;
	}

	proc_node = proc_create("ux_flag", 0666, d_oplus_binder, &proc_binder_async_ux_flag_fops);
	if (!proc_node) {
		pr_err("failed to create proc node proc_binder_async_ux_flag_fops\n");
		goto err_create_async_ux_flag;
	}

	proc_node = proc_create("all_tasks_ux_sts", 0444, d_oplus_binder, &proc_binder_all_tasks_ux_sts_fops);
	if (!proc_node) {
		pr_err("failed to create proc node proc_binder_all_tasks_ux_sts_fops\n");
		goto err_create_all_tasks_ux_sts;
	}

	proc_node = proc_create("sched_debug", 0666, d_oplus_binder, &proc_binder_sched_debug_enable_fops);
	if (!proc_node) {
		pr_err("failed to create proc node proc_binder_sched_debug_enable_fops\n");
		goto err_create_sched_debug;
	}

	proc_node = proc_create("use_t_vendordata", 0666, d_oplus_binder, &proc_use_t_vendordata_fops);
	if (!proc_node) {
		pr_err("failed to create proc node proc_use_t_vendordata_fops\n");
		goto err_create_use_t_vendordata;
	}

	proc_node = proc_create("unset_async_ux_inrestore", 0666, d_oplus_binder, &proc_unset_async_ux_inrestore_fops);
	if (!proc_node) {
		pr_err("failed to create proc node unset_async_ux_inrestore\n");
		goto err_create_unset_async_ux_inrestore;
	}

	proc_node = proc_create("get_random_binder_task", 0666, d_oplus_binder, &proc_get_random_binder_task_fops);
	if (!proc_node) {
		pr_err("failed to create proc node get_random_binder_task\n");
		goto err_create_get_random_binder_task;
	}

	pr_info("%s success\n", __func__);
	return 0;

err_create_get_random_binder_task:
	remove_proc_entry("unset_async_ux_inrestore", d_oplus_binder);
err_create_unset_async_ux_inrestore:
	remove_proc_entry("use_t_vendordata", d_oplus_binder);
err_create_use_t_vendordata:
	remove_proc_entry("sched_debug", d_oplus_binder);
err_create_sched_debug:
	remove_proc_entry("all_tasks_ux_sts", d_oplus_binder);
err_create_all_tasks_ux_sts:
	remove_proc_entry("ux_flag", d_oplus_binder);

err_create_async_ux_flag:
	remove_proc_entry("set_async_ux_after_pending", d_oplus_binder);

err_create_set_async_ux_after_pending:
	remove_proc_entry("set_last_async_ux", d_oplus_binder);
err_create_set_last_async_ux:
	remove_proc_entry("async_ux_enable", d_oplus_binder);

err_create_async_ux_enable:
	remove_proc_entry(OPLUS_BINDER_PROC_DIR, NULL);

err_create_d_oplus_binder:
	return -ENOENT;
}

void oplus_binder_sysfs_deinit(void)
{
	remove_proc_entry("async_ux_enable", d_oplus_binder);
	remove_proc_entry("set_last_async_ux", d_oplus_binder);
	remove_proc_entry("set_async_ux_after_pending", d_oplus_binder);
	remove_proc_entry("ux_flag", d_oplus_binder);
	remove_proc_entry("all_tasks_ux_sts", d_oplus_binder);
	remove_proc_entry("sched_debug", d_oplus_binder);
	remove_proc_entry("use_t_vendordata", d_oplus_binder);
	remove_proc_entry("unset_async_ux_inrestore", d_oplus_binder);
	remove_proc_entry("get_random_binder_task", d_oplus_binder);
	remove_proc_entry(OPLUS_BINDER_PROC_DIR, NULL);
}
