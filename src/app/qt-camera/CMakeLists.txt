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


# get_cmake_property(_variableNames VARIABLES)
# foreach (_variableName ${_variableNames})
#   message(STATUS "${_variableName}=${${_variableName}}")
# endforeach()
# message("qt_lib_dir: ${qt_lib_dir}")

# i.e., ${HUNTER_ROOT}/_Base/xxxxxxx/07d21d3/18855aa/Install
set(qt_lib_dir ${_qt5Core_install_prefix})

# https://github.com/ruslo/hunter/issues/253#issuecomment-147019505
set(qt_qml_libs
  "-L${qt_lib_dir}/plugins/qmltooling"
  qmldbg_tcp
  "-L${qt_lib_dir}/qml/QtQuick.2/"
  qtquick2plugin
  "-L${qt_lib_dir}/qml/QtQuick/Window.2/"
  windowplugin
  # Multimedia
  "-L${qt_lib_dir}/lib"
  Qt5MultimediaQuick_p
  "-L${qt_lib_dir}/qml/QtMultimedia"
  declarative_multimedia
  )


# Missing: _SCDynamicStoreCopyProxies   (SystemConfiguration.framework

set(qt_multimedia_libs
  "-L${qt_lib_dir}/plugins/mediaservice/"
  qavfcamera
  qavfmediaplayer
  qtmedia_audioengine
  "-L${qt_lib_dir}/plugins/audio/"
  qtaudio_coreaudio
  "-framework AudioUnit"
  "-framework AudioToolbox"
  "-framework CoreAudio"
  "-framework AVFoundation"
  "-framework CoreMedia"
  "-framework CoreVideo"
  "-framework QuartzCore"
  "-framework SystemConfiguration"
  "-framework Foundation"
  "-framework CoreGraphics"
  "-framework CoreText"
  "-framework Security"
  )

set(qt_qcocoa_libs
  "-framework IOKit"
  )

#/Install/qml/QtQuick//LocalStorage/libqmllocalstorageplugin.a
#/Install/qml/QtQuick//Particles.2/libparticlesplugin.a
#/Install/qml/QtQuick//Window.2/libwindowplugin.a

# set(qt_quick_layout_libs
#   "-L${qt_lib_dir}/qml/QtQuick/Layouts"
#   qquicklayoutsplugin
#   )

target_link_libraries(qt-camera
  Qt5::Widgets
  Qt5::Multimedia
  Qt5::MultimediaWidgets
  Qt5::OpenGL
  Qt5::Qml
  Qt5::Quick
  ${qt_qml_libs}
  ${qt_multimedia_libs}
  ${qt_cocoa_libs}
  ${qt_quick_layout_libs}
  #${OpenCV_LIBS}
  #gatherer_graphics
)

set_property(TARGET qt-camera PROPERTY FOLDER "app/gui")


