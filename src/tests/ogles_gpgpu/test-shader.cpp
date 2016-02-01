#include <gtest/gtest.h>

#include "OGLESGPGPUTest.h"

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
		// Load the ground truth data:
		loadTruth();

		// Load sample input images
		loadImages();
	}

	// Cleanup
	virtual ~OGLESGPGPUTest()
	{

	}

	// Called after constructor for each test
	virtual void SetUp() {}

	// Called after destructor for each test
	virtual void TearDown() {}

	// Utility methods:
	void loadImages()
	{
		assert(imageFilename);
		image = cv::imread(imageFilename, cv::IMREAD_COLOR);
		assert(!image.empty());
	}

	void loadTruth()
	{
		assert(truthFilename);
		truth = cv::imread(truthFilename, cv::IMREAD_COLOR);
		assert(!image.empty());
	}

    std::shared_ptr<gatherer::graphics::OEGLGPGPUTest> m_pipeline;
    
	// Test images:
    cv::Mat image, truth;
};

/*
 * Fixture tests
 */

TEST_F(OGLESGPGPUTest, grayscale)
{

}

END_EMPTY_NAMESPACE
    
