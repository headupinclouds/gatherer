//
//  GLWarpShader.cpp
//  gatherer
//
//  Created by David Hirvonen on 8/28/15.
//
//

#include "graphics/GLWarpShader.h"
#include "graphics/RenderTexture.h"
#include "graphics/GLTexture.h"
#include "graphics/GLExtra.h"

_GATHERER_GRAPHICS_BEGIN

WarpShader::WarpShader(int width, int height) : m_size(width, height)
{
    compileShadersPlanar();
}

void WarpShader::compileShadersPlanar()
{
    using gatherer::graphics::RenderTexture;
    
    // vertex shader
    const char *kPlanarVertexShaderString = R"(
    attribute vec4 position;
    attribute vec4 inputTextureCoordinate;
    varying vec2 textureCoordinate;
    uniform mat4 modelViewProjMatrix;
    void main()
    {
        gl_Position = modelViewProjMatrix * position;
        textureCoordinate = inputTextureCoordinate.xy;
    })";
    
    const char *kPlanarFragmentShaderString = R"(
    //varying highp vec2 textureCoordinate;
    varying vec2 textureCoordinate;
    uniform sampler2D texture;
    void main()
    {
        gl_FragColor = texture2D(texture, textureCoordinate);
    })";
    
    // m_frameVertices, i.e. (0, 0, w, h)
    // m_textureCoordinates, i.e. (-1, -1, 1, 1)
    const GLchar * vShaderStr[] = { kPlanarVertexShaderString };
    const GLchar * fShaderStr[] = { kPlanarFragmentShaderString };
    std::vector< std::pair<int, const char *> > attributes;
    attributes.push_back( std::pair<int, const char*>(RenderTexture::ATTRIB_VERTEX, "position") );
    attributes.push_back( std::pair<int, const char*>(RenderTexture::ATTRIB_TEXTUREPOSITION, "inputTextureCoordinate") );
    
    m_pPlanarShaderProgram = make_unique<gatherer::graphics::shader_prog>(vShaderStr, fShaderStr, attributes);
    m_PlanarUniformMVP = m_pPlanarShaderProgram->GetUniformLocation("modelViewProjMatrix");
    m_PlanarUniformTexture =  m_pPlanarShaderProgram->GetUniformLocation("texture");
}

GLuint WarpShader::operator()(int texture)
{
    const float w = m_size.width;
    const float h = m_size.height;
    
    // Transform to rectangle defined by (-1,-1) (+1,+1)
    cv::Matx33f T(1,0,-w/2,0,1,-h/2,0,0,1);
    cv::Matx33f S(2.0/w,0,0,0,2.0/h,0,0,0,1);
    cv::Matx33f H = S * T;
    
    
    // Otionally add some effect:
    const float theta = (m_count % 360) * M_PI / 180.f;
    const float c = std::cos(theta);
    const float s = std::sin(theta);
    cv::Matx33f R(+c, -s, 0, +s, +c, 0, 0, 0, 1);
    cv::Matx33f Q = T.inv() * R * T;
    
    (*this)(texture, H * Q);
    
    m_count ++;
    
    return 0;
}

void WarpShader::operator()(int texture, const cv::Matx33f &H)
{
    using gatherer::graphics::RenderTexture;
    using gatherer::graphics::GLTexRect;
    
    cv::Matx44f MVPt;
    R3x3To4x4(H.t(), MVPt);
    
    GLTexRect roi(m_size);
    
    // {-1,-1}, {1,-1}, {-1,1}, {1,1}
    auto vertices = roi.GetVertices();
    auto coords = GLTexRect::GetTextureCoordinates();
    
    std::vector<cv::Vec4f> vertices4d;
    for(const auto &p : vertices)
        vertices4d.emplace_back(p.x, (m_size.height - p.y), 0, 1);
    
    (*m_pPlanarShaderProgram)();
    glUniformMatrix4fv(m_PlanarUniformMVP, 1, 0, (GLfloat *)&MVPt(0,0));
    
    glClearColor(0, 0, 0, 0);
    glClear(GL_COLOR_BUFFER_BIT);
    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, texture);
    
    glVertexAttribPointer(RenderTexture::ATTRIB_VERTEX, 4, GL_FLOAT, 0, 0, &vertices4d[0][0]);
    glEnableVertexAttribArray(RenderTexture::ATTRIB_VERTEX);
    glVertexAttribPointer(RenderTexture::ATTRIB_TEXTUREPOSITION, 2, GL_FLOAT, 0, 0, &coords[0]);
    glEnableVertexAttribArray(RenderTexture::ATTRIB_TEXTUREPOSITION);
    
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
}

_GATHERER_GRAPHICS_END