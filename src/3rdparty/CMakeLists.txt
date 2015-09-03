if(is_ios OR ANDROID)
  # TODO: need to add opengl support for non mobile platforms
  add_subdirectory(ogles_gpgpu)
endif()