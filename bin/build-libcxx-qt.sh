#!/bin/bash

build.py --toolchain libcxx --verbose --fwd HUNTER_CONFIGURATION_TYPES=Release BUILD_QT=ON \
--config Release \
--install \
--clear \
--jobs 8

