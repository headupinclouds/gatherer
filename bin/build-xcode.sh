#!/bin/bash

DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
. ${DIR}/common.sh

EXTRA_ARGS=""
[ $# -ge 1 ] && EXTRA_ARGS="--clear"

TOOLCHAIN=xcode

function build_all
{
	build.py --toolchain ${TOOLCHAIN} --verbose --fwd HUNTER_CONFIGURATION_TYPES=Release \
			 ${GATHERER_BUILD_ARGS[*]} \
			 CMAKE_XCODE_ATTRIBUTE_OSX_DEPLOYMENT_TARGET=10.10 \
			 CMAKE_OSX_DEPLOYMENT_TARGET=10.10 \
			 --config Release \
			 --jobs 8 \
			 --open \
			 ${EXTRA_ARGS}
}

(cd ${DIR}/.. && build_all)

