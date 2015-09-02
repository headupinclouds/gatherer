# Copyright (c) 2015, Ruslan Baratov, David Hirvonen
# All rights reserved.

set(CMAKE_INCLUDE_CURRENT_DIR ON)

add_executable(
    preview-qt
#    MACOSX_BUNDLE
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

target_link_libraries(preview-qt 
  Qt5::Widgets
  ${OpenCV_LIBS} 
  gatherer_graphics
)

install(TARGETS preview-qt DESTINATION bin)
set_property(TARGET preview-qt PROPERTY FOLDER "app/gui")