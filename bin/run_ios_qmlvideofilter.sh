#!/bin/bash

#  
# Installation instructions here:
#
# https://github.com/phonegap/ios-deploy

### Check application

#├── Release-iphoneos
#│   └── QmlVideoFilterExample.app

DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
. ${DIR}/common.sh

TOOLCHAIN=ios-9-1-arm64

GATHERER=${DIR}/..

APP=${GATHERER}/_builds/${TOOLCHAIN}/src/app/qmlvideofilter/Release-iphoneos/QmlVideoFilterExample.app
BUNDLE_ID=com.github.headupinclouds.gatherer.qmlvideofilter

ios-deploy --list_bundle_id
ios-deploy --justlaunch --bundle ${APP}



