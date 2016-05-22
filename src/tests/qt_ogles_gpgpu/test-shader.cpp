#include <gtest/gtest.h>

#include "QGLContext.h"

#include "graphics/gatherer_graphics.h"
#include "OGLESGPGPUTest.h"
#include "common/proc/video.h"
#include "graphics/Logger.h"
#include "ogles_gpgpu/common/proc/blend.h"

#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>

#include <fstream>
#include <memory>

#define DISPLAY_OUTPUT 1

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
        
        dst.create(m_pipeline->getOutputSize(), CV_8UC4);
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


static void extract(const cv::Mat &image, cv::Size size, std::vector<cv::Mat> &pyramid, int n = 8)
{
    cv::Point tl(0,0);
    for (int i = 0; i <= n; ++i)
    {
        pyramid.push_back( image(cv::Rect(tl, size)) );
        
        if(i % 2)
        {
            tl.y += size.height;
        }
        else
        {
            tl.x += size.width;
        }
        size.width >>= 1;
        size.height >>= 1;
    }
}

static cv::Mat getImage(ogles_gpgpu::ProcInterface &proc)
{
    cv::Mat result(proc.getOutFrameH(), proc.getOutFrameW(), CV_8UC4);
    proc.getResultData(result.ptr());
    return result;
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

/*
 * Fixture tests
 */

TEST_F(QOGLESGPGPUTest, blend)
{
    // ### Configure the pipeline architecture ###
    ogles_gpgpu::GrayscaleProc grayscaleProc;
    ogles_gpgpu::GrayscaleProc colorProc;
    
    // Here we make colorProc a simple pass through/noop shader:
    colorProc.setGrayscaleConvType(ogles_gpgpu::GRAYSCALE_INPUT_CONVERSION_NONE);

    ogles_gpgpu::BlendProc blenderProc;
    colorProc.add(&blenderProc, 0);
    grayscaleProc.add(&blenderProc, 1);

    // ### Configure the color and grayscale inpput "video" sources ###
    ogles_gpgpu::VideoSource videoColor;
    videoColor.set(&colorProc);
    
    ogles_gpgpu::VideoSource videoGray;
    videoGray.set(&grayscaleProc);
    videoGray({image.cols, image.rows}, image.ptr(), true, 0, GL_BGRA);
    
    for(int i = 0; i < 100; i++)
    {
        float alpha = float(i % 11) / 10.f; // std::cout << "alpha: " << alpha << std::endl;
        blenderProc.setAlpha(alpha);
        
        // Load and trigger the color input pipeline (source #0):
        // Note: here image is static, but these could be dynamic video frames
        videoColor({image.cols, image.rows}, image.ptr(), true, 0, GL_BGRA);
        
        // Just trigger the grayscale pipeline (source #1) with existing texture:
        grayscaleProc.process(1);

        cv::Mat result = getImage(blenderProc);
        
#if DISPLAY_OUTPUT
        cv::imshow("blend", result); cv::waitKey(0);
#endif
    }
}

TEST_F(QOGLESGPGPUTest, resize)
{
    ogles_gpgpu::VideoSource video;
    ogles_gpgpu::GrayscaleProc grayscaleProc;
    
    video.set(&grayscaleProc);
    
    video({image.cols, image.rows}, image.ptr(), true, 0, GL_BGRA);
    cv::Mat result0 = getImage(grayscaleProc);
    
    cv::Mat half;
    cv::pyrDown(image, half);
    video({half.cols, half.rows}, half.ptr(), true, 0, GL_BGRA);
    cv::Mat result1 = getImage(grayscaleProc);
    
#if DISPLAY_OUTPUT
    cv::imshow("gray0", result0);
    cv::imshow("gray1", result1);
    cv::waitKey(0);
#endif
}

TEST_F(QOGLESGPGPUTest, pyramid)
{
    ogles_gpgpu::VideoSource video;
    ogles_gpgpu::PyramidProc pyrProc; // gradient, gradient magnitude, orientation
    
    video.set(&pyrProc);
    video({image.cols, image.rows}, image.ptr(), true, 0, GL_BGRA);
    cv::Mat result = getImage(pyrProc);
    
#if DISPLAY_OUTPUT
    cv::imshow("pyramid", result);
#endif
}

TEST_F(QOGLESGPGPUTest, multiscale)
{
    ogles_gpgpu::VideoSource video;
    ogles_gpgpu::PyramidProc pyrProc;
    
    ogles_gpgpu::Size2d size(image.cols, image.rows);
    std::vector<ogles_gpgpu::Size2d> scales;
    for(int i = 0; i < 4; i++)
    {
        scales.push_back(size);
        size.width = float(size.width) * 0.95;
        size.height = float(size.height) * 0.95;
    }
    pyrProc.setScales(scales);

    video.set(&pyrProc);
    video({image.cols, image.rows}, image.ptr(), true, 0, GL_BGRA);
    cv::Mat result = getImage(pyrProc);
    
#if DISPLAY_OUTPUT
    cv::imshow("scales", result);
#endif
}

TEST_F(QOGLESGPGPUTest, lbp)
{
    ogles_gpgpu::VideoSource video;
    ogles_gpgpu::GrayscaleProc grayProc; // gradient, gradient magnitude, orientation
    ogles_gpgpu::LbpProc lbpProc;
    
    video.set(&grayProc);
    grayProc.add(&lbpProc);
    video({image.cols, image.rows}, image.ptr(), true, 0, GL_BGRA);
    cv::Mat result = getImage(lbpProc);

    // #### GPU ####
    cv::Mat lbpGPU;
    cv::extractChannel(result, lbpGPU, 0);
    
    // #### CPU ####
    cv::Mat gray, lbpCPU;
    cv::cvtColor(image, gray, cv::COLOR_BGR2GRAY);
    olbp_<unsigned char>(gray, lbpCPU);
    
#if DISPLAY_OUTPUT
    cv::imshow("lbpCPU", lbpCPU);
    cv::imshow("lbpGPU", lbpGPU);
#endif
}

TEST_F(QOGLESGPGPUTest, pyredge)
{
    ogles_gpgpu::VideoSource video;
    ogles_gpgpu::PyramidProc pyrProc;
    ogles_gpgpu::GrayscaleProc grayscaleProc;
    ogles_gpgpu::GradProc gradProc;
    ogles_gpgpu::GaussProc gaussProc;

    video.set(&pyrProc);
    pyrProc.add(&grayscaleProc);
    grayscaleProc.add(&gradProc);
    gradProc.add(&gaussProc);
    
    video({image.cols, image.rows}, image.ptr(), true, 0, GL_BGRA);
    cv::Mat result = getImage(gaussProc);
    
#define EXTRACT_PYRAMID 0
#if EXTRACT_PYRAMID
    std::vector<cv::Mat> pyramid;
    extract(result, image.size(), pyramid, 8);
    for(auto &l : pyramid)
    {
        cv::imshow("l", l);
    }
#endif // EXTRACT_PYRAMID
    
    std::vector<cv::Mat> channels;
    cv::split(result, channels);
    
    // #### GPU ####
    SobelResult gpu;
    gpu.mag = channels[0];
    gpu.theta = channels[1];
    gpu.dx = channels[2];
    gpu.dy = channels[3];
    
    cv::Mat gpuCanvas = gpu.getAll();
    cv::resize(gpuCanvas, gpuCanvas, {}, 0.5, 0.5);

    cv::imshow("pyredge", gpuCanvas);
}

TEST_F(QOGLESGPGPUTest, warp)
{
    ogles_gpgpu::VideoSource video;
    ogles_gpgpu::TransformProc transformProc;
    
    video.set(&transformProc);
    
    auto interpolation = ogles_gpgpu::TransformProc::BICUBIC;
    
    transformProc.setInterpolation(interpolation);
    float theta = 15.0 * M_PI / 180.0;
    float ct = std::cos(theta);
    float st = std::sin(theta);
    ogles_gpgpu::Mat44f transformMatrix =
    {{
        {+ct,-st,0.f,0.f},
        {+st,+ct,0.f,0.f},
        {0.f,0.f,0.f,0.f},
        {0.f,0.f,0.f,1.f}
    }};
    
    // Use this to place the texture upright for processing (object, detection, etc)
    transformProc.setTransformMatrix(transformMatrix); // don't rotate
    transformProc.setOutputRenderOrientation(ogles_gpgpu::RenderOrientationDiagonalMirrored);
    transformProc.setOutputSize(0.25);
    
    video({image.cols, image.rows}, image.ptr(), true, 0, GL_BGRA);
    cv::Mat result = getImage(transformProc);
    
#if DISPLAY_OUTPUT
    cv::imshow("warp", result);
#endif
}

TEST_F(QOGLESGPGPUTest, grayscale)
{
    ogles_gpgpu::VideoSource video;
    ogles_gpgpu::GrayscaleProc grayscaleProc;
    
    video.set(&grayscaleProc);
    
    video({image.cols, image.rows}, image.ptr(), true, 0, GL_BGRA);
    cv::Mat result = getImage(grayscaleProc);
    
#if DISPLAY_OUTPUT
    cv::imshow("gray", result);
#endif
}

TEST_F(QOGLESGPGPUTest, grad)
{
    ogles_gpgpu::VideoSource video;
    ogles_gpgpu::GrayscaleProc grayscaleProc;
    ogles_gpgpu::GradProc gradProc;
    
    video.set(&grayscaleProc);
    grayscaleProc.add(&gradProc);
    
    video({image.cols, image.rows}, image.ptr(), true, 0, GL_BGRA);
    cv::Mat result = getImage(gradProc);
    
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

#if DISPLAY_OUTPUT
    // TODO: add comparison
    cv::Mat cpuCanvas = cpu.getAll(); cv::resize(cpuCanvas, cpuCanvas, {}, 0.25, 0.25);
    cv::Mat gpuCanvas = gpu.getAll(); cv::resize(gpuCanvas, gpuCanvas, {}, 0.25, 0.25);
    cv::imshow("cpuCanvas", cpuCanvas);
    cv::imshow("gpuCanvas", gpuCanvas);
#endif
}

TEST_F(QOGLESGPGPUTest, corner)
{
    ogles_gpgpu::VideoSource video;
    ogles_gpgpu::GrayscaleProc grayscaleProc;
    ogles_gpgpu::TensorProc tensorProc;
    ogles_gpgpu::GaussProc gaussProc;
    ogles_gpgpu::ShiTomasiProc shiTomasiProc;
    ogles_gpgpu::NmsProc nmsProc;
    
    video.set(&grayscaleProc);
    grayscaleProc.add(&tensorProc);
    tensorProc.add(&gaussProc);
    gaussProc.add(&shiTomasiProc);
    shiTomasiProc.add(&nmsProc);
    
    tensorProc.setEdgeStrength(1.0);
    shiTomasiProc.setSensitivity(10.0);
    nmsProc.setThreshold(0.1);
    
    video({image.cols, image.rows}, image.ptr(), true, 0, GL_BGRA);
    cv::Mat result = getImage(nmsProc);
    
    // ### GPU ###
    cv::Mat mask;
    cv::extractChannel(result, mask, 0);
    
    std::vector<cv::Point> points;
    try { cv::findNonZero(mask, points); } catch(...) {}

    cv::Mat canvas = image.clone();
    for(const auto &p : points)
    {
        cv::circle(canvas, p, 2, {0,255,0}, -1, 8);
    }

#if DISPLAY_OUTPUT
    cv::imshow("corner", canvas);
    cv::waitKey(0);
#endif
}

END_EMPTY_NAMESPACE
    
