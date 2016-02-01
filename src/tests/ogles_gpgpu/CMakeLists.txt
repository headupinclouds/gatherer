# found variable?
#include_directories(${GTEST_INCLUDE_DIRS})

set(TEST_OGLES_GPGPU_SRCS
  test-ogles-gpgpu.cpp
  test-shader.cpp
  GLContextWindow.h # temporary os x only solution
  GLContextWindow.cpp
  )

add_executable(test-ogles-gpgpu ${TEST_OGLES_GPGPU_SRCS})

target_link_libraries(test-ogles-gpgpu
  PRIVATE
  #ogles_gpgpu::ogles_gpgpu
  ogles_gpgpu
  ${OpenCV_LIBS}
  ${GLFW_LIBRARIES}
  OGLESGPGPUTest
  gatherer_graphics  
  glew
  GTest::main
  "-framework OpenGL"
  "-framework IOKit"
  "-framework Cocoa"
  "-framework OpenGL"
  "-framework QuartzCore"
  "-framework CoreMedia"
  "-framework AVFoundation"
  "-framework AppKit"
  "-framework CoreData"
  "-framework Foundation"
  ) ## OS X only...

enable_testing()
add_test(TestOGLESGPGP test-ogles-gpgpu
  "${CMAKE_SOURCE_DIR}/assets/images/lena512color.png"
  "${CMAKE_SOURCE_DIR}/assets/images/lena512color.png"
  )

set_property(TARGET test-ogles-gpgpu PROPERTY FOLDER "app/tests")

