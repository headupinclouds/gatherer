#include "CVPixelBufferWrapper.hpp"

#include <QVideoFrame>
#include <cassert> // assert

#import <CoreVideo/CoreVideo.h> // CVPixelBufferCreateWithBytes

CVPixelBufferWrapper::CVPixelBufferWrapper(QVideoFrame& frame):
    frame_(frame),
    pixelBuffer_(nullptr) {
  bool ok = frame_.map(QAbstractVideoBuffer::ReadOnly);
  assert(ok);

  CVPixelBufferRef x;
  auto result = CVPixelBufferCreateWithBytes(
      kCFAllocatorDefault,
      frame_.size().width(),
      frame_.size().height(),
      kCVPixelFormatType_32BGRA,
      frame_.bits(),
      frame_.bytesPerLine(),
      nullptr, // releaseCallback
      nullptr, // releaseRefCon
      nullptr, // pixelBufferAttributes
      &x
  );

  assert(result == kCVReturnSuccess);
  assert(x != nullptr);

  pixelBuffer_ = x;
}

CVPixelBufferWrapper::~CVPixelBufferWrapper() {
  CVPixelBufferRef x = (CVPixelBufferRef)pixelBuffer_;
  CVPixelBufferRelease(x);
  frame_.unmap();
}
