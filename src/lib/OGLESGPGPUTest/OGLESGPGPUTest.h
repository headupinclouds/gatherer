#ifndef OGLESGPGPUTEST_H
#define OGLESGPGPUTEST_H

#include "graphics/gatherer_graphics.h"

#include "ogles_gpgpu/ogles_gpgpu.h"
#include "ogles_gpgpu/common/gl/memtransfer.h"

#include "common/proc/yuv2rgb.h"

#include <opencv2/core/core.hpp>

_GATHERER_GRAPHICS_BEGIN

#if __ANDROID__
#  define DFLT_PIX_FORMAT GL_RGBA
#else
#  define DFLT_PIX_FORMAT GL_BGRA
#endif

class OEGLGPGPUTest
{
public:
    
    typedef std::function<void(const cv::Mat &)> FrameHandler;
    
    OEGLGPGPUTest(void *glContext, const cv::Size &size, const float resolution=1.f, int type=1);
    OEGLGPGPUTest(void *glContext, const float resolution=1.f, int type=1);
    ~OEGLGPGPUTest();
    void initOGLESGPGPU(void* glContext, int type=1);
    void initGPUPipeline(int type);
    void prepareForFrameOfSize(const cv::Size &size);
    void captureOutput(cv::Size size, void* pixelBuffer, bool useRawPixels, GLuint inputTexture=0, GLenum inputPixFormat=DFLT_PIX_FORMAT);
    void initCam();
    void setDisplaySize(int width, int height);
    cv::Size getOutputSize() const;

    GLuint getDisplayTexture() const;
    GLuint getInputTexture() const;
    GLuint getOutputTexture() const;
    GLuint getLastShaderOutputTexture() const;
    
    void getInputData(unsigned char *data) const;
    void getOutputData(unsigned char *data) const;
    
    void setFrameHandler(FrameHandler &handler) { frameHandler = handler; }

    void setDoDisplay(bool flag) { m_doDisplay = flag; }
    
protected:

    void configurePipeline(const cv::Size &size, GLenum inputPixFormat);

    void *glContext = 0;
    float resolution = 1.f;
    
    bool m_doDisplay = true;

    unsigned int selectedProcType;  // selected processors
    bool showCamPreview;        // is YES if the camera preview is shown or NO if the processed frames are shown
    bool firstFrame;            // is YES when the current frame is the very first camera frame
    bool prepared;              // is YES when everything is ready to process camera frames
    cv::Size frameSize;         // original camera frame size
    cv::Size screenSize;        // screen size

    FrameHandler frameHandler;
    
    ogles_gpgpu::Core *gpgpuMngr;                   // ogles_gpgpu manager
    ogles_gpgpu::MemTransfer *gpgpuInputHandler;    // input handler for direct access to the camera frames. weak ref!
    
    ogles_gpgpu::MemTransfer *yuvInputHandler; // special yuv input handler (weak ref!)
    
    ogles_gpgpu::TransformProc transformProc1;  // 2D parametric transformations
    ogles_gpgpu::TransformProc transformProc2;  // 2D parametric transformations
    
    ogles_gpgpu::GrayscaleProc grayscaleProc;       // pipeline processor 1: convert input to grayscale image
    ogles_gpgpu::ThreshProc simpleThreshProc;       // pipeline processor 2 (alternative 1): simple thresholding
    ogles_gpgpu::AdaptThreshProc adaptThreshProc;   // pipeline processor 2 (alternative 2): adaptive thresholding (two passes)
    ogles_gpgpu::GaussProc gaussProc;               // pipeline processor 2 (alternative 3): gaussian smoothing (two passes)
    ogles_gpgpu::Disp *outputDispRenderer;          // display renderer to directly display the output in the GL view. weak ref!
    ogles_gpgpu::RenderOrientation dispRenderOrientation;   // current output orientation of the display renderer
    
    ogles_gpgpu::Yuv2RgbProc yuv2RgbProc;
};

_GATHERER_GRAPHICS_END

#endif // OGLESGPGPUTEST_H
