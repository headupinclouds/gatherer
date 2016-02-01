#!/bin/bash

DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
. ${DIR}/common.sh

EXTRA_ARGS=""
if [ $# -ge 1 ]; then
	EXTRA_ARGS="--clear --reconfig"
fi

# Must be <= 19 for qt qml camera:
TOOLCHAIN=${ANDROID_TOOLCHAIN}

rename_tab gatherer $TOOLCHAIN

[ -n ${ANDROID_SDK_ROOT} ] && unset ANDROID_SDK_ROOT
[ -n ${ANDROID_SDK_ROOT} ] && unset ANDROID_HOME
echo $ANDROID_SDK_ROOT

printenv | grep -i android

function build_all
{
	build.py --toolchain ${TOOLCHAIN} --verbose --fwd ANDROID=TRUE \
			 ${GATHERER_BUILD_ARGS[*]} \
			 --config Release \
			 --strip \
			 --jobs 8  \
             --reconfig \
			 ${EXTRA_ARGS}
}

(cd ${DIR}/.. && build_all)
