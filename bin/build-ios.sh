#!/bin/bash

TOOLCHAIN=ios-8-4-arm64

build.py --toolchain ${TOOLCHAIN} --verbose --fwd \
--config Release \
--framework \
--install \
--jobs 8 \
--open \
--nobuild
#--reconfig \
#--clear


