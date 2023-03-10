/* Copyright (c) 2017 Marvell.
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _SNPS_H_
#define _SNPS_H_

#include "mv_ddr_atf_wrapper.h"
/*#define SNPS_DEBUG*/
#if defined(SNPS_DEBUG)
#define pr_debug(args...)	printf(args)
#define debug_enter()	printf("----> Enter %s\n", __func__);
#define debug_exit()	printf("<---- Exit  %s\n", __func__);
#else
#define debug_enter()
#define debug_exit()
#define pr_debug(args...)
#endif

/*#define SNPS_DEBUG_MSG*/
#if defined(SNPS_DEBUG_MSG) || defined(SNPS_DEBUG)
#define pr_debug_msg(args...)	printf(args)
#else
#define pr_debug_msg(args...)
#endif


/* Dynamic update struct:
 * Dynamic initialization sequence is done with these adress & get_value API pairs:
 * CPU runs over these entries and sequentially calls get_update_value to derive
 * the variables required value for the current state,
 * and eventually writes address = get_update_value() */
struct snps_address_dynamic_update {
	u32 addr;
	u16 (*get_update_value)(void);
};

/* initialization flow is bisected to 3 stages:
 * 1. Static initialization: with Synopsys delivery input
 * 2. Static update: with fixed values for certain variables
 * 3. Dynamic update: with dynamic values for certain variable
 *    (topology Or run-time dependant)
 */
enum snps_section_load_state {
	LOAD_STATIC = 0,
	LOAD_STATIC_UPDATE,
	LOAD_DYNAMIC_UPDATE,
};

/* Section IDs (training procedure is represented by it's functional sections) */
enum snps_section_id {
	SECTION_PHY_CONFIG = 0,
	SECTION_1D_IMEM,
	SECTION_1D_DMEM,
	SECTION_2D_IMEM,
	SECTION_2D_DMEM,
	SECTION_PHY_INIT_ENGINE
};

enum snps_training_state {
	PHY_CONFIG = 0,
	TRAINING_1D,
	TRAINING_2D,
	PHY_INIT_ENGINE,
};

/* snps_static_update: set as yes if static section include static update state */
enum snps_static_update {
	STATIC_UPDATE_YES = 0,
	STATIC_UPDATE_NO
};

/* snps_training_method: select if training procedure will be done sequentially
 * or step by step (Step by step allows more thorough debug & more precise
 * result evaluation capabilities
 */
enum snps_training_method {
	SEQUENCE_STEP_BY_STEP = 0,
	SEQUENCE_FULL,
};

enum snps_training_result {
	TRAINING_SEQUENCE_PASS = 0,
	TRAINING_SEQUENCE_RETRY,
	TRAINING_SEQUENCE_FAIL,
};

enum snps_load_source_type {
	LOAD_SEQUENTIAL,
	LOAD_RANDOM,
};

/* struct to associate section ID and it's name
 * (used for more informative prints to user
 */
struct snps_section_name {
	enum snps_section_id section_id;
	char *section_name;
};

/* snps_section_content will specify each section's content and it's location.
 * each struct must include static initialization struct,
 * but static_update and dynamic_update parts are optional only
 */
struct snps_section_content {
	enum snps_section_id		section_id;
	enum snps_load_source_type  load_type;
	union {
		const struct snps_address_data	*random;
		const struct snps_seq_data *sequential;
	} load_static;
	const struct snps_address_data	*load_static_update;
	const struct snps_address_dynamic_update	*load_dynamic_update;
};


/* ************ Message Block definitions ************ */
#define SNPS_MAX_RUN_COUNT 10
/* NOTE:
 * This enum below must be aligned with message block field's definition
 * (defined in snps_msg_block_result msg_blk_data)
 */
enum snps_msg_block_result_id {
	MSG_BLK_PMU_REV = 0,
	MSG_BLK_CS_TEST_FAIL,
	MSG_BLK_1D_CDD_RW_1_0,
	MSG_BLK_1D_CDD_RW_0_3,
	MSG_BLK_2D_R0_RX_CLK_DELAY_MARGIN,
	MSG_BLK_MAX_ENTRY
};

/* snps_msg_block_result describes each message block and training results fields
 * with this info CPU will read all required results and log them accordingly */
struct snps_msg_block_result {
	enum snps_msg_block_result_id result_id;
	int addr;
	u16 mask;
	u16 offset;
};

/* snps_msg_block_result_log logs all required training results for each
 * performed training step
 */
struct snps_msg_block_result_log {
	int run_number;
	/* TODO: add current state: PHY INIT/ 1D/ 2D/ PIE */
	int sequence_flags;
	u16 msg_blk[MSG_BLK_MAX_ENTRY];
};

/* ************ Mail Box definitions ************ */
/* TODO: update maximum supported mailbox according to actual message count */
#define MAILBOX_MSG_MAX_COUNT	1000
/* Maximum supported arguments is 32, so we'll take some buffer */
#define MAILBOX_MSG_ARGUMENT_MAX_COUNT 36

/* snps_mailbox_msg_type uses unique values to distinct between 'Major' and
 * 'Stream' type of messages.
 * enum 'MB_MSG_TYPE_MAJOR' below is used to mark 'Major' type of messages in
 * argument array of messages log.
 * This way, when printing the messages after we finished logging them in
 * messages array, we know how to distinct between 'major' and 'stream' types.
 */
enum snps_mailbox_msg_type {
	MB_MSG_TYPE_STREAM = 0xfffe,
	MB_MSG_TYPE_MAJOR
};

/* TODO: update retry count & polling delay according to actual requirements */
#define MAIL_BOX_ERROR		-100
#define MAILBOX_RETRY_COUNT		20000
#define MAILBOX_POLLING_DELAY	500	/* 300ms between reads */

/* mail_box_major_message: describe 'Major' type of messages with it's ID and string content */
struct mail_box_major_message {
	u16 message_id;
	char *string;
};

/* mail_box_stream_message: describe 'Stream' type of messages with it's ID and string & argument array content */
struct mail_box_stream_message {
	u16 message_id;
	u16 argument_count;
	u16 log_level;
	/* TODO: explain why multiplying arg_count by 2 and adding 1 */
	const char *string[(MAILBOX_MSG_ARGUMENT_MAX_COUNT * 2) + 1];
};

enum snps_major_msg_id {
	MB_MAJOR_ID_END_INIT = 0,
	MB_MAJOR_ID_FINE_WR_LVL,
	MB_MAJOR_ID_READ_EN_TRANING,
	MB_MAJOR_ID_READ_DELAY_CNTR_OPT,
	MB_MAJOR_ID_WRITE_DELAY_CNTR_OPT,
	MB_MAJOR_ID_2D_WRITE_DELAY_CNTR_OPT,	/* read delay/voltage center */
	MB_MAJOR_ID_2D_READ_DELAY_CNTR_OPT,	/* write delay /voltage center */
	MB_MAJOR_ID_TRAINING_SUCCESS,
	MB_MAJOR_ID_STREAM_MSG,
	MB_MAJOR_ID_MAX_READ_LATENCY,
	MB_MAJOR_ID_MAX_READ_DQ_DESKEW,
	MB_MAJOR_ID_RESERVED,
	MB_MAJOR_ID_LRDIMM_SPECIFIC_TRAINING,
	MB_MAJOR_ID_CA_TRAINING,
	MB_MAJOR_ID_MPR_READ_DELAY_CENTER_OPT = 0xfd,
	MB_MAJOR_ID_WRITE_LVL_COARSE_DELAY = 0xfd,
	MB_MAJOR_ID_TRAINING_FAILED = 0xff
};

enum snps_log_level {
	LOG_LEVEL_10 = 0,
	LOG_LEVEL_20,
	LOG_LEVEL_30,
	LOG_LEVEL_40,
	LOG_LEVEL_50
};
/*************************************************/

/* global data, settings, mail box, message block, etc..) */
struct snps_global_data {
	unsigned long  base_address;	/* SNPS unit Base address */
	enum snps_training_state training_state;

	/* Mail Box */
	int messages[MAILBOX_MSG_MAX_COUNT][MAILBOX_MSG_ARGUMENT_MAX_COUNT];
	int message_log_index;

	/* Message Block & training results data */
	struct snps_msg_block_result_log results[SNPS_MAX_RUN_COUNT];
	int current_run_num;
};

extern struct snps_global_data gd;


extern int static_section_completed;
#define SNPS_LAST_RUN	(gd.current_run_num - 1)
/* this macro writes 'data' to specified 'address' offset,
 * without support for debug prints, since static sections are too large to be printed */
#define SNPS_STATIC_WRITE(offset, data)	mmio_write_16((uint64_t)(gd.base_address + (2 * offset)), (uint16_t)data)

int snps_message_block_read(int sequence_flags);
int snps_get_result(enum snps_msg_block_result_id result_id, int run_number, int sequence_ctrl);
void snps_fw_write(u32 addr, u16 data);
u16 snps_read(u32 addr);
void snps_csr_access_set(u16 enable);
void snps_sequence_ctrl_set(int sequence_flags);
u16 snps_sequence_ctrl_get(void);
void snps_trigger_pmu_training(void);
int snps_mail_box_log_init(void);
int snps_mail_box_read(void);
void snps_mail_box_log_dump(void);
int snps_poll_calibration_completion(void);
enum snps_training_state snps_get_state(void);
void snps_set_state(enum snps_training_state training_state);
void snps_crx_1d_fix(void);
#endif	/* _SNPS_H_ */
