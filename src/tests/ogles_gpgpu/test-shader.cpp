#include <gtest/gtest.h>

#include <GL/glew.h> // include GLEW and new version of GL on Windows
#include <GLFW/glfw3.h>
#include "GLContextWindow.h"

#include "graphics/gatherer_graphics.h"
#include "OGLESGPGPUTest.h"
#include "graphics/Logger.h"

#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>

#include <fstream>
#include <memory>

// https://code.google.com/p/googletest/wiki/Primer

const char* imageFilename;
const char* truthFilename;

#define BEGIN_EMPTY_NAMESPACE namespace {
#define END_EMPTY_NAMESPACE }

BEGIN_EMPTY_NAMESPACE

class OGLESGPGPUTest : public ::testing::Test
{
protected:

	// Setup
	OGLESGPGPUTest()
	{
        m_logger = gatherer::graphics::Logger::create("test-shader");
        
		// Load the ground truth data:
        image = loadImage(imageFilename);
        
        // TODO: we need to load ground truth output for each shader
        // (some combinations could be tested, but that is probably excessive!)
        truth = loadImage(truthFilename);
        
        m_context = std::make_shared<gatherer::graphics::GLContextWindow>(image.size(), "test-shader");

        float resolution = 2.f;
        void *glContext = nullptr;
        m_pipeline = std::make_shared<gatherer::graphics::OEGLGPGPUTest>(glContext, resolution);
        m_pipeline->setDoDisplay(true); // TODO: temporary display for debug
	}

	// Cleanup
	virtual ~OGLESGPGPUTest()
	{

	}

	// Called after constructor for each test
	virtual void SetUp() {}

	// Called after destructor for each test
	virtual void TearDown() {}
    
    static cv::Mat loadImage(const std::string &filename)
    {
        assert(!filename.empty());
        cv::Mat image = cv::imread(filename, cv::IMREAD_COLOR);
        assert(!image.empty() && image.type() == CV_8UC3);
        
        cv::Mat tmp;
        cv::cvtColor(image, tmp, cv::COLOR_BGR2BGRA);
        cv::swap(image, tmp);
        return image;
    }

    std::shared_ptr<gatherer::graphics::GLContextWindow> m_context;
    std::shared_ptr<spdlog::logger> m_logger;
    std::shared_ptr<gatherer::graphics::OEGLGPGPUTest> m_pipeline;
    
	// Test images:
    cv::Mat image, truth;
};

/*
 * Fixture tests
 */

TEST_F(OGLESGPGPUTest, grayscale)
{
    void * pixelBuffer = image.ptr();
    bool useRawPixels = true;
    m_pipeline->captureOutput(image.size(), pixelBuffer, useRawPixels, 0, GL_BGRA);
    
    cv::Mat result(image.size(), CV_8UC4);
    m_pipeline->getOutputData(result.ptr());
    
    // TODO: compare result with expected ground truth result

    cv::imshow("result", result);
    m_context->swapBuffers();
    cv::waitKey(2000);
}

END_EMPTY_NAMESPACE
    
