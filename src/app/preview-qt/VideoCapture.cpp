#include "VideoCapture.h"

#include <opencv2/core/core.hpp>
#include <iostream>

VideoCapture::VideoCapture(QObject * parent, bool synth) : QObject(parent), m_synth(synth) {}

bool VideoCapture::getFrame(cv::Mat &frame)
{
    if(m_synth)
    {
        frame.create(640, 480, CV_8UC3);
        cv::RNG rng;
        rng.state = rand();
        cv::Point center(rng.uniform(0, frame.cols), rng.uniform(0, frame.rows));
        for(int r = 20; r < std::max(frame.cols, frame.rows)/2;  r+= 20)
            cv::circle(frame, center, r, {0,255,0}, 1, 8);
        
        return true;
    }
    else
    {
        return m_videoCapture->read(frame);
    }
}

void VideoCapture::start(int cam)
{
    if (!m_videoCapture && !m_synth)
        m_videoCapture.reset(new cv::VideoCapture(cam));

    if (m_synth || m_videoCapture->isOpened())
    {
        m_timer.start(0, this);
        emit started();
    }
    
    assert(m_synth || m_videoCapture->isOpened()); // for now
}
void VideoCapture::stop() { m_timer.stop(); }

void VideoCapture::timerEvent(QTimerEvent * ev)
{
    if (ev->timerId() != m_timer.timerId()) return;
    cv::Mat frame;
    if (!getFrame(frame))
    { // Blocks until a new frame is ready
        std::cout << "Didn't capture frame " << std::endl;
        m_timer.stop();
        return;
    }
    else
    {
        emit matReady(frame);
    }
}
