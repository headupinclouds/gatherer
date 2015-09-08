#!/bin/bash

TOOLCHAIN=ios-8-4-arm64

build.py --toolchain ${TOOLCHAIN} --verbose --fwd HUNTER_CONFIGURATION_TYPES=Release BUILD_QT=ON \
--config Release \
--framework \
--install \
--jobs 8 \
--open \
#--reconfig \
#--clear


