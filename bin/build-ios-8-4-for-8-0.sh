#!/bin/bash

TOOLCHAIN=ios-8-4-hid

build.py --toolchain ${TOOLCHAIN} --verbose --fwd HUNTER_CONFIGURATION_TYPES=Release BUILD_QT=ON \
--config Release \
--framework-device \
--install \
--jobs 8 \
--open \
--reconfig \
--clear


