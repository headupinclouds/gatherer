# Copyright (c) 2015, Ruslan Baratov, David Hirvonen
# All rights reserved.

set(CMAKE_INCLUDE_CURRENT_DIR ON)


# USE_OGLESGPGPU=1 for ogles_gpgpu shader samples, else gatherer/graphics
# Note: ogles_gpgpu currently not working in this project, but same code is ok 
# in ogles_gpgpu_test with glfw
add_definitions(-DUSE_OGLESGPGPU=0)

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

if(IOS)
  set_target_properties(
      preview-qt
      PROPERTIES
      MACOSX_BUNDLE_INFO_PLIST "${CMAKE_CURRENT_LIST_DIR}/plist.in"
      XCODE_ATTRIBUTE_PRODUCT_NAME
      "PreviewQtCameraExample"
      XCODE_ATTRIBUTE_BUNDLE_IDENTIFIER
      "com.github.headupinclouds.gatherer.previewqt"
      XCODE_ATTRIBUTE_TARGETED_DEVICE_FAMILY "1,2" # iPhone/iPad
  )
endif()

target_link_libraries(preview-qt
  Qt5::Widgets
  ${OpenCV_LIBS}
  gatherer_graphics
  OGLESGPGPUTest
  ogles_gpgpu
)

if(APPLE AND NOT IOS)
  target_link_libraries(preview-qt
    ${GLFW_LIBRARIES}
    glew
    "-framework OpenGL"
    "-framework IOKit"
    "-framework Cocoa"
    "-framework OpenGL"
    "-framework QuartzCore"
    "-framework CoreMedia"
    "-framework AVFoundation"
    "-framework AppKit"
    "-framework CoreData"
    "-framework Foundation"
  )
endif()

set_property(TARGET preview-qt PROPERTY FOLDER "app/gui")


