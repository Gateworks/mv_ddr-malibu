/* Copyright (c) 2017 Marvell.
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _MV_DDR_PLAT_H
#define _MV_DDR_PLAT_H

#include "mv_ddr_mc6.h"
#include "mv_ddr_topology.h"

#define INTER_REGS_BASE		0xC0000000

#define MC6_BASE		0x0

#define MAX_DEVICE_NUM		1
#define MAX_INTERFACE_NUM	1
#define MAX_BUS_NUM		9

void ddr_controller_init(struct mv_ddr_topology_map *map);
int ddr_reg_write(u32 reg, u32 value);

#endif /* _MV_DDR_PLAT_H */
