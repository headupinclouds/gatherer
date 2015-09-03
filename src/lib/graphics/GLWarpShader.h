//
//  GLWarpShader.h
//  gatherer
//
//  Created by David Hirvonen on 8/28/15.
//
//

#ifndef __gatherer__GLWarpShader__
#define __gatherer__GLWarpShader__

#include <stdio.h>

#include "graphics/gatherer_graphics.h"
#include "graphics/GLSLShaderProgram.h"
#include <opencv2/core/core.hpp>

_GATHERER_GRAPHICS_BEGIN

class WarpShader
{
public:

    WarpShader(const cv::Size &size, const cv::Point2f &resolution);
    void compileShadersPlanar();
    GLuint operator()(int texture);
    void operator()(int texture, const cv::Matx33f &H);

protected:

    int m_count = 0;

    cv::Size m_size;
    cv::Point2f m_resolution;

    // Planar shader program
    std::unique_ptr<gatherer::graphics::shader_prog> m_pPlanarShaderProgram;
    GLint m_PlanarUniformMVP;
    GLint m_PlanarUniformTexture;
};

_GATHERER_GRAPHICS_END

#endif /* defined(__gatherer__GLWarpShader__) */
