// OS X:
// brew tap homebrew/versions
// brew install --build-bottle --static glfw3


#include <GL/glew.h> // include GLEW and new version of GL on Windows
#include <GLFW/glfw3.h>

#include "graphics/GLWarpShader.h"
#include "graphics/GLTexture.h"

#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>

#include <iterator>
#include <locale>
#include <iostream>
#include <fstream>

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

/// program:


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
        cv::Mat tmp;
        (*video) >> tmp;
        
        GLContextWindow window(tmp.size(), "display");
        
        gatherer::graphics::WarpShader shader(tmp.cols, tmp.rows);
        
        // Encapsulate:
        gatherer::graphics::GLTexture texture;

        while(video->isOpened())
        {
            cv::Mat frame;
            (*video) >> frame;
            
            // Load frame into texture (slow!)
            texture.load(frame);
            shader(texture.get());
            
            glfwSwapBuffers (window);
            
            //cv::imshow("frame", frame);
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
