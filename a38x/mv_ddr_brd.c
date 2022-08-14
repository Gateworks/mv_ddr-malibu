/* Copyright (C) 2016 Marvell.
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "../ddr3_init.h"

/*
 * Define the DDR layout / topology here in the board file. This will
 * be used by the DDR3 init code in the SPL U-Boot version to configure
 * the DDR3 controller.
 */

#if defined(CONFIG_DDR4)
#define SPEED_BIN_DDR_DB_68XX	SPEED_BIN_DDR_2400R
#define BUS_WIDTH_DB_68XX	MV_DDR_DEV_WIDTH_16BIT
#else /* CONFIG_DDR4 */
#define SPEED_BIN_DDR_DB_68XX	SPEED_BIN_DDR_1866L
#define BUS_WIDTH_DB_68XX	MV_DDR_DEV_WIDTH_8BIT
#endif /* CONFIG_DDR4 */

#if 0
/* Marvell board - Board_ID = DB_68XX_ID = 1 (DDR3/4)*/
static struct mv_ddr_topology_map board_topology_map = {
	DEBUG_LEVEL_ERROR,
	0x1, /* active interfaces */
	/* cs_mask, mirror, dqs_swap, ck_swap X PUPs */
	{ { { {0x3, 0x2, 0, 0},
	      {0x3, 0x2, 0, 0},
	      {0x3, 0x2, 0, 0},
	      {0x3, 0x2, 0, 0},
	      {0x3, 0x2, 0, 0} },
	    SPEED_BIN_DDR_DB_68XX,	/* speed_bin */
	    BUS_WIDTH_DB_68XX,		/* sdram device width */
	    MV_DDR_DIE_CAP_4GBIT,	/* die capacity */
	    MV_DDR_FREQ_SAR,		/* frequency */
	    0, 0,			/* cas_l, cas_wl */
	    MV_DDR_TEMP_LOW} },		/* temperature */
	BUS_MASK_32BIT,			/* subphys mask */
	MV_DDR_CFG_DEFAULT,		/* ddr configuration data source */
	{ {0} },			/* raw spd data */
	{0}				/* timing parameters */
};
#else
/* Marvell board - Board_ID = DB_GP_68XX_ID = 4 */
static struct mv_ddr_topology_map board_topology_map = {
	DEBUG_LEVEL_ERROR,
	0x1, /* active interfaces */
	/* cs_mask, mirror, dqs_swap, ck_swap X PUPs */
	{ { { {0x1, 0, 0, 0},
	      {0x1, 0, 0, 0},
	      {0x1, 0, 0, 0},
	      {0x1, 0, 0, 0},
	      {0x1, 0, 0, 0} },
	    SPEED_BIN_DDR_DB_68XX,	/* speed_bin */
	    BUS_WIDTH_DB_68XX,		/* sdram device width */
	    MV_DDR_DIE_CAP_4GBIT,	/* die capacity */
	    MV_DDR_FREQ_SAR,		/* frequency */
	    0, 0,			/* cas_l cas_wl */
	    MV_DDR_TEMP_LOW} },		/* temperature */
	BUS_MASK_32BIT,			/* subphys mask */
	MV_DDR_CFG_DEFAULT,		/* ddr configuration data source */
	NOT_COMBINED,			/* ddr twin-die combined*/
	{ {0} },			/* raw spd data */
	{0}				/* timing parameters */
};
#endif

struct mv_ddr_topology_map *mv_ddr_topology_map_get(void)
{
	/* Return the board topology as defined in the board code */
	return &board_topology_map;
}

