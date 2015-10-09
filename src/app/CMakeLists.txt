add_subdirectory(camera_test)

if(NOT GATHERER_OPENGL_ES)
  add_subdirectory(preview)
  add_subdirectory(ogles_gpgpu_test)
endif()

if(BUILD_QT)
  add_subdirectory(qt-camera-demo)
  add_subdirectory(preview-qt)
  if(NOT GATHERER_OPENGL_ES)
    add_subdirectory(opencv-qt-video)
  endif()
endif()
