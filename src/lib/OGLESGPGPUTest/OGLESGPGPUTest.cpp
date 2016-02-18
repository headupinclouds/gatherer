#include "OGLESGPGPUTest.h"

#include "ogles_gpgpu/common/gl/memtransfer_optimized.h"

#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>

#define INITIAL_PROC_TYPE 1

_GATHERER_GRAPHICS_BEGIN

class MemTransferScopeLock
{
public:
    MemTransferScopeLock(ogles_gpgpu::MemTransferOptimized *transfer) : transfer(transfer)
    {
        ptr = transfer->lockBufferAndGetPtr(ogles_gpgpu::BUF_TYPE_OUTPUT);
    }
    ~MemTransferScopeLock()
    {
        transfer->unlockBuffer(ogles_gpgpu::BUF_TYPE_OUTPUT);
    }
    const void *data() const { return ptr; }
    operator const void *() const { return ptr; }
protected:
    const void *ptr = 0;
    ogles_gpgpu::MemTransferOptimized *transfer = 0;
};

OEGLGPGPUTest::OEGLGPGPUTest(void *glContext, const float resolution, int type)
: glContext(glContext)
, resolution(resolution)
, dispRenderOrientation(ogles_gpgpu::RenderOrientationStd)
{
    initCam();
    initOGLESGPGPU(glContext, type);
}

OEGLGPGPUTest::OEGLGPGPUTest(void *glContext, const cv::Size &screenSize, const float resolution, int type)
: glContext(glContext)
, screenSize(screenSize)
, resolution(resolution)
, dispRenderOrientation(ogles_gpgpu::RenderOrientationDiagonalMirrored)
{
    initCam();
    initOGLESGPGPU(glContext, type);
}


OEGLGPGPUTest::~OEGLGPGPUTest()
{
    ogles_gpgpu::Core::destroy();
    gpgpuMngr = 0;
}

void OEGLGPGPUTest::initCam()
{
    /*
     * Some temporary code to test the optimized gpu to cpu frame handler.
     * This should be instantiated form a higher level (i.e., main.cpp)
     * but this class hasn't been allocated at that point yet.  This at
     * least supports testing of the concept.
     */
    
#define TEST_FRAME_HANDLER 0
#if TEST_FRAME_HANDLER
    frameHandler = [](const cv::Mat &frame)
    {
        std::stringstream ss;
        ss << getenv("HOME") << "/Documents/frame.png";
        cv::imwrite(ss.str(), frame);
    };
#endif
}

void OEGLGPGPUTest::initOGLESGPGPU(void* glContext, int type)
{
    // get ogles_gpgpu::Core singleton instance
    gpgpuMngr = ogles_gpgpu::Core::getInstance();

    // enable iOS optimizations (fast texture access)
    ogles_gpgpu::Core::tryEnablePlatformOptimizations();

    // do not use mipmaps (will not work with NPOT images)
    gpgpuMngr->setUseMipmaps(false);

    // create the pipeline
    initGPUPipeline(type);
    
    outputDispRenderer->setOutputSize(screenSize.width, screenSize.height);

    // initialize the pipeline (TODO)
    gpgpuMngr->init(glContext);
}

void OEGLGPGPUTest::setDisplaySize(int width, int height)
{
    outputDispRenderer->setOutputSize(width, height);
}

void OEGLGPGPUTest::initTransform1()
{
#define USE_TRANSFORM1 1
#define USE_TRANSFORM2 1
    
#if USE_TRANSFORM1
    ogles_gpgpu::Mat44f transformMatrix =
    {{
        {1.f,0.f,0.f,0.f},
        {0.f,1.f,0.f,0.f},
        {0.f,0.f,0.f,0.f},
        {0.f,0.f,0.f,1.f}
    }};
    
    // Use this to place the texture upright for processing (object, detection, etc)
    transformProc1.setTransformMatrix(transformMatrix);
    transformProc1.setOutputRenderOrientation(ogles_gpgpu::RenderOrientationDiagonalMirrored);
    transformProc1.setOutputSize(0.25);
    
    gpgpuMngr->addProcToPipeline(&transformProc1);
#endif
    
    gpgpuMngr->addProcToPipeline(&grayscaleProc);
    
#if USE_TRANSFORM2
    // Use this to place the texture back in the native orientation (and aspect ratio)
    // provided by the QML Camera since the QML VideoOutput object that displays the
    // frames is expecting that and I don't see a trivial way to work around that.
    // It seems we need to override either the VideoObject class or encapsulate this
    // native processing pipeline in some kind of ExtendedCamera qml object that
    // reports the size and orientation of the final processed texture.  Since this
    // is all on the GPU it may not matter much from a performance standpoint.
    //
    // TODO: investigate QT mechanism for avoiding this.
    transformProc2.setTransformMatrix(transformMatrix);
    transformProc2.setOutputRenderOrientation(ogles_gpgpu::RenderOrientationDiagonalFlipped);
    transformProc2.setOutputSize(0.25);
    
    gpgpuMngr->addProcToPipeline(&transformProc2);
#endif

}

void OEGLGPGPUTest::initTransform2()
{
    auto interpolation = ogles_gpgpu::TransformProc::BICUBIC;
    
    transformProc1.setInterpolation(interpolation);
    float theta = 15.0 * M_PI / 180.0;
    float ct = std::cos(theta);
    float st = std::sin(theta);
    ogles_gpgpu::Mat44f transformMatrix1, transformMatrix2;
    
    transformMatrix1 = transformMatrix2 =
    {{
        {+ct,-st,0.f,0.f},
        {+st,+ct,0.f,0.f},
        {0.f,0.f,0.f,0.f},
        {0.f,0.f,0.f,1.f}
    }};
    
    transformMatrix2.data[0][0] = +ct;
    transformMatrix2.data[1][1] = +ct;
    transformMatrix2.data[1][0] = +st;
    transformMatrix2.data[0][1] = -st;
    
    // Use this to place the texture upright for processing (object, detection, etc)
    transformProc1.setTransformMatrix(transformMatrix1); // don't rotate
    transformProc1.setOutputRenderOrientation(ogles_gpgpu::RenderOrientationDiagonalMirrored);
    transformProc1.setOutputSize(0.25);
    
    gpgpuMngr->addProcToPipeline(&transformProc1);
    
    transformProc2.setInterpolation(interpolation);
    transformProc2.setTransformMatrix(transformMatrix2); // rotate output
    transformProc2.setOutputRenderOrientation(ogles_gpgpu::RenderOrientationDiagonalFlipped);
    transformProc2.setOutputSize(1.0);
    
    gpgpuMngr->addProcToPipeline(&transformProc2);
}

void OEGLGPGPUTest::initMultiscale()
{
    ogles_gpgpu::PyramidProc::Size size;
    size.width = 512;
    size.height = 512;
    std::vector<ogles_gpgpu::PyramidProc::Size> scales;
    for(int i = 0; i < 4; i++)
    {
        scales.push_back(size);
        size.width = float(size.width) * 0.95;
        size.height = float(size.height) * 0.95;
    }
    pyrProc.setScales(scales);
    gpgpuMngr->addProcToPipeline(&pyrProc);
}

void OEGLGPGPUTest::initGPUPipeline(int type)
{
    if (selectedProcType == type) return;   // no change

    // reset the pipeline
    gpgpuMngr->reset();
    
    switch(type)
    {
        case 1:
            gpgpuMngr->addProcToPipeline(&grayscaleProc);
            break;
        case 2:
            gpgpuMngr->addProcToPipeline(&grayscaleProc);
            gpgpuMngr->addProcToPipeline(&adaptThreshProc);
            break;
        case 3:
            gpgpuMngr->addProcToPipeline(&grayscaleProc);
            gpgpuMngr->addProcToPipeline(&simpleThreshProc);
            break;
        case 4:
            gpgpuMngr->addProcToPipeline(&gaussProc);
            break;
        case 5:
            initTransform1();
            break;
        case 6:
            initTransform2();
            break;
        case 7:
            gpgpuMngr->addProcToPipeline(&grayscaleProc);
            gpgpuMngr->addProcToPipeline(&gradProc);
            break;
        case 8:
            gpgpuMngr->addProcToPipeline(&grayscaleProc);
            gpgpuMngr->addProcToPipeline(&lbpProc);
            break;
        case 9:
            gpgpuMngr->addProcToPipeline(&grayscaleProc);
            gpgpuMngr->addProcToPipeline(&tensorProc);
            gpgpuMngr->addProcToPipeline(&gaussProc);
            gpgpuMngr->addProcToPipeline(&shiTomasiProc);
            gpgpuMngr->addProcToPipeline(&nmsProc);
            
            tensorProc.setEdgeStrength(1.0);
            shiTomasiProc.setSensitivity(10.0);
            nmsProc.setThreshold(0.1);
            break;
        case 10:
            gpgpuMngr->addProcToPipeline(&pyrProc);
            break;
        case 11:
            gpgpuMngr->addProcToPipeline(&pyrProc);
            gpgpuMngr->addProcToPipeline(&grayscaleProc);
            gpgpuMngr->addProcToPipeline(&gradProc);
            gpgpuMngr->addProcToPipeline(&gaussProc);
            break;
        case 12:
            initMultiscale();
            break;
        default:
            std::cout << "GPU pipeline definition #%d not supported" << type << std::endl;
            break;
    }

    if(m_doDisplay)
    {
        // create the display renderer with which we can directly render the output
        // to the screen via OpenGL
        outputDispRenderer = gpgpuMngr->createRenderDisplay(screenSize.width, screenSize.height, dispRenderOrientation);
        outputDispRenderer->setDisplayResolution(resolution, resolution);
    }

    // reset this to call prepareForFramesOfSize again
    firstFrame = true;
    if (prepared)
    {
        prepared = false;
    }
}

GLuint OEGLGPGPUTest::getDisplayTexture() const
{
    return outputDispRenderer->getOutputTexId();
}

GLuint OEGLGPGPUTest::getInputTexture() const
{
    return gpgpuInputHandler->getInputTexId();
}

GLuint OEGLGPGPUTest::getOutputTexture() const
{
    return gpgpuInputHandler->getOutputTexId();
}

GLuint OEGLGPGPUTest::getLastShaderOutputTexture() const
{
    return gpgpuMngr->getOutputTexId();
}

void OEGLGPGPUTest::getInputData(unsigned char *data) const
{
    gpgpuMngr->getInputData(data);
}

void OEGLGPGPUTest::getOutputData(unsigned char *data) const
{
    gpgpuMngr->getOutputData(data);
}

cv::Size OEGLGPGPUTest::getOutputSize() const
{
    return cv::Size(gpgpuMngr->getOutputFrameW(), gpgpuMngr->getOutputFrameH());
}

void OEGLGPGPUTest::configurePipeline(const cv::Size &size, GLenum inputPixFormat)
{
    frameSize = size;
    if(!screenSize.area())
    {
        screenSize = frameSize;
    }
    prepareForFrameOfSize(frameSize);

    outputDispRenderer->setOutputSize(screenSize.width, screenSize.height);
        
    // YUV:
    if(inputPixFormat == 0)
    {
        yuv2RgbProc.init(frameSize.width, frameSize.height, 0, true); // TODO: NEW
        yuv2RgbProc.createFBOTex(false);
    }
}

void OEGLGPGPUTest::captureOutput(cv::Size size, void* pixelBuffer, bool useRawPixels, GLuint inputTexture, GLenum inputPixFormat)
{
    // when we get the first frame, prepare the system for the size of the incoming frames

    if (firstFrame)
    {
        configurePipeline(size, inputPixFormat);
        firstFrame = false;
    }

    gpgpuInputHandler->setUseRawPixels(useRawPixels);

    // on each new frame, this will release the input buffers and textures, and prepare new ones
    // texture format must be GL_BGRA because this is one of the native camera formats (see initCam)
    if(pixelBuffer)
    {
        if(inputPixFormat == 0)
        {
            // YUV: Special case NV12=>BGR
            auto manager = yuv2RgbProc.getMemTransferObj();
            if (useRawPixels)
            {
                manager->setUseRawPixels(true);
            }
            manager->prepareInput(frameSize.width, frameSize.height, inputPixFormat, pixelBuffer);

            yuv2RgbProc.setTextures(manager->getLuminanceTexId(), manager->getChrominanceTexId());
            yuv2RgbProc.render();
            glFinish();

            gpgpuInputHandler->prepareInput(frameSize.width, frameSize.height, GL_NONE, nullptr);
            inputTexture = yuv2RgbProc.getOutputTexId(); 
        }
        else
        {
            gpgpuInputHandler->prepareInput(frameSize.width, frameSize.height, inputPixFormat, pixelBuffer);
            gpgpuMngr->setInputData(reinterpret_cast< const unsigned char *>(pixelBuffer));
            inputTexture = gpgpuMngr->getInputMemTransfer()->getInputTexId();             
        }
    }

    assert(inputTexture); // inputTexture must be defined at this point
    gpgpuMngr->setInputTexId(inputTexture);
    
    // run processing pipeline
    gpgpuMngr->process();

#if 1
    if(frameHandler)
    {
        auto transfer = dynamic_cast<ogles_gpgpu::MemTransferOptimized *>(gpgpuMngr->getOutputMemTransfer());
        if(transfer)
        {
            MemTransferScopeLock data(transfer);
            cv::Size outSize(transformProc1.getOutFrameW(), transformProc1.getOutFrameH());
            cv::Mat frame(outSize.height, outSize.width, CV_8UC4, (void *)data.data());
            frameHandler(frame);
        }
    }
#endif
    
#if !defined(NDEBUG)
    std::cerr << "Skipping render..." << std::endl;
#endif

    if(m_doDisplay)
    {
        // update the GL view to display the output directly
        outputDispRenderer->render();
    }
}

void OEGLGPGPUTest::prepareForFrameOfSize(const cv::Size &size)
{
    float frameAspectRatio = size.width / size.height;

    fprintf(stderr, "camera frames are of size %dx%d (aspect %f)\n", (int)size.width, (int)size.height, frameAspectRatio);

    // prepare ogles_gpgpu for the incoming frame size
    // GL_NONE means that the input memory transfer object is NOT prepared
    // this will be done in captureOutput: on each new frame
#if __ANDROID__
    gpgpuMngr->prepare(size.width, size.height, true ? GL_RGBA : GL_NONE);
#else
    gpgpuMngr->prepare(size.width, size.height, true ? GL_BGRA : GL_NONE);
#endif
    gpgpuInputHandler = gpgpuMngr->getInputMemTransfer();

    // everything prepared
    prepared = true;
}

_GATHERER_GRAPHICS_END
