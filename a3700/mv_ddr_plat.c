/* Copyright (c) 2017 Marvell.
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <stdio.h>
#include "ddr3_init.h"
#include "mv_ddr_common.h"

#define DDR_INTERFACE_OCTETS_NUM	9

/*
 * Accessor functions for the registers
 */
void mdelay(unsigned int msec)
{
}

void reg_write(u32 addr, u32 val)
{
}

u32 reg_read(u32 addr)
{
	return 0;
}

void reg_bit_set(u32 addr, u32 mask)
{
}

void reg_bit_clr(u32 addr, u32 mask)
{
}

void reg_bit_clrset(u32 addr, u32 val, u32 mask)
{
	ddr_reg_write(INTER_REGS_BASE + addr, val);
}

void mmio_write2_32(u32 val, u32 addr)
{
}

unsigned int mv_ddr_cs_num_get(void)
{
	unsigned int cs_num = 0;
	unsigned int cs, sphy;
	struct mv_ddr_topology_map *tm = mv_ddr_topology_map_get();
	struct if_params *iface_params = &(tm->interface_params[0]);
	unsigned int sphy_max = ddr3_tip_dev_attr_get(0, MV_ATTR_OCTET_PER_INTERFACE);

	for (sphy = 0; sphy < sphy_max; sphy++) {
		VALIDATE_BUS_ACTIVE(tm->bus_act_mask, sphy);
		break;
	}

	for (cs = 0; cs < MAX_CS_NUM; cs++) {
		VALIDATE_ACTIVE(iface_params->as_bus_params[sphy].cs_bitmask, cs);
		cs_num++;
	}

	return cs_num;
}
/*
 * Translates topology map definitions to real memory size in bits
  * (per values in ddr3_training_ip_def.h)
 */
u32 mem_size[] = {
	ADDR_SIZE_512MB,
	ADDR_SIZE_1GB,
	ADDR_SIZE_2GB,
	ADDR_SIZE_4GB,
	ADDR_SIZE_8GB
};

uint64_t mv_ddr_mem_sz_per_cs_get(void)
{
	uint64_t memory_size_per_cs;

	u32 bus_cnt, num_of_active_bus = 0;
	u32 num_of_sub_phys_per_ddr_unit = 0;

	struct mv_ddr_topology_map *tm = mv_ddr_topology_map_get();
	struct if_params *iface_params = &(tm->interface_params[0]);

	u32 octets_per_if_num = ddr3_tip_dev_attr_get(0, MV_ATTR_OCTET_PER_INTERFACE);

#if 0
	/* count the number of active bus */
	for (bus_cnt = 0; bus_cnt < octets_per_if_num - 1/* ignore ecc octet */; bus_cnt++) {
		VALIDATE_BUS_ACTIVE(tm->bus_act_mask, bus_cnt);
		num_of_active_bus++;
	}
#endif
	for (bus_cnt = 0; bus_cnt < octets_per_if_num - 1/* ignore ecc octet */; bus_cnt++) {
		if ((((tm->bus_act_mask) >> (bus_cnt)) & 1) == 0)
			continue;
		num_of_active_bus++;
	}

	/* calculate number of sub-phys per ddr unit */
	if (iface_params->bus_width/* supports only single interface */ == MV_DDR_DEV_WIDTH_16BIT)
		num_of_sub_phys_per_ddr_unit = MV_DDR_TWO_SPHY_PER_DUNIT;
	if (iface_params->bus_width/* supports only single interface */ == MV_DDR_DEV_WIDTH_8BIT)
		num_of_sub_phys_per_ddr_unit = MV_DDR_ONE_SPHY_PER_DUNIT;

	/* calculate dram size per cs */
	memory_size_per_cs = (uint64_t)mem_size[iface_params->memory_size] * (uint64_t)num_of_active_bus
		/ (uint64_t)num_of_sub_phys_per_ddr_unit;

	return memory_size_per_cs;
}

int mv_ddr_sw_db_init(u32 dev_num, u32 board_id)
{
	/* set device attributes*/
	ddr3_tip_dev_attr_init(dev_num);
	ddr3_tip_dev_attr_set(dev_num, MV_ATTR_TIP_REV, MV_TIP_REV_4);
	ddr3_tip_dev_attr_set(dev_num, MV_ATTR_PHY_EDGE, MV_DDR_PHY_EDGE_NEGATIVE);
	ddr3_tip_dev_attr_set(dev_num, MV_ATTR_OCTET_PER_INTERFACE, DDR_INTERFACE_OCTETS_NUM);
	ddr3_tip_dev_attr_set(dev_num, MV_ATTR_INTERLEAVE_WA, 0);

	return MV_OK;
}


static void mv_ddr_topology_map_set(struct mv_ddr_topology_map *map)
{
	struct mv_ddr_topology_map *tm = mv_ddr_topology_map_get();
	/* Return the board topology as defined in the board code */
	memcpy(tm, map, sizeof(struct mv_ddr_topology_map));
}

void ddr_controller_init(struct mv_ddr_topology_map *map)
{
	unsigned long iface_base_addr = 0x0;
	mv_ddr_topology_map_set(map);

	mv_ddr_sw_db_init(0, 0);

	mv_ddr_mc6_and_dram_timing_set(MC6_BASE);

	mv_ddr_mc6_sizes_cfg(MC6_BASE, iface_base_addr);
}
