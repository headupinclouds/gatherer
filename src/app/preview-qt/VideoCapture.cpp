#include "VideoCapture.h"

#include <opencv2/core/core.hpp>
#include <iostream>

VideoCapture::VideoCapture(QObject * parent) : QObject(parent) {}

void VideoCapture::start(int cam)
{
    if (!m_videoCapture)
        m_videoCapture.reset(new cv::VideoCapture(cam));
    if (m_videoCapture->isOpened())
    {
        m_timer.start(0, this);
        emit started();
    }
}
void VideoCapture::stop() { m_timer.stop(); }

void VideoCapture::timerEvent(QTimerEvent * ev)
{
    if (ev->timerId() != m_timer.timerId()) return;
    cv::Mat frame;
    if (!m_videoCapture->read(frame))
    { // Blocks until a new frame is ready

        std::cout << "got frame " << std::endl;

        m_timer.stop();
        return;
    }
    emit matReady(frame);
}
