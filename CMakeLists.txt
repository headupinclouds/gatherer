# Copyright (c) 2015, David Hirvonen, Ruslan Baratov
# All rights reserved.

cmake_minimum_required(VERSION 3.0)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake/modules")

set(custom_config "${CMAKE_CURRENT_BINARY_DIR}/hunter-config.cmake")
configure_file(cmake/Hunter/config.cmake "${custom_config}" @ONLY)

### Hunter snapshot that will be used ###
include("cmake/HunterGate.cmake")
HunterGate(
    URL "https://github.com/ruslo/hunter/archive/v0.12.7.tar.gz"
    SHA1 "b5ae065573ea2706ae1389a9048f1cabb2451f23"
    FILEPATH "${custom_config}"
)

set(CMAKE_MODULE_PATH @CMAKE_MODULE_PATH@)

project(gatherer VERSION 0.1.0)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

# See: cmake/Hunter/config.cmake
hunter_add_package(Sugar)
include("${SUGAR_ROOT}/cmake/Sugar")
include(sugar_include)

set(is_android ${ANDROID}) # syntax compatibility
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

option(BUILD_QT "Build with Qt" ON)

if(BUILD_QT)
  hunter_add_package(Qt)
  find_package(Qt5Widgets 5.5 REQUIRED)
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


