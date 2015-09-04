#include "GLContextWindow.h"
#include "OGLESGPGPUTest.h"

#define INITIAL_PROC_TYPE 1

_GATHERER_GRAPHICS_BEGIN

OEGLGPGPUTest::OEGLGPGPUTest(GLContextWindow *glContextWindow) : glContextWindow(glContextWindow)
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
    std::cout <<  "initializing ogles_gpgpu" << std::endl;
    
    // get ogles_gpgpu::Core singleton instance
    gpgpuMngr = ogles_gpgpu::Core::getInstance();
    
    // enable iOS optimizations (fast texture access)
    ogles_gpgpu::Core::tryEnablePlatformOptimizations();
    
    // do not use mipmaps (will not work with NPOT images)
    gpgpuMngr->setUseMipmaps(false);
    
    // needed, because we actually have BGRA input data when we use iOS optimized memory access
    
    // set up grayscale processor
    grayscaleProc.setOutputSize(0.5f);  // downscale to half size
    
    //grayscaleProc.setGrayscaleConvType(ogles_gpgpu::GRAYSCALE_INPUT_CONVERSION_BGR);
    
    // create the pipeline
    initGPUPipeline(INITIAL_PROC_TYPE);
    
    // initialize the pipeline (TODO)
    //gpgpuMngr->init((void *)eaglContext); // <= diff context
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
    else
    {
        std::cout << "GPU pipeline definition #%d not supported" << type << std::endl;
    }
    
    // create the display renderer with which we can directly render the output
    // to the screen via OpenGL
    outputDispRenderer = gpgpuMngr->createRenderDisplay();
    outputDispRenderer->setOutputRenderOrientation(dispRenderOrientation);
    
    // reset this to call prepareForFramesOfSize again
    firstFrame = true;
    if (prepared)
    {
        prepared = false;
        // TODO:
        //[glView setFrame:baseFrame];
    }
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
    gpgpuInputHandler->prepareInput(frameSize.width, frameSize.height, GL_BGRA, (void *)&image);
    
    // set the input texture id - we do not copy any data, we use the camera frame directly as texture!
    gpgpuMngr->setInputTexId(gpgpuInputHandler->getInputTexId());
    
    // run processing pipeline
    gpgpuMngr->process();
    
    // update the GL view to display the output directly
    
    // TODO:
    //[glView display];
}

void OEGLGPGPUTest::prepareForFrameOfSize(const cv::Size &size)
{
    float frameAspectRatio = size.width / size.height;
    
    fprintf(stderr, "camera frames are of size %dx%d (aspect %f)", (int)size.width, (int)size.height, frameAspectRatio);
    
    
#if 0
    // TODO:
    // float uiScaling =  [[UIScreen mainScreen] scale];
    float uiScaling = 1.f;
    fprintf(stderr, "device's UI scaling factor is %f", uiScaling);
    
    // update gl frame view size
    float newViewH = glView.frame.size.width / frameAspectRatio;   // calc new height
    float viewYOff = (glView.frame.size.height - newViewH) / 2.0f;
    
    cv::Rect correctedViewRect(0, viewYOff, glView.frame.size.width, newViewH);
    
    //[glView setFrame:correctedViewRect];
    
    // update output the display renderer's output size
    outputDispRenderer->setOutputSize(glView.frame.size.width * uiScaling, newViewH * uiScaling);
#else
    // update output the display renderer's output size
    outputDispRenderer->setOutputSize(size.width, size.height);
#endif
    
    // prepare ogles_gpgpu for the incoming frame size
    // GL_NONE means that the input memory transfer object is NOT prepared
    // this will be done in captureOutput: on each new frame
    gpgpuMngr->prepare(size.width, size.height, GL_NONE);
    gpgpuInputHandler = gpgpuMngr->getInputMemTransfer();
    
    // everything prepared
    prepared = true;
}

_GATHERER_GRAPHICS_END
