# Copyright (c) 2015, Ruslan Baratov, David Hirvonen
# All rights reserved.

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(
    SOURCES
	src/activationeventfilter.cpp
	src/activationeventfilter.h
	src/applicationactivationlistener.cpp
	src/applicationactivationlistener.h
	src/customcamera.cpp
	src/customcamera.h
	src/histogram.cpp
	src/histogram.h
	src/imageanalyzer.cpp
	src/imageanalyzer.h
	src/main.cpp
	src/performancemeter.cpp
	src/performancemeter.h
	src/profilingdata.h
	src/videosurface.cpp
	src/videosurface.h
	src/volumekeys.cpp
	src/volumekeys.h
)

if(ANDROID)
  add_library(qt-camera-demo SHARED ${SOURCES})
  set_target_properties(qt-camera-demo PROPERTIES VERSION 1.0)

  hunter_add_package(QtAndroidCMake)
  list(APPEND CMAKE_MODULE_PATH "${QTANDROIDCMAKE_ROOT}")
  include(AddQtAndroidApk)

  add_qt_android_apk(
      TARGET qt-camera-demo-apk
      BASE_TARGET qt-camera-demo
      LAUNCH_TARGET qt-camera-demo-launch
      PACKAGE_NAME preview.qt # must be no '-' (API 16 error)
      MANIFEST "${CMAKE_CURRENT_LIST_DIR}/AndroidManifest.xml.in"
  )

  hunter_add_package(Android-SDK)
  message("Path to `android`: ${ANDROID-SDK_ROOT}/android-sdk/tools/android")
  message("Path to `emulator`: ${ANDROID-SDK_ROOT}/android-sdk/tools/emulator")
  message("Path to `adb`: ${ANDROID-SDK_ROOT}/android-sdk/platform-tools/adb")

else()
  add_executable(qt-camera-demo ${SOURCES})
  install(TARGETS qt-camera-demo DESTINATION bin)
endif()

target_link_libraries(qt-camera-demo
  Qt5::Widgets
  Qt5::Declarative
  ${OpenCV_LIBS}
  gatherer_graphics
)

set_property(TARGET qt-camera-demo PROPERTY FOLDER "app/gui")


