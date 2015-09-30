# Copyright (c) 2015, Ruslan Baratov, David Hirvonen
# All rights reserved.

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(
    SOURCES
    VideoCapture.cpp
    VideoCapture.h
    glwidget.cpp
    glwidget.h
    main.cpp
    mainwindow.cpp
    mainwindow.h
    window.cpp
    window.h
)

if(ANDROID)
  add_library(preview-qt SHARED ${SOURCES})
  set_target_properties(preview-qt PROPERTIES VERSION 1.0)

  hunter_add_package(QtAndroidCMake)
  list(APPEND CMAKE_MODULE_PATH "${QTANDROIDCMAKE_ROOT}")
  include(AddQtAndroidApk)

  add_qt_android_apk(
      TARGET preview-qt-apk
      BASE_TARGET preview-qt
      LAUNCH_TARGET preview-qt-launch
      PACKAGE_NAME preview.qt # must be no '-' (API 16 error)
      MANIFEST "${CMAKE_CURRENT_LIST_DIR}/AndroidManifest.xml.in"
  )

  hunter_add_package(Android-SDK)
  message("Path to `android`: ${ANDROID-SDK_ROOT}/android-sdk/tools/android")
  message("Path to `emulator`: ${ANDROID-SDK_ROOT}/android-sdk/tools/emulator")
  message("Path to `adb`: ${ANDROID-SDK_ROOT}/android-sdk/platform-tools/adb")

else()
  add_executable(preview-qt ${SOURCES})
  install(TARGETS preview-qt DESTINATION bin)
endif()

target_link_libraries(preview-qt
  Qt5::Widgets
  ${OpenCV_LIBS}
  gatherer_graphics
)

set_property(TARGET preview-qt PROPERTY FOLDER "app/gui")


