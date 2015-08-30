# Copyright (c) 2015, David Hirvonen
# All rights reserved.

add_executable(
  opencv-qt-video
  MACOSX_BUNDLE
  main.cpp
)

target_link_libraries(opencv-qt-video 
  Qt5::Widgets
  ${OpenCV_LIBS} 
)

install(TARGETS opencv-qt-video DESTINATION bin)
set_property(TARGET opencv-qt-video PROPERTY FOLDER "app/gui")