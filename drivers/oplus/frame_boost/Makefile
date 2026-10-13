# SPDX-License-Identifier: GPL-2.0-only
# Copyright (C) 2018-2020 Oplus. All rights reserved.

ccflags-y += -I$(srctree)/drivers/oplus/frame_boost/multi_rtg \
             -I$(srctree)/drivers/oplus/frame_boost \
             -I$(src)/multi_rtg \
             -I$(srctree)/kernel/sched \
             -I$(srctree)/drivers/android

obj-$(CONFIG_OPLUS_FEATURE_FRAME_BOOST) += oplus_bsp_frame_boost.o

oplus_bsp_frame_boost-y := multi_rtg/frame_boost.o \
                           multi_rtg/frame_sysctl.o \
                           multi_rtg/frame_info.o \
                           multi_rtg/frame_group.o \
                           multi_rtg/cluster_boost.o \
                           multi_rtg/frame_debug.o
