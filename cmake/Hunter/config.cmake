message(">:>:>:>:>:>:>:>:>:>:>:>:>:>:>:>:>: ${CMAKE_MODULE_PATH} <:<:<:<:<:<:<:<:<:<:<:<:<:<:<:<:<:<:<:<:<:<:<:")

### OpenCV
if(ANDROID)
  message("ANDROID =====================================================================")
  include(SetOpenCVCMakeArgs-android) 
  set_opencv_cmake_args_android()
elseif(is_ios)
  message("is_ios ======================================================================")
  include(SetOpenCVCMakeArgs-iOS) 
  set_opencv_cmake_args_ios()
elseif(APPLE) 
  message("APPLE =======================================================================")
  include(SetOpenCVCMakeArgs-osx)
  set_opencv_cmake_args_osx()
elseif(is_linux)
  message("is_linux ====================================================================")
  set(OPENCV_CMAKE_ARGS BUILD_SHARED_LIBS=OFF WITH_FFMPEG=OFF WITH_IPP=OFF) #TBD
elseif(MSCV)
  message("MSVC ========================================================================")
  set(OPENCV_CMAKE_ARGS BUILD_SHARED_LIBS=OFF WITH_FFMPEG=OFF WITH_IPP=OFF) #TBD
endif()

list(APPEND OPENCV_CMAKE_ARGS 
  BUILD_opencv_world=ON 
  BUILD_EIGEN=OFF
  CMAKE_CXX_FLAGS=-Wno-c++11-narrowing
)
#hunter_config(OpenCV VERSION ${HUNTER_OpenCV_VERSION} CMAKE_ARGS "${OPENCV_CMAKE_ARGS}")
hunter_config(OpenCV VERSION 3.0.0-p4 CMAKE_ARGS "${OPENCV_CMAKE_ARGS}")


hunter_config(Qt VERSION ${HUNTER_Qt_VERSION})

#  CMAKE_ARGS 
#  CMAKE_REQUIRED_FLAGS "-Wno-error=unused-command-line-argument-hard-error-in-future"
