/**
 * Copyright (c) 2012 Nokia Corporation.
 */

#ifndef CUSTOMCAMERA_H
#define CUSTOMCAMERA_H

#include <QCamera>
#include <QDeclarativeItem>
#include <QMediaRecorder>
#include <QVideoFrame>

class QCameraImageCapture;
class ImageAnalyzer;
class VideoSurface;


// Observer design pattern to provide the view finder frame for the CustomCamera
// object.
class FrameObserver {
public:
    virtual bool updateFrame(const QVideoFrame &frame) = 0;
};


class CustomCamera : public QDeclarativeItem, public FrameObserver
{
    Q_OBJECT

    Q_ENUMS(ExposureMode)
    Q_ENUMS(FlashMode)
    Q_ENUMS(State)
    Q_ENUMS(WhiteBalanceMode)
    Q_ENUMS(CaptureMode)
    Q_ENUMS(VideoRecorderState)

    // Zoom properties
    Q_PROPERTY(qreal digitalZoom READ digitalZoom NOTIFY digitalZoomChanged)
    Q_PROPERTY(qreal maximumDigitalZoom READ maximumDigitalZoom NOTIFY maximumDigitalZoomChanged)

    // Exposure properties
    Q_PROPERTY(qreal exposureCompensation READ exposureCompensation WRITE setExposureCompensation NOTIFY exposureCompensationChanged)
    Q_PROPERTY(ExposureMode exposureMode READ exposureMode WRITE setExposureMode NOTIFY exposureModeChanged)
    Q_PROPERTY(QList<int> supportedExposureModes READ supportedExposureModes NOTIFY supportedExposureModesChanged)

    // Flash properties
    Q_PROPERTY(int flashMode READ flashMode WRITE setFlashMode NOTIFY flashModeChanged)
    Q_PROPERTY(QList<int> supportedFlashModes READ supportedFlashModes NOTIFY supportedFlashModesChanged)

    // ISO properties
    Q_PROPERTY(QString isoValue READ isoValue WRITE setIsoValue NOTIFY isoValueChanged)
    Q_PROPERTY(QStringList supportedIsoValues READ supportedIsoValues NOTIFY supportedIsoValuesChanged)

    // State properties
    Q_PROPERTY(State cameraState READ cameraState NOTIFY cameraStateChanged)

    // Image processing properties
    Q_PROPERTY(WhiteBalanceMode whiteBalanceMode READ whiteBalanceMode WRITE setWhiteBalanceMode NOTIFY whiteBalanceModeChanged)
    Q_PROPERTY(QList<int> supportedWhiteBalanceModes READ supportedWhiteBalanceModes NOTIFY supportedWhiteBalanceModesChanged)
    Q_PROPERTY(int sharpeningLevel READ sharpeningLevel WRITE setSharpeningLevel NOTIFY sharpeningLevelChanged)
    Q_PROPERTY(int contrast READ contrast WRITE setContrast NOTIFY contrastChanged)

    // Devices properties
    Q_PROPERTY(QStringList availableDevices READ availableDevices)
    Q_PROPERTY(QString currentDevice READ currentDevice NOTIFY currentDeviceChanged)
    Q_PROPERTY(bool frontCamera READ frontCamera NOTIFY frontCameraChanged)

    // Image analyzer properties
    Q_PROPERTY(ImageAnalyzer *imageAnalyzer READ imageAnalyzer WRITE setImageAnalyzer NOTIFY imageAnalyzerChanged)

    // Capture mode properties
    Q_PROPERTY(CaptureMode captureMode READ captureMode WRITE setCaptureMode NOTIFY captureModeChanged)

    // Video recording properties
    Q_PROPERTY(bool muted READ muted WRITE setMuted NOTIFY mutedChanged)
    Q_PROPERTY(QStringList videoResolutions READ videoResolutions NOTIFY videoResolutionsChanged)
    Q_PROPERTY(QList<int> frameRates READ frameRates NOTIFY frameRatesChanged)
    Q_PROPERTY(QString videoResolution READ videoResolution WRITE setVideoResolution NOTIFY videoResolutionChanged)
    Q_PROPERTY(int frameRate READ frameRate WRITE setFrameRate NOTIFY frameRateChanged)
    Q_PROPERTY(VideoRecorderState videoRecorderState READ videoRecorderState NOTIFY videoRecorderStateChanged)
    Q_PROPERTY(QString videoDuration READ videoDuration NOTIFY videoDurationChanged)

public:
    enum ExposureMode {
        ExposureAuto = QCameraExposure::ExposureAuto,
        ExposureManual = QCameraExposure::ExposureManual,
        ExposurePortrait = QCameraExposure::ExposurePortrait,
        ExposureNight = QCameraExposure::ExposureNight,
        ExposureBacklight = QCameraExposure::ExposureBacklight,
        ExposureSpotlight = QCameraExposure::ExposureSpotlight,
        ExposureSports = QCameraExposure::ExposureSports,
        ExposureSnow = QCameraExposure::ExposureSnow,
        ExposureBeach = QCameraExposure::ExposureBeach,
        ExposureLargeAperture = QCameraExposure::ExposureLargeAperture,
        ExposureSmallAperture = QCameraExposure::ExposureSmallAperture,
        ExposureModeVendor = QCameraExposure::ExposureModeVendor
    };

    enum FlashMode {
        FlashAuto = QCameraExposure::FlashAuto,
        FlashOff = QCameraExposure::FlashOff,
        FlashOn = QCameraExposure::FlashOn,
        FlashRedEyeReduction  = QCameraExposure::FlashRedEyeReduction,
        FlashFill = QCameraExposure::FlashFill,
        FlashTorch = QCameraExposure::FlashTorch,
        FlashSlowSyncFrontCurtain = QCameraExposure::FlashSlowSyncFrontCurtain,
        FlashSlowSyncRearCurtain = QCameraExposure::FlashSlowSyncRearCurtain,
        FlashManual = QCameraExposure::FlashManual
    };

    enum State {
        ActiveState = QCamera::ActiveState,
        LoadedState = QCamera::LoadedState,
        UnloadedState = QCamera::UnloadedState
    };

    enum WhiteBalanceMode {
        WhiteBalanceAuto = QCameraImageProcessing::WhiteBalanceAuto,
        WhiteBalanceManual = QCameraImageProcessing::WhiteBalanceManual,
        WhiteBalanceSunlight = QCameraImageProcessing::WhiteBalanceSunlight,
        WhiteBalanceCloudy = QCameraImageProcessing::WhiteBalanceCloudy,
        WhiteBalanceShade = QCameraImageProcessing::WhiteBalanceShade,
        WhiteBalanceTungsten = QCameraImageProcessing::WhiteBalanceTungsten,
        WhiteBalanceFluorescent = QCameraImageProcessing::WhiteBalanceFluorescent,
        WhiteBalanceIncandescent = QCameraImageProcessing::WhiteBalanceIncandescent,
        WhiteBalanceFlash = QCameraImageProcessing::WhiteBalanceFlash,
        WhiteBalanceSunset = QCameraImageProcessing::WhiteBalanceSunset,
        WhiteBalanceVendor = QCameraImageProcessing::WhiteBalanceVendor
    };

    enum CaptureMode {
        CaptureStillImage = QCamera::CaptureStillImage,
        CaptureVideo = QCamera::CaptureVideo
    };

    enum VideoRecorderState {
        StoppedState = QMediaRecorder::StoppedState,
        RecordingState = QMediaRecorder::RecordingState,
        PausedState = QMediaRecorder::PausedState
    };

    explicit CustomCamera(QDeclarativeItem *parent = 0);
    virtual ~CustomCamera();

    virtual void paint(QPainter *painter,
                       const QStyleOptionGraphicsItem *option,
                       QWidget *widget);

    virtual bool updateFrame(const QVideoFrame &frame);

protected slots:
    void cameraStateChanged(QCamera::State state);
    void handleImageSaved(int id, const QString &fileName);

    void handleVideoError(QMediaRecorder::Error error);

    void emitImageSavedSignal();

public slots:
    void start(const QString &device);
    void stop();

    void captureImage();
    void beginRecording();
    void pauseRecording();
    void endRecording();

    void focusToCoordinate(int x, int y);
    void zoomTo(qreal optical, qreal digital);
    void unlock();

signals:
    void digitalZoomChanged();
    void maximumDigitalZoomChanged();

    void exposureCompensationChanged();
    void exposureModeChanged();
    void supportedExposureModesChanged();

    void flashModeChanged();
    void supportedFlashModesChanged();

    void locked();
    void lockFailed();

    void isoValueChanged();
    void supportedIsoValuesChanged();

    void imageSaved(const QString &fileName);

    void cameraStateChanged();

    void whiteBalanceModeChanged();
    void supportedWhiteBalanceModesChanged();
    void sharpeningLevelChanged();
    void contrastChanged();

    void currentDeviceChanged();
    void frontCameraChanged();

    void imageAnalyzerChanged();

    void captureModeChanged();

    void mutedChanged();
    void videoResolutionsChanged();
    void frameRatesChanged();
    void videoResolutionChanged();
    void frameRateChanged();

    void videoRecorderStateChanged();

    void videoDurationChanged();

protected:
    void destroyResources();

    // Getters
    qreal digitalZoom() const;
    qreal maximumDigitalZoom() const;

    qreal exposureCompensation() const;
    ExposureMode exposureMode() const;
    QList<int> supportedExposureModes() const;

    int flashMode() const;
    QList<int> supportedFlashModes() const;

    QString isoValue() const;
    QStringList supportedIsoValues() const;

    State cameraState() const;

    WhiteBalanceMode whiteBalanceMode() const;
    QList<int> supportedWhiteBalanceModes() const;
    int sharpeningLevel() const;
    int contrast() const;

    QStringList availableDevices() const;
    QString currentDevice() const;
    bool frontCamera() const;

    ImageAnalyzer* imageAnalyzer() const;

    CaptureMode captureMode() const;

    bool muted() const;
    QStringList videoResolutions() const;
    QList<int> frameRates() const;
    QString videoResolution() const;
    int frameRate() const;
    VideoRecorderState videoRecorderState() const;
    QString videoDuration() const;



    // Setters
    void setExposureCompensation(qreal exposureCompensation);
    void setExposureMode(ExposureMode mode);

    void setFlashMode(int mode);

    void setIsoValue(QString isoValue);

    void setWhiteBalanceMode(WhiteBalanceMode mode);
    void setSharpeningLevel(int level);
    void setContrast(int value);

    void setImageAnalyzer(ImageAnalyzer *imageAnalyzer);

    void setCaptureMode(CaptureMode mode);

    void setMuted(bool muted);
    void setVideoResolution(QString resolution);
    void setFrameRate(int frameRate);

protected:
    QCamera *m_camera; // Owned

    QString m_currentDevice;
    bool m_frontCamera;

    // Still image capturing
    QCameraImageCapture *m_cameraImageCapture; // Owned

    // Video capturing
    QMediaRecorder *m_mediaRecorder; // Owned

    // For showing view finder image
    VideoSurface *m_videoSurface; // Owned

    // To analyze view finder image
    ImageAnalyzer *m_imageAnalyzer; // Not owned

    // For storing the latest frame
    unsigned int m_processedFrameCounter;
    unsigned int m_incomingFrameCounter;
    QImage m_imageFrame;

    // File to be opened in the gallery.
    QString m_galleryImage;
};

QML_DECLARE_TYPE(CustomCamera)

#endif // CUSTOMCAMERA_H
