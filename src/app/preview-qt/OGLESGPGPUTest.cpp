#include "OGLESGPGPUTest.h"

#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>

#define INITIAL_PROC_TYPE 1
#define USE_INPUT_TEXTURE 0

_GATHERER_GRAPHICS_BEGIN

// ==========

OEGLGPGPUTest::OEGLGPGPUTest(void *glContext, const float resolution)
: glContext(glContext)
, resolution(resolution)
, dispRenderOrientation(ogles_gpgpu::RenderOrientationFlipped)
{
    initCam();
    initOGLESGPGPU();
}

OEGLGPGPUTest::~OEGLGPGPUTest()
{
    ogles_gpgpu::Core::destroy();
    gpgpuMngr = 0;
}

void OEGLGPGPUTest::initCam()
{
    
}

void OEGLGPGPUTest::initOGLESGPGPU()
{
    // get ogles_gpgpu::Core singleton instance
    gpgpuMngr = ogles_gpgpu::Core::getInstance();
    
    // enable iOS optimizations (fast texture access)
    ogles_gpgpu::Core::tryEnablePlatformOptimizations();
    
    // do not use mipmaps (will not work with NPOT images)
    gpgpuMngr->setUseMipmaps(false);
    
    // set up grayscale processor
    //grayscaleProc.setOutputSize(0.5f);  // downscale to half size

    // needed, because we actually have BGRA input data when we use iOS optimized memory access
    //grayscaleProc.setGrayscaleConvType(ogles_gpgpu::GRAYSCALE_INPUT_CONVERSION_BGR);
    
    // create the pipeline
    initGPUPipeline(4);
    
    // initialize the pipeline (TODO)
    gpgpuMngr->init(glContext); // <= diff context
    
}

void OEGLGPGPUTest::setDisplaySize(int width, int height)
{
    outputDispRenderer->setOutputSize(width, height);
}

void OEGLGPGPUTest::initGPUPipeline(int type)
{
    if (selectedProcType == type) return;   // no change
    
    // reset the pipeline
    gpgpuMngr->reset();
    
    // create the pipeline
    if (type == 1)
    {
        gpgpuMngr->addProcToPipeline(&grayscaleProc);
        gpgpuMngr->addProcToPipeline(&adaptThreshProc);
    }
    else if (type == 2)
    {
        gpgpuMngr->addProcToPipeline(&grayscaleProc);
        gpgpuMngr->addProcToPipeline(&simpleThreshProc);
    }
    else if (type == 3)
    {
        gpgpuMngr->addProcToPipeline(&gaussProc);
    }
    else if (type == 4)
    {
        gpgpuMngr->addProcToPipeline(&grayscaleProc);
    }
    else
    {
        std::cout << "GPU pipeline definition #%d not supported" << type << std::endl;
    }
    
    // create the display renderer with which we can directly render the output
    // to the screen via OpenGL
    outputDispRenderer = gpgpuMngr->createRenderDisplay();
    outputDispRenderer->setOutputRenderOrientation(dispRenderOrientation);
    outputDispRenderer->setDisplayResolution(resolution, resolution);
    
    // reset this to call prepareForFramesOfSize again
    firstFrame = true;
    if (prepared)
    {
        prepared = false;
    }
}

GLuint OEGLGPGPUTest::getTexture()
{
    return outputDispRenderer->getInputTexId();
}

void OEGLGPGPUTest::captureOutput(const cv::Mat &image)
{
    // when we get the first frame, prepare the system for the size of the incoming frames
    if (firstFrame)
    {
        frameSize = image.size();
        prepareForFrameOfSize(frameSize);
        firstFrame = false;
    }
    
    // on each new frame, this will release the input buffers and textures, and prepare new ones
    // texture format must be GL_BGRA because this is one of the native camera formats (see initCam)
    gpgpuInputHandler->prepareInput(frameSize.width, frameSize.height, GL_BGRA, (void *)image.ptr());
    
#if USE_INPUT_TEXTURE
    // set the input texture id - we do not copy any data, we use the camera frame directly as texture!
    gpgpuMngr->setInputTexId(gpgpuInputHandler->getInputTexId());
#else
    gpgpuMngr->setInputData(image.ptr());
#endif
    
    // run processing pipeline
    gpgpuMngr->process();
    
    // update the GL view to display the output directly
    outputDispRenderer->render();
}

void OEGLGPGPUTest::prepareForFrameOfSize(const cv::Size &size)
{
    float frameAspectRatio = size.width / size.height;
    
    fprintf(stderr, "camera frames are of size %dx%d (aspect %f)", (int)size.width, (int)size.height, frameAspectRatio);
 
    // update the display renderer's output size
    outputDispRenderer->setOutputSize(size.width, size.height);
    
    // prepare ogles_gpgpu for the incoming frame size
    // GL_NONE means that the input memory transfer object is NOT prepared
    // this will be done in captureOutput: on each new frame
    gpgpuMngr->prepare(size.width, size.height, true ? GL_BGRA : GL_NONE);
    gpgpuInputHandler = gpgpuMngr->getInputMemTransfer();
    
    // everything prepared
    prepared = true;
}

_GATHERER_GRAPHICS_END
