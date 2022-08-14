/* Copyright (c) 2017 Marvell.
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _SNPS_UPDATE_H_
#define _SNPS_UPDATE_H_
#include "snps_regs.h"

u16 sar_get_ddr_freq(void);
u16 snps_get_hdtctrl(void);
/* per platform function providing SNPS driver with non-default
 * configuration of address lines' PHY to IO mapping
 */
u32 *snps_ext_swizzle_cfg_get(void);
u16 init_phy_pllctrl2_get(void);
u16 init_phy_ardptrinitval_get(void);
u16 init_phy_procodttimectl_get(void);
u16 init_phy_caluclkinfo_get(void);
u16 init_phy_seq0bdly0_get(void);
u16 init_phy_seq0bdly1_get(void);
u16 init_phy_seq0bdly2_get(void);
u16 init_odt_ctrl_get(void);
u16 atx_impedance_ctrl_get(void);
u16 dmem_1d_2d_dram_freq_get(void);
u16 dmem_1d_2d_drv_imp_phy_odt_imp_get(void);
u16 dmem_1d_2d_cs_present_get(void);
u16 dmem_1d_2d_addr_mirror_get(void);
u16 dmem_1d_2d_mr0_get(void);	/* TODO get the mrs from data base */
u16 dmem_1d_2d_mr2_get(void);	/* TODO get the mrs from data base */
u16 dmem_1d_2d_mr5_get(void);	/* TODO get the mrs from data base */
u16 dmem_1d_2d_mr6_get(void);	/* TODO get the mrs from data base */
u16 dmem_1d_2d_rtt_nom_wr_park_get(void);
u16 dmem_1d_2d_en_dq_dis_dbyte_get(void);
u16 dmem_1d_2d_gear_down_x16_present_get(void);

struct snps_address_data init_phy_static_update[] = {
	{PHY_REG_ADDR_MAP(P_STATE_0, BLK_TYPE_MASTER, INST_NUM_0, REG_88_PHY_CAL_RATE),
	 CAL_RATE_VAL},
	{PHY_REG_ADDR_MAP(P_STATE_0, BLK_TYPE_MASTER, INST_NUM_0, REG_B2_VREF_IN_GLOBAL),
	 VREF_IN_GLOBAL_VAL},
	{REG_20019_TRISTATE_MODE_CA_TG0, REG_20019_TRISTATE_MODE_CA_VAL},
	{ -1	, -1  }
};

/*init phy dynamic update*/
struct snps_address_dynamic_update init_phy_dynamic_update[] = {
	{PHY_REG_ADDR_MAP(P_STATE_0, BLK_TYPE_DBYTE, INST_NUM_0, REG_X4D_PHY_TX_ODT_DRV_STREN(LOW_NIBL)),
	 init_odt_ctrl_get},
	{PHY_REG_ADDR_MAP(P_STATE_0, BLK_TYPE_DBYTE, INST_NUM_0, REG_X4D_PHY_TX_ODT_DRV_STREN(UP_NIBL)),
	 init_odt_ctrl_get},
	{PHY_REG_ADDR_MAP(P_STATE_0, BLK_TYPE_DBYTE, INST_NUM_1, REG_X4D_PHY_TX_ODT_DRV_STREN(LOW_NIBL)),
	 init_odt_ctrl_get},
	{PHY_REG_ADDR_MAP(P_STATE_0, BLK_TYPE_DBYTE, INST_NUM_1, REG_X4D_PHY_TX_ODT_DRV_STREN(UP_NIBL)),
	 init_odt_ctrl_get},
	{PHY_REG_ADDR_MAP(P_STATE_0, BLK_TYPE_DBYTE, INST_NUM_2, REG_X4D_PHY_TX_ODT_DRV_STREN(LOW_NIBL)),
	 init_odt_ctrl_get},
	{PHY_REG_ADDR_MAP(P_STATE_0, BLK_TYPE_DBYTE, INST_NUM_2, REG_X4D_PHY_TX_ODT_DRV_STREN(UP_NIBL)),
	 init_odt_ctrl_get},
	{PHY_REG_ADDR_MAP(P_STATE_0, BLK_TYPE_DBYTE, INST_NUM_3, REG_X4D_PHY_TX_ODT_DRV_STREN(LOW_NIBL)),
	 init_odt_ctrl_get},
	{PHY_REG_ADDR_MAP(P_STATE_0, BLK_TYPE_DBYTE, INST_NUM_3, REG_X4D_PHY_TX_ODT_DRV_STREN(UP_NIBL)),
	 init_odt_ctrl_get},
	{PHY_REG_ADDR_MAP(P_STATE_0, BLK_TYPE_DBYTE, INST_NUM_4, REG_X4D_PHY_TX_ODT_DRV_STREN(LOW_NIBL)),
	 init_odt_ctrl_get},
	{PHY_REG_ADDR_MAP(P_STATE_0, BLK_TYPE_DBYTE, INST_NUM_4, REG_X4D_PHY_TX_ODT_DRV_STREN(UP_NIBL)),
	 init_odt_ctrl_get},
	{PHY_REG_ADDR_MAP(P_STATE_0, BLK_TYPE_DBYTE, INST_NUM_5, REG_X4D_PHY_TX_ODT_DRV_STREN(LOW_NIBL)),
	 init_odt_ctrl_get},
	{PHY_REG_ADDR_MAP(P_STATE_0, BLK_TYPE_DBYTE, INST_NUM_5, REG_X4D_PHY_TX_ODT_DRV_STREN(UP_NIBL)),
	 init_odt_ctrl_get},
	{PHY_REG_ADDR_MAP(P_STATE_0, BLK_TYPE_DBYTE, INST_NUM_6, REG_X4D_PHY_TX_ODT_DRV_STREN(LOW_NIBL)),
	 init_odt_ctrl_get},
	{PHY_REG_ADDR_MAP(P_STATE_0, BLK_TYPE_DBYTE, INST_NUM_6, REG_X4D_PHY_TX_ODT_DRV_STREN(UP_NIBL)),
	 init_odt_ctrl_get},
	{PHY_REG_ADDR_MAP(P_STATE_0, BLK_TYPE_DBYTE, INST_NUM_7, REG_X4D_PHY_TX_ODT_DRV_STREN(LOW_NIBL)),
	 init_odt_ctrl_get},
	{PHY_REG_ADDR_MAP(P_STATE_0, BLK_TYPE_DBYTE, INST_NUM_7, REG_X4D_PHY_TX_ODT_DRV_STREN(UP_NIBL)),
	 init_odt_ctrl_get},
	{PHY_REG_ADDR_MAP(P_STATE_0, BLK_TYPE_DBYTE, INST_NUM_8, REG_X4D_PHY_TX_ODT_DRV_STREN(LOW_NIBL)),
	 init_odt_ctrl_get},
	{PHY_REG_ADDR_MAP(P_STATE_0, BLK_TYPE_DBYTE, INST_NUM_8, REG_X4D_PHY_TX_ODT_DRV_STREN(UP_NIBL)),
	 init_odt_ctrl_get},
	{PHY_REG_ADDR_MAP(P_STATE_0, BLK_TYPE_ANIB, INST_NUM_0, REG_43_ATX_IMPEDANCE),
	 atx_impedance_ctrl_get},
	{PHY_REG_ADDR_MAP(P_STATE_0, BLK_TYPE_ANIB, INST_NUM_1, REG_43_ATX_IMPEDANCE),
	 atx_impedance_ctrl_get},
	{PHY_REG_ADDR_MAP(P_STATE_0, BLK_TYPE_ANIB, INST_NUM_2, REG_43_ATX_IMPEDANCE),
	 atx_impedance_ctrl_get},
	{PHY_REG_ADDR_MAP(P_STATE_0, BLK_TYPE_ANIB, INST_NUM_3, REG_43_ATX_IMPEDANCE),
	 atx_impedance_ctrl_get},
	{PHY_REG_ADDR_MAP(P_STATE_0, BLK_TYPE_ANIB, INST_NUM_4, REG_43_ATX_IMPEDANCE),
	 atx_impedance_ctrl_get},
	{PHY_REG_ADDR_MAP(P_STATE_0, BLK_TYPE_ANIB, INST_NUM_5, REG_43_ATX_IMPEDANCE),
	 atx_impedance_ctrl_get},
	{PHY_REG_ADDR_MAP(P_STATE_0, BLK_TYPE_ANIB, INST_NUM_6, REG_43_ATX_IMPEDANCE),
	  atx_impedance_ctrl_get},
	{PHY_REG_ADDR_MAP(P_STATE_0, BLK_TYPE_ANIB, INST_NUM_7, REG_43_ATX_IMPEDANCE),
	 atx_impedance_ctrl_get},
	{PHY_REG_ADDR_MAP(P_STATE_0, BLK_TYPE_ANIB, INST_NUM_8, REG_43_ATX_IMPEDANCE),
	 atx_impedance_ctrl_get},
	{PHY_REG_ADDR_MAP(P_STATE_0, BLK_TYPE_ANIB, INST_NUM_9, REG_43_ATX_IMPEDANCE),
	 atx_impedance_ctrl_get},

	{REG_200C5_PHY_PLLCTRL2, init_phy_pllctrl2_get},
	{REG_2002E_PHY_ARDPTRINITVAL, init_phy_ardptrinitval_get},
	{REG_20056_PHY_PROCODTTIMECTL, init_phy_procodttimectl_get},
	{REG_20008_PHY_CALUCLKINFO, init_phy_caluclkinfo_get},
	{-1, NULL}
};

/* --------------------------- */
/* 1D static & dynamic updates */
/* --------------------------- */

/* 1D IMEM static update*/
struct snps_address_data one_d_imem_static_update[] = {
	{ -1	, -1  }
};

/* 1D DMEM static update*/
struct snps_address_data one_d_dmem_static_update[] = {
	{REG_54000_1D_2D_MSGMISC, (SIMULATION_DIS << BYTE_OFFSET) |
				   (TRAIN_VREFDAC0_DURING_1D_EN | ENABLE_HIGH_EFFORT_WRDQ1D_EN)},
	{REG_54006_1D_2D_DRAM_TYPE_PHY_VREF, (DRAM_TYPE_REG_VAL << BYTE_OFFSET) | PHY_VREF_REG_VAL},
	{REG_5400A_1D_2D_PHY_CFG_CSTEST_FAIL, PHY_CFG_REG_VAL << BYTE_OFFSET},
	{REG_5400C_1D_2D_RESV19_HDT_CTRL, HDT_CTRL_REG_VAL},
	{ -1	, -1 }
};

/* 1D DMEM dynamic update*/
struct snps_address_dynamic_update one_d_dmem_dynamic_update[] = {
	{REG_54003_1D_2D_DRAM_FREQ, dmem_1d_2d_dram_freq_get},
	{REG_54005_1D_2D_DRV_IMP_PHY_ODT_IMP, dmem_1d_2d_drv_imp_phy_odt_imp_get},
	{REG_54007_1D_2D_EN_DQ_DIS_DBYTE, dmem_1d_2d_en_dq_dis_dbyte_get},
	{REG_54008_1D_2D_CS_PRESENT, dmem_1d_2d_cs_present_get},
	{REG_54009_1D_2D_ADDR_MIRROR, dmem_1d_2d_addr_mirror_get},
	{REG_5402F_1D_2D_MR0, dmem_1d_2d_mr0_get},
	{REG_54031_1D_2D_MR2, dmem_1d_2d_mr2_get},
	{REG_54034_1D_2D_MR5, dmem_1d_2d_mr5_get},
	{REG_54035_1D_2D_MR6, dmem_1d_2d_mr6_get},
	{REG_54036_1D_2D_CS_SETUP_GDDEC_X16_PRESENT, dmem_1d_2d_gear_down_x16_present_get},
	{REG_54037_1D_2D_RTT_NOM_WR_PARK0, dmem_1d_2d_rtt_nom_wr_park_get},
	{REG_54038_1D_2D_RTT_NOM_WR_PARK1, dmem_1d_2d_rtt_nom_wr_park_get},
	{-1, NULL}
};


/* --------------------------- */
/* 2D static & dynamic updates */
/* --------------------------- */

/* 2D IMEM static update*/
struct snps_address_data two_d_imem_static_update[] = {
/*	{0x3333, 0xb},	*/
	{ -1	, -1  }
};

/* 2D DMEM static update*/
struct snps_address_data two_d_dmem_static_update[] = {
	{REG_54000_1D_2D_MSGMISC, (SIMULATION_DIS << BYTE_OFFSET) |
				   (TRAIN_VREFDAC0_DURING_1D_EN | ENABLE_HIGH_EFFORT_WRDQ1D_EN)},
	{REG_54006_1D_2D_DRAM_TYPE_PHY_VREF, (DRAM_TYPE_REG_VAL << BYTE_OFFSET) | PHY_VREF_REG_VAL},
	{REG_5400A_1D_2D_PHY_CFG_CSTEST_FAIL, PHY_CFG_REG_VAL << BYTE_OFFSET},
	{REG_5400C_1D_2D_RESV19_HDT_CTRL, HDT_CTRL_REG_VAL | RX2D_TRAIN_OPT_REG_VAL},
	{REG_5400D_1D_2D_RSRV1B_RSRV1A, (RSRV1A_VAL << BYTE_OFFSET) | RSRV1B_VAL},
	{ -1	, -1 }
};

/* 2D DMEM dynamic update*/
struct snps_address_dynamic_update two_d_dmem_dynamic_update[] = {
	{REG_54003_1D_2D_DRAM_FREQ, dmem_1d_2d_dram_freq_get},
	{REG_54005_1D_2D_DRV_IMP_PHY_ODT_IMP, dmem_1d_2d_drv_imp_phy_odt_imp_get},
	{REG_54007_1D_2D_EN_DQ_DIS_DBYTE, dmem_1d_2d_en_dq_dis_dbyte_get},
	{REG_54008_1D_2D_CS_PRESENT, dmem_1d_2d_cs_present_get},
	{REG_54009_1D_2D_ADDR_MIRROR, dmem_1d_2d_addr_mirror_get},
	{REG_5402F_1D_2D_MR0, dmem_1d_2d_mr0_get},
	{REG_54031_1D_2D_MR2, dmem_1d_2d_mr2_get},
	{REG_54034_1D_2D_MR5, dmem_1d_2d_mr5_get},
	{REG_54035_1D_2D_MR6, dmem_1d_2d_mr6_get},
	{REG_54036_1D_2D_CS_SETUP_GDDEC_X16_PRESENT, dmem_1d_2d_gear_down_x16_present_get},
	{REG_54037_1D_2D_RTT_NOM_WR_PARK0, dmem_1d_2d_rtt_nom_wr_park_get},
	{REG_54038_1D_2D_RTT_NOM_WR_PARK1, dmem_1d_2d_rtt_nom_wr_park_get},
	{ -1	, NULL }
};


/* --------------------------- */
/* pie static & dynamic updates */
/* --------------------------- */
/* 2D DMEM dynamic update*/
struct snps_address_dynamic_update pie_dynamic_update[] = {
	{REG_2000B_PHY_SEQ0BDLY0, init_phy_seq0bdly0_get},
	{REG_2000C_PHY_SEQ0BDLY1, init_phy_seq0bdly1_get},
	{REG_2000D_PHY_SEQ0BDLY2, init_phy_seq0bdly2_get},
	{ -1	, NULL }
};

#endif	/* _SNPS_UPDATE_H_ */
