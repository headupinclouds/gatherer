#!/bin/bash

TOOLCHAIN=android-ndk-r10e-api-16-armeabi-v7a-neon-clang-35-hid
CONFIG=Debug

if [ -z "${GATHERER}" ]; then
    >&2 echo "Must set GATHERER to top level directory"
    exit
fi

function get_apks
{
    find ${GATHERER}/_builds/${TOOLCHAIN}-${CONFIG} -name "*.apk" | awk '{print NR " " $1 }'
}

if [ $# == 0 ]; then
    get_apks
else
    get_apks | awk 'NR=='$1' {print $2}'
fi



