# Copyright (c) 2015, David Hirvonen, Ruslan Baratov
# All rights reserved.

cmake_minimum_required(VERSION 3.0)

### Hunter snapshot that will be used ###
include("cmake/HunterGate.cmake")
HunterGate(
    URL "https://github.com/ruslo/hunter/archive/v0.12.7.tar.gz"
    SHA1 "7103f0ab481523ca93a06c1c55abcd56741d3a97"
)

project(gatherer VERSION 0.1.0)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

# See: cmake/Hunter/config.cmake
hunter_add_package(Sugar)
include("${SUGAR_ROOT}/cmake/Sugar")
include(sugar_include)

set(is_android ANDROID) # syntax compatibility
string(COMPARE EQUAL "${CMAKE_OSX_SYSROOT}" "iphoneos" is_ios)
string(COMPARE EQUAL "${CMAKE_SYSTEM_NAME}" "Linux" is_linux)
string(COMPARE EQUAL "${CMAKE_BUILD_TYPE}" "DEBUG" is_debug)
string(COMPARE EQUAL "${CMAKE_GENERATOR}" "Xcode" is_xcode)

if(is_xcode)
  set(CMAKE_CONFIGURATION_TYPES "Debug;Release")
endif()

## #################################################################
## Dependencies - Qt
## #################################################################

## Needs:
#-Wno-error=unused-command-line-argument-hard-error-in-future

option(BUILD_QT ON)

if(BUILD_QT)
  hunter_add_package(Qt)
  find_package(Qt5Widgets REQUIRED version 5.5)
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

### Manage libs and source code in src folder
add_subdirectory(src)


