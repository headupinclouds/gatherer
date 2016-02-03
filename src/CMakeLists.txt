add_subdirectory(3rdparty)
include_directories("${CMAKE_SOURCE_DIR}/src/3rdparty/thread-pool-cpp/thread_pool")

add_subdirectory(lib)
add_subdirectory(app)

if(BUILD_TESTS)
  add_subdirectory(tests)
endif()

