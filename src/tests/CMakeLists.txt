# GTEST:
#
# https://github.com/forexample/gtest-pass-filename/blob/master/CMakeLists.txt
# https://github.com/ruslo/hunter/blob/master/examples/GTest/CMakeLists.txt
# https://github.com/ruslo/hunter/wiki/pkg.gtest

# Currently only for local host tests
# See potential extensions for test resource bundling here:
#     https://github.com/headupinclouds/gatherer/issues/123

if(NOT is_ios AND NOT is_android)

  # Not avabile on Android?
  hunter_add_package(GTest)
  find_package(GTest CONFIG REQUIRED)

  # Portable QT context for desktop and mobile systems:
  add_subdirectory(qt_ogles_gpgpu)

endif()

add_subdirectory(compute)