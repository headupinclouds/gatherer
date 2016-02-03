# GTEST:
#
# https://github.com/forexample/gtest-pass-filename/blob/master/CMakeLists.txt
# https://github.com/ruslo/hunter/blob/master/examples/GTest/CMakeLists.txt
# https://github.com/ruslo/hunter/wiki/pkg.gtest

#if(is_osx OR is_ios) 
if(is_osx)

  # Not avabile on Android?
  hunter_add_package(GTest)
  find_package(GTest CONFIG REQUIRED)

  # Portable QT context for desktop and mobile systems:
  add_subdirectory(qt_ogles_gpgpu)

endif()
