/****************************************************************************
**
** Copyright (C) 2015 The Qt Company Ltd.
** Copyright (C) 2015 Ruslan Baratov
** Contact: http://www.qt.io/licensing/
**
** This file is part of the examples of the Qt Multimedia module.
**
** $QT_BEGIN_LICENSE:LGPL21$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see http://www.qt.io/terms-conditions. For further
** information use the contact form at http://www.qt.io/contact-us.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 2.1 or version 3 as published by the Free
** Software Foundation and appearing in the file LICENSE.LGPLv21 and
** LICENSE.LGPLv3 included in the packaging of this file. Please review the
** following information to ensure the GNU Lesser General Public License
** requirements will be met: https://www.gnu.org/licenses/lgpl.html and
** http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** As a special exception, The Qt Company gives you certain additional
** rights. These rights are described in The Qt Company LGPL Exception
** version 1.1, included in the file LGPL_EXCEPTION.txt in this package.
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include "VideoFilterRunnable.hpp"

#include <cassert> // assert

#include <graphics/GLExtra.h> // GATHERER_OPENGL_DEBUG

#include "VideoFilter.hpp"
#include "TextureBuffer.hpp"

VideoFilterRunnable::VideoFilterRunnable(VideoFilter *filter) :
    m_filter(filter),
    m_tempTexture(0),
    m_outTexture(0),
    m_lastInputTexture(0) {
  QOpenGLFunctions *f = QOpenGLContext::currentContext()->functions();

  const char *vendor = (const char *) f->glGetString(GL_VENDOR);
  qDebug("GL_VENDOR: %s", vendor);
}

VideoFilterRunnable::~VideoFilterRunnable() {
  releaseTextures();
}

QVideoFrame VideoFilterRunnable::run(
    QVideoFrame *input, const QVideoSurfaceFormat &surfaceFormat, RunFlags flags
) {
  Q_UNUSED(surfaceFormat);
  Q_UNUSED(flags);

  // This example supports RGB data only, either in system memory (typical with
  // cameras on all platforms) or as an OpenGL texture (e.g. video playback on
  // OS X).  The latter is the fast path where everything happens on GPU. The
  // former involves a texture upload.

  if (!isFrameValid(*input)) {
    qWarning("Invalid input format");
    return *input;
  }

  if (isFrameFormatYUV(*input)) {
    qWarning("YUV data is not supported");
    return *input;
  }

  if (m_size != input->size()) {
    releaseTextures();
    m_size = input->size();
  }

  // Image objects cannot be read and written at the same time. So use
  // a separate texture for the result.
  // TODO: do something with texture
  //if (!m_outTexture)
  //    m_outTexture = newTexture();
  m_outTexture = createTextureForFrame(input);

  // Accessing dynamic properties on the filter element is simple:
  qreal factor = m_filter->factor();
  return TextureBuffer::createVideoFrame(
      m_outTexture, m_size, input->pixelFormat()
  );
}

void VideoFilterRunnable::releaseTextures() {
  QOpenGLFunctions *f = QOpenGLContext::currentContext()->functions();
  if (m_tempTexture) {
    f->glDeleteTextures(1, &m_tempTexture);
  }
  if (m_outTexture) {
    f->glDeleteTextures(1, &m_outTexture);
  }
  m_tempTexture = m_outTexture = m_lastInputTexture = 0;
}

uint VideoFilterRunnable::newTexture() {
  QOpenGLFunctions *f = QOpenGLContext::currentContext()->functions();
  GLuint texture;
  f->glGenTextures(1, &texture);
  f->glBindTexture(GL_TEXTURE_2D, texture);
  f->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  f->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  f->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  f->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

  // http://stackoverflow.com/a/9617429/2288008
  f->glTexImage2D(
      GL_TEXTURE_2D, // target
      0, // level
      GL_RGBA, // internalformat
      m_size.width(),
      m_size.height(),
      0, // border
#if defined(Q_OS_IOS)
      GL_BGRA, // format
#else
      GL_RGBA, // format
#endif
      GL_UNSIGNED_BYTE, // type
      0 // data
  );
  GATHERER_OPENGL_DEBUG;
  assert(texture != 0);
  return texture;
}

bool VideoFilterRunnable::isFrameValid(const QVideoFrame& frame) {
  if (!frame.isValid()) {
    return false;
  }
  if (frame.handleType() == QAbstractVideoBuffer::NoHandle) {
    return true;
  }
  if (frame.handleType() == QAbstractVideoBuffer::GLTextureHandle) {
    return true;
  }
  return false;
}

bool VideoFilterRunnable::isFrameFormatYUV(const QVideoFrame& frame) {
  if (frame.pixelFormat() == QVideoFrame::Format_YUV420P) {
    return true;
  }
  if (frame.pixelFormat() == QVideoFrame::Format_YV12) {
    return true;
  }
  return false;
}

// Create a texture from the image data.
GLuint VideoFilterRunnable::createTextureForFrame(QVideoFrame* input) {
  QOpenGLContext* openglContext = QOpenGLContext::currentContext();
  if (!openglContext) {
    qWarning("Can't get context!");
    return 0;
  }
  assert(openglContext->isValid());

  QOpenGLFunctions *f = openglContext->functions();
  assert(f != 0);

  // Already an OpenGL texture.
  if (input->handleType() == QAbstractVideoBuffer::GLTextureHandle) {
    GLuint texture = input->handle().toUInt();
    assert(texture != 0);
    f->glBindTexture(GL_TEXTURE_2D, texture);
    m_lastInputTexture = texture;
    return texture;
  }

  assert(input->handleType() == QAbstractVideoBuffer::NoHandle);

  // Upload.
  if (m_tempTexture) {
    f->glBindTexture(GL_TEXTURE_2D, m_tempTexture);
    GATHERER_OPENGL_DEBUG;
  }
  else {
    m_tempTexture = newTexture();
  }

  bool ok = input->map(QAbstractVideoBuffer::ReadOnly);
  if (!ok) {
    qWarning("Can't map!");
    return 0;
  }

  // glTexImage2D only once and use TexSubImage later on. This avoids the need
  // to recreate the CL image object on every frame.

  f->glTexSubImage2D(
      GL_TEXTURE_2D, // target
      0, // level
      0, // xoffset
      0, // yoffset
      m_size.width(),
      m_size.height(),
#if defined(Q_OS_IOS)
      GL_BGRA, // format
#else
      GL_RGBA, // format
#endif
      GL_UNSIGNED_BYTE, // type
      input->bits() // data
  );
  GATHERER_OPENGL_DEBUG;

  input->unmap();
  return m_tempTexture;
}
