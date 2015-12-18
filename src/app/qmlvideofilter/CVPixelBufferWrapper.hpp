#ifndef QMLVIDEOFILTER_CVPIXELBUFFER_WRAPPER_HPP_
#define QMLVIDEOFILTER_CVPIXELBUFFER_WRAPPER_HPP_

class QVideoFrame;

class CVPixelBufferWrapper {
 public:
  CVPixelBufferWrapper(QVideoFrame& frame);

  CVPixelBufferWrapper(const CVPixelBufferWrapper&) = delete;
  CVPixelBufferWrapper& operator= (const CVPixelBufferWrapper&) = delete;

  void* getCVPixelBufferRef() const {
    return pixelBuffer_;
  }

  ~CVPixelBufferWrapper();

 private:
  QVideoFrame& frame_;
  void* pixelBuffer_; // instance of CVPixelBufferRef
};

#endif // QMLVIDEOFILTER_CVPIXELBUFFER_WRAPPER_HPP_
