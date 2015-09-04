#include <GL/glew.h> // include GLEW and new version of GL on Windows
#include <GLFW/glfw3.h>

#include "graphics/gatherer_graphics.h"

#include <opencv2/core/core.hpp>

#include <iostream>

_GATHERER_GRAPHICS_BEGIN

// See: http://www.codeincodeblock.com/2013/05/introduction-to-modern-opengl-3x-with.html
class GLContextWindow
{
public:
    GLContextWindow(const cv::Size &size, const std::string &name);
    ~GLContextWindow();
    cv::Point2f getResolution() const;
    operator GLFWwindow * () const { return m_window; }
protected:
    bool m_initialized = false;
    cv::Size m_size;
    std::string m_name;
    GLFWwindow* m_window;
};

_GATHERER_GRAPHICS_END
