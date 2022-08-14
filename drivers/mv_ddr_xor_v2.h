/* Copyright (C) 2016 Marvell.
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _MV_DDR_XOR_V2_H
#define _MV_DDR_XOR_V2_H

/**
 * set memory region to data using dma
 *
 * @param    start_addr	start address of memory region
 * @param    size	size of memory region
 * @param    data	data to be written
 *
 * @retval 0 success
 */
int mv_ddr_dma_memset(uint64_t start_addr, uint64_t size, uint64_t data);

/**
 * copy memory region from source to destination using a few dma engines
 *
 * @param    src	start addresses of source memory regions per dma
 * @param    dst	start addresses of destination memory regions per dma
 * @param    size	size of memory region to copy
 * @param    dma_num	number of dma engines to use
 * @param    desc_num	number of descriptors per dma engine to use
 *
 * @retval 0 success
 */
int mv_ddr_dma_memcpy(uint64_t *src, uint64_t *dst, uint64_t size, u32 dma_num, u32 desc_num);

/**
 * compare memory regions at source and destination using a few dma engines
 *
 * @param    src	start addresses of source memory regions per dma
 * @param    dst	start addresses of destination memory regions per dma
 * @param    size	size of memory region to compare
 * @param    dma_num	number of dma engines to use
 * @param    desc_num	number of descriptors per dma engine to use
 *
 * @retval   fail_cnt	number of comparison failures or -1 on func's fail
 */
int mv_ddr_dma_memcmp(uint64_t *src, uint64_t *dst, uint64_t size, u32 dma_num, u32 desc_num);

#endif /* _MV_DDR_XOR_V2_H */
