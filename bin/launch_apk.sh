#!/bin/bash

# http://stackoverflow.com/a/17290311

if [ $# -lt 1 ]; then
    >&2 echo "Must specify apk name"
    exit 1
fi

if [ -z "${ANDROID_HOME}" ]; then
    >&2 echo "Must have ANDROID_HOME set to android-sdk location"
    exit 1
fi

if [ $(adb devices | wc -l | awk '{print $1}') -lt 3 ]; then
    >&2 echo "Must have valid android device connected"
    exit 1
fi

APK_NAME=${1}
export PATH="${PATH}:${ANDROID_HOME}/build-tools/21.1.1"

PKG_NAME=$(aapt dump badging $1|awk -F" " '/package/ {print $2}'|awk -F"'" '/name=/ {print $2}')
ACT_NAME=$(aapt dump badging $1|awk -F" " '/launchable-activity/ {print $2}'|awk -F"'" '/name=/ {print $2}')

if [ $# -ge 2 ]; then
    adb uninstall $PKG_NAME
else
    adb install -r $APK_NAME
    adb shell am start -n $PKG_NAME/$ACT_NAME
fi
