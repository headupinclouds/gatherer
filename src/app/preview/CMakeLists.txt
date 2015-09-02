include_directories(${GLFW_INCLUDE_DIRS})

### preview ###
add_executable(preview preview.cpp)
target_link_libraries(preview 
  PRIVATE
  gatherer_graphics 
  ${OpenCV_LIBS} 
  ${GLFW_LIBRARIES}
  glew
  "-framework OpenGL"
  "-framework IOKit"
  "-framework Cocoa"
  "-framework OpenGL"
  "-framework QuartzCore"
  "-framework CoreMedia"
  "-framework AVFoundation"
  "-framework AppKit"
  "-framework CoreData"
  "-framework Foundation"

)
install(TARGETS preview DESTINATION bin)
set_property(TARGET preview PROPERTY FOLDER "app/console")
