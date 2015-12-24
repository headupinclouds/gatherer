hunter_add_package(OpenCV)
find_package(OpenCV REQUIRED)

add_library(OGLESGPGPUTest OGLESGPGPUTest.h OGLESGPGPUTest.cpp)
target_include_directories(OGLESGPGPUTest PUBLIC "${CMAKE_CURRENT_LIST_DIR}")
target_link_libraries(OGLESGPGPUTest PUBLIC ${OpenCV_LIBS} ogles_gpgpu)
