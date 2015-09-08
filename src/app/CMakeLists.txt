add_subdirectory(camera_test)

if(NOT GATHERER_OPENGL_ES)
  add_subdirectory(preview)
endif()

if(BUILD_QT)
  add_subdirectory(preview-qt)
  if(NOT GATHERER_OPENGL_ES)
    add_subdirectory(opencv-qt-video)
  endif()
endif()
