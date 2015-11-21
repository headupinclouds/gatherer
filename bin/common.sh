#!/bin/bash

ANDROID_TOOLCHAIN=android-ndk-r10e-api-19-armeabi-v7a-neon

HUNTER_CONFIGURATION_TYPES=Release
BUILD_QT=ON

GATHERER_BUILD_ARGS=\
(
	"HUNTER_CONFIGURATION_TYPES=Release"
	"BUILD_QT=${BUILD_QT}"
)

function rename_tab
{
	echo -ne "\033]0;$1:$2\007"
}
