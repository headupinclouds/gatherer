#!/bin/bash

if [ -z "${GATHERER}" ]; then
    >&2 echo "Must set GATHERER to top level directory"
    exit
fi

if [ $(adb devices | wc -l | awk '{print $1}') -lt 3 ]; then
    >&2 echo "Must have valid android device connected"
    exit
fi

APP=camera_test

ANDROID_PATH=${GATHERER}/_install/android-ndk-r10e-api-16-armeabi-v7a-neon-clang-35-hid/bin
EXE_DIR=/data/local/tmp
adb push ${ANDROID_PATH}/camera_test ${EXE_DIR}/

cat <<EOF | adb shell
chmod 777 ${EXE_DIR}/eyeseg
[ ! -d ${EXE_DIR}/tmp/ ] && mkdir -p ${EXE_DIR}/tmp/
${EXE_DIR}/${APP}
EOF

#adb pull ${EXE_DIR}/tmp/some_output.txt

