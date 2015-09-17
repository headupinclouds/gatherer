macro(set_opencv_cmake_args_android)
    set(OPENCV_CMAKE_ARGS 
      #### Repeat HUNTER ARGS ###
      BUILD_DOCS=OFF
      BUILD_TESTS=OFF
      BUILD_PERF_TESTS=OFF
      BUILD_opencv_apps=OFF
      BUILD_EXAMPLES=OFF
      ENABLE_NEON=ON 
      BUILD_ANDROID_SERVICE=OFF
      BUILD_ANDROID_EXAMPLES=OFF
      # Find packages in Hunter (instead of building from OpenCV sources)
      BUILD_ZLIB=OFF
      BUILD_TIFF=OFF
      BUILD_PNG=OFF
      ### Custom ARGS ###
      WITH_IPP=OFF # keep things portable
      WITH_GSTREAMER=OFF
      WITH_OPENCLAMDBLAS=OFF
      WITH_CUDA=OFF
      WITH_FFMPEG=OFF
      WITH_WEBP=OFF
      WITH_OPENEXR=OFF
      BUILD_PACKAGE=OFF
      WITH_GIGEAPI=OFF
      WITH_CUFFT=OFF
      WITH_V4L=OFF
      WITH_OPENCLAMDFFT=OFF
      WITH_VTK=OFF
      BUILD_PERF_TESTS=OFF
      BUILD_DOCS=OFF
      WITH_1394=OFF
      WITH_LIBV4L=OFF
      WITH_EIGEN=OFF
      BUILD_OPENEXR=OFF
      BUILD_FAT_JAVA_LIB=OFF
      BUILD_SHARED_LIBS=OFF
      WITH_OPENCL=ON
      WITH_PVAPI=OFF
      BUILD_TESTS=OFF
      ENABLE_PRECOMPILED_HEADERS=OFF
      OPENCV_WARNINGS_ARE_ERRORS=OFF
)
endmacro(set_opencv_cmake_args_android)