#!/bin/bash

DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"

. ${DIR}/common.sh

build.py --toolchain ${ANDROID_TOOLCHAIN} --verbose --fwd ANDROID=TRUE \
		 ${GATHERER_BUILD_ARGS[*]} \
		 --config Release \
		 --jobs 8 \
		 --target declarative-camera-launch

