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
	}

	// Cleanup
	virtual ~QOGLESGPGPUTest()
	{
        gatherer::graphics::Logger::drop("test-shader");
	}
    
    void createPipelien(int number)
    {
        float resolution = 1.f; // only for retina display
        void *glContext = nullptr;
        m_pipeline = std::make_shared<gatherer::graphics::OEGLGPGPUTest>(glContext, resolution, number);
        m_pipeline->setDoDisplay(true); // TODO: temporary display for debug
    }
    
    void processFrame(const cv::Mat &src, cv::Mat &dst, int number = 1)
    {
        createPipelien(number);
        
        void * pixelBuffer = (void *)src.ptr();
        bool useRawPixels = true;
        m_pipeline->captureOutput(src.size(), pixelBuffer, useRawPixels, 0, GL_BGRA);
        
        dst.create(image.size(), CV_8UC4);
        m_pipeline->getOutputData(dst.ptr());
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
    cv::Mat result;
    processFrame(image, result, 1);
    
    // Here we use OpenCV for ground truth (approximate)
    cv::extractChannel(result, result, 0);
    cv::Mat gray, diff;
    cv::cvtColor(image, gray, cv::COLOR_BGRA2GRAY);
    cv::absdiff(gray, result, diff);
    
    double mu = cv::mean(diff)[0];
    EXPECT_LE(mu, 0.05);
}

struct SobelResult
{
    cv::Mat mag, theta, dx, dy;
    
    cv::Mat getAll()
    {
        cv::Mat channels[4], canvas;
        cv::normalize(mag, channels[0], 0, 1, cv::NORM_MINMAX, CV_32F);
        cv::normalize(theta, channels[1], 0, 1, cv::NORM_MINMAX, CV_32F);
        cv::normalize(dx, channels[2], 0, 1, cv::NORM_MINMAX, CV_32F);
        cv::normalize(dy, channels[3], 0, 1, cv::NORM_MINMAX, CV_32F);
        cv::vconcat(channels, 4, canvas);
        return canvas;
    }
};

TEST_F(QOGLESGPGPUTest, grad)
{
    cv::Mat result;
    processFrame(image, result, 7); // 7 == gradient
    
    std::vector<cv::Mat> channels;
    cv::split(result, channels);

    // #### GPU ####
    SobelResult gpu;
    gpu.mag = channels[0];
    gpu.theta = channels[1];
    gpu.dx = channels[2];
    gpu.dy = channels[3];
    
    
    // Compute ground truth
    cv::Mat gray;
    cv::cvtColor(image, gray, cv::COLOR_BGR2GRAY);

    // #### CPU ####
    SobelResult cpu;
    int ksize = 3;
    double scale = 1;
    double delta = 0;
    int borderType = cv::BORDER_DEFAULT;
    cv::Sobel(gray, cpu.dx, CV_32F, 1, 0, ksize, scale, delta, borderType);
    cv::Sobel(gray, cpu.dy, CV_32F, 0, 1, ksize, scale, delta, borderType);
    cv::cartToPolar(cpu.dx, cpu.dy, cpu.mag, cpu.theta);

    // TODO: add comparison
    //cv::Mat cpuCanvas = cpu.getAll();
    //cv::Mat gpuCanvas = gpu.getAll();
    //cv::imshow("cpuCanvas", cpuCanvas);
    //cv::imshow("gpuCanvas", gpuCanvas); cv::waitKey(0);
}


// Source: OpenCV face recognition
template <typename _Tp> static
void olbp_(cv::InputArray _src, cv::OutputArray _dst)
{
    // get matrices
    cv::Mat src = _src.getMat();
    // allocate memory for result
    _dst.create(src.rows-2, src.cols-2, CV_8UC1);
    cv::Mat dst = _dst.getMat();
    // zero the result matrix
    dst.setTo(0);
    // calculate patterns
    for(int i=1;i<src.rows-1;i++) {
        for(int j=1;j<src.cols-1;j++) {
            _Tp center = src.at<_Tp>(i,j);
            unsigned char code = 0;
            code |= (src.at<_Tp>(i-1,j-1) >= center) << 7;
            code |= (src.at<_Tp>(i-1,j) >= center) << 6;
            code |= (src.at<_Tp>(i-1,j+1) >= center) << 5;
            code |= (src.at<_Tp>(i,j+1) >= center) << 4;
            code |= (src.at<_Tp>(i+1,j+1) >= center) << 3;
            code |= (src.at<_Tp>(i+1,j) >= center) << 2;
            code |= (src.at<_Tp>(i+1,j-1) >= center) << 1;
            code |= (src.at<_Tp>(i,j-1) >= center) << 0;
            dst.at<unsigned char>(i-1,j-1) = code;
        }
    }
}

#define DISPLAY_OUTPUT 1

TEST_F(QOGLESGPGPUTest, lbp)
{
    cv::Mat result;
    processFrame(image, result, 8); // 7 == lbp
    
    std::cout << "lbp: " << cv::mean(result) << std::endl;
    
    cv::Mat lbpGPU;
    cv::extractChannel(result, lbpGPU, 0);
    
    // #### CPU ####
    cv::Mat gray, lbpCPU;
    cv::cvtColor(image, gray, cv::COLOR_BGR2GRAY);
    olbp_<unsigned char>(gray, lbpCPU);
    
#if DISPLAY_OUTPUT
    cv::imshow("lbpCPU", lbpCPU);
    cv::imshow("lbpGPU", lbpGPU);
    cv::waitKey(0);
#endif
}

END_EMPTY_NAMESPACE
    
