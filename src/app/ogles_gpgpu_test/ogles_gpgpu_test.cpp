#include <GL/glew.h> // include GLEW and new version of GL on Windows
#include <GLFW/glfw3.h>

#include "graphics/gatherer_graphics.h"
#include "GLContextWindow.h"
#include "OGLESGPGPUTest.h"

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

#include <iostream>


int main(int argc, char **argv)
{
    cv::Size size(640, 480);
    
    // Create the context
    gatherer::graphics::GLContextWindow window(size, "display");
    gatherer::graphics::OEGLGPGPUTest test(&window);
    
    cv::VideoCapture capture(0);
    while( /*capture */ true )
    {
        cv::Mat frame;
        capture >> frame;
        test.captureOutput(frame);
    }
}
