#!/usr/bin/env bash

unset ZEPHYR_SDK_INSTALL_DIR
export ZEPHYR_BASE=./zephyr
source $ZEPHYR_BASE/zephyr-env.sh

export ZEPHYR_GCC_VARIANT=gccarmemb
export GCCARMEMB_TOOLCHAIN_PATH=/usr
