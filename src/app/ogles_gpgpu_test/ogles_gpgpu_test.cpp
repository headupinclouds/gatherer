#include <GL/glew.h> // include GLEW and new version of GL on Windows
#include <GLFW/glfw3.h>

#include "graphics/gatherer_graphics.h"
#include "GLContextWindow.h"
#include "OGLESGPGPUTest.h"

#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>

#include <iostream>


int main(int argc, char **argv)
{
    cv::VideoCapture capture(0);
    cv::Size size(int(capture.get(cv::CAP_PROP_FRAME_WIDTH)), int(capture.get(cv::CAP_PROP_FRAME_HEIGHT)));
    
    size = size / 4;
    
    // Create the context
    gatherer::graphics::GLContextWindow window(size, "display");
    gatherer::graphics::OEGLGPGPUTest test(&window, window.getResolution().x);
    
    while( /*capture */ true )
    {
        cv::Mat frame;
        capture >> frame;
        cv::resize(frame, frame, size);
        cv::cvtColor(frame, frame, cv::COLOR_BGR2BGRA);
        test.captureOutput(frame);
        window.swapBuffers();
    }
}
