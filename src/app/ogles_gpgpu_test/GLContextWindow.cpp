#include "GLContextWindow.h"

_GATHERER_GRAPHICS_BEGIN

// See: http://www.codeincodeblock.com/2013/05/introduction-to-modern-opengl-3x-with.html
GLContextWindow::GLContextWindow(const cv::Size &size, const std::string &name) : m_size(size), m_name(name)
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

GLContextWindow::~GLContextWindow()
{
    //TODO: other tear down
    if(m_window)
        glfwDestroyWindow(m_window);
    
    if(m_initialized)
        glfwTerminate();
}

cv::Point2f GLContextWindow::getResolution() const
{
    int width, height;
    glfwGetFramebufferSize(m_window, &width, &height);
    return cv::Point2f(float(width)/float(m_size.width), float(height)/float(m_size.height));
}

_GATHERER_GRAPHICS_END
