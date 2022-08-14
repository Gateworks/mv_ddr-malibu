/* Copyright (c) 2017 Marvell.
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _MV_DDR_STATIC_H
#define _MV_DDR_STATIC_H

#ifdef CONFIG_MC_STATIC
int mv_ddr_mc_static_config(void);
#endif /* CONFIG_MC_STATIC */

#ifdef CONFIG_PHY_STATIC
void mv_ddr_phy_static_config(void);
#endif /* CONFIG_PHY_STATIC */

#endif /* _MV_DDR_STATIC_H */
