// OS X:
// brew tap homebrew/versions
// brew install --build-bottle --static glfw3


#include <GL/glew.h> // include GLEW and new version of GL on Windows
#include <GLFW/glfw3.h>

#include "graphics/GLWarpShader.h"
#include "graphics/GLTexture.h"
#include "graphics/RenderTexture.h"

#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>

#include <iterator>
#include <locale>
#include <iostream>
#include <fstream>

_GATHERER_GRAPHICS_BEGIN

// See: http://www.codeincodeblock.com/2013/05/introduction-to-modern-opengl-3x-with.html
class GLContextWindow
{
public:
    GLContextWindow(const cv::Size &size, const std::string &name) : m_size(size), m_name(name)
    {
        if (!glfwInit())
        {
            m_initialized = true;
            throw std::runtime_error("glfwInit failed");
        }

        // Note: Pecularitiy for retina displays must be handled
        // http://www.glfw.org/faq.html
        // 4.3 - Why is my output in the lower-left corner of the window?
        // You are passing the window size, which is in screen coordinates, to glViewport,
        // which works with pixels. On OS X with a Retina display, and possibly on other
        // platforms in the future, screen coordinates and pixels do not map 1:1.
        // Use the framebuffer size, which is in pixels, instead of the window size
        //
        // see: glfwGetFramebufferSize(window, &width, &height);
        //
        // or for resizable window
        //
        // see: glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

        m_window = glfwCreateWindow(m_size.width, m_size.height, "Frame", NULL, NULL);
        if(!m_window)
            throw std::runtime_error("glfwOpenWindow failed.");
        
        glfwMakeContextCurrent(m_window);
        
        //glewExperimental = GL_TRUE; //stops glew crashing on OSX :-/
        
        if(glewInit() != GLEW_OK)
            throw std::runtime_error("glewInit failed");
        
        // print out some info about the graphics drivers
        std::cout << "OpenGL version: " << glGetString(GL_VERSION) << std::endl;
        std::cout << "GLSL version: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;
        std::cout << "Vendor: " << glGetString(GL_VENDOR) << std::endl;
        std::cout << "Renderer: " << glGetString(GL_RENDERER) << std::endl;
    }
    
    cv::Point2f getResolution() const
    {
        int width, height;
        glfwGetFramebufferSize(m_window, &width, &height);
        return cv::Point2f(float(width)/float(m_size.width), float(height)/float(m_size.height));
    }
    
    ~GLContextWindow()
    {
        //TODO: other tear down
        if(m_window)
            glfwDestroyWindow(m_window);
        
        if(m_initialized)
            glfwTerminate();
    }
    
    operator GLFWwindow * () const { return m_window; }
    
    bool m_initialized = false;
    cv::Size m_size;
    std::string m_name;
    GLFWwindow* m_window;
};


class FXShader : public gatherer::graphics::RenderTexture
{
public:

    FXShader(const cv::Size &size, const cv::Point2f &resolution)
    : gatherer::graphics::RenderTexture(size.width, size.height, resolution.x, resolution.y)
    , m_size(size)
    , m_count(0)
    {
        compileShaders();
    }
    
    // TODO: Base class needs a clear API
    GLuint operator()(GLuint texture)
    {
        m_inputTexture = texture;
        
        render(); // virtual method
        
        return getTexture();
    }

    // Called from base class render sequence: start*; draw(); finish*;
    virtual void draw()
    {
        GLTexRect roi(m_size);
        
        // {-1,-1}, {1,-1}, {-1,1}, {1,1}

        std::vector<cv::Point2f> vertices = { {-1.f, -1.f}, {+1.f, -1.f}, {-1.f, +1.f}, {+1.f, +1.f} };
        //auto vertices = roi.GetVertices();
        auto coords = GLTexRect::GetTextureCoordinates();
        
        std::vector<cv::Vec4f> vertices4d;
        for(const auto &p : vertices)
            vertices4d.emplace_back(p.x, p.y, 0, 1);
        
        (*m_pFXShaderProgram)();
        
        glClearColor(0, 0, 0, 0);
        glClear(GL_COLOR_BUFFER_BIT);
        glBindTexture(GL_TEXTURE_2D, m_inputTexture);
        
        glUniform1f(m_FXUniformTime, float(m_count)/10.f);
        
        glVertexAttribPointer(RenderTexture::ATTRIB_VERTEX, 4, GL_FLOAT, 0, 0, &vertices4d[0][0]);
        glEnableVertexAttribArray(RenderTexture::ATTRIB_VERTEX);
        glVertexAttribPointer(RenderTexture::ATTRIB_TEXTUREPOSITION, 2, GL_FLOAT, 0, 0, &coords[0]);
        glEnableVertexAttribArray(RenderTexture::ATTRIB_TEXTUREPOSITION);
        
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

        glFlush();
        
        m_count++;
    }
    
    void compileShaders()
    {
        using gatherer::graphics::RenderTexture;
        
        // vertex shader
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
        //varying highp vec2 textureCoordinate; /* opengl */
        varying vec2 textureCoordinate;         /* opengl es 2.0 */
        uniform sampler2D texture;
        uniform float time;
        void main()
        {
            vec4 delta = vec4(cos(time+textureCoordinate.x), cos(time+textureCoordinate.y), 0.0, 0.0);
            vec4 value = texture2D(texture, textureCoordinate);
            gl_FragColor = value + delta;
        })";
        
        // m_frameVertices, i.e. (0, 0, w, h)
        // m_textureCoordinates, i.e. (-1, -1, 1, 1)
        const GLchar * vShaderStr[] = { kVertexShaderString };
        const GLchar * fShaderStr[] = { kFragmentShaderString };
        std::vector< std::pair<int, const char *> > attributes;
        attributes.push_back( std::pair<int, const char*>(RenderTexture::ATTRIB_VERTEX, "position") );
        attributes.push_back( std::pair<int, const char*>(RenderTexture::ATTRIB_TEXTUREPOSITION, "inputTextureCoordinate") );
        
        m_pFXShaderProgram = make_unique<gatherer::graphics::shader_prog>(vShaderStr, fShaderStr, attributes);
        m_FXUniformTexture =  m_pFXShaderProgram->GetUniformLocation("texture");
        m_FXUniformTime = m_pFXShaderProgram->GetUniformLocation("time");
    }

    int m_count = 0;
    cv::Size m_size;
    std::unique_ptr<gatherer::graphics::shader_prog> m_pFXShaderProgram;
    GLuint m_FXUniformTexture;
    GLuint m_FXUniformTime;
    GLuint m_inputTexture;
};

_GATHERER_GRAPHICS_END

const char *version = "v0.1";

#define SECOND_FIELD ""
static bool has(cv::CommandLineParser &p, const std::string &tag) { return p.has(tag); }
void print(cv::CommandLineParser &p) { p.printMessage(); }

int sanity(int argc, char *argv[], cv::CommandLineParser &parser)
{
    if(argc < 2 || has(parser, "help"))
    {
        print(parser);
        return 1;
    }
    else if(has(parser, "build"))
    {
        std::cout << cv::getBuildInformation() << std::endl;
        return 1;
    }
    else if(has(parser, "version"))
    {
        std::cout << argv[0] << " v" << version << std::endl;
        return 1;
    }
    return 0;
}

const char *keys =
{
    "{  " SECOND_FIELD " index     | 0     | camera index                           }"
    "{i " SECOND_FIELD " input     |       | input filename                         }"
    "{v " SECOND_FIELD " verbose   | false | print verbose diagnostics              }"
    "{b " SECOND_FIELD " build     | false | print the OpenCV build information     }"
    "{h " SECOND_FIELD " help      | false | print help message                     }"
    "{n " SECOND_FIELD " version   | false | print the application version          }"
};

#define USE_GL_WINDOW 1

int process(int argc, char **argv)
{
    cv::CommandLineParser parser(argc, argv, keys);
    std::string input = parser.get<std::string>("input");
    
    std::shared_ptr<cv::VideoCapture> video;
    if(input.empty())
    {
        int index = parser.get<int>("index");
        video = std::make_shared<cv::VideoCapture>(index); // front, back?
    }
    else
    {
        video = std::make_shared<cv::VideoCapture>(input);
    }
    
    if(video)
    {
        cv::Size size(int(video->get( cv::CAP_PROP_FRAME_WIDTH)), int(video->get(cv::CAP_PROP_FRAME_HEIGHT)));
        
        gatherer::graphics::GLContextWindow window(size, "display");
        gatherer::graphics::FXShader fx(size, {1.f, 1.f});
        gatherer::graphics::WarpShader shader(size, window.getResolution());
        gatherer::graphics::GLTexture texture;

        while(video->isOpened())
        {
            cv::Mat frame;
            (*video) >> frame;
            
#if USE_GL_WINDOW
            // Load frame into texture using simple glTexImage2D()
            //
            // On a 2013 MacBook Pro, a1280x720 texture loads in about 1 ms
            //
            //   OpenGL version: 2.1 NVIDIA-10.4.2 310.41.35f01
            //   GLSL version: 1.20
            //   Vendor: NVIDIA Corporation
            //   Renderer: NVIDIA GeForce GT 650M OpenGL Engine
            //
            // Texture [1280 x 720]load time: 0.000994985

            auto tic = cv::getTickCount();
            texture.load(frame);
            auto toc = cv::getTickCount();
            double elapsed = (toc - tic) / double(cv::getTickFrequency());
            
            std::cout << "Texture " <<  size << "load time: " <<  elapsed << std::endl;
            
            // TODO:  This is an initial attempt to put together a few basic classes for
            // a GLSL rendering pipeline.  More work is needed to iron out a consistent and
            // flexible API.  For example, currently the WarpShader is configured to render
            // to the screen, and the FXShader is configured to render to a framebuffer.
            // Currently it is not possible to switch the order of these.  Also chaining
            // shaders together via operator overload A(B(C(D(teture)))) is convenient for
            // simple shader chains, but it should be possible to support multiple source
            // and desinations shaders for each one.  One such example being:
            //
            //     B=>E\
            // A=> C=>F ==>H
            //     D=>G/
            //
            // This is handled well by GPUImage, for example.
            
            shader(fx(texture));
            
            glfwSwapBuffers (window);
#else
            
            // Use the siple opencv highguig display window:
            cv::imshow("frame", frame);
#endif
        }
    }
    return 0;
}

int main(int argc, char **argv)
{
    try
    {
        return process(argc, argv);
    }
    catch(std::exception &e)
    {
        std::cerr << "Exception: " << e.what() << std::endl;
    }
    catch(...)
    {
        std::cerr << "Unknown error" << std::endl;
    }
}
