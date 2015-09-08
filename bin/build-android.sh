#!/bin/bash

TOOLCHAIN=android-ndk-r10e-api-16-armeabi-v7a-neon-clang-35-hid

build.py --toolchain ${TOOLCHAIN} --verbose --fwd HUNTER_CONFIGURATION_TYPES=Release BUILD_QT=ON ANDROID=TRUE \
--config Release \
--install \
--strip \
--clear \
--jobs 8 

