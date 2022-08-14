/* Copyright (c) 2019 Marvell.
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _SNPS_STATIC_H_
#define _SNPS_STATIC_H_

/* Synopsys DDR43 PHY FIRMWARE:Version A-2017.11 */
#define SNPS_DDR_PHY_FW_VERSION	"A-2017.11"

/* General definitions to make header standalone */
typedef unsigned short u16;

struct snps_seq_data {
	int start_addr;
	 int end_addr;
	int data_count;
	const u16 *data;
};

/* Address-Data pair struct:
 * Static initialization sequence is done with these address-data pairs:
 * CPU runs over these entries and sequentially write address = data */
struct snps_address_data {
	int addr;
	u16 data;
};

/* init_phy static section */
extern struct snps_address_data init_phy_static[];

/* pie static section */
extern struct snps_address_data pie_static[];
extern const struct snps_seq_data one_d_imem_static;
extern const struct snps_seq_data one_d_dmem_static;
extern const struct snps_seq_data two_d_imem_static;
extern const struct snps_seq_data two_d_dmem_static;
#endif	/* _SNPS_STATIC_H_ */
