# Copyright (c) 2015, Ruslan Baratov, David Hirvonen
# All rights reserved.

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(
    SOURCES
	camera.cpp
	camera.h
	#camera.pro
	#camera.ui
	imagesettings.cpp
	imagesettings.h
	#imagesettings.ui
	main.cpp
	videosettings.cpp
	videosettings.h
	#videosettings.ui
)

if(ANDROID)
  add_library(qt-camera SHARED ${SOURCES})
  set_target_properties(qt-camera PROPERTIES VERSION 1.0)

  hunter_add_package(QtAndroidCMake)
  list(APPEND CMAKE_MODULE_PATH "${QTANDROIDCMAKE_ROOT}")
  include(AddQtAndroidApk)

  add_qt_android_apk(
      TARGET qt-camera-apk
      BASE_TARGET qt-camera
      LAUNCH_TARGET qt-camera-launch
      PACKAGE_NAME camera.qt # must be no '-' (API 16 error)
      MANIFEST "${CMAKE_CURRENT_LIST_DIR}/AndroidManifest.xml.in"
  )

  hunter_add_package(Android-SDK)
  message("Path to `android`: ${ANDROID-SDK_ROOT}/android-sdk/tools/android")
  message("Path to `emulator`: ${ANDROID-SDK_ROOT}/android-sdk/tools/emulator")
  message("Path to `adb`: ${ANDROID-SDK_ROOT}/android-sdk/platform-tools/adb")

else()
  add_executable(qt-camera ${SOURCES})
  install(TARGETS qt-camera DESTINATION bin)
endif()

target_link_libraries(qt-camera
  Qt5::Widgets
  Qt5::Multimedia
  Qt5::MultimediaWidgets
  Qt5::OpenGL
  #${OpenCV_LIBS}
  #gatherer_graphics
)

set_property(TARGET qt-camera PROPERTY FOLDER "app/gui")


