# Copyright (c) 2015, David Hirvonen
# All rights reserved.

set(CMAKE_INCLUDE_CURRENT_DIR ON)

add_executable(
  opencv-qt-video
  MACOSX_BUNDLE
  main.cpp
)

#include (${QT_USE_FILE})
#QT5_WRAP_CPP(LIB_HEADER_MOC main.cpp)
#add_library(foo SHARED ${LIB_SOURCE} ${LIB_HEADER_MOC})

target_link_libraries(opencv-qt-video
  Qt5::Widgets
  ${OpenCV_LIBS}
)

if(APPLE)
  find_package(Qt5PrintSupport REQUIRED)
  target_link_libraries(opencv-qt-video Qt5::PrintSupport)
endif()

install(TARGETS opencv-qt-video DESTINATION bin)
set_property(TARGET opencv-qt-video PROPERTY FOLDER "app/gui")
