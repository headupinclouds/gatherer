//
//  RenderTexture.cpp
//  SprayCam
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

RenderTexture::RenderTexture(GLuint p_width, GLuint p_height) : m_width(p_width), m_height(p_height)
{
    glGenFramebuffers(1, &m_fbo);
    glErrorTest();
    glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);
    glErrorTest();
    newTexture();
}

int RenderTexture::newTexture()
{
    glGenTextures(1, &m_texID);
    glErrorTest();
    glBindTexture(GL_TEXTURE_2D, m_texID);
    glErrorTest();
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glErrorTest();
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glErrorTest();
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glErrorTest();
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glErrorTest();
    glBindTexture(GL_TEXTURE_2D, 0);
    glErrorTest();

    glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);
    glErrorTest();
    glBindTexture(GL_TEXTURE_2D, m_texID);
    glErrorTest();
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_width, m_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
    glErrorTest();
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_texID, 0);
    glErrorTest();

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
    glErrorTest(); //  free our texture
    glDeleteFramebuffers( 1, &m_fbo );
    glErrorTest(); //  free our framebuffer
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
    glErrorTest(); //  bind the framebuffer
    glViewport(0,0,m_width,m_height);
    glErrorTest(); //  set the viewport to our texture dimensions
}

void RenderTexture::finishRender()
{
    //  unbind our framebuffer, return to default state
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glErrorTest();
    //  remember to restore the viewport when you are ready to render to the screen!
}

void RenderTexture::bind(GLenum texture)
{
    glActiveTexture(texture);
    glBindTexture( GL_TEXTURE_2D, m_texID );
}

_GATHERER_GRAPHICS_END

