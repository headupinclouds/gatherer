add_executable(camera_test camera_test.cpp)

target_link_libraries(camera_test PRIVATE ${OpenCV_LIBS})

if(IOS)
  target_link_libraries(camera_test PRIVATE "-fobjc-arc")
endif()

install(TARGETS camera_test DESTINATION bin)
set_property(TARGET camera_test PROPERTY FOLDER "app/console")
