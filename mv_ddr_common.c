/* Copyright (C) 2016 Marvell.
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "mv_ddr_common.h"
#if defined(MV_DDR) /* U-BOOT MARVELL 2013.01 */
#include "ddr_mv_wrapper.h"
#elif defined(MV_DDR_ATF) /* MARVELL ATF */
#include "mv_ddr_atf_wrapper.h"
#elif defined(CONFIG_A3700)
#include "mv_ddr_a3700_wrapper.h"
#else /* U-BOOT SPL */
#include "ddr_ml_wrapper.h"
#endif

void mv_ddr_ver_print(void)
{
	printf("%s %s\n", mv_ddr_version_string, mv_ddr_build_message);
}

/* ceiling division for positive integers */
unsigned int ceil_div(unsigned int x, unsigned int y)
{
	return (x % y) ? (x / y + 1) : (x / y);
}

/*
 * time to number of clocks calculation based on the rounding algorithm
 * using 97.4% inverse factor per JEDEC Standard No. 21-C, 4.1.2.L-4:
 * Serial Presence Detect (SPD) for DDR4 SDRAM Modules
 */
unsigned int time_to_nclk(unsigned int t, unsigned int tclk)
{
	/* t & tclk parameters are in ps */
	return ((unsigned long)t * 1000 / tclk + 974) / 1000;
}

/* round division of two positive integers to the nearest whole number */
int round_div(unsigned int dividend, unsigned int divisor, unsigned int *quotient)
{
	if (quotient == NULL) {
		printf("%s: error: NULL quotient pointer found\n", __func__);
		return MV_FAIL;
	}

	if (divisor == 0) {
		printf("%s: error: division by zero\n", __func__);
		return MV_FAIL;
	} else {
		*quotient = (dividend + divisor / 2) / divisor;
	}

	return MV_OK;
}
