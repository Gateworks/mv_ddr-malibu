/* Copyright (C) 2016 Marvell.
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "ddr3_init.h"

/* U-BOOT MARVELL 2013.01 SUPPORT */
#if defined(MV_DDR)
/* __udelay() implemented in tools/marvell/bin_hdr/platform/drivers/mv_time.c */
void mdelay(unsigned long msec)
{
	while (msec--)
		__udelay(1000);
}

MV_U32 ddr_init(void)
{
	ddr3_init();

	return MV_OK;
}
/* MARVELL ATF SUPPORT */
#elif defined(MV_DDR_ATF)
int dram_init(void)
{
	ddr3_init();
	return 0;
}
#endif
