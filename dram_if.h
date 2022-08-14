/* Copyright (C) 2016 Marvell.
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _DRAM_IF_H_
#define _DRAM_IF_H_

/* TODO: update atf to this new prototype */
int dram_init(void);
void dram_mmap_config(void);
unsigned long long dram_iface_mem_sz_get(void);
#endif /* _DRAM_IF_H_ */
