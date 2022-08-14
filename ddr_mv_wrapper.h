/* Copyright (C) 2016 Marvell.
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _DDR_MV_WRAPPER_H
#define _DDR_MV_WRAPPER_H

#if defined(CONFIG_ARMADA_38X) || defined(CONFIG_ARMADA_39X)
#define INTER_REGS_BASE	0xd0000000
#endif

#include "mv_os.h"
#include "printf.h"
#include "mvUart.h"
#include "util.h"

typedef unsigned long long uint64_t;
typedef uint64_t uintptr_t;

static inline void mmio_write_64(uintptr_t addr, uint64_t value)
{
}

static inline uint64_t mmio_read_64(uintptr_t addr)
{
	return (uint64_t)0;
}

/* u-boot/tools/marvell/bin_hdr/platform/utils/printf.c */
#define printf mvPrintf

#define ARRAY_SIZE(x) (sizeof(x) / sizeof((x)[0]))
#define reg_write MV_REG_WRITE
#define reg_read MV_REG_READ
#define reg_bit_set MV_REG_BIT_SET

/* uboot/tools/marvell/bin_hdr/platform/drivers/mv_time.c */
void mdelay(unsigned long);

/* TODO: Check if LE/BE support is needed */
#define MV_MEMIO_LE32_WRITE2(data,addr) \
		MV_MEMIO32_WRITE(addr, MV_32BIT_LE_FAST(data))
#define writel MV_MEMIO_LE32_WRITE2
#define readl MV_MEMIO_LE32_READ
#define writeq mmio_write_64
#define readq mmio_read_64

#endif /* _DDR_MV_WRAPPER_H */
