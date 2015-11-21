#!/bin/bash

DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"

. ${DIR}/common.sh

build.py --toolchain ${ANDROID_TOOLCHAIN} --target declarative-camera-launch --verbose
