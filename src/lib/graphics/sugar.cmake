# This file generated automatically by:
#   generate_sugar_files.py
# see wiki for more info:
#   https://github.com/ruslo/sugar/wiki/Collecting-sources

if(DEFINED SRC_LIB_GRAPHICS_SUGAR_CMAKE_)
  return()
else()
  set(SRC_LIB_GRAPHICS_SUGAR_CMAKE_ 1)
endif()

include(sugar_files)

sugar_files(
    GATHERER_GRAPHICS_SRC
    GLExtra.cpp
    GLSLShaderProgram.cpp
    GLWarpShader.cpp
    RenderTexture.cpp
    RenderTextureCopy.cpp
    Logger.cpp
)

sugar_files(
    GATHERER_GRAPHICS_HDRS
    GLExtra.h
    GLSLShaderProgram.h
    GLTexture.h
    GLWarpShader.h
    RenderTexture.h
    RenderTextureCopy.h
    gatherer_graphics.h
    Logger.h
)
