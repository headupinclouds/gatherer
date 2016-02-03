# Copyright (c) 2015, Ruslan Baratov
# All rights reserved.

set(SOURCES
  test-shader.cpp
  QGLContext.h
  QGLContext.cpp
  qt_ogles_gpgpu.cpp
)

add_executable(qt_ogles_gpgpu ${SOURCES})

target_link_libraries(qt_ogles_gpgpu
  Qt5::Widgets
  ogles_gpgpu
  ${OpenCV_LIBS}
  ${GLFW_LIBRARIES}
  OGLESGPGPUTest
  gatherer_graphics  
  GTest::main
  )

find_package(Qt5PrintSupport REQUIRED)
target_link_libraries(qt_ogles_gpgpu Qt5::PrintSupport)

