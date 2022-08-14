/* Copyright (c) 2018 Marvell.
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "mv_ddr_atf_wrapper.h"
#include "mv_ddr_plat.h"
#include "mv_ddr_common.h"
#include "ddr_topology_def.h"
#include "mv_ddr_validate.h"
#include "mv_ddr_xor_v2.h"
#include "snps_regs.h"
#include "snps.h"
#include "mv_ddr_mc6.h"

#define RX_MAX_ADLL			32
#define MAX_RX_VREF			127
#define TX_VREF_RANGE_0			0
#define TX_VREF_RANGE_1			1
#define TX_MAX_ADLL			64
#define NIBBLE				4

#define DBG_DMA_DATA_SIZE		0x2000		/* 4KB; can be modified with proper alignment */
#define DBG_DMA_DESC_NUM		128
#define DBG_DMA_ENG_NUM			4
#define DBG_DMA_SRC_ADDR		(1 << 20)	/* can be modified with proper alignment */
#define DBG_DMA_DST_ADDR		(10 << 20)	/* can be modified with proper alignment */
#define DBG_MAX_CS_NUM			2
#if ((DBG_DMA_DST_ADDR) < ((DBG_DMA_SRC_ADDR) + (DBG_DMA_DATA_SIZE)))
#error "dma src and dst addr too close to each other..."
#endif

static uint64_t dma_src[DBG_MAX_CS_NUM][DBG_DMA_ENG_NUM], dma_dst[DBG_MAX_CS_NUM][DBG_DMA_ENG_NUM];

static uint64_t dma_gap_calc(uint64_t size)
{
	switch (size) {
	case _1G:
	case _2G:
		return _256M;
	case _4G:
	case _8G:
		return _512M;
	default:
		return 0;
	}
}

static int dma_offs_config(uint64_t dma_src[][DBG_DMA_ENG_NUM],
			   uint64_t dma_dst[][DBG_DMA_ENG_NUM],
			   u32 cs,
			   u32 iface_id)
{
	u32 dma_id;
	uint64_t dma_gap;
	uint64_t mem_size = mv_ddr_mc6_mem_size_get(MC6_BASE);
	uint64_t memory_offset = mv_ddr_mc6_mem_offs_get(cs, MC6_BASE);

	dma_gap = dma_gap_calc(mem_size);
	for (dma_id = 0; dma_id < DBG_DMA_ENG_NUM; dma_id++) {
		dma_src[cs][dma_id] = memory_offset + DBG_DMA_SRC_ADDR + (dma_id * dma_gap);
		dma_dst[cs][dma_id] = memory_offset + DBG_DMA_DST_ADDR + (dma_id * dma_gap);
	}

	return 0;
}

/* dma pattern */
static uint64_t dma_pattern[] = {
	0xf7f7f7f7f7f7f7f7,
	0x0808080808080808,
	0xf7f7f7f7f7f7f7f7,
	0xfbfbfbfbfbfbfbfb,
	0x0404040404040404,
	0x7f7f7f7f7f7f7f7f,
	0x8080808080808080,
	0x4040404040404040,
	0xbfbfbfbfbfbfbfbf,
	0x4040404040404040,
	0x2020202020202020,
	0xdfdfdfdfdfdfdfdf,
	0x2020202020202020,
	0x1010101010101010,
	0xefefefefefefefef,
	0x1010101010101010,
	0x0808080808080808,
	0xf7f7f7f7f7f7f7f7,
	0x0808080808080808,
	0x0404040404040404,
	0xfbfbfbfbfbfbfbfb,
	0x0404040404040404,
	0x0202020202020202,
	0xfdfdfdfdfdfdfdfd,
	0x0202020202020202,
	0x0101010101010101,
	0xfefefefefefefefe,
	0x0101010101010101,
	0x8080808080808080,
	0x7f7f7f7f7f7f7f7f,
	0x8080808080808080,
	0x4040404040404040,
	0xbfbfbfbfbfbfbfbf,
	0x4040404040404040,
	0x2020202020202020,
	0xdfdfdfdfdfdfdfdf,
	0x2020202020202020,
	0x1010101010101010,
	0xefefefefefefefef,
	0x1010101010101010,
	0x0808080808080808,
	0xf7f7f7f7f7f7f7f7,
	0x0808080808080808,
	0x0404040404040404,
	0xfbfbfbfbfbfbfbfb,
	0x0404040404040404,
	0x0202020202020202,
	0xfdfdfdfdfdfdfdfd,
	0x0202020202020202,
	0x0101010101010101,
	0xfefefefefefefefe,
	0x0101010101010101,
	0x8080808080808080,
	0x7f7f7f7f7f7f7f7f,
	0x8080808080808080,
	0x4040404040404040,
	0xbfbfbfbfbfbfbfbf,
	0x4040404040404040,
	0x2020202020202020,
	0xdfdfdfdfdfdfdfdf,
	0x2020202020202020,
	0x1010101010101010,
	0xefefefefefefefef,
	0x1010101010101010,
	0x0808080808080808,
	0xf7f7f7f7f7f7f7f7,
	0x0808080808080808,
	0x0404040404040404,
	0xfbfbfbfbfbfbfbfb,
	0xffffffffffffffff,
	0x0000000000000000,
	0xffffffffffffffff,
	0x0000000000000000,
	0xffffffffffffffff,
	0x0000000000000000,
	0xffffffffffffffff,
	0x0000000000000000,
	0x0000000000000000,
	0xffffffffffffffff,
	0x0000000000000000,
	0xffffffffffffffff,
	0x0000000000000000,
	0xffffffffffffffff,
	0x0000000000000000,
	0xffffffffffffffff,
	0xaaaaaaaaaaaaaaaa,
	0x5555555555555555,
	0xaaaaaaaaaaaaaaaa,
	0x5555555555555555,
	0xaaaaaaaaaaaaaaaa,
	0x5555555555555555,
	0xaaaaaaaaaaaaaaaa,
	0x5555555555555555,
	0x5555555555555555,
	0xaaaaaaaaaaaaaaaa,
	0x5555555555555555,
	0xaaaaaaaaaaaaaaaa,
	0x5555555555555555,
	0xaaaaaaaaaaaaaaaa,
	0x5555555555555555,
	0xaaaaaaaaaaaaaaaa
};

static void rx_validate_per_adll(u32 bit,
				 u32 dbyte,
				 u32 cs,
				 u32 current_vref,
				 u32 vref,
				 u32 current_adll_val_nibble0,
				 u32 current_adll_val_nibble1,
				 u32 vref_step)
{
	u32 res;

	for (u32 rx_adll = 0; rx_adll < RX_MAX_ADLL; rx_adll++) {
		if (bit < NIBBLE)
			snps_fw_write((((cs == 0) ?
				      REG_1008C_RX_CLK_DLY_TG0_NIBBLE0_BASE :
				      REG_1008D_RX_CLK_DLY_TG1_NIBBLE0_BASE) +
				      (dbyte << DBYTE_INSTANCE_OFFSET)), rx_adll);
		else
			snps_fw_write((((cs == 0) ?
				      REG_1018C_RX_CLK_DLY_TG0_NIBBLE1_BASE :
				      REG_1018D_RX_CLK_DLY_TG1_NIBBLE1_BASE) +
				      (dbyte << DBYTE_INSTANCE_OFFSET)), rx_adll);

		if (dbyte == 8)
			reg_write(MC6_BASE + MC6_CH0_ECC_1BIT_ERR_COUNTER_REG, 0x0);

		/* copy from src to dst */
		mv_ddr_dma_memcpy(dma_src[cs],
				  dma_dst[cs],
				  DBG_DMA_DATA_SIZE,
				  DBG_DMA_ENG_NUM,
				  DBG_DMA_DESC_NUM);
		/* compare src to dst */
		res = mv_ddr_dma_memcmp(dma_src[cs],
					dma_dst[cs],
					DBG_DMA_DATA_SIZE,
					DBG_DMA_ENG_NUM,
					DBG_DMA_DESC_NUM);

		if (dbyte == 8)
			res = reg_read(MC6_BASE + MC6_CH0_ECC_1BIT_ERR_COUNTER_REG);

		if (bit < NIBBLE) {
			if ((current_adll_val_nibble0 == rx_adll) &&
			    ((current_vref <= vref) &&
			    (current_vref >= vref - vref_step)))
				printf(" -1 ");
			else
				printf(" %d ", res);
		} else {
			if ((current_adll_val_nibble1 == rx_adll) &&
			    ((current_vref <= vref) &&
			    (current_vref >= vref - vref_step)))
				printf(" -1 ");
			else
				printf(" %d ", res);
		}
	}
}

static void rx_validate_per_vref(u32 dbyte,
				 u32 bit,
				 u32 cs,
				 u32 current_adll_val_nibble0,
				 u32 current_adll_val_nibble1,
				 u32 current_vref)
{
	u32 vref;
	u32 vref_step = 4;

	for (vref = 0; vref < MAX_RX_VREF; vref += vref_step) {
		printf("%d : ", vref);
		snps_fw_write((REG_10040_VREF_DAC0_BASE +
			      (dbyte << DBYTE_INSTANCE_OFFSET) +
			      (bit << DBYTE_BIT_OFFSET)), vref);

		rx_validate_per_adll(bit,
				     dbyte,
				     cs,
				     current_vref,
				     vref,
				     current_adll_val_nibble0,
				     current_adll_val_nibble1,
				     vref_step);

		printf("\n");
	}
}

static void rx_validate_per_bit(u32 current_adll_val_nibble0,
				u32 current_adll_val_nibble1,
				u32 max_bit_in_byte,
				u32 dbyte,
				u32 cs)
{
	u32 bit;
	u32 current_vref;

	for (bit = 0; bit < max_bit_in_byte; bit++) {
		printf("@@@@@@@@@@@@ bit: %d @@@@@@@@@@@@\n", bit);
		current_vref =
			snps_read((REG_10040_VREF_DAC0_BASE +
			(dbyte << DBYTE_INSTANCE_OFFSET) +
			(bit << DBYTE_BIT_OFFSET)));

		rx_validate_per_vref(dbyte,
				     bit,
				     cs,
				     current_adll_val_nibble0,
				     current_adll_val_nibble1,
				     current_vref);

		/*restore bit current adll and vref*/
		snps_fw_write((REG_10040_VREF_DAC0_BASE +
			      (dbyte << DBYTE_INSTANCE_OFFSET) +
			      (bit << DBYTE_BIT_OFFSET)),
			      current_vref);
		snps_fw_write((((cs == 0) ?
			      REG_1008C_RX_CLK_DLY_TG0_NIBBLE0_BASE :
			      REG_1008D_RX_CLK_DLY_TG1_NIBBLE0_BASE) +
			      (dbyte << DBYTE_INSTANCE_OFFSET)),
			      current_adll_val_nibble0);
		snps_fw_write((((cs == 0) ?
			      REG_1018C_RX_CLK_DLY_TG0_NIBBLE1_BASE :
			      REG_1018D_RX_CLK_DLY_TG1_NIBBLE1_BASE) +
			      (dbyte << DBYTE_INSTANCE_OFFSET)),
			      current_adll_val_nibble1);
	}
}

static void rx_validate_per_dbyte(u32 cs,
				  u32 max_bit_in_byte)
{
	u32 dbyte;
	u32 subphys_num = DDR_INTERFACE_OCTETS_NUM;
	struct mv_ddr_topology_map *tm = mv_ddr_topology_map_get();
	u32 current_adll_val_nibble0;
	u32 current_adll_val_nibble1;

	for (dbyte = 0; dbyte < subphys_num; dbyte++) {
		VALIDATE_BUS_ACTIVE(tm->bus_act_mask, dbyte);

		if (mv_ddr_is_ecc_ena() && (dbyte != 8))
			reg_bit_clrset(MC6_BASE + MC6_RAS_CTRL_REG, 0x0 << ECC_EN_OFFS, ECC_EN_MASK << ECC_EN_OFFS);

		printf("############# dbyte: %d ###################\n", dbyte);
		current_adll_val_nibble0 =
			snps_read((((cs == 0) ?
				  REG_1008C_RX_CLK_DLY_TG0_NIBBLE0_BASE :
				  REG_1008D_RX_CLK_DLY_TG1_NIBBLE0_BASE) + (dbyte << DBYTE_INSTANCE_OFFSET)));
				  current_adll_val_nibble1 =
			snps_read((((cs == 0) ?
				  REG_1018C_RX_CLK_DLY_TG0_NIBBLE1_BASE :
				  REG_1018D_RX_CLK_DLY_TG1_NIBBLE1_BASE) + (dbyte << DBYTE_INSTANCE_OFFSET)));

		rx_validate_per_bit(current_adll_val_nibble0,
				    current_adll_val_nibble1,
				    max_bit_in_byte,
				    dbyte,
				    cs);
		if (mv_ddr_is_ecc_ena() && (dbyte != 8))
			reg_bit_clrset(MC6_BASE + MC6_RAS_CTRL_REG, 0x1 << ECC_EN_OFFS, ECC_EN_MASK << ECC_EN_OFFS);
	}
}

static void rx_validate_per_cs(u32 max_bit_in_byte)
{
	unsigned int max_cs = mv_ddr_cs_num_get();
	u32 cs;

	for (cs = 0; cs < max_cs; cs++) {
		printf("^^^^^^^^^^^^^^ CS: %d ^^^^^^^^^^^^^^^^^^\n", cs);

		rx_validate_per_dbyte(cs,
				      max_bit_in_byte);
	}
}

static void tx_validate_per_adll(u32 bit,
				 u32 dbyte,
				 u32 cs,
				 u32 current_vref,
				 u32 vref,
				 u32 vref_step,
				 u32 abs_current_adll_val,
				 u32 range,
				 u32 current_range)
{
	u32 res;
	u32 coarse;

	for (u32 tx_adll = 0; tx_adll < TX_MAX_ADLL; tx_adll++) {
		res = 0;
		if (tx_adll < (TX_MAX_ADLL / 2))
			coarse = 0;
		else
			coarse = tx_adll / (TX_MAX_ADLL / 2);

		snps_fw_write((((cs == 0) ?
			      REG_100C0_TX_DLY_TG0_BASE :
			      REG_100C1_TX_DLY_TG1_BASE) +
			      (dbyte << DBYTE_INSTANCE_OFFSET) +
			      (bit << DBYTE_BIT_OFFSET)),
			      (coarse << TX_COARSE_DELAY_OFFS) | tx_adll);
		if (dbyte == 8)
			reg_write(MC6_BASE + MC6_CH0_ECC_1BIT_ERR_COUNTER_REG, 0x0);

		/* copy from src to dst */
		mv_ddr_dma_memcpy(dma_src[cs],
				  dma_dst[cs],
				  DBG_DMA_DATA_SIZE,
				  DBG_DMA_ENG_NUM,
				  DBG_DMA_DESC_NUM);
		/* compare src to dst */
		res = mv_ddr_dma_memcmp(dma_src[cs],
					dma_dst[cs],
					DBG_DMA_DATA_SIZE,
					DBG_DMA_ENG_NUM,
					DBG_DMA_DESC_NUM);

		if (dbyte == 8)
			res = reg_read(MC6_BASE + MC6_CH0_ECC_1BIT_ERR_COUNTER_REG);

		if ((range == current_range) &&
		    (abs_current_adll_val == tx_adll) &&
		    ((current_vref <= vref) &&
		    (current_vref >= vref - vref_step)))
			printf(" -1 ");
		else
			printf(" %d ", res);
	}
}

static void tx_validate_per_vref(int init_vref,
				 u32 range,
				 u32 current_range,
				 u32 current_vref,
				 u32 dbyte,
				 u32 bit,
				 u32 abs_current_adll_val,
				 u32 cs,
				 u32 iface_id)
{
	int vref;
	u32 vref_step = 3;

	for (vref = init_vref; vref >= 0; vref = vref - vref_step) {
		printf("%d %d: ", range, vref);

		reg_bit_clrset(MC6_BASE + MC6_CH0_DRAM_CFG4_REG,
			       (range << VREF_TRAINING_RANGE_DQ_OFFS) |
			       (vref << VREF_TRAINING_VALUE_DQ_OFFS),
			       (VREF_TRAINING_RANGE_DQ_MASK << VREF_TRAINING_RANGE_DQ_OFFS) |
			       (VREF_TRAINING_VALUE_DQ_MASK << VREF_TRAINING_VALUE_DQ_OFFS));

		reg_bit_clrset(MC6_BASE + MC6_USER_CMD2_REG,
			       (CMD2_LMRG_EN << CMD2_LMR6_REQ_OFFS) |
			       (CMD2_CS_EN << (cs + CMD2_CS_OFFS)) |
			       (CMD2_CH0_EN << CMD2_CH0_OFFS),
			       (CMD2_LMR6_REQ_MASK << CMD2_LMR6_REQ_OFFS) |
			       (CMD2_CS_MASK << (cs + CMD2_CS_OFFS)) |
			       (CMD2_CH0_MASK << CMD2_CH0_OFFS));

		tx_validate_per_adll(bit,
				     dbyte,
				     cs,
				     current_vref,
				     vref,
				     vref_step,
				     abs_current_adll_val,
				     range,
				     current_range);

		printf("\n");
	}
}

static void tx_validate_per_bit(u32 max_bit_in_byte,
				u32 dbyte,
				u32 cs,
				u32 current_vref,
				u32 current_range,
				u32 iface_id)
{
	u32 bit;
	u32 current_adll_val;
	u32 current_adll_coarse;
	u32 current_adll_fine;
	u32 abs_current_adll_val;

	for (bit = 0 ; bit < max_bit_in_byte; bit++) {
		printf("@@@@@@@@@@@@ bit: %d @@@@@@@@@@@@\n", bit);
		current_adll_val =
			snps_read(((cs == 0) ?
				  REG_100C0_TX_DLY_TG0_BASE :
				  REG_100C1_TX_DLY_TG1_BASE) +
				  (dbyte << DBYTE_INSTANCE_OFFSET) +
				  (bit << DBYTE_BIT_OFFSET));
		current_adll_coarse = (current_adll_val >> TX_COARSE_DELAY_OFFS) &
				       TX_COARSE_DELAY_MASK;
		current_adll_fine = (current_adll_val >> TX_FINE_DELAY_OFFS) &
				     TX_FINE_DELAY_MASK;
		abs_current_adll_val = (current_adll_coarse * (TX_MAX_ADLL / 2)) +
					current_adll_fine;

		tx_validate_per_vref(50,
				     TX_VREF_RANGE_0,
				     current_range,
				     current_vref,
				     dbyte,
				     bit,
				     abs_current_adll_val,
				     cs,
				     iface_id);

		tx_validate_per_vref(23,
				     TX_VREF_RANGE_1,
				     current_range,
				     current_vref,
				     dbyte,
				     bit,
				     abs_current_adll_val,
				     cs,
				     iface_id);

		/*restore bit current adll */
		snps_fw_write((((cs == 0) ?
			      REG_100C0_TX_DLY_TG0_BASE :
			      REG_100C1_TX_DLY_TG1_BASE) +
			      (dbyte << DBYTE_INSTANCE_OFFSET) +
			      (bit << DBYTE_BIT_OFFSET)),
			      (((current_adll_coarse & TX_COARSE_DELAY_MASK) <<
			      TX_COARSE_DELAY_OFFS) |
			      current_adll_fine));

		reg_bit_clrset(MC6_BASE + MC6_CH0_DRAM_CFG4_REG,
			       (VREF_TRAINING_RANGE_1 << VREF_TRAINING_RANGE_DQ_OFFS) |
			       (current_vref << VREF_TRAINING_VALUE_DQ_OFFS),
			       (VREF_TRAINING_RANGE_DQ_MASK << VREF_TRAINING_RANGE_DQ_OFFS) |
			       (VREF_TRAINING_VALUE_DQ_MASK << VREF_TRAINING_VALUE_DQ_OFFS));

		reg_bit_clrset(MC6_BASE + MC6_USER_CMD2_REG,
			       (CMD2_LMRG_EN << CMD2_LMR6_REQ_OFFS) |
			       (CMD2_CS_EN << (cs + CMD2_CS_OFFS)) |
			       (CMD2_CH0_EN << CMD2_CH0_OFFS),
			       (CMD2_LMR6_REQ_MASK << CMD2_LMR6_REQ_OFFS) |
			       (CMD2_CS_MASK << (cs + CMD2_CS_OFFS)) |
			       (CMD2_CH0_MASK << CMD2_CH0_OFFS));
	}
}

static void tx_validate_per_dbyte(u32 cs,
				  u32 max_bit_in_byte,
				  u32 iface_id)
{
	u32 dbyte;
	u32 subphys_num = DDR_INTERFACE_OCTETS_NUM;
	struct mv_ddr_topology_map *tm = mv_ddr_topology_map_get();
	u32 current_vref_and_range, current_vref, current_range;

	for (dbyte = 0; dbyte < subphys_num; dbyte++) {
		VALIDATE_BUS_ACTIVE(tm->bus_act_mask, dbyte);

		if (mv_ddr_is_ecc_ena() && (dbyte != 8))
			reg_bit_clrset(MC6_BASE + MC6_RAS_CTRL_REG, 0x0 << ECC_EN_OFFS, ECC_EN_MASK << ECC_EN_OFFS);

		printf("############# dbyte: %d ###################\n", dbyte);
		current_vref_and_range =
			snps_read(((cs == 0) ?
				  REG_54043_VREF_DQ_R0_BASE :
				  REG_5404D_VREF_DQ_R1_BASE) + dbyte);
		current_vref = current_vref_and_range & MR6_VREF_MASK;
		current_range = (current_vref_and_range >> MR6_VREF_RANGE_OFFS) & MR6_VREF_RANGE_MASK;

		tx_validate_per_bit(max_bit_in_byte,
				    dbyte,
				    cs,
				    current_vref,
				    current_range,
				    iface_id);

		if (mv_ddr_is_ecc_ena() && (dbyte != 8))
			reg_bit_clrset(MC6_BASE + MC6_RAS_CTRL_REG, 0x1 << ECC_EN_OFFS, ECC_EN_MASK << ECC_EN_OFFS);
	}
}

static void tx_validate_per_cs(u32 max_bit_in_byte, u32 iface_id)
{
	u32 cs;
	unsigned int max_cs = mv_ddr_cs_num_get();

	for (cs = 0; cs < max_cs; cs++) {
		printf("^^^^^^^^^^^^^^CS: %d ^^^^^^^^^^^^^^^^^^\n", cs);

		tx_validate_per_dbyte(cs,
				      max_bit_in_byte,
				      iface_id);
	}
}

int mv_ddr_validate(void)
{
	uint64_t curr_dst;
	uint64_t val;
	int i, j;
	u32 cs;
	struct mv_ddr_iface *iface = mv_ddr_iface_get();
	unsigned int max_cs = mv_ddr_cs_num_get();
	u32 max_bit_in_byte = 8;

	if (max_cs > DBG_MAX_CS_NUM) {
		printf("mv_ddr: error: DBG_MAX_CS_NUM limit (%d) reached\n",
			DBG_MAX_CS_NUM);
		return 1; /* fail */
	}

	for (cs = 0; cs < max_cs; cs++) {
		dma_offs_config(dma_src, dma_dst, cs, iface->id);
		for (j = 0; j < DBG_DMA_ENG_NUM; j++) {
			/* build dma data unit from dma pattern and write to dram */
			for (i = 0, curr_dst = dma_src[cs][j];
				i < DBG_DMA_DATA_SIZE / sizeof(dma_pattern[0]);
				i++, curr_dst += sizeof(dma_pattern[0])) {
				val = dma_pattern[i % (sizeof(dma_pattern) / sizeof(dma_pattern[0]))];
				writeq(curr_dst, val);
			}
		}

		/* copy dma data unit from src to dst */
		mv_ddr_dma_memcpy(dma_src[cs], dma_dst[cs],
				  DBG_DMA_DATA_SIZE, DBG_DMA_ENG_NUM, DBG_DMA_DESC_NUM);
		/* compare src to dst */
		if (mv_ddr_dma_memcmp(dma_src[cs], dma_dst[cs],
				      DBG_DMA_DATA_SIZE, DBG_DMA_ENG_NUM, DBG_DMA_DESC_NUM)){
			printf("dma memcmp fail\n");
#if defined(T9130)
			reg_write(0x6f0084, 0x0);
#endif
		} else {
			printf("dma memcmp pass\n");
		}
	}

	snps_csr_access_set(MICRO_CONT_MUX_SEL_ENABLE);

	snps_fw_write(REG_C0080_UCCLK_HCLK_EN,
		      ((CLK_EN & UCCLK_EN_MASK) << UCCLK_EN_OFFS) |
		      ((CLK_EN & HCLK_EN_MASK) << HCLK_EN_OFFS));

	if ((iface->validation == MV_DDR_VAL_RX) ||
	    (iface->validation == MV_DDR_VAL_RX_TX)) {
		printf("#######################################\n");
		printf("############# RX SWEEP ################\n");
		printf("#######################################\n");

		rx_validate_per_cs(max_bit_in_byte);
	}

	if ((iface->validation == MV_DDR_VAL_TX) ||
	    (iface->validation == MV_DDR_VAL_RX_TX)) {
		printf("#######################################\n");
		printf("############# TX SWEEP ################\n");
		printf("#######################################\n");

		reg_bit_clrset(MC6_BASE + MC6_CH0_DRAM_CFG3_REG,
			       (VREF_TRAIN_EN << VREF_TRAIN_OFFS) |
			       (DLL_RESET_VAL << DLL_RESET_OFFS),
			       (VREF_TRAIN_MASK << VREF_TRAIN_OFFS) |
			       (DLL_RESET_MASK << DLL_RESET_OFFS));

		tx_validate_per_cs(max_bit_in_byte, iface->id);

		reg_bit_clrset(MC6_BASE + MC6_CH0_DRAM_CFG3_REG,
			       (VREF_TRAIN_DIS << VREF_TRAIN_OFFS) |
			       (DLL_RESET_VAL << DLL_RESET_OFFS),
			       (VREF_TRAIN_MASK << VREF_TRAIN_OFFS) |
			       (DLL_RESET_MASK << DLL_RESET_OFFS));

		reg_bit_clrset(MC6_BASE + MC6_USER_CMD2_REG,
			       (CMD2_LMRG_EN << CMD2_LMR6_REQ_OFFS) |
			       (CMD2_CS_EN << (cs + CMD2_CS_OFFS)) |
			       (CMD2_CH0_EN << CMD2_CH0_OFFS),
			       (CMD2_LMR6_REQ_MASK << CMD2_LMR6_REQ_OFFS) |
			       (CMD2_CS_MASK << (cs + CMD2_CS_OFFS)) |
			       (CMD2_CH0_MASK << CMD2_CH0_OFFS));
	}

	return 0;
}
