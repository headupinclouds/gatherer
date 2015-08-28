//
//  GLTexture.h
//  SprayCam
//
//  Created by David Hirvonen on 12/11/12.
//  Copyright (c) 2012 David Hirvonen. All rights reserved.
//

#ifndef SprayCam_GLTexture_h
#define SprayCam_GLTexture_h

#include "graphics/gatherer_graphics.h"
#include <opencv2/core/core.hpp>

_GATHERER_GRAPHICS_BEGIN

class GLTexture
{
public:
    GLTexture() { init(); }
    GLTexture(const cv::Mat &image) { init(); load(image); }
    void init()
    {
        glGenTextures(1,&m_texture);
        glBindTexture(GL_TEXTURE_2D, m_texture);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    }
    virtual ~GLTexture() { glDeleteTextures((GLsizei)1, (GLuint *)&m_texture); }
    virtual operator unsigned int() const { return m_texture; }
    unsigned int & get() { return m_texture; }
    void load( const cv::Mat &image )
    {
        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
        glBindTexture(GL_TEXTURE_2D, m_texture);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image.cols, image.rows, 0, GL_BGR, GL_UNSIGNED_BYTE, image.ptr());
        glFlush();
    }
    
protected:
    unsigned int m_texture;
};


// Source: dhirvonen@elucideye.com
// drishti/lib/graphics/graphics/MosaicRenderGL.cpp: GLTexRect
class GLTexRect : public cv::Rect
{
public:
    GLTexRect(const cv::Rect &roi) : cv::Rect(roi) {}
    GLTexRect(const cv::Size &size) : cv::Rect({0,0}, size) {}
    GLTexRect(int x, int y, int w, int h) : cv::Rect(x, y, w, h) { }
    const cv::Point tr() { return cv::Point(br().x, tl().y); };
    const cv::Point bl() { return cv::Point(tl().x, br().y); };
    std::vector<cv::Point2f> GetVertices() { return std::vector<cv::Point2f>{ tl(), tr(), bl(), br() }; }
    std::vector<cv::Point2f> GetTextureCoordinates(const cv::Rect &roi)
    {
        GLTexRect roi_(roi);
        std::vector<cv::Point2f> coords{ roi_.tl(), roi_.tr(), roi_.bl(), roi_.br() };
        for(auto &p : coords)
        {
            p.x *= (1.0 / width);
            p.y *= (1.0 / height);
        }
        return coords;
    }
    
    static std::vector<float> GetTextureCoordinates() { return std::vector<float> { 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f }; }
};



_GATHERER_GRAPHICS_END

#endif
