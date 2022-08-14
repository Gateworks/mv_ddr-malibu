/* Copyright (c) 2018 Marvell.
 * SPDX-License-Identifier: BSD-3-Clause
 */

#if defined(MV_DDR) /* U-BOOT MARVELL 2013.01 */
#include "ddr_mv_wrapper.h"
#elif defined(MV_DDR_ATF) /* MARVELL ATF */
#include "mv_ddr_atf_wrapper.h"
#elif defined(CONFIG_A3700)
#include "mv_ddr_a3700_wrapper.h"
#else /* U-BOOT SPL */
#include "ddr_ml_wrapper.h"
#endif

#include "mv_ddr_init.h"
#include "mv_ddr_plat.h"
#include "mv_ddr_topology.h"

int dram_init(void)
{
	return mv_ddr_init();
}

void dram_mmap_config(void)
{
	mv_ddr_mmap_config();
}

unsigned long long dram_iface_mem_sz_get(void)
{
	/*
	 * call mv_ddr_pre_config to update topology
	 * prior to mv_ddr_mem_sz_get call
	 */
	mv_ddr_pre_config();

	return mv_ddr_mem_sz_get();
}
