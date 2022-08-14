/* Copyright (c) 2018 Marvell.
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "mv_ddr_atf_wrapper.h"
#include "snps_if.h"
#include "mv_ddr_mc6.h"
#include "mv_ddr_xor_v2.h"
#include "mv_ddr_plat.h"
#include "ddr_topology_def.h"
#include "mv_ddr_common.h"
/* TODO: remove this include when removing attribute mechanism */
#include "ddr3_training_ip_db.h"
#include "mv_ddr_validate.h"

#define DDR_INTERFACES_NUM		1

static unsigned int ap_regs_base;
static int is_pre_configured;

void mv_ddr_base_set(unsigned int base)
{
	ap_regs_base = base;
}

/* TODO: relocate to wrapper file */
/* register accessor functions */
void reg_write(u32 addr, u32 val)
{
	mmio_write_32(ap_regs_base + addr, val);
}

u32 reg_read(u32 addr)
{
	return mmio_read_32(ap_regs_base + addr);
}

void reg_bit_set(u32 addr, u32 mask)
{
	mmio_write_32(ap_regs_base + addr,
		      mmio_read_32(ap_regs_base + addr) | mask);
}

void reg_bit_clr(u32 addr, u32 mask)
{
	mmio_write_32(ap_regs_base + addr,
		      mmio_read_32(ap_regs_base + addr) & ~mask);
}

void reg_bit_clrset(u32 addr, u32 val, u32 mask)
{
	mmio_clrsetbits_32(ap_regs_base + addr, mask, val);
}

void mmio_write2_32(u32 val, u32 addr)
{
	mmio_write_32(addr, val);
}

u32 *snps_ext_swizzle_cfg_get(void)
{
	return NULL;
}

static void mv_ddr_mem_scrubbing(void)
{
	uint64_t val = 0;
	uint64_t tot_mem_sz;
	struct mv_ddr_iface *curr_iface = mv_ddr_iface_get();

	tot_mem_sz = mv_ddr_mem_sz_get();

	printf("mv_ddr: scrubbing memory...\n");

	/* scrub memory up to non-dram memory region */
	if ((curr_iface->iface_base_addr + tot_mem_sz < NON_DRAM_MEM_RGN_START_ADDR) ||
	    (curr_iface->iface_base_addr >= NON_DRAM_MEM_RGN_END_ADDR))
		mv_ddr_dma_memset(curr_iface->iface_base_addr, tot_mem_sz, val);
	else
		mv_ddr_dma_memset(curr_iface->iface_base_addr, NON_DRAM_MEM_RGN_START_ADDR, val);

	/* scrub memory up to the end */
	if ((tot_mem_sz >= NON_DRAM_MEM_RGN_END_ADDR) &&
	    (curr_iface->iface_base_addr < NON_DRAM_MEM_RGN_START_ADDR))
		mv_ddr_dma_memset(NON_DRAM_MEM_RGN_END_ADDR,
				  tot_mem_sz - NON_DRAM_MEM_RGN_START_ADDR, val);
}

int mv_ddr_pre_config(void)
{
	struct mv_ddr_iface *curr_iface;

	/* ensure the function is called once */
	if (is_pre_configured)
		return 0;

	curr_iface = mv_ddr_iface_get();
	mv_ddr_base_set(curr_iface->ap_base);

	/* TODO: remove attribute mechanism */
	ddr3_tip_dev_attr_init(0);
	ddr3_tip_dev_attr_set(0, MV_ATTR_OCTET_PER_INTERFACE, DDR_INTERFACE_OCTETS_NUM);

	if (mv_ddr_topology_map_update()) {
		printf("mv_ddr: failed to update topology\n");
		return -1;
	}

	/* ensure the function is called once */
	is_pre_configured = 1;

	return 0;
}

int mv_ddr_post_config(void)
{
	struct mv_ddr_iface *iface = mv_ddr_iface_get();

	if (mv_ddr_is_ecc_ena())
		mv_ddr_mem_scrubbing();

	/* Update the size of the iface in bytes */
	iface->iface_byte_size = mv_ddr_mem_sz_get();

	if (iface->validation != MV_DDR_VAL_DIS) {
		if (mv_ddr_validate())
			printf("DRAM validation interface %d start address 0x%llx\n",
				iface->id, iface->iface_base_addr);
	}

	/* unset is_pre_configured variable */
	is_pre_configured = 0;

	return 0;
}

unsigned int mv_ddr_init_freq_get(void)
{
	printf("error: %s: unset frequency found\n", __func__);

	return MV_DDR_FREQ_800;
}

int mv_ddr_mc_config(void)
{
	struct mv_ddr_iface *iface = mv_ddr_iface_get();
	int ecc_is_ena;

	ecc_is_ena = mv_ddr_is_ecc_ena();
	if (mv_ddr_mc6_config(MC6_BASE(iface->id), iface->iface_base_addr, ecc_is_ena)) {
		printf("error: %s failed\n", __func__);
		return -1;
	}

	return 0;
}


/* enable memory controllers */
int mv_ddr_mc_ena(void)
{
	struct mv_ddr_iface *iface = mv_ddr_iface_get();
	mv_ddr_mc6_init(MC6_BASE(iface->id));

	return 0;
}

int mv_ddr_phy_config(void)
{
	return snps_init(ap_regs_base + SNPS_PHY_OFFSET);
}

void mv_ddr_mmap_config(void)
{
	struct mv_ddr_iface *curr_iface = mv_ddr_iface_get();
	mv_ddr_mc6_sizes_cfg(MC6_BASE(curr_iface->id), curr_iface->iface_base_addr);
}
