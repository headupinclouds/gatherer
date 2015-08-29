//
//  RenderTexture.cpp
//  gatherer
//
//  Created by David Hirvonen on 9/24/12.
//  Copyright (c) 2012 David Hirvonen. All rights reserved.
//

#include "graphics/gatherer_graphics.h"
#include "graphics/RenderTexture.h"
#include "graphics/GLExtra.h"
#include <opencv2/imgproc/imgproc.hpp>
#include <stdio.h>

_GATHERER_GRAPHICS_BEGIN

void LoadFrameTexture( const cv::Mat &image, GLuint texture)
{
    // Do BGR -> BGRA conversion
    cv::Mat4b bytes(image.size());
    cv::cvtColor(image, bytes, cv::COLOR_BGR2BGRA);

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glErrorTest();
    glBindTexture(GL_TEXTURE_2D, texture);
    glErrorTest();

#if __ANDROID__
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image.cols, image.rows, 0, GL_RGBA, GL_UNSIGNED_BYTE, bytes.ptr());
    glErrorTest();
#else
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image.cols, image.rows, 0, GL_BGRA, GL_UNSIGNED_BYTE, bytes.ptr());
    glErrorTest();
#endif
    glFlush();
}

RenderTexture::RenderTexture(GLuint p_width, GLuint p_height, float resX, float resY)
: m_width(p_width)
, m_height(p_height)
, m_resX(resX)
, m_resY(resY)
{
    glGenFramebuffers(1, &m_fbo);
    glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);
    newTexture();
}

int RenderTexture::newTexture()
{
    glGenTextures(1, &m_texID);
    glBindTexture(GL_TEXTURE_2D, m_texID);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glBindTexture(GL_TEXTURE_2D, 0);

    glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);
    glBindTexture(GL_TEXTURE_2D, m_texID);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_width, m_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_texID, 0);

    //  you can check to see if the framebuffer is 'complete' with no errors
    if(glCheckFramebufferStatus(GL_FRAMEBUFFER)!=GL_FRAMEBUFFER_COMPLETE)
    {
        printf("RENDER TEXTURE ERROR!\n");
        fflush(stdout);
    }
    glBindTexture(GL_TEXTURE_2D, 0);    // unbind frame buffer and texture
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    return m_texID;
}

RenderTexture::~RenderTexture()
{
    glDeleteTextures( 1, &m_texID );
    glDeleteFramebuffers( 1, &m_fbo );
}

GLuint RenderTexture::render()
{
    startRender();
    draw();
    finishRender();
    return m_texID;
}

void RenderTexture::startRender()
{
    glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);
    glViewport(0,0, int(m_resX * m_width + 0.5f), int(m_resY * m_height + 0.5f));
}

void RenderTexture::finishRender()
{
    //  unbind our framebuffer, return to default state
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    //  remember to restore the viewport when you are ready to render to the screen!
}

void RenderTexture::bind()
{
    glBindTexture( GL_TEXTURE_2D, m_texID );
}

_GATHERER_GRAPHICS_END

