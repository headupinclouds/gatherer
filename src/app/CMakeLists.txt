add_subdirectory(preview)

if(BUILD_QT)
  add_subdirectory(preview-qt)
  add_subdirectory(opencv-qt-video)
endif()