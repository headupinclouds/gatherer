#include <QApplication>
#include <QDesktopWidget>
#include <QSurfaceFormat>
#include <QtGlobal> // Q_OS_IOS
#include <QApplication>
#include <QBasicTimer>
#include <QThread>
#include <QImage>
#include <QWidget>
#include <QPainter>
#include <QPaintEvent>
#include <QDebug>

#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>

#ifndef _CAPTURE_H_
#define _CAPTURE_H_

// http://stackoverflow.com/questions/21246766/how-to-efficiently-display-opencv-video-in-qt
class VideoCapture : public QObject
{
    Q_OBJECT
    QBasicTimer m_timer;
    QScopedPointer<cv::VideoCapture> m_videoCapture;
    bool getFrame(cv::Mat &);
public:
    VideoCapture(QObject * parent = 0, bool synth=false);
    Q_SIGNAL void started();
    Q_SLOT void start(int cam = 0);
    Q_SLOT void stop();
    Q_SIGNAL void matReady(const cv::Mat &);
private:
    void timerEvent(QTimerEvent * ev);
    
    bool m_synth = false;
};

#endif // _CAPTURE_H_
