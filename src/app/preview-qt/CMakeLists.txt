# Copyright (c) 2015, Ruslan Baratov
# All rights reserved.

add_executable(
    hellogl2
    MACOSX_BUNDLE
    glwidget.cpp
    glwidget.h
    logo.cpp
    logo.h
    main.cpp
    mainwindow.cpp
    mainwindow.h
    window.cpp
    window.h
)

target_link_libraries(hellogl2 Qt5::Widgets)
