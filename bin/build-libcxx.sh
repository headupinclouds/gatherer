#!/bin/bash

DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
. ${DIR}/common.sh

EXTRA_ARGS=""
[ $# -ge 1 ] && EXTRA_ARGS="--clear" 

TOOLCHAIN=libcxx

rename_tab gatherer $TOOLCHAIN

function build_all
{
	build.py --toolchain ${TOOLCHAIN} --verbose --fwd HUNTER_CONFIGURATION_TYPES=Release \
			 ${GATHERER_BUILD_ARGS[*]} \
			 --config Release \
			 --install \
			 --jobs 8 \
			 --reconfig \
			 ${EXTRA_ARGS}
}

(cd ${DIR}/.. && build_all)
