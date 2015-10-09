/**
 * Copyright (c) 2012-2014 Microsoft Mobile.
 */

#include "customcamera.h"

#include <QtGui>
#include <QCameraImageCapture>
#include "imageanalyzer.h"
#include "videosurface.h"


/*!
  \class CustomCamera
  \brief Provides QML CustomCamera element to access the QCamera object.
         Also creates a video surface to show the viewfinder of the camera,
         acts as proxy to access the settings of the QCamera exposure, focus
         and image processing settings.
*/


/*!
  Constructor.
*/
CustomCamera::CustomCamera(QDeclarativeItem *parent)
    : QDeclarativeItem(parent),
      m_camera(0),
      m_frontCamera(false),
      m_cameraImageCapture(0),
      m_mediaRecorder(0),
      m_videoSurface(0),
      m_imageAnalyzer(0),
      m_processedFrameCounter(0),
      m_incomingFrameCounter(0)
{
    // Important, otherwise the paint method is never called
    setFlag(QGraphicsItem::ItemHasNoContents, false);
}


/*!
  Destructor.
*/
CustomCamera::~CustomCamera()
{
    destroyResources();
}


/*!
  Releases and destroyes all camera resources.
*/
void CustomCamera::destroyResources()
{
    if (m_camera) {
        m_camera->stop();
    }
    
    m_imageFrame = QImage();
    
    if (m_cameraImageCapture) {
        delete m_cameraImageCapture;
        m_cameraImageCapture = 0;
    }
    
    if (m_mediaRecorder) {
        delete m_mediaRecorder;
        m_mediaRecorder = 0;
    }
    
    delete m_camera;
    m_camera = 0;
    
    m_incomingFrameCounter = 0;
    m_processedFrameCounter = 0;
    
    m_currentDevice = "";
    emit currentDeviceChanged();
}


/*!
  Paints the camera view finder frame.
*/
void CustomCamera::paint(QPainter *painter,
                         const QStyleOptionGraphicsItem *option,
                         QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);
    
    if (!m_imageFrame.isNull()) {
        
        if (m_processedFrameCounter != m_incomingFrameCounter) {
            if (m_imageAnalyzer) {
                m_imageAnalyzer->analyze((unsigned int*)m_imageFrame.bits(),
                                         m_imageFrame.width(),
                                         m_imageFrame.height(),
                                         m_imageFrame.bytesPerLine() / 4,
                                         false);
            }
            
            m_processedFrameCounter = m_incomingFrameCounter;
        }
        
        
        QPointF upperLeft = boundingRect().center() -
                QPointF(m_imageFrame.width() / 2,
                        m_imageFrame.height() / 2);
        
        // Draw the black borders.
        painter->fillRect(0, 0, upperLeft.x(), boundingRect().height(),
                          Qt::black);
        painter->fillRect(upperLeft.x() + m_imageFrame.width(), 0,
                          boundingRect().right(), boundingRect().bottom(),
                          Qt::black);
        
        painter->drawImage(QRect(upperLeft.x(), upperLeft.y(),
                                 m_imageFrame.width(),
                                 m_imageFrame.height()), m_imageFrame);
    }
    else {
        painter->fillRect(boundingRect(), Qt::black);
    }
}


/*!
  Stores the frame as member to allow it to be processed on paint.
  Returns false when there is error, otherwise returns true.
*/
bool CustomCamera::updateFrame(const QVideoFrame &frame)
{
    if (!frame.isValid()) {
        qDebug() << "CustomCameras::updateFrame: Invalid frame";
        return false;
    }
    
    if (m_processedFrameCounter != m_incomingFrameCounter) {
        // Discard frame.
        return true;
    }
    
    m_incomingFrameCounter++;
    
    QVideoFrame f = frame;
    
    if (f.map(QAbstractVideoBuffer::ReadOnly)) {
        if (m_imageFrame.isNull() || m_imageFrame.width() != f.width() ||
                m_imageFrame.height() != f.height()) {
            m_imageFrame = QImage(f.width(), f.height(), QImage::Format_RGB32);
        }
        
        memcpy(m_imageFrame.bits(), f.bits(), f.mappedBytes());
        
        f.unmap();
        
        update();
    }
    
    return true;
}


/*!
  Starts the camera with the given device. The available devices
  can be queried with the availableDevices method. Starting will release
  and destroy all earlier camera resources before creating new ones.
*/
void CustomCamera::start(const QString &device)
{
    destroyResources();
    
    m_camera = new QCamera(device.toLatin1(), this);
    
    m_videoSurface = new VideoSurface(this, m_camera);
    m_camera->setViewfinder(m_videoSurface);
    
    // Set the image capturing objects.
    m_cameraImageCapture = new QCameraImageCapture(m_camera);
    m_cameraImageCapture->setCaptureDestination(
                QCameraImageCapture::CaptureToFile);
    
    // The following code finds a 16:9 resolution and sets it as capture
    // resolution. The view finder image should also change to corresponding
    // aspect ratio.
    
    // Find resolution that matches to device's full screen, 16:9
    QImageEncoderSettings imageSettings;
    imageSettings.setCodec("image/jpeg");
    
    QList<QSize> resolutions = m_cameraImageCapture->supportedResolutions();
    QSize resolution;
    
    foreach (const QSize size, resolutions) {
        float a = size.width() * 1.0f / (size.height() * 1.0f);
        float b = 640.0f / 360.0f;
        
        if (qAbs(a - b) <= 0.1f * qMin(qAbs(a), qAbs(b))) {
            resolution = size;
            break;
        }
    }
    
    if (!resolution.isNull()) {
        imageSettings.setResolution(resolution);
        
        m_cameraImageCapture->setEncodingSettings(imageSettings);
    }
    
    // Set the video recording objects.
    m_mediaRecorder = new QMediaRecorder(m_camera);

    // Camera API
    connect(m_camera, SIGNAL(locked()), this, SIGNAL(locked()));
    connect(m_camera, SIGNAL(lockFailed()), this, SIGNAL(lockFailed()));
    
    connect(m_camera->focus(), SIGNAL(digitalZoomChanged(qreal)),
            this, SIGNAL(digitalZoomChanged()));
    connect(m_camera->focus(), SIGNAL(maximumDigitalZoomChanged(qreal)),
            this, SIGNAL(maximumDigitalZoomChanged()));
    
    connect(m_camera->exposure(), SIGNAL(exposureCompensationChanged(qreal)),
            this, SIGNAL(exposureCompensationChanged()));
    connect(m_camera->exposure(), SIGNAL(isoSensitivityChanged(int)),
            this, SIGNAL(isoValueChanged()));
    
    connect(m_camera, SIGNAL(stateChanged(QCamera::State)),
            this, SLOT(cameraStateChanged(QCamera::State)));
    connect(m_camera, SIGNAL(stateChanged(QCamera::State)),
            this, SIGNAL(cameraStateChanged()));
    
    // Image capture API
    connect(m_cameraImageCapture, SIGNAL(imageSaved(int, QString)),
            this, SLOT(handleImageSaved(int, QString)));
    
    // Capture mode
    connect(m_camera, SIGNAL(captureModeChanged(QCamera::CaptureMode)),
            this, SIGNAL(captureModeChanged()));

    // Video recording API
    connect(m_mediaRecorder, SIGNAL(stateChanged(QMediaRecorder::State)),
            this, SIGNAL(videoRecorderStateChanged()));
    
    connect(m_mediaRecorder, SIGNAL(error(QMediaRecorder::Error)),
            this, SLOT(handleVideoError(QMediaRecorder::Error)));
    
    connect(m_mediaRecorder, SIGNAL(mutedChanged(bool)),
            this, SIGNAL(mutedChanged()));

    connect(m_mediaRecorder, SIGNAL(durationChanged(qint64)),
            this, SIGNAL(videoDurationChanged()));

    // Begin the receiving of view finder frames.
    m_camera->start();

    m_currentDevice = device;
    emit currentDeviceChanged();
    
    if (m_currentDevice == availableDevices()[0]) {
        m_frontCamera = false;
    }
    else {
        m_frontCamera = true;
    }
    
    emit frontCameraChanged();
}


/*!
  Stops the camera and releases all resources.
*/
void CustomCamera::stop()
{
    destroyResources();
}


/*!
  Returns the list of supported ISO values as string list.
*/
QStringList CustomCamera::supportedIsoValues() const
{
    if (!m_camera) {
        return QStringList();
    }
    
    QStringList isoValues;
    isoValues << "Auto";
    
    QList<int> values = m_camera->exposure()->supportedIsoSensitivities();
    
    foreach (int value, values) {
        isoValues << QString("%1").arg(value);
    }
    
    return isoValues;
}


/*!
  Returns the state camera is currently.
*/
CustomCamera::State CustomCamera::cameraState() const
{
    if (!m_camera) {
        return UnloadedState;
    }
    
    return State(m_camera->state());
}


/*!
  Returns the current White Balance mode.
*/
CustomCamera::WhiteBalanceMode CustomCamera::whiteBalanceMode() const
{
    if (!m_camera) {
        return WhiteBalanceAuto;
    }
    
    return WhiteBalanceMode(m_camera->imageProcessing()->whiteBalanceMode());
}


/*!
  Returns the list of supported white balance modes.
*/
QList<int> CustomCamera::supportedWhiteBalanceModes() const
{
    QList<int> modes;
    
    if (!m_camera) {
        return modes;
    }
    
    QCameraImageProcessing *p = m_camera->imageProcessing();
    if (p->isWhiteBalanceModeSupported(
                QCameraImageProcessing::WhiteBalanceAuto))
        modes << WhiteBalanceAuto;
    if (p->isWhiteBalanceModeSupported(
                QCameraImageProcessing::WhiteBalanceManual))
        modes << WhiteBalanceManual;
    if (p->isWhiteBalanceModeSupported(
                QCameraImageProcessing::WhiteBalanceSunlight))
        modes << WhiteBalanceSunlight;
    if (p->isWhiteBalanceModeSupported(
                QCameraImageProcessing::WhiteBalanceCloudy))
        modes << WhiteBalanceCloudy;
    if (p->isWhiteBalanceModeSupported(
                QCameraImageProcessing::WhiteBalanceShade))
        modes << WhiteBalanceShade;
    if (p->isWhiteBalanceModeSupported(
                QCameraImageProcessing::WhiteBalanceTungsten))
        modes << WhiteBalanceTungsten;
    if (p->isWhiteBalanceModeSupported(
                QCameraImageProcessing::WhiteBalanceFluorescent))
        modes << WhiteBalanceFluorescent;
    if (p->isWhiteBalanceModeSupported(
                QCameraImageProcessing::WhiteBalanceIncandescent))
        modes << WhiteBalanceIncandescent;
    if (p->isWhiteBalanceModeSupported(
                QCameraImageProcessing::WhiteBalanceFlash))
        modes << WhiteBalanceFlash;
    if (p->isWhiteBalanceModeSupported(
                QCameraImageProcessing::WhiteBalanceSunset))
        modes << WhiteBalanceSunset;
    if (p->isWhiteBalanceModeSupported(
                QCameraImageProcessing::WhiteBalanceVendor))
        modes << WhiteBalanceVendor;
    
    return modes;
}



/*!
  Returns the current sharpening level.
*/
int CustomCamera::sharpeningLevel() const
{
    if (!m_camera) {
        return 0;
    }
    
    return m_camera->imageProcessing()->sharpeningLevel();
}


/*!
  Returns the current contrast value.
  
*/
int CustomCamera::contrast() const
{
    if (!m_camera) {
        return 0;
    }
    
    return m_camera->imageProcessing()->contrast();
}


/*!
  Returns the current zoom value.
*/
qreal CustomCamera::digitalZoom() const
{
    if (!m_camera) {
        return 1;
    }
    
    return m_camera->focus()->digitalZoom();
}


/*!
  Returns the maximum digital zoom.
*/
qreal CustomCamera::maximumDigitalZoom() const
{
    if (!m_camera) {
        return 1;
    }
    
    return m_camera->focus()->maximumDigitalZoom();
}


/*!
  Returns the current exposure compensation in use.
*/
qreal CustomCamera::exposureCompensation() const
{
    if (!m_camera) {
        return 0;
    }
    
    return m_camera->exposure()->exposureCompensation();
}


/*!
  Returns the current exposure mode in use.
*/
CustomCamera::ExposureMode CustomCamera::exposureMode() const
{
    if (!m_camera) {
        return ExposureAuto;
    }
    
    return ExposureMode(m_camera->exposure()->exposureMode());
}


/*!
  Returns the list of supported exposure modes.
*/
QList<int> CustomCamera::supportedExposureModes() const
{
    QList<int> modes;

    if (!m_camera) {
        return modes;
    }

    QCameraExposure *exposure = m_camera->exposure();
    if (exposure->isExposureModeSupported(QCameraExposure::ExposureAuto))
        modes << ExposureAuto;
    if (exposure->isExposureModeSupported(QCameraExposure::ExposureManual))
        modes << ExposureManual;
    if (exposure->isExposureModeSupported(QCameraExposure::ExposurePortrait))
        modes << ExposurePortrait;
    if (exposure->isExposureModeSupported(QCameraExposure::ExposureNight))
        modes << ExposureNight;
    if (exposure->isExposureModeSupported(QCameraExposure::ExposureBacklight))
        modes << ExposureBacklight;
    if (exposure->isExposureModeSupported(QCameraExposure::ExposureSpotlight))
        modes << ExposureSpotlight;
    if (exposure->isExposureModeSupported(QCameraExposure::ExposureSports))
        modes << ExposureSports;
    if (exposure->isExposureModeSupported(QCameraExposure::ExposureSnow))
        modes << ExposureSnow;
    if (exposure->isExposureModeSupported(QCameraExposure::ExposureBeach))
        modes << ExposureBeach;
    if (exposure->isExposureModeSupported(
                QCameraExposure::ExposureLargeAperture))
        modes << ExposureLargeAperture;
    if (exposure->isExposureModeSupported(
                QCameraExposure::ExposureSmallAperture))
        modes << ExposureSmallAperture;
    if (exposure->isExposureModeSupported(QCameraExposure::ExposureModeVendor))
        modes << ExposureModeVendor;

    return modes;
}


/*!
  Returns the current flash mode in use.
*/
int CustomCamera::flashMode() const
{
    if (!m_camera) {
        return FlashAuto;
    }

    return m_camera->exposure()->flashMode();
}


/*!
  Returns the supported flash modes.
*/
QList<int> CustomCamera::supportedFlashModes() const
{
    QList<int> modes;
    
    if (!m_camera) {
        return modes;
    }
    
    QCameraExposure *exposure = m_camera->exposure();
    
    if (exposure->isFlashModeSupported(QCameraExposure::FlashOff))
        modes << FlashOff;
    if (exposure->isFlashModeSupported(QCameraExposure::FlashOn))
        modes << FlashOn;
    if (exposure->isFlashModeSupported(QCameraExposure::FlashAuto))
        modes << FlashAuto;
    if (exposure->isFlashModeSupported(QCameraExposure::FlashRedEyeReduction))
        modes << FlashRedEyeReduction;

    return modes;
}


/*!
  Returns the current ISO value in use.
*/
QString CustomCamera::isoValue() const
{
    if (!m_camera) {
        return "Auto";
    }
    
    if (m_camera->exposure()->isoSensitivity() == 0) {
        return "Auto";
    }
    
    return QString("%1").arg(m_camera->exposure()->isoSensitivity());
}


/*!
  Returns the list of available devices. Eg. primary camera and front camera.
*/
QStringList CustomCamera::availableDevices() const
{
    QStringList list;
    foreach (const QByteArray array, QCamera::availableDevices()) {
        list << QString(array);
    }
    
    return list;
}


/*!
  Returns the current camera in use.
*/
QString CustomCamera::currentDevice() const
{
    return m_currentDevice;
}


/*!
  Returns true if the secondary camera (front) is in use.
*/
bool CustomCamera::frontCamera() const
{
    return m_frontCamera;
}


/*!
  Returns the pointer to ImageAnalyzer.
*/
ImageAnalyzer* CustomCamera::imageAnalyzer() const
{
    return m_imageAnalyzer;
}


/*!
  Returns the current capture mode.
*/
CustomCamera::CaptureMode CustomCamera::captureMode() const
{
    if (!m_camera) {
        return CaptureStillImage;
    }

    return CaptureMode(m_camera->captureMode());
}


/*!
  Returns false if audio is recorded during the video recording.
*/
bool CustomCamera::muted() const
{
    if (!m_mediaRecorder) {
        return false;
    }

    return m_mediaRecorder->isMuted();
}


/*!
  Returns the list of supported resolutions for video recording. The list is
  restricted to only hold resolutions 1280x720, 640x480, 320x240.
*/
QStringList CustomCamera::videoResolutions() const
{
    if (!m_mediaRecorder) {
        return QStringList();
    }

    QStringList list;

    QList<QSize> resolutions = m_mediaRecorder->supportedResolutions();

    if (qFind(resolutions, QSize(1280, 720)) != resolutions.end()) {
        list << "1280x720";
    }

    if (qFind(resolutions, QSize(640, 480)) != resolutions.end()) {
        list << "640x480";
    }

    if (qFind(resolutions, QSize(320, 240)) != resolutions.end()) {
        list << "320x240";
    }

    return list;
}


/*!
  Returns the list of supported frame rates on video recording.
*/
QList<int> CustomCamera::frameRates() const
{
    if (!m_mediaRecorder) {
        return QList<int>();
    }

    QList<int> frameRates;

    QList<qreal> list = m_mediaRecorder->supportedFrameRates();
    foreach (qreal rate, list) {
        frameRates << rate;
    }

    return frameRates;
}


/*!
  Returns the current resolution for the video recording.
*/
QString CustomCamera::videoResolution() const
{
    if (!m_mediaRecorder) {
        return "0x0";
    }

    QSize resolution = m_mediaRecorder->videoSettings().resolution();

    return QString("%1x%2").arg(resolution.width()).arg(resolution.height());
}


/*!
  Returns the current frame rate of video recorder.
*/
int CustomCamera::frameRate() const
{
    if (!m_mediaRecorder) {
        return 0;
    }

    return m_mediaRecorder->videoSettings().frameRate();
}


/*!
  Returns the current state of the video recorder.
*/
CustomCamera::VideoRecorderState CustomCamera::videoRecorderState() const
{
    if (!m_mediaRecorder) {
        return StoppedState;
    }

    return VideoRecorderState(m_mediaRecorder->state());
}


/*!
  Returns the duration of the currently recorder video in seconds.
*/
QString CustomCamera::videoDuration() const
{
    if (!m_mediaRecorder) {
        return 0;
    }

    QTime time;
    time = time.addMSecs(m_mediaRecorder->duration());

    return time.toString("mm:ss");
}


/*!
  Sets the exposure compensation value for the camera.
*/
void CustomCamera::setExposureCompensation(qreal exposureCompensation)
{
    if (!m_camera) {
        return;
    }
    
    m_camera->unlock();
    
    m_camera->exposure()->setExposureCompensation(exposureCompensation);
}


/*!
  Sets the exposure mode for the camera.
*/
void CustomCamera::setExposureMode(ExposureMode mode)
{
    if (!m_camera) {
        return;
    }
    
    m_camera->unlock();
    
    if (exposureMode() != mode) {
        m_camera->exposure()->setExposureMode(
                    QCameraExposure::ExposureMode(mode));
        
        emit exposureModeChanged();
    }
}


/*!
  Sets the flash mode for the camera.
*/
void CustomCamera::setFlashMode(int mode)
{
    if (!m_camera) {
        return;
    }
    
    m_camera->unlock();
    
    if (flashMode() != mode) {
        m_camera->exposure()->setFlashMode(
                    QCameraExposure::FlashMode(mode));
        
        emit flashModeChanged();
    }
}


/*!
  Sets the ISO value for the camera. Values are 50, 100, 200.., Auto
*/
void CustomCamera::setIsoValue(QString isoValue)
{
    if (!m_camera) {
        return;
    }
    
    m_camera->unlock();
    
    bool ok;
    int value = isoValue.toInt(&ok);
    
    if (ok) {
        m_camera->exposure()->setManualIsoSensitivity(value);
    }
    else {
        if (isoValue == "Auto") {
            m_camera->exposure()->setAutoIsoSensitivity();
        }
    }
}


/*!
  Sets the White Balance mode for the camera.
*/
void CustomCamera::setWhiteBalanceMode(WhiteBalanceMode mode)
{
    if (!m_camera) {
        return;
    }
    
    m_camera->unlock();
    
    if (whiteBalanceMode() != mode)     {
        m_camera->imageProcessing()->setWhiteBalanceMode(
                    QCameraImageProcessing::WhiteBalanceMode(mode));
        
        emit whiteBalanceModeChanged();
    }
}


/*!
  Sets the sharpening level, the values are
     -1 = default sharpening level
      0 = sharpening disabled
    100 = maximum sharpening
*/
void CustomCamera::setSharpeningLevel(int level)
{
    if (!m_camera) {
        return;
    }
    
    if (sharpeningLevel() != level) {
        m_camera->imageProcessing()->setSharpeningLevel(level);
    }
}


/*!
  Sets the contrast value, the values are between -100 and 100,
  the default is 0.
*/
void CustomCamera::setContrast(int value)
{
    if (!m_camera) {
        return;
    }
    
    if (contrast() != value) {
        m_camera->imageProcessing()->setContrast(value);
    }
}


/*!
  Sets the image analyzer pointer. The image will be analyzed on paint.
*/
void CustomCamera::setImageAnalyzer(ImageAnalyzer *imageAnalyzer)
{
    m_imageAnalyzer = imageAnalyzer;
    
    emit imageAnalyzerChanged();
}


/*!
  Sets the capture mode of the camera.
*/
void CustomCamera::setCaptureMode(CaptureMode mode)
{
    if (!m_camera) {
        return;
    }

    m_camera->setCaptureMode(QCamera::CaptureMode(mode));
}


/*!
  Mutes / unmutes the audio recording on video recording.
*/
void CustomCamera::setMuted(bool muted)
{
    if (!m_mediaRecorder) {
        return;
    }

    m_mediaRecorder->setMuted(muted);
}


/*!
  Sets the resolution for the video recording. The resolution must be one of the
  resolutions returned by the videoResolutions method.
*/
void CustomCamera::setVideoResolution(QString resolution)
{
    if (!m_mediaRecorder) {
        return;
    }

    QStringList list = resolution.split('x');
    if (list.size() != 2) {
        return;
    }

    int width = list[0].toInt();
    int height = list[1].toInt();

    QAudioEncoderSettings audioSettings = m_mediaRecorder->audioSettings();

    QVideoEncoderSettings videoSettings = m_mediaRecorder->videoSettings();
    videoSettings.setResolution(QSize(width, height));

    m_mediaRecorder->setEncodingSettings(audioSettings, videoSettings);

    emit videoResolutionChanged();
}


/*!
  Sets the current frame rate for the video recorder.
*/
void CustomCamera::setFrameRate(int frameRate)
{
    if (!m_mediaRecorder) {
        return;
    }

    QAudioEncoderSettings audioSettings = m_mediaRecorder->audioSettings();

    QVideoEncoderSettings videoSettings = m_mediaRecorder->videoSettings();
    videoSettings.setFrameRate(frameRate);

    m_mediaRecorder->setEncodingSettings(audioSettings, videoSettings);

    emit frameRateChanged();
}


/*!
  Gets info about the state change. When the camera is loaded we must emit
  the camera API signal in order to QML side to know about the changed camera.
*/
void CustomCamera::cameraStateChanged(QCamera::State state)
{
    if (state == QCamera::ActiveState) {
        emit digitalZoomChanged();
        emit maximumDigitalZoomChanged();
        
        emit exposureCompensationChanged();
        emit exposureModeChanged();
        emit supportedExposureModesChanged();
        
        emit flashModeChanged();
        emit supportedFlashModesChanged();
        
        emit isoValueChanged();
        emit supportedIsoValuesChanged();
        
        emit whiteBalanceModeChanged();
        emit supportedWhiteBalanceModesChanged();
        emit sharpeningLevelChanged();
        emit contrastChanged();

        emit captureModeChanged();

        emit mutedChanged();
        emit videoResolutionsChanged();
        emit frameRatesChanged();
        emit videoResolutionChanged();
        emit frameRateChanged();

        emit videoDurationChanged();
    }
}


/*!
  Logs the error in video recordings.
*/
void CustomCamera::handleVideoError(QMediaRecorder::Error error)
{
    qDebug() << "CustomCamera::handleVideoError: Error: " << error;
}


/*!
  Capture image with the currently set attributes.
*/
void CustomCamera::captureImage()
{
    if (!m_cameraImageCapture) {
        return;
    }
    
    QString fileName =
            QDateTime::currentDateTime().toString("yyyy-MM-dd-hh-mm-ss.jpg");
    
    m_cameraImageCapture->capture(fileName);
}


/*!
  Begins the recording of video.
*/
void CustomCamera::beginRecording()
{
    if (!m_mediaRecorder) {
        return;
    }
    
    m_mediaRecorder->record();
}


/*!
  Pauses the recording of video.
*/
void CustomCamera::pauseRecording()
{
    if (!m_mediaRecorder) {
        return;
    }

    m_mediaRecorder->pause();
}


/*!
  Ends the recording of video.
*/
void CustomCamera::endRecording()
{
    qDebug() << "CustomCamera::endRecording";

    if (!m_mediaRecorder) {
        return;
    }
    
    m_mediaRecorder->stop();
}


/*!
  Focuses on the given coordinate. Currently the manual focus is not supported
  by the backend.
*/
void CustomCamera::focusToCoordinate(int x, int y)
{
    Q_UNUSED(x);
    Q_UNUSED(y);
    
    if (!m_camera) {
        return;
    }
    
    m_camera->searchAndLock();
}


/*!
  Requests to set the zoom to given values.
*/
void CustomCamera::zoomTo(qreal optical, qreal digital)
{
    if (!m_camera) {
        return;
    }
    
    m_camera->focus()->zoomTo(optical, digital);
}


/*!
  Unlocks the focus.
*/
void CustomCamera::unlock()
{
    if (!m_camera) {
        return;
    }
    
    m_camera->unlock();
}


/*!
  Handles the image saved signal. Emits imageSaved signal delayed, this must be
  done in order to be possible to stop the camera from the QML. If
  QCameraImageCapture::imageSaved signal was directly connected to the QML and
  the camera would be stopped, the application would crash as the stop will
  destroy the QCameraImageCapture instance.
*/
void CustomCamera::handleImageSaved(int id, const QString &fileName)
{
    Q_UNUSED(id);

    m_galleryImage = fileName;

    QTimer::singleShot(0, this, SLOT(emitImageSavedSignal()));
}


/*!
  Emits imageSaved signal.
*/
void CustomCamera::emitImageSavedSignal()
{
    emit imageSaved(m_galleryImage);
}
