#!/bin/bash

DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
. ${DIR}/common.sh

EXTRA_ARGS=""
if [ $# -ge 1 ]; then
	EXTRA_ARGS="--clear"
fi

#TOOLCHAIN=android-ndk-r10e-api-16-armeabi-v7a-neon-clang-35-hid
# broken

# Tested by ruslo:
#TOOLCHAIN=android-ndk-r10-api-19-armeabi-v7a

TOOLCHAIN=android-ndk-r10e-api-19-armeabi-v7a-neon

export ANDROID_SDK_ROOT=${ANDROID_HOME}

function build_all
{
	build.py --toolchain ${TOOLCHAIN} --verbose --fwd ANDROID=TRUE \
			 ${GATHERER_BUILD_ARGS[*]} \
			 --config Release \
			 --install \
			 --strip \
			 --jobs 8  \
			 ${EXTRA_ARGS}
}

(cd ${DIR}/.. && build_all)
