#!/bin/bash

build.py --toolchain xcode --verbose --fwd HUNTER_CONFIGURATION_TYPES=Release BUILD_QT=ON \
--config Release \
--jobs 8 \
--open \
--clear
