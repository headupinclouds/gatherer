/**
 * Copyright (c) 2012 Nokia Corporation.
 */

#ifndef VIDEOSURFACE_H
#define VIDEOSURFACE_H

#include <QAbstractVideoSurface>
#include <QVideoSurfaceFormat>

class FrameObserver;

class VideoSurface : public QAbstractVideoSurface
{
    Q_OBJECT
public:
    explicit VideoSurface(FrameObserver *frameObserver, QObject *parent = 0);
    virtual ~VideoSurface();
    
    virtual QList<QVideoFrame::PixelFormat> supportedPixelFormats(QAbstractVideoBuffer::HandleType handleType = QAbstractVideoBuffer::NoHandle) const;
    virtual bool present(const QVideoFrame &frame);
    virtual bool start(const QVideoSurfaceFormat &format);

protected:
    FrameObserver *m_frameObserver; // Not owned
};

#endif // VIDEOSURFACE_H
