### Add Sugar packages for cmake wrappers ###
hunter_add_package(Sugar)
include(${SUGAR_ROOT}/cmake/Sugar)
include(sugar_include)
sugar_include(.)

hunter_add_package(spdlog)
find_package(spdlog CONFIG REQUIRED)

add_library(gatherer_graphics STATIC ${GATHERER_GRAPHICS_SRC} ${GATHERER_GRAPHICS_HDRS})
target_link_libraries(gatherer_graphics PUBLIC ${OpenCV_LIBS} spdlog::spdlog)
if(NOT GATHERER_OPENGL_ES)
  target_link_libraries(gatherer_graphics PRIVATE glew)
endif()

if(GATHERER_OPENGL_ES)
  target_compile_definitions(gatherer_graphics PUBLIC GATHERER_OPENGL_ES)
endif()

target_compile_definitions(
    gatherer_graphics
    PUBLIC "$<$<CONFIG:Debug>:GATHERER_ENABLE_OPENGL_DEBUG>"
)

set(GATHERER_LIBS
  gatherer_graphics
  ## TODO
)

foreach(library ${GATHERER_LIBS})
  set_property(TARGET ${library} PROPERTY FOLDER "libs/gatherer")
endforeach()

###################
##### install #####
###################

set(generated_dir "${CMAKE_CURRENT_BINARY_DIR}/generated")
set(include_install_dir "include")
set(config_install_dir "share/${CMAKE_PROJECT_NAME}")
set(version_config "${generated_dir}/${CMAKE_PROJECT_NAME}ConfigVersion.cmake")
set(project_config "${generated_dir}/${CMAKE_PROJECT_NAME}Config.cmake")
set(targets_export_name "${CMAKE_PROJECT_NAME}Targets")
set(namespace "${CMAKE_PROJECT_NAME}::")

include(CMakePackageConfigHelpers)
write_basic_package_version_file("${version_config}" VERSION "${gatherer_VERSION}" COMPATIBILITY SameMajorVersion)

# Note: variable 'targets_export_name' used
configure_file("${CMAKE_SOURCE_DIR}/cmake/Config.cmake.in" "${project_config}" @ONLY)

#install(FILES "${PROJECT_BINARY_DIR}/gatherer_export.h" DESTINATION "${include_install_dir}")

install(
  TARGETS ${GATHERER_LIBS}
  EXPORT "${targets_export_name}"
  LIBRARY DESTINATION "lib"
  ARCHIVE DESTINATION "lib"
  RUNTIME DESTINATION "bin"
  INCLUDES DESTINATION "${include_install_dir}"
  )

install(
  FILES 
  ${TARGET_PUBLIC_HDRS}
  DESTINATION "${include_install_dir}/${CMAKE_PROJECT_NAME}"
  )

install(
  FILES "${project_config}" "${version_config}"
  DESTINATION "${config_install_dir}"
  )

install(
  EXPORT "${targets_export_name}"
  NAMESPACE "${namespace}"
  DESTINATION "${config_install_dir}"
  )

###################

add_subdirectory("OGLESGPGPUTest")
