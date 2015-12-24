include_directories(${GLFW_INCLUDE_DIRS})

#find_package(ogles_gpgpu)

### preview ###
set(OGLES_GPGPU_TEST_SRCS
  ogles_gpgpu_test.cpp 
  GLContextWindow.h 
  GLContextWindow.cpp
)

add_executable(ogles_gpgpu_test ${OGLES_GPGPU_TEST_SRCS})
target_link_libraries(ogles_gpgpu_test
  PRIVATE
  #ogles_gpgpu::ogles_gpgpu
  ogles_gpgpu
  ${OpenCV_LIBS}
  ${GLFW_LIBRARIES}
  OGLESGPGPUTest
  glew
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
)

install(TARGETS ogles_gpgpu_test DESTINATION bin)
set_property(TARGET ogles_gpgpu_test PROPERTY FOLDER "app/console")


