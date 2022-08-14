/* Copyright (C) 2016 Marvell.
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _DDR3_TRAINING_IP_CENTRALIZATION_H
#define _DDR3_TRAINING_IP_CENTRALIZATION_H

int ddr3_tip_centralization_tx(u32 dev_num);
int ddr3_tip_centralization_rx(u32 dev_num);
int ddr3_tip_print_centralization_result(u32 dev_num);
int ddr3_tip_special_rx(u32 dev_num);

#endif /* _DDR3_TRAINING_IP_CENTRALIZATION_H */
