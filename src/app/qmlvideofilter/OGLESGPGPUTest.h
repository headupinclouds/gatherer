#include "graphics/gatherer_graphics.h"

#include "ogles_gpgpu/ogles_gpgpu.h"
#include "ogles_gpgpu/common/gl/memtransfer.h"

#include <opencv2/core/core.hpp>

#ifndef OGLESGPGPUTEST_H
#define OGLESGPGPUTEST_H

_GATHERER_GRAPHICS_BEGIN

class OEGLGPGPUTest
{

public:

    OEGLGPGPUTest(void *glContext, const float resolution=1.f);
    ~OEGLGPGPUTest();
    void initOGLESGPGPU() ;
    void initGPUPipeline(int type);
    void prepareForFrameOfSize(const cv::Size &size);
    void captureOutput(cv::Size, void* pixelBuffer) ;
    void initCam();
    void setDisplaySize(int width, int height);
 
    GLuint getDisplayTexture() const;
    GLuint getInputTexture() const;
    GLuint getOutputTexture() const;
    
protected:

    void *glContext = 0;
    float resolution = 1.f;

    unsigned int selectedProcType;  // selected processors
    bool showCamPreview;        // is YES if the camera preview is shown or NO if the processed frames are shown
    bool firstFrame;            // is YES when the current frame is the very first camera frame
    bool prepared;              // is YES when everything is ready to process camera frames
    cv::Size frameSize;           // original camera frame size

    ogles_gpgpu::Core *gpgpuMngr;                   // ogles_gpgpu manager
    ogles_gpgpu::MemTransfer *gpgpuInputHandler;    // input handler for direct access to the camera frames. weak ref!
    ogles_gpgpu::GrayscaleProc grayscaleProc;       // pipeline processor 1: convert input to grayscale image
    ogles_gpgpu::ThreshProc simpleThreshProc;       // pipeline processor 2 (alternative 1): simple thresholding
    ogles_gpgpu::AdaptThreshProc adaptThreshProc;   // pipeline processor 2 (alternative 2): adaptive thresholding (two passes)
    ogles_gpgpu::GaussProc gaussProc;               // pipeline processor 2 (alternative 3): gaussian smoothing (two passes)
    ogles_gpgpu::Disp *outputDispRenderer;          // display renderer to directly display the output in the GL view. weak ref!
    ogles_gpgpu::RenderOrientation dispRenderOrientation;   // current output orientation of the display renderer
};

_GATHERER_GRAPHICS_END

#endif // OGLESGPGPUTEST_H

