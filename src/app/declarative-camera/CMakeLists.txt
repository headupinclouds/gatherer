# Copyright (c) 2015, Ruslan Baratov
# All rights reserved.

find_package(Qt5Quick REQUIRED) # Qt5::Quick

set(
    sources
    # cpp source
    qmlcamera.cpp
    # qrc
    declarative-camera.qrc
    # qrc content
    CameraButton.qml
    CameraListButton.qml
    CameraListPopup.qml
    CameraPropertyButton.qml
    CameraPropertyPopup.qml
    FocusButton.qml
    PhotoCaptureControls.qml
    PhotoPreview.qml
    Popup.qml
    VideoCaptureControls.qml
    VideoPreview.qml
    ZoomControl.qml
    declarative-camera.qml
    images/camera_auto_mode.png
    images/camera_camera_setting.png
    images/camera_flash_auto.png
    images/camera_flash_fill.png
    images/camera_flash_off.png
    images/camera_flash_redeye.png
    images/camera_white_balance_cloudy.png
    images/camera_white_balance_flourescent.png
    images/camera_white_balance_incandescent.png
    images/camera_white_balance_sunny.png
    images/toolbutton.png
    images/toolbutton.sci
)

if(ANDROID)
  add_library(declarative-camera SHARED ${sources})

  target_link_libraries(declarative-camera Qt5::Quick)

  set_target_properties(declarative-camera PROPERTIES VERSION 1.0)

  hunter_add_package(QtAndroidCMake)
  list(APPEND CMAKE_MODULE_PATH "${QTANDROIDCMAKE_ROOT}")
  include(AddQtAndroidApk)

  add_qt_android_apk(
      TARGET declarative-camera-apk
      BASE_TARGET declarative-camera
      LAUNCH_TARGET declarative-camera-launch
      PACKAGE_NAME declarative.camera.qt
      MANIFEST "${CMAKE_CURRENT_LIST_DIR}/AndroidManifest.xml"
      QML_ROOT "${CMAKE_CURRENT_LIST_DIR}"
  )

  hunter_add_package(Android-SDK)
  message("Path to `android`: ${ANDROID-SDK_ROOT}/android-sdk/tools/android")
  message("Path to `emulator`: ${ANDROID-SDK_ROOT}/android-sdk/tools/emulator")
  message("Path to `adb`: ${ANDROID-SDK_ROOT}/android-sdk/platform-tools/adb")
else()
  find_package(Qt5Quick REQUIRED) # Qt5::Quick
  find_package(Qt5Qml REQUIRED) # Qt5::QmlQtQuick2Plugin
  find_package(Qt5Multimedia REQUIRED) # Qt5::QMultimediaDeclarativeModule

  # We should load this manually because of cyclic dependencies in Qt
  find_package(Qt5MultimediaWidgets REQUIRED)

  set(qt_conf "${CMAKE_CURRENT_LIST_DIR}/qt.conf")
  set(qml_dir "${CMAKE_CURRENT_BINARY_DIR}/qt_qml")

  hunter_add_package(QtQmlManager)
  list(APPEND CMAKE_MODULE_PATH "${QTQMLMANAGER_ROOT}/cmake")
  include(QtCopyQmlTo)
  QtCopyQmlTo(${qml_dir})

  add_executable(declarative-camera ${sources} ${qt_conf} ${qml_dir})

  set_target_properties(
      declarative-camera
      PROPERTIES
      RESOURCE "${qt_conf};${qml_dir}"
  )

  target_link_libraries(declarative-camera Qt5::Quick)

  ### Plugins
  target_link_libraries(declarative-camera Qt5::QMultimediaDeclarativeModule)
  target_link_libraries(declarative-camera Qt5::QmlQtQuick2Plugin)
  target_link_libraries(declarative-camera ${Qt5Gui_PLUGINS})
  target_link_libraries(declarative-camera ${Qt5Multimedia_PLUGINS})
endif()
