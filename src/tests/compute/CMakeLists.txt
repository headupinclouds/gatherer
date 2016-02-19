if(BoostCompute_FOUND)
  add_executable(test-compute test-compute.cpp)
  target_link_libraries(test-compute BoostCompute::boost_compute)
endif()

