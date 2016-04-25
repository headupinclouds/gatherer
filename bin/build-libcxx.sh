#!/bin/bash

DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
. ${DIR}/common.sh

EXTRA_ARGS=""
if [ $# -ge 1 ] && [ $1 -gt 0 ]; then
    EXTRA_ARGS="${EXTRA_ARGS} --clear "
fi

if [ $# -ge 2 ]; then
    EXTRA_ARGS="${EXTRA_ARGS} --home ${2} "
fi

TOOLCHAIN=libcxx

rename_tab gatherer $TOOLCHAIN

function build_all
{
    COMMAND=(
        "--toolchain ${TOOLCHAIN} "
        "--verbose --fwd HUNTER_CONFIGURATION_TYPES=Release "
        "${GATHERER_BUILD_ARGS[*]} "
        "--config Release "
        "--install "
        "--jobs 8 "
        "--reconfig "
        "${EXTRA_ARGS} "
    )
    
	build.py ${COMMAND[*]}
}

build_all
#(cd ${DIR}/.. && build_all)
