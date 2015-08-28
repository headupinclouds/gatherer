# Copyright (c) 2015, David Hirvonen, Ruslan Baratov
# All rights reserved.

cmake_minimum_required(VERSION 3.0)

### Hunter snapshot that will be used ###
include("cmake/HunterGate.cmake")
HunterGate(
    URL "https://github.com/ruslo/hunter/archive/v0.8.13.tar.gz"
    SHA1 "7103f0ab481523ca93a06c1c55abcd56741d3a97"
    LOCAL
)

project(gatherer VERSION 0.1.0)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

# See: cmake/Hunter/config.cmake
hunter_add_package(Sugar)
include("$ENV{SUGAR_ROOT}/cmake/Sugar")
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

#-Wno-error=unused-command-line-argument-hard-error-in-future

# hunter_add_package(Qt)
# find_package(Qt5Widgets REQUIRED HINTS ${QT_ROOT})
# set(CMAKE_AUTOMOC ON)
# set(CMAKE_AUTOUIC ON)
# set(CMAKE_AUTORCC ON)

## #################################################################
## Dependencies - OpenCV
## #################################################################

# Are explicit includes of opencv deps still required by hunter?

### ZLIB: ZLIB_INCLUDE_DIRS; ZLIB_LIBRARIES; ZLIB_FOUND 
hunter_add_package(ZLIB)
find_package(ZLIB CONFIG REQUIRED)
message("ZLIB_INCLUDE_DIRS = ${ZLIB_INCLUDE_DIRS}")

### TIFF: TIFF_INCLUDE_DIR; TIFF_LIBRARIES; TIFF_FOUND
hunter_add_package(TIFF)
find_package(TIFF CONFIG REQUIRED)
message("TIFF_INCLUDE_DIRS = ${TIFF_INCLUDE_DIR}")

### PNG: PNG_INCLUDE_DIR; PNG_LIBRARIES; PNG_FOUND
hunter_add_package(PNG)
find_package(PNG CONFIG REQUIRED)
message("PNG_INCLUDE_DIRS = ${PNG_INCLUDE_DIR}")

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

find_package(PkgConfig REQUIRED)

pkg_search_module(GLFW REQUIRED glfw3)
include_directories(${GLFW_INCLUDE_DIRS})

find_package(GLEW REQUIRED)
include_directories(${GLEW_INCLUDE_DIRS})

find_package(OpenGL REQUIRED)

## #################################################################
## Project
## #################################################################

include_directories(src/lib)

### Manage libs and source code in src folder
add_subdirectory(src)

