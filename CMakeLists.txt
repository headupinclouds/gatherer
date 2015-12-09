# Copyright (c) 2015, David Hirvonen, Ruslan Baratov
# All rights reserved.

cmake_minimum_required(VERSION 3.0)

### Hunter snapshot that will be used ###
include("cmake/HunterGate.cmake")
HunterGate(
    URL "https://github.com/ruslo/hunter/archive/v0.12.22.tar.gz"
    SHA1 "868408f1f3d8deeaab13f4ebca28b02ec2dfcade"
    LOCAL
)
project(gatherer VERSION 0.1.0)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake/modules")

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

set(is_android ${ANDROID}) # syntax compatibility
string(COMPARE EQUAL "${CMAKE_OSX_SYSROOT}" "iphoneos" is_ios)
string(COMPARE EQUAL "${CMAKE_SYSTEM_NAME}" "Linux" is_linux)
string(COMPARE EQUAL "${CMAKE_BUILD_TYPE}" "DEBUG" is_debug)
string(COMPARE EQUAL "${CMAKE_GENERATOR}" "Xcode" is_xcode)

# See: cmake/Hunter/config.cmake
# First call to 'hunter_add_package' will contains read of Hunter/config.cmake
# Next variables will be used:
#   * is_ios
#   * is_linux
hunter_add_package(Sugar)
include("${SUGAR_ROOT}/cmake/Sugar")
include(sugar_include)

if(is_xcode)
  option(GATHERER_DISABLE_DSYM "Disable generation of dSYM information" ON)
  if(GATHERER_DISABLE_DSYM)
    set(CMAKE_XCODE_ATTRIBUTE_DEBUG_INFORMATION_FORMAT "dwarf")
  endif()
  set(CMAKE_CONFIGURATION_TYPES "Debug;Release")
  set(CMAKE_XCODE_ATTRIBUTE_ENABLE_BITCODE NO)
endif()

## #################################################################
## Dependencies - Qt
## #################################################################

## Needs:
#-Wno-error=unused-command-line-argument-hard-error-in-future

option(BUILD_QT "Build with Qt" ON)

if(BUILD_QT)

  # Some background: https://github.com/ruslo/hunter/issues/253
  hunter_add_package(Qt COMPONENTS qtmultimedia qtdeclarative)
  find_package(Qt5Widgets REQUIRED)
  find_package(Qt5Multimedia REQUIRED)
  find_package(Qt5MultimediaWidgets REQUIRED)
  find_package(Qt5OpenGL REQUIRED)
  find_package(Qt5Qml REQUIRED)
  find_package(Qt5Quick REQUIRED)
  find_package(Qt5QuickTest REQUIRED)
  find_package(Qt5QuickWidgets REQUIRED)

  message("Used Qt:")
  message("  ${QT_ROOT} (root from Hunter)")
  message("  ${Qt5Widgets_DIR} (find_package result)")
  set(CMAKE_AUTOMOC ON)
  set(CMAKE_AUTOUIC ON)
  set(CMAKE_AUTORCC ON)
endif()

## #################################################################
## Dependencies - OpenCV
## #################################################################

hunter_add_package(OpenCV)
find_package(OpenCV REQUIRED)

## #################################################################
## Dependencies - cereal (lightweight boost serialization alt)
## #################################################################

hunter_add_package(cereal)
find_package(cereal CONFIG REQUIRED)

## #################################################################
## Dependencies - libyuv
## #################################################################

hunter_add_package(libyuv)
find_package(libyuv CONFIG REQUIRED)

## #################################################################
## Dependencies - OpenGL stuff
## #################################################################

if(IOS OR ANDROID)
  set(GATHERER_OPENGL_ES TRUE)
else()
  set(GATHERER_OPENGL_ES FALSE)
endif()

if(NOT GATHERER_OPENGL_ES)
  find_package(PkgConfig REQUIRED)

  pkg_search_module(GLFW REQUIRED glfw3)
  include_directories(AFTER ${GLFW_INCLUDE_DIRS})

  if(XCODE)
    link_directories(${GLFW_LIBRARY_DIRS}) # Fix Xcode build
  endif()

  find_package(GLEW REQUIRED)

  # This gives me a build error in the preview test app
  #include <GL/glew.h> // include GLEW and new version of GL on Windows

  option(USE_GLEW_INTERFACE ON)

  add_library(glew INTERFACE IMPORTED)
  set_target_properties(
    glew PROPERTIES
    INTERFACE_INCLUDE_DIRECTORIES "${GLEW_INCLUDE_DIRS}"
    INTERFACE_LINK_LIBRARIES "${GLEW_LIBRARIES}"
    )

  find_package(OpenGL REQUIRED)
endif()

## #################################################################
## Project
## #################################################################

include_directories(src/lib)

### Eventually we will use ogles_gpgpu lib as a package through hunter
### for now we need to include header paths appropriately
include_directories(src/3rdparty/ogles_gpgpu)
include_directories(src/3rdparty/ogles_gpgpu/ogles_gpgpu)
include_directories(src/3rdparty/ogles_gpgpu/ogles_gpgpu/common)

enable_testing()

### Manage libs and source code in src folder
add_subdirectory(src)


