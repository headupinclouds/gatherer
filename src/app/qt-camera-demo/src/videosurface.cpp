/**
 * Copyright (c) 2012-2014 Microsoft Mobile.
 */

#include "videosurface.h"

#include <QtGui>
#include "customcamera.h"

/*!
  \class VideoSurface
  \brief Receives the frames from the camera.
*/


/*!
  Constructor.
*/
VideoSurface::VideoSurface(FrameObserver *frameObserver, QObject *parent)
    : QAbstractVideoSurface(parent),
      m_frameObserver(frameObserver)
{
    setError(QAbstractVideoSurface::NoError);
}


/*!
  Destructor.
*/
VideoSurface::~VideoSurface()
{
}


/*!
  Sets the list of supported pixel formats that we are intrested of.
*/
QList<QVideoFrame::PixelFormat> VideoSurface::supportedPixelFormats(
        QAbstractVideoBuffer::HandleType handleType) const
{
    Q_UNUSED(handleType);

    return QList<QVideoFrame::PixelFormat>() << QVideoFrame::Format_ARGB32;
}


/*!
  Starts the receiving of the frames for the view finder.
*/
bool VideoSurface::start(const QVideoSurfaceFormat &format)
{
    if (isActive()) {
        stop();
    }

    if (format.frameSize().isEmpty()) {
        return false;
    }

    if (error() == QAbstractVideoSurface::NoError) {
        return QAbstractVideoSurface::start(format);
    }
    else {
        stop();

    }

    return false;
}


/*!
  Handles the just taken frame from camera and passes it to the observer.
  Returning false will stop the surface.
*/
bool VideoSurface::present(const QVideoFrame &frame)
{
    return m_frameObserver->updateFrame(frame);
}
