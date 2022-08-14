#!/bin/sh
# Copyright (c) 2017 Marvell.
# SPDX-License-Identifier: BSD-3-Clause

# get mv_ddr directory path
MV_DDR_ROOT=$1

# get mv_ddr build message and version string c-file name
MV_DDR_VER_CSRC=$2

# get mv_ddr git commit id
MV_DDR_COMMIT_ID=`git -C $MV_DDR_ROOT rev-parse --verify --quiet --short HEAD 2> /dev/null`

# check for uncommitted changes in mv_ddr git
MV_DDR_DIRTY_CHK=`git -C $MV_DDR_ROOT diff-index --name-only HEAD 2> /dev/null`
if [ "$MV_DDR_DIRTY_CHK" != "" ]; then
	MV_DDR_COMMIT_ID="${MV_DDR_COMMIT_ID}-dirty"
fi

# get mv_ddr version from localversion file in mv_ddr directory
MV_DDR_VERSION=`awk '{$1=$1;print}' $MV_DDR_ROOT/localversion 2> /dev/null`
if [ "$MV_DDR_VERSION" = "" ]; then
	MV_DDR_VERSION="unknown"
fi

# set mv_ddr version string to be printed out
if [ "$MV_DDR_COMMIT_ID" = "" ]; then
MV_DDR_VERSION_STRING="mv_ddr: Marvell-${MV_DDR_VERSION} (release)"
else
MV_DDR_VERSION_STRING="mv_ddr: ${MV_DDR_VERSION}-g${MV_DDR_COMMIT_ID}"
fi

# get date and time to set mv_ddr build message
MONTH=`LC_TIME=en_US date +%b`
DAY=`LC_TIME=en_US date +%d`
YEAR=`LC_TIME=en_US date +%Y`
DATE="$MONTH $DAY $YEAR"
TIME=`LC_TIME=en_US date +%X`

# write mv_ddr build message and version string to c-file
echo "const char mv_ddr_build_message[] = \"($DATE - $TIME)\"; \
      const char mv_ddr_version_string[] = \"$MV_DDR_VERSION_STRING\";" > \
      $MV_DDR_ROOT/$MV_DDR_VER_CSRC
