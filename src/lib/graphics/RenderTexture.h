//
//  RenderTexture.h
//  gatherer
//
//  Created by David Hirvonen on 9/24/12.
//  Copyright (c) 2012 David Hirvonen. All rights reserved.
//

#ifndef __gatherer__RenderTexture__
#define __gatherer__RenderTexture__

#include "graphics/gatherer_graphics.h"
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

    float m_resX;
    float m_resY;

public:

    enum {  ATTRIB_VERTEX, ATTRIB_TEXTUREPOSITION, NUM_ATTRIBUTES };

    RenderTexture(GLuint p_width, GLuint p_height, float resX=1.f, float resY=1.f);
    virtual ~RenderTexture();
    virtual void startRender();
    virtual void finishRender();
    void bind();
    virtual void draw() = 0;
    virtual GLuint render();
    int newTexture();
    GLuint getTexture() const { return m_texID; }
    GLuint getWidth() const { return m_width; }
    GLuint getHeight() const { return m_height; }
};

void LoadFrameTexture( const cv::Mat &image, GLuint texture );

_GATHERER_GRAPHICS_END

#endif /* defined(__gatherer__RenderTexture__) */
