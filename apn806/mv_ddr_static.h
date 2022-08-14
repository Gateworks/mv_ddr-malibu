/* Copyright (c) 2017 Marvell.
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _MV_DDR_STATIC_H
#define _MV_DDR_STATIC_H

#ifdef CONFIG_MC_STATIC
int mv_ddr_mc_static_config(void);
#endif

#ifdef CONFIG_MC_STATIC_PRINT
int mv_ddr_mc_static_print(void);
#endif

#ifdef CONFIG_PHY_STATIC
int mv_ddr_phy_static_config(void);
#endif

#ifdef CONFIG_PHY_STATIC_PRINT
int mv_ddr_phy_static_print(void);
#endif

#endif /* _MV_DDR_STATIC_H */
