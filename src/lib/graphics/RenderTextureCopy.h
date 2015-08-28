//
//  RenderTextureCopy.h
//  SprayCam
//
//  Created by David Hirvonen on 10/14/12.
//  Copyright (c) 2012 David Hirvonen. All rights reserved.
//

#ifndef __SprayCam__RenderTextureCopy__
#define __SprayCam__RenderTextureCopy__

#include <iostream>

#include "graphics/gatherer_graphics.h"
#include "graphics/RenderTexture.h"
#include "graphics/GLSLShaderProgram.h"
#include "graphics/GLExtra.h"
#include <memory>

_GATHERER_GRAPHICS_BEGIN

class RenderTextureCopy : public RenderTexture
{
public:

    typedef RenderTexture Super;

    RenderTextureCopy(int width, int height);
    void SetTextureCoordinates(const GLfloat *textureCoords)
    {
        m_textureCoordinates = textureCoords;
    }
    void SetFrameVertices(const GLfloat *frameVertices)
    {
        m_frameVertices = frameVertices;
    }
    void draw(GLuint texture, GLuint unit );

    virtual void draw();
    virtual void startRender();
    virtual void finishRender();
    void SetTextureUnit( int unit, int texture );

protected:

    void CompileShaders();
    const GLfloat *m_textureCoordinates;
    const GLfloat *m_frameVertices;
    std::auto_ptr<shader_prog> m_pShaderProgram;
    GLint m_UniformTexture;
    GLint m_TextureUnit;
    GLint m_Texture;
    GLint m_UniformAlpha;
};

_GATHERER_GRAPHICS_END

#endif /* defined(__SprayCam__RenderTextureCopy__) */
