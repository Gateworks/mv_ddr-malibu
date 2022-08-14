# Copyright (C) 2016 Marvell.
# SPDX-License-Identifier: BSD-3-Clause

# set mv_ddr directory
ifdef CONFIG_SPL_BUILD
MV_DDR_ROOT = $(src)
else
MV_DDR_ROOT = .
endif

# set mv_ddr build message and version string source file
MV_DDR_VER_CSRC = mv_ddr_build_message.c

# create mv_ddr build message and version string source file
$(shell $(MV_DDR_ROOT)/scripts/localversion.sh $(MV_DDR_ROOT) $(MV_DDR_VER_CSRC) 2> /dev/null)

# ******************
# U-BOOT SPL SUPPORT
# ******************
ifdef CONFIG_SPL_BUILD
obj-$(CONFIG_SPL_BUILD) += a38x/mv_ddr_plat.o
obj-$(CONFIG_SPL_BUILD) += a38x/mv_ddr_brd.o
obj-$(CONFIG_SPL_BUILD) += a38x/mv_ddr_static.o
obj-$(CONFIG_SPL_BUILD) += a38x/mv_ddr_sys_env_lib.o
obj-$(CONFIG_SPL_BUILD) += ddr_init.o
obj-$(CONFIG_SPL_BUILD) += ddr3_debug.o
obj-$(CONFIG_SPL_BUILD) += ddr3_init.o
obj-$(CONFIG_SPL_BUILD) += ddr3_training.o
obj-$(CONFIG_SPL_BUILD) += ddr3_training_bist.o
obj-$(CONFIG_SPL_BUILD) += ddr3_training_centralization.o
obj-$(CONFIG_SPL_BUILD) += ddr3_training_db.o
obj-$(CONFIG_SPL_BUILD) += ddr3_training_hw_algo.o
obj-$(CONFIG_SPL_BUILD) += ddr3_training_ip_engine.o
obj-$(CONFIG_SPL_BUILD) += ddr3_training_leveling.o
obj-$(CONFIG_SPL_BUILD) += ddr3_training_pbs.o
obj-$(CONFIG_SPL_BUILD) += mv_ddr_build_message.o
obj-$(CONFIG_SPL_BUILD) += mv_ddr_common.o
obj-$(CONFIG_SPL_BUILD) += mv_ddr_spd.o
obj-$(CONFIG_SPL_BUILD) += mv_ddr_topology.o
obj-$(CONFIG_SPL_BUILD) += mv_ddr4_mpr_pda_if.o
obj-$(CONFIG_SPL_BUILD) += mv_ddr4_training.o
obj-$(CONFIG_SPL_BUILD) += mv_ddr4_training_calibration.o
obj-$(CONFIG_SPL_BUILD) += mv_ddr4_training_db.o
obj-$(CONFIG_SPL_BUILD) += mv_ddr4_training_leveling.o
obj-$(CONFIG_SPL_BUILD) += xor.o

# ******************************
# TOOLS SUPPORT
# ******************************
else ifneq ($(TOOL),)

CC       = gcc
RM       = @rm -rf
ECHO     = @echo

TOOL_PATH = $(MV_DDR_ROOT)/tools/$(TOOL)
TOOL_CSRC = $(foreach DIR,$(TOOL_PATH),$(wildcard $(DIR)/*.c))
TOOL_COBJ = $(patsubst %.c,%.o,$(TOOL_CSRC))

.NOTPARALLEL:
.SILENT:
all: header $(TOOL)

$(TOOL_PATH)/%.o: %.c
	$(CC) -c -o $@ $<

$(TOOL): $(TOOL_COBJ)
	$(CC) -o $@ $(TOOL_COBJ)

header:
	echo "Building mv_ddr tool: $(TOOL)"

clean:
	$(ECHO) "Cleaning mv_ddr tool: $(TOOL)"
	$(RM) $(TOOL_COBJ) $(TOOL)

# ******************************
# U-BOOT MARVELL 2013.01 SUPPORT
# ******************************
else ifeq ($(DDR3LIB), 3)
BH_PATH ?= $(MV_DDR_ROOT)/../..
include $(BH_PATH)/base.mk

OBJ_DIR = $(BH_PATH)/src_ddr/lib
MV_DDR_SRCPATH = $(MV_DDR_ROOT) $(MV_DDR_ROOT)/a38x
INCPATH = $(MV_DDR_SRCPATH)
INCLUDE = $(addprefix -I,$(INCPATH))
INCLUDE += -I$(BH_PATH)/inc/common

MV_DDR_LIBNAME = $(MV_DDR_ROOT)/$(DDRTYPE)_training_$(LIBNAME).lib
MV_DDR_LIB = $(OBJ_DIR)/$(MV_DDR_LIBNAME)

MV_DDR_CSRC += ddr3_debug.c
MV_DDR_CSRC += ddr3_training_bist.c
MV_DDR_CSRC += mv_ddr_build_message.c
MV_DDR_CSRC += ddr3_training_leveling.c
MV_DDR_CSRC += ddr3_training_db.c
MV_DDR_CSRC += mv_ddr4_training_leveling.c
MV_DDR_CSRC += mv_ddr_common.c
MV_DDR_CSRC += ddr3_init.c
MV_DDR_CSRC += ddr3_training.c
MV_DDR_CSRC += mv_ddr_spd.c
MV_DDR_CSRC += ddr_init.c
MV_DDR_CSRC += mv_ddr_topology.c
MV_DDR_CSRC += ddr3_training_ip_engine.c
MV_DDR_CSRC += ddr3_training_centralization.c
MV_DDR_CSRC += mv_ddr4_training_calibration.c
MV_DDR_CSRC += mv_ddr4_training_db.c
MV_DDR_CSRC += ddr3_training_hw_algo.c
MV_DDR_CSRC += xor.c
MV_DDR_CSRC += mv_ddr4_training.c
MV_DDR_CSRC += ddr3_training_pbs.c
MV_DDR_CSRC += mv_ddr4_mpr_pda_if.c
MV_DDR_CSRC += a38x/mv_ddr_brd.c
MV_DDR_CSRC += a38x/mv_ddr_static.c
MV_DDR_CSRC += a38x/mv_ddr_plat.c
MV_DDR_CSRC += a38x/mv_ddr_sys_env_lib.c

MV_DDR_COBJ = $(patsubst %.c,$(OBJ_DIR)/%.o,$(MV_DDR_CSRC))
# add mv_ddr build message and version string object
MV_DDR_VER_COBJ = $(patsubst %.c,$(OBJ_DIR)/%.o,$(MV_DDR_VER_CSRC))
MV_DDR_COBJ += $(MV_DDR_VER_COBJ)

.SILENT:
all: header create_dir $(MV_DDR_LIB)

# mv_ddr code compilation
$(OBJ_DIR)/%.o: %.c
	$(ECHO) "  CC      $<"
	$(CC) $(INCLUDE) $(CFLAGS) $(CPPFLAGS) -c -o $@ $<

$(MV_DDR_LIB): $(MV_DDR_COBJ)
	$(ECHO) "  AR      $(MV_DDR_LIBNAME)"
	$(AR) rcs $(MV_DDR_LIB) $(MV_DDR_COBJ)

$(MV_DDR_VER_COBJ):
	$(ECHO) "  CC      $(MV_DDR_VER_CSRC)"
	$(CC) -c $(CFLAGS) -o $@ $(MV_DDR_VER_CSRC)

create_dir:
	$(MKDIR) $(OBJ_DIR)/a38x

header:
	$(ECHO) "\nBuilding DRAM driver"

clean:
	$(ECHO) "  CLEAN"
	@$(RM) $(MV_DDR_COBJ) $(MV_DDR_LIB)

# *********************
# MARVELL A3700 SUPPORT
# *********************
else ifneq ($(findstring a3700,$(PLATFORM)),)
CC       = gcc
RM       = @rm -rf
ECHO     = @echo

OBJ_DIR ?= $(MV_DDR_ROOT)

MV_DDR_SRCPATH = $(MV_DDR_ROOT)/a3700
MV_DDR_DRVPATH = $(MV_DDR_ROOT)/drivers

INCPATH = $(MV_DDR_ROOT) $(MV_DDR_SRCPATH) $(MV_DDR_DRVPATH)
INCLUDE = $(addprefix -I,$(INCPATH))

CFLAGS = $(INCLUDE) -Wall -Werror

ifneq ($(findstring DDR3,$(DDR_TYPE)),)
CFLAGS += -DCONFIG_DDR3
else ifneq ($(findstring DDR4,$(DDR_TYPE)),)
CFLAGS += -DCONFIG_DDR4
else
$(error set DDR_TYPE to DDR3 or DDR4)
endif
$(shell test "`cat a3700_ddr_type 2>/dev/null`" = "$(DDR_TYPE)" || echo $(DDR_TYPE) > a3700_ddr_type)

CFLAGS += -DCONFIG_A3700 -DSILENT_LIB

MV_DDR_CSRC = $(foreach DIR,$(MV_DDR_SRCPATH),$(wildcard $(DIR)/*.c))
MV_DDR_CSRC += $(MV_DDR_DRVPATH)/mv_ddr_mc6.c
MV_DDR_CSRC += $(MV_DDR_ROOT)/mv_ddr4_training_db.c
MV_DDR_CSRC += $(MV_DDR_ROOT)/ddr3_training_db.c
MV_DDR_CSRC += $(MV_DDR_ROOT)/mv_ddr_common.c

MV_DDR_COBJ = $(patsubst %.c,$(OBJ_DIR)/%.o,$(MV_DDR_CSRC))
# add mv_ddr build message and version string object
MV_DDR_VER_COBJ = $(patsubst %.c,$(OBJ_DIR)/%.o,$(MV_DDR_VER_CSRC))
MV_DDR_COBJ += $(MV_DDR_VER_COBJ)

.SILENT:
all: check_env header a3700_tool

$(OBJ_DIR)/%.o: %.c a3700_ddr_type
	$(ECHO) "  CC      $<"
	$(CC) -c $(CFLAGS) -o $@ $<

a3700_tool: $(MV_DDR_COBJ)
	$(CC) -o a3700_tool $(MV_DDR_COBJ)

$(MV_DDR_VER_COBJ): a3700_ddr_type
	$(ECHO) "  CC      $(MV_DDR_VER_CSRC)"
	$(CC) -c $(CFLAGS) -o $@ $(MV_DDR_VER_CSRC)

header:
	$(ECHO) "\nBuilding a3700 tool"

check_env:
ifndef DDR_TYPE
	$(error DDR_TYPE is undefined; set DDR_TYPE to DDR3 or DDR4)
endif

clean:
	$(ECHO) "Cleaning a3700 tool"
	@$(RM) $(MV_DDR_COBJ) a3700_tool a3700_ddr_type

# *******************
# MARVELL ATF SUPPORT
# *******************
else
CROSS    = $(CROSS_COMPILE)

LD       = $(CROSS)ld
CC       = $(CROSS)gcc
AS       = $(CROSS)gcc
AR       = $(CROSS)ar
OBJCOPY  = $(CROSS)objcopy
OBJDUMP  = $(CROSS)objdump
STRIP    = $(CROSS)strip

RM       = @rm -rf
MKDIR    = @mkdir -p
CD       = @cd
MV       = @mv
CP       = @cp
CAT      = @cat
PWD      = @pwd
ECHO     = @echo

# OBJ_DIR set in ble/ble.mk
OBJ_DIR ?= $(MV_DDR_ROOT)

CFLAGS = -DMV_DDR_ATF -DCONFIG_DDR4
CFLAGS += -Wall -Werror -Os -ffreestanding -mlittle-endian -g -gdwarf-2 -nostdinc
CFLAGS += -march=armv8-a -fpie

# PLATFORM is set in ble/ble.mk
ifneq ($(findstring a80x0,$(PLATFORM)),)
ifneq ($(findstring ocp,$(PLATFORM)),)
CFLAGS += -DNO_DM_TUNING
endif
CFLAGS += -DA80X0
MV_DDR_PLAT = apn806
MV_DDR_TIP = y
MV_DDR_FLOW = y
ifeq ($(findstring 32bit_ddr,$(PLATFORM)),)
ifndef DDR32
CFLAGS += -DCONFIG_64BIT
endif
endif
endif
ifneq ($(findstring a70x0,$(PLATFORM)),)
CFLAGS += -DA70X0
MV_DDR_PLAT = apn806
MV_DDR_TIP = y
MV_DDR_FLOW = y
endif
ifneq ($(findstring a7040,$(PLATFORM)),)
CFLAGS += -DA70X0
CFLAGS += -DNO_DM_TUNING
MV_DDR_PLAT = apn806
MV_DDR_TIP = y
MV_DDR_FLOW = y
endif
ifneq ($(findstring a3900,$(PLATFORM)),)
CFLAGS += -DCONFIG_64BIT -DCONFIG_MC6P -DAPN807
MV_DDR_PLAT = apn807
MV_DDR_SNPS = y
MV_DDR_FLOW_V2 = y
endif

ifneq ($(findstring t9130, $(PLATFORM)),)
CFLAGS += -DCONFIG_64BIT -DCONFIG_MC6P -DAPN807
CFLAGS += -DT9130
MV_DDR_PLAT = apn807
MV_DDR_SNPS = y
MV_DDR_FLOW_V2 = y
endif

ifneq ($(findstring a8xx,$(PLATFORM)),)
CFLAGS += -DCONFIG_MC6P
MV_DDR_PLAT = apn810
MV_DDR_SNPS = y
MV_DDR_FLOW_V2 = y
endif

ifneq ($(ARCH),)
CFLAGS += -D$(ARCH)
endif

MV_DDR_LIBNAME = mv_ddr_lib.a
MV_DDR_LIB = $(OBJ_DIR)/$(MV_DDR_LIBNAME)

MV_DDR_SRCPATH = $(MV_DDR_ROOT)
MV_DDR_PLATPATH = $(MV_DDR_ROOT)/$(MV_DDR_PLAT)
MV_DDR_DRVPATH = $(MV_DDR_ROOT)/drivers

INCPATH = $(MV_DDR_SRCPATH) $(MV_DDR_PLATPATH) $(MV_DDR_DRVPATH)
ifeq ($(MV_DDR_SNPS),y)
MV_DDR_SNPSPATH = $(MV_DDR_DRVPATH)/snps
INCPATH += $(MV_DDR_SNPSPATH)
INCPATH += $(MV_DDR_SNPSPATH)/fw
endif
INCLUDE = $(addprefix -I,$(INCPATH))
# PLAT_INCLUDES set in ble/ble.mk
INCLUDE += $(PLAT_INCLUDES)

CFLAGS += $(INCLUDE)
#CFLAGS += -DCONFIG_MC_STATIC
#CFLAGS += -DCONFIG_MC_STATIC_PRINT
#CFLAGS += -DCONFIG_PHY_STATIC
#CFLAGS += -DCONFIG_PHY_STATIC_PRINT

LDFLAGS = -Xlinker --discard-all -Wl,--build-id=none -static -nostartfiles

MV_DDR_CSRC = $(foreach DIR,$(MV_DDR_PLATPATH),$(wildcard $(DIR)/*.c))
ifeq ($(MV_DDR_FLOW),y)
MV_DDR_CSRC += ddr_init.c
MV_DDR_CSRC += ddr3_init.c
else ifeq ($(MV_DDR_FLOW_V2),y)
MV_DDR_CSRC += mv_ddr_init.c
MV_DDR_CSRC += dram_if.c
endif
MV_DDR_CSRC += ddr3_training_db.c
MV_DDR_CSRC += mv_ddr_build_message.c
MV_DDR_CSRC += mv_ddr_common.c
MV_DDR_CSRC += mv_ddr_spd.c
MV_DDR_CSRC += mv_ddr_mrs.c
MV_DDR_CSRC += mv_ddr_topology.c
MV_DDR_CSRC += mv_ddr4_training_db.c
MV_DDR_CSRC += $(MV_DDR_DRVPATH)/mv_ddr_mc6.c
MV_DDR_CSRC += $(MV_DDR_DRVPATH)/mv_ddr_xor_v2.c
ifeq ($(MV_DDR_SNPS),y)
MV_DDR_CSRC += $(foreach DIR,$(MV_DDR_SNPSPATH),$(wildcard $(DIR)/*.c))
endif
ifeq ($(MV_DDR_TIP),y)
MV_DDR_CSRC += ddr3_debug.c
MV_DDR_CSRC += ddr3_training.c
MV_DDR_CSRC += ddr3_training_bist.c
MV_DDR_CSRC += ddr3_training_centralization.c
MV_DDR_CSRC += ddr3_training_hw_algo.c
MV_DDR_CSRC += ddr3_training_ip_engine.c
MV_DDR_CSRC += ddr3_training_leveling.c
MV_DDR_CSRC += ddr3_training_pbs.c
MV_DDR_CSRC += mv_ddr4_mpr_pda_if.c
MV_DDR_CSRC += mv_ddr4_training.c
MV_DDR_CSRC += mv_ddr4_training_calibration.c
MV_DDR_CSRC += mv_ddr4_training_leveling.c
endif

MV_DDR_COBJ = $(patsubst %.c,$(OBJ_DIR)/%.o,$(MV_DDR_CSRC))
# add mv_ddr build message and version string object
MV_DDR_VER_COBJ = $(patsubst %.c,$(OBJ_DIR)/%.o,$(MV_DDR_VER_CSRC))
MV_DDR_COBJ += $(MV_DDR_VER_COBJ)
ifeq ($(MV_DDR_SNPS),y)
MV_DDR_COBJ += $(MV_DDR_SNPSPATH)/fw/snps_static.o
endif

.NOTPARALLEL:
.SILENT:

all: check_env header create_dir $(MV_DDR_LIB)

# mv_ddr code compilation
$(OBJ_DIR)/%.o: %.c
	$(ECHO) "  CC      $<"
	$(CC) -c $(CFLAGS) -o $@ $<

$(MV_DDR_LIB): $(MV_DDR_COBJ)
	$(ECHO) "  AR      $(MV_DDR_LIBNAME)"
	$(AR) rcs $(MV_DDR_LIB) $(MV_DDR_COBJ)

$(MV_DDR_VER_COBJ):
	$(ECHO) "  CC      $(MV_DDR_VER_CSRC)"
	$(CC) -c $(CFLAGS) -o $@ $(MV_DDR_VER_CSRC)

create_dir:
	$(MKDIR) $(OBJ_DIR)/drivers
	$(MKDIR) $(OBJ_DIR)/$(MV_DDR_PLAT)
ifeq ($(MV_DDR_SNPS),y)
	$(MKDIR) $(OBJ_DIR)/drivers/snps
endif

header:
	$(ECHO) "\nBuilding DRAM driver"

check_env:
ifndef PLATFORM
	$(error PLATFORM is undefined. please set PLATFORM variable)
endif

clean:
	$(ECHO) "  CLEAN"
	@$(RM) $(MV_DDR_COBJ) $(MV_DDR_LIB)

endif
