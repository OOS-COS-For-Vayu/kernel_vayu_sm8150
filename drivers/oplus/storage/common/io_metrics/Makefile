# SPDX-License-Identifier: GPL-2.0-only
ccflags-y += -DSOC_PLATFORM_IS_QCOM

obj-$(CONFIG_OPLUS_FEATURE_STORAGE_IO_METRICS) += oplus_bsp_storage_io_metrics.o
oplus_bsp_storage_io_metrics-y += procfs.o
oplus_bsp_storage_io_metrics-y += io_metrics_entry.o
oplus_bsp_storage_io_metrics-y += block_metrics.o
oplus_bsp_storage_io_metrics-y += f2fs_metrics.o
oplus_bsp_storage_io_metrics-y += ufs_metrics.o
oplus_bsp_storage_io_metrics-y += abnormal_io.o
