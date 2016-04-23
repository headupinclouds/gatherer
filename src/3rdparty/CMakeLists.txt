# TODO: need to add opengl support for non mobile platforms
add_subdirectory(ogles_gpgpu)

# Move ogles_gpgpu targets to 3rdparty folder
set_property(TARGET ogles_gpgpu PROPERTY FOLDER "libs/3rdparty")

