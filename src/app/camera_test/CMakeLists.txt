add_executable(camera_test camera_test.cpp)

target_link_libraries(camera_test PRIVATE ${OpenCV_LIBS})

if(IOS)
  target_link_libraries(camera_test PRIVATE "-fobjc-arc")
endif()

install(TARGETS camera_test DESTINATION bin)
set_property(TARGET camera_test PROPERTY FOLDER "app/console")

if(ANDROID)
  hunter_add_package(Android-Apk)
  list(APPEND CMAKE_MODULE_PATH "${ANDROID-APK_ROOT}")

  include(AndroidApk)

  android_add_test(NAME CameraTest COMMAND camera_test)
endif()
