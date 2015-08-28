//
//  RenderTextureCopy.cpp
//  SprayCam
//
//  Created by David Hirvonen on 10/14/12.
//  Copyright (c) 2012 David Hirvonen. All rights reserved.
//

#include "graphics/RenderTextureCopy.h"
#include "graphics/GLExtra.h"

_GATHERER_GRAPHICS_BEGIN

RenderTextureCopy::RenderTextureCopy(int width, int height) : Super(width, height)
{
    CompileShaders();
}

void RenderTextureCopy::CompileShaders() //
{
    const char *kVertexShaderString = R"(
        attribute vec4 position;
        attribute vec4 inputTextureCoordinate;
        varying vec2 textureCoordinate;
        void main()
        {
            gl_Position = position;
            textureCoordinate = inputTextureCoordinate.xy;
        })";

    const char *kFragmentShaderString = R"(
        varying highp vec2 textureCoordinate;
        uniform sampler2D texture;
        void main()
        {
            gl_FragColor = texture2D(texture, textureCoordinate);
        })";

    const GLchar * vShaderStr[] = { kVertexShaderString };
    const GLchar * fShaderStr[] = { kFragmentShaderString };

    std::vector< std::pair<int, const char *> > attributes;
    attributes.push_back( std::pair<int, const char*>(ATTRIB_VERTEX, "position") );
    attributes.push_back( std::pair<int, const char*>(ATTRIB_TEXTUREPOSITION, "inputTextureCoordinate") );

    try
    {
        m_pShaderProgram = std::auto_ptr< shader_prog>( new shader_prog(vShaderStr, fShaderStr, attributes) );
    }
    catch(std::logic_error &e)
    {
        std::cerr << e.what() << std::endl;
        throw;
    }
    m_UniformTexture =  m_pShaderProgram->GetUniformLocation("texture");
}

void RenderTextureCopy::SetTextureUnit( int unit, int texture )
{
    m_TextureUnit = unit;
    m_Texture = texture;
}

void RenderTextureCopy::startRender()
{
    (void) newTexture(); // create a new texture to render into
    RenderTexture::startRender(); // glBindFrameBuffer(...m_fBO);
}
void RenderTextureCopy::finishRender()
{
    RenderTexture::finishRender();
}

void RenderTextureCopy::draw()
{
    glClearColor(1.0f, 0.0f, 0.0f, 1.0f);
    glErrorTest();
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
    glErrorTest();
    draw(m_Texture, m_TextureUnit);
    glFlush();
}

void RenderTextureCopy::draw(GLuint texture, GLuint unit )
{
    glActiveTexture(GL_TEXTURE0 + unit);
    glBindTexture(GL_TEXTURE_2D, texture);
    glErrorTest();

    (*m_pShaderProgram)();

    glUniform1i(m_UniformTexture, unit);
    glErrorTest(); // TODO: 4 should be texture unit input variable

    glVertexAttribPointer(ATTRIB_VERTEX, 4, GL_FLOAT, 0, 0, m_frameVertices);
    glErrorTest();
    glEnableVertexAttribArray(ATTRIB_VERTEX);
    glErrorTest();
    glVertexAttribPointer(ATTRIB_TEXTUREPOSITION, 2, GL_FLOAT, 0, 0, m_textureCoordinates);
    glErrorTest();
    glEnableVertexAttribArray(ATTRIB_TEXTUREPOSITION);
    glErrorTest();

    // Render to texture associated with currently bound RenderTexture::defaultFramebuffer
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    glErrorTest();
}

_GATHERER_GRAPHICS_END
