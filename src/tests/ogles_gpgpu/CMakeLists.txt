# found variable?
#include_directories(${GTEST_INCLUDE_DIRS})

add_executable(test-ogles-gpgpu test-ogles-gpgpu.cpp test-shader.cpp)
target_link_libraries(test-ogles-gpgpu ogles_gpgpu ${OpenCV_LIBS} GTest::main OGLESGPGPUTest)

enable_testing()
add_test(TestOGLESGPGP test-ogles-gpgpu
  "${CMAKE_SOURCE_DIR}/assets/images/lena512color.png"
  "${CMAKE_SOURCE_DIR}/assets/images/lena512color.png"
  )

set_property(TARGET test-ogles-gpgpu PROPERTY FOLDER "app/tests")

