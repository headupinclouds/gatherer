#include <gtest/gtest.h>

#include "QGLContext.h"

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

class QOGLESGPGPUTest : public ::testing::Test
{
protected:
    
	// Setup
	QOGLESGPGPUTest()
	{
        m_logger = gatherer::graphics::Logger::create("test-shader");
        
		// Load the ground truth data:
        image = loadImage(imageFilename);
        
        // TODO: we need to load ground truth output for each shader
        // (some combinations could be tested, but that is probably excessive!)
        truth = loadImage(truthFilename);

        m_context = std::make_shared<QGLContext>();

        float resolution = 1.f; // only for retina display
        void *glContext = nullptr;
        m_pipeline = std::make_shared<gatherer::graphics::OEGLGPGPUTest>(glContext, resolution, 1);
        m_pipeline->setDoDisplay(true); // TODO: temporary display for debug
	}

	// Cleanup
	virtual ~QOGLESGPGPUTest()
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

    std::shared_ptr<QGLContext> m_context;
    std::shared_ptr<spdlog::logger> m_logger;
    std::shared_ptr<gatherer::graphics::OEGLGPGPUTest> m_pipeline;
    
	// Test images:
    cv::Mat image, truth;
};

/*
 * Fixture tests
 */

TEST_F(QOGLESGPGPUTest, grayscale)
{
    void * pixelBuffer = image.ptr();
    bool useRawPixels = true;
    m_pipeline->captureOutput(image.size(), pixelBuffer, useRawPixels, 0, GL_BGRA);
    
    cv::Mat result(image.size(), CV_8UC4);
    m_pipeline->getOutputData(result.ptr());
    cv::extractChannel(result, result, 0);

    // Here we use OpenCV for ground truth (approximate)
    cv::Mat gray;
    cv::cvtColor(image, gray, cv::COLOR_BGRA2GRAY);

    cv::imwrite("/tmp/gray.png", gray);
    
    cv::Mat diff;
    cv::absdiff(gray, result, diff);
    
    double mu = cv::mean(diff)[0];
    EXPECT_LE(mu, 0.05);
    
    // TODO: compare result with expected ground truth result
    //cv::imshow("result", result);
    //cv::imshow("gray", gray);
    //cv::waitKey(2000);
}

END_EMPTY_NAMESPACE
    
