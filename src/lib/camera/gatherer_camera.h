//
//  gatherer_camera.h
//  GATHERER
//
//  Created by David Hirvonen on 9/5/15.
//
//

#ifndef GATHERER_gatherer_camera_h
#define GATHERER_gatherer_camera_h

#define _GATHERER_CAMERA_BEGIN namespace gatherer { namespace camera {
#define _GATHERER_CAMERA_END } }

_GATHERER_CAMERA_BEGIN

// Modeled on opencv interface:
// http://docs.opencv.org/modules/highgui/doc/reading_and_writing_images_and_video.html#videocapture
class VideoCapture
{
public:
    VideoCapture() {}
    VideoCapture(const std::string &filename) {}
    VideoCapture(int device) {}

    virtual ~VideoCapture() = 0;
    virtual bool open(const string& filename) = 0;
    virtual bool open(int device) = 0;
    virtual bool isOpened() = 0;
    virtual void release() = 0;
    virtual bool grab() = 0;
    virtual bool retrieve(cv::Mat& image, int channel=0) = 0;

    // cv::Mat output
    virtual VideoCapture& operator>>(cv::Mat& image) = 0;
    virtual bool read(cv::Mat& image) = 0;

    // opengl texture output
    virtual VideoCapture& operator>>(unsigned int texture) = 0;
    virtual bool read(unsigned int texture) = 0;

    virtual double get(int propId) = 0;
    virtual bool set(int propId, double value) = 0;
};

_GATHERER_CAMERA_END

#endif
