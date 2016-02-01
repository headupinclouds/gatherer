# GTEST:
#
# https://github.com/forexample/gtest-pass-filename/blob/master/CMakeLists.txt
# https://github.com/ruslo/hunter/blob/master/examples/GTest/CMakeLists.txt
# https://github.com/ruslo/hunter/wiki/pkg.gtest

hunter_add_package(GTest)
find_package(GTest CONFIG REQUIRED)

add_subdirectory(ogles_gpgpu)
