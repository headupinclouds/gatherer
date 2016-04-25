#!/bin/bash

DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
. ${DIR}/common.sh

EXTRA_ARGS=""
[ $# -ge 1 ] && EXTRA_ARGS="--clear"

TOOLCHAIN=xcode

rename_tab gatherer $TOOLCHAIN

function build_all
{
    COMMANDS=(
	    "--toolchain ${TOOLCHAIN} "
        "--verbose "
        "--fwd HUNTER_CONFIGURATION_TYPES=Release "
        "${GATHERER_BUILD_ARGS[*]} "
        "CMAKE_XCODE_ATTRIBUTE_OSX_DEPLOYMENT_TARGET=10.10 "
        "CMAKE_OSX_DEPLOYMENT_TARGET=10.10 "
        "--config Release "
        "--jobs 8 "
        "--reconfig "
        "--test "
        "--open "
        "${EXTRA_ARGS} "
    )

     build.py ${COMMANDS[*]}
}

(cd ${DIR}/.. && build_all)

