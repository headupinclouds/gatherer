#!/bin/bash

BUILD_DIR=${1}

if [ ! -d ${BUILD_DIR} ] ; then
	2> echo "Specify valid build dir"
	exit
fi

SHA1=$(shasum ${BUILD_DIR}/_3rdParty/Hunter/config-id/config.cmake | awk '{print $1}')
CONFIG_ID=${SHA1:0:7}

SHA1=$(shasum ${BUILD_DIR}/_3rdParty/Hunter/toolchain/toolchain.info  | awk '{print $1}')
TOOLCHAIN_ID=${SHA1:0:7}

## Not always "xxxxxx"
HUNTER_INSTALL_DIR=${HUNTER_ROOT}/_Base/xxxxxxx/${CONFIG_ID}/${TOOLCHAIN_ID}/Install/

echo ${HUNTER_INSTALL_DIR}
