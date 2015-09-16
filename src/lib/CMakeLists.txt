### Add Sugar packages for cmake wrappers ###
hunter_add_package(Sugar)
include(${SUGAR_ROOT}/cmake/Sugar)
include(sugar_include)
sugar_include(.)

hunter_add_package(spdlog)
find_package(spdlog CONFIG REQUIRED)

add_library(gatherer_graphics STATIC ${GATHERER_GRAPHICS_SRC} ${GATHERER_GRAPHICS_HDRS})
target_link_libraries(gatherer_graphics PRIVATE ${OpenCV_LIBS} spdlog::spdlog)
if(NOT GATHERER_OPENGL_ES)
  target_link_libraries(gatherer_graphics PRIVATE glew)
endif()

if(GATHERER_OPENGL_ES)
  target_compile_definitions(gatherer_graphics PUBLIC GATHERER_OPENGL_ES)
endif()

set(GATHERER_LIBS
  gatherer_graphics
  ## TODO
)

foreach(library ${GATHERER_LIBS})
  set_property(TARGET ${library} PROPERTY FOLDER "libs/gatherer")
endforeach()
