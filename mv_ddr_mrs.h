/* Copyright (c) 2018 Marvell.
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _MV_DDR_MRS_H
#define _MV_DDR_MRS_H

/*
 * Based on Proposed DDR4 Full spec update (79-4B), Item No. 1716.78C
 */

/* MR1 DIC, [2:1] bits */
#define MV_DDR_MR1_DIC_OFFS		1

/* MR1 RTT_NOM, [10:8] bits */
#define MV_DDR_MR1_RTT_NOM_OFFS		8
#define MV_DDR_MR1_RTT_NOM_MASK		0x7
#define MV_DDR_MR1_RTT_NOM_DISABLE	0x0	/* 0b000_0000_0000 */
#define MV_DDR_MR1_RTT_NOM_RZQ_DIV4	0x100	/* 0b001_0000_0000; 60-Ohm */
#define MV_DDR_MR1_RTT_NOM_RZQ_DIV2	0x200	/* 0b010_0000_0000; 120-Ohm */
#define MV_DDR_MR1_RTT_NOM_RZQ_DIV6	0x300	/* 0b011_0000_0000; 40-Ohm */
#define MV_DDR_MR1_RTT_NOM_RZQ_DIV1	0x400	/* 0b100_0000_0000; 240-Ohm */
#define MV_DDR_MR1_RTT_NOM_RZQ_DIV5	0x500	/* 0b101_0000_0000; 48-Ohm */
#define MV_DDR_MR1_RTT_NOM_RZQ_DIV3	0x600	/* 0b110_0000_0000; 80-Ohm */
#define MV_DDR_MR1_RTT_NOM_RZQ_DIV7	0x700	/* 0b111_0000_0000; 34-Ohm */

/* MR2 RTT_WR, [11:9] bits */
#define MV_DDR_MR2_RTT_WR_OFFS		9
#define MV_DDR_MR2_RTT_WR_MASK		0x7
#define MV_DDR_MR2_RTT_WR_DYN_ODT_OFF	0x0	/* 0b0000_0000_0000 */
#define MV_DDR_MR2_RTT_WR_RZQ_DIV2	0x200	/* 0b0010_0000_0000; 120-Ohm */
#define MV_DDR_MR2_RTT_WR_RZQ_DIV1	0x400	/* 0b0100_0000_0000; 240-Ohm */
#define MV_DDR_MR2_RTT_WR_HIZ		0x600	/* 0b0110_0000_0000 */
#define MV_DDR_MR2_RTT_WR_RZQ_DIV3	0x800	/* 0b1000_0000_0000; 80-Ohm */

/* MR5 ODT Input Buffer during Power Down mode, bit 5 */
#define MV_DDR_MR5_PD_ODT_IBUF_OFFS	5
#define MV_DDR_MR5_PD_ODT_IBUF_MASK	0x1
#define MV_DDR_MR5_PD_ODT_IBUF_ENA	0
#define MV_DDR_MR5_PD_ODT_IBUF_DIS	1

/* MR5 Data Mask, bit 10 */
#define MV_DDR_MR5_DM_OFFS		10
#define MV_DDR_MR5_DM_MASK		0x1
#define MV_DDR_MR5_DM_ENA		1
#define MV_DDR_MR5_DM_DIS		0

/* MR5 RTT_PARK, [8:6] bits */
#define MV_DDR_MR5_RTT_PARK_OFFS	6
#define MV_DDR_MR5_RTT_PARK_MASK	0x7
#define MV_DDR_MR5_RTT_PARK_DISABLE	0x0	/* 0b0_0000_0000 */
#define MV_DDR_MR5_RTT_PARK_RZQ_DIV4	0x40	/* 0b0_0100_0000; 60-Ohm */
#define MV_DDR_MR5_RTT_PARK_RZQ_DIV2	0x80	/* 0b0_1000_0000; 120-Ohm */
#define MV_DDR_MR5_RTT_PARK_RZQ_DIV6	0xc0	/* 0b0_1100_0000; 40-Ohm */
#define MV_DDR_MR5_RTT_PARK_RZQ_DIV1	0x100	/* 0b1_0000_0000; 240-Ohm */
#define MV_DDR_MR5_RTT_PARK_RZQ_DIV5	0x140	/* 0b1_0100_0000; 48-Ohm */
#define MV_DDR_MR5_RTT_PARK_RZQ_DIV3	0x180	/* 0b1_1000_0000; 80-Ohm */
#define MV_DDR_MR5_RTT_PARK_RZQ_DIV7	0x1c0	/* 0b1_1100_0000; 34-Ohm */

/* MR6 VrefDQ Training Enable, bit 7 */
#define MV_DDR_MR6_VREFDQ_TRNG_ENA_OFFS	7
#define MV_DDR_MR6_VREFDQ_TRNG_ENA_MASK	0x1
#define MV_DDR_MR6_VREFDQ_TRNG_ENA	1	/* training mode */
#define MV_DDR_MR6_VREFDQ_TRNG_DIS	0	/* normal operation mode */

/* MR6 VrefDQ Training Range, bit 6 */
#define MV_DDR_MR6_VREFDQ_TRNG_RNG_OFFS	6
#define MV_DDR_MR6_VREFDQ_TRNG_RNG_MASK	0x1
#define MV_DDR_MR6_VREFDQ_TRNG_RNG1	0	/* range 1 */
#define MV_DDR_MR6_VREFDQ_TRNG_RNG2	1	/* range 2 */

/* MR6 VrefDQ Training Range Values, [5:0] bits */
#define MV_DDR_MR6_VREFDQ_TRNG_VAL_OFFS	0
#define MV_DDR_MR6_VREFDQ_TRNG_VAL_MASK	0x3f
#define MV_DDR_MR6_VREFDQ_TRNG_VAL	0x9	/* range 1: 65.85%; range 2: 50.85% */

int mv_ddr_mr0_wr_get(unsigned int wr, unsigned int *mr0_wr);
int mv_ddr_mr0_cl_get(unsigned int cl, unsigned int *mr0_cl);
int mv_ddr_mr2_cwl_get(unsigned int cwl, unsigned int *mr2_cwl);
int mv_ddr_mr6_tccdl_get(unsigned int tccdl, unsigned int *mr6_tccdl);

#endif /* _MV_DDR_MRS_H */
