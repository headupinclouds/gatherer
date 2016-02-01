add_subdirectory(lib)
add_subdirectory(app)
add_subdirectory(3rdparty)

if(BUILD_TESTS)
  add_subdirectory(tests)
endif()

