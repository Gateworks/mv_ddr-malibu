/* Copyright (c) 2018 Marvell.
 * SPDX-License-Identifier: BSD-3-Clause
 */

/* TODO: need this wrapper block for ddr_topology_def.h resolution */
#if defined(MV_DDR) /* U-BOOT MARVELL 2013.01 */
#include "ddr_mv_wrapper.h"
#elif defined(MV_DDR_ATF) /* MARVELL ATF */
#include "mv_ddr_atf_wrapper.h"
#elif defined(CONFIG_A3700)
#include "mv_ddr_a3700_wrapper.h"
#else /* U-BOOT SPL */
#include "ddr_ml_wrapper.h"
#endif

#include "mv_ddr_common.h"
#include "mv_ddr_topology.h"
#include "mv_ddr_plat.h"
#include "ddr_topology_def.h"
#include "dram_if.h"

int mv_ddr_init(void)
{
	/* print mv_ddr version */
	mv_ddr_ver_print();

	/* preliminary mv_ddr configuration */
	if (mv_ddr_pre_config()) {
		printf("error: %s failed\n", __func__);
		return -1;
	}

	/* remap overlapping dram region to the top */
	if (mv_ddr_mc_remap() != 0)
		return -1;

#if defined(CONFIG_MC_STATIC)
	/* TODO: implement for apn810 */
	if (mv_ddr_mc_static_config()) {
		printf("error: %s failed\n", __func__);
		return -1;
	}
#else
	/* mv_ddr_mc */
	if (mv_ddr_mc_config()) {
		printf("error: %s failed\n", __func__);
		return -1;
	}
#endif

#if defined(CONFIG_MC_STATIC_PRINT)
	/* TODO: implement for apn810 */
	if (mv_ddr_mc_static_print()) {
		printf("error: %s failed\n", __func__);
		return -1;
	}
#endif

#if defined(CONFIG_PHY_STATIC)
	/* TODO: implement for apn810 */
	if (mv_ddr_phy_static_config()) {
		printf("error: %s failed\n", __func__);
		return -1;
	}
#else
	if (mv_ddr_phy_config()) {
		printf("error: %s failed\n", __func__);
#if defined(T9130)
		reg_write(0x6f0084, 0x0);
#endif
		return -1;
	}

#endif

#if defined(CONFIG_PHY_STATIC_PRINT)
	/* TODO: implement for apn810 */
	if (mv_ddr_phy_static_print()) {
		printf("error: %s failed\n", __func__);
		return -1;
	}
#endif
	if (mv_ddr_mc_ena()) {
		printf("error: %s failed\n", __func__);
		return -1;
	}

	/* post mv_ddr configuration */
	if (mv_ddr_post_config()) {
		printf("error: %s failed\n", __func__);
		return -1;
	}

	printf("mv_ddr: completed successfully\n");

	return 0;
}
