#!/bin/bash

build.py --toolchain libcxx --verbose --fwd HUNTER_CONFIGURATION_TYPES=Release BUILD_SHARED_LIBS=OFF \
BUILD_EXAMPLES=OFF \
--config Release \
--jobs 8 \
--install \
--clear

