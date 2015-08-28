//
//  RenderTexture.h
//  SprayCam
//
//  Created by David Hirvonen on 9/24/12.
//  Copyright (c) 2012 David Hirvonen. All rights reserved.
//

#ifndef __SprayCam__RenderTexture__
#define __SprayCam__RenderTexture__

#include "gatherer_graphics.h"
#include <opencv2/core/core.hpp>
#include <vector>

_GATHERER_GRAPHICS_BEGIN

class RenderTexture
{
protected:

    //  we don't want to allow copies to be made
    RenderTexture&  operator = (const RenderTexture& other)
    {
        return *this;
    }
    RenderTexture(const RenderTexture& other) {}

protected:

    std::vector<GLuint> m_texIDs;
    GLuint m_texID;
    GLuint m_fbo;
    GLuint m_width;
    GLuint m_height;

public:

    enum {  ATTRIB_VERTEX, ATTRIB_TEXTUREPOSITION, NUM_ATTRIBUTES };

    RenderTexture(GLuint p_width, GLuint p_height);
    virtual ~RenderTexture();
    virtual void startRender();
    virtual void finishRender();
    void bind(GLenum texture);
    virtual void draw() = 0;
    virtual GLuint render();
    int newTexture();
    GLuint getTexture() const { return m_texID; }
    GLuint getWidth() const { return m_width; }
    GLuint getHeight() const { return m_height; }
};

void LoadFrameTexture( const cv::Mat &image, GLuint texture );

_GATHERER_GRAPHICS_END

#endif /* defined(__SprayCam__RenderTexture__) */
