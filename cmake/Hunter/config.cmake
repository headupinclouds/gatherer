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
elseif(${is_linux})
  message("is_linux =======================================================================")
  include(SetOpenCVCMakeArgs-nix) 
  set_opencv_cmake_args_nix()
elseif(${is_msvc})
  message("is_msvc=========================================================================")
  include(SetOpenCVCMakeArgs-windows) 
  set_opencv_cmake_args_windows()
endif()

list(APPEND OPENCV_CMAKE_ARGS 
  BUILD_opencv_world=ON 
  BUILD_EIGEN=OFF
  CMAKE_CXX_FLAGS=-Wno-c++11-narrowing
)

hunter_config(PNG VERSION 1.6.16-p4)
hunter_config(TIFF VERSION 4.0.2-p3)
hunter_config(cereal VERSION 1.1.2-p5)
hunter_config(OpenCV VERSION 3.0.0-p6 CMAKE_ARGS "${OPENCV_CMAKE_ARGS}")
hunter_config(spdlog VERSION 1.0.0-p0)
hunter_config(Qt VERSION 5.5.1-cvpixelbuffer-2-p3)
hunter_config(BoostCompute VERSION 0.5-p0)

#  CMAKE_ARGS 
#  CMAKE_REQUIRED_FLAGS "-Wno-error=unused-command-line-argument-hard-error-in-future"

# From hunter/cmake/configs/default.cmake
if(ANDROID)
  string(COMPARE EQUAL "${ANDROID_NATIVE_API_LEVEL}" "" _is_empty)
  if(_is_empty)
    hunter_user_error("ANDROID_NATIVE_API_LEVEL is empty")
  endif()

  string(COMPARE EQUAL "${ANDROID_NATIVE_API_LEVEL}" "21" _is_api_21)
  string(COMPARE EQUAL "${ANDROID_NATIVE_API_LEVEL}" "19" _is_api_19)
  string(COMPARE EQUAL "${ANDROID_NATIVE_API_LEVEL}" "16" _is_api_16)

  if(_is_api_21)
    hunter_config(Android-Google-APIs VERSION 21)
    hunter_config(Android-Google-APIs-Intel-x86-Atom-System-Image VERSION 21)
    hunter_config(Android-Intel-x86-Atom-System-Image VERSION 21)
    hunter_config(Android-SDK-Platform VERSION 21_r02)
    hunter_config(Sources-for-Android-SDK VERSION 21)
  elseif(_is_api_19)
    hunter_config(Android-Google-APIs VERSION 19)
    hunter_config(Android-Intel-x86-Atom-System-Image VERSION 19)
    hunter_config(Android-SDK-Platform VERSION 19_r04)
    hunter_config(Sources-for-Android-SDK VERSION 19)

    # There is no package for API 19, use version 21
    # (simplify build of Android-SDK package)
    hunter_config(Android-Google-APIs-Intel-x86-Atom-System-Image VERSION 21)
  elseif(_is_api_16)
    hunter_config(Android-Google-APIs VERSION 16)
    hunter_config(Android-Intel-x86-Atom-System-Image VERSION 16)
    hunter_config(Android-SDK-Platform VERSION 16_r05)
    hunter_config(Sources-for-Android-SDK VERSION 16)

    # There is no package for API 16, use version 21
    # (simplify build of Android-SDK package)
    hunter_config(Android-Google-APIs-Intel-x86-Atom-System-Image VERSION 21)
  else()
    hunter_user_error(
        "Android API (ANDROID_NATIVE_API_LEVEL)"
        " Expected: `21`, `19`, `16`"
        " Got: `${ANDROID_NATIVE_API_LEVEL}`"
    )
  endif()
endif()
