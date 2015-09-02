/****************************************************************************
**
** Copyright (C) 2015 The Qt Company Ltd.
** Contact: http://www.qt.io/licensing/
**
** This file is part of the examples of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:BSD$
** You may use this file under the terms of the BSD license as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**   * Neither the name of The Qt Company Ltd nor the names of its
**     contributors may be used to endorse or promote products derived
**     from this software without specific prior written permission.
**
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include "glwidget.h"
#include <QMouseEvent>
#include <QOpenGLShaderProgram>
#include <QCoreApplication>
#include <math.h>

#include "graphics/GLWarpShader.h"
#include "graphics/GLTexture.h"

#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>

#include <memory>

GLWidget::GLWidget(QWidget *parent) : QOpenGLWidget(parent)
{
    m_core = QCoreApplication::arguments().contains(QStringLiteral("--coreprofile"));
    // --transparent causes the clear color to be transparent. Therefore, on systems that
    // support it, the widget will become transparent apart from the logo.
}

QSize GLWidget::minimumSizeHint() const
{
    return QSize(50, 50);
}

QSize GLWidget::sizeHint() const
{
    return QSize(400, 400);
}

GLWidget::~GLWidget()
{
    cleanup();
}

// Call at startup only:
void GLWidget::setVideoDimensions(int width, int height)
{
    m_videoWidth = width;
    m_videoHeight = height;
}

void GLWidget::cleanup()
{

}

void GLWidget::initializeGL()
{
    // In this example the widget's corresponding top-level window can change
    // several times during the widget's lifetime. Whenever this happens, the
    // QOpenGLWidget's associated context is destroyed and a new one is created.
    // Therefore we have to be prepared to clean up the resources on the
    // aboutToBeDestroyed() signal, instead of the destructor. The emission of
    // the signal will be followed by an invocation of initializeGL() where we
    // can recreate all resources.
    connect(context(), &QOpenGLContext::aboutToBeDestroyed, this, &GLWidget::cleanup);

    initializeOpenGLFunctions();
    
    // TODO: need to populate these form our video source in our video event loop
    m_program = std::make_shared<gatherer::graphics::WarpShader>( cv::Size(m_windowWidth, m_windowHeight), cv::Point2f(1,1) );
    m_videoTexture = std::make_shared<gatherer::graphics::GLTexture>();
}

// TODO: need to pass real texture from our video event loop
#define USE_REAL_TEXTURE_INPUT 1

void GLWidget::paintGL()
{
    std::unique_lock<std::mutex> lock(m_mutex);
    if(!m_currentFrame.empty())
    {
	m_videoTexture->load(m_currentFrame);
	m_texture = (*m_videoTexture);
	(*m_program)(m_texture);
    }
}

void GLWidget::resizeGL(int w, int h)
{
    // TODO:
    m_windowWidth = w;
    m_windowHeight = h;
    
    m_program = std::make_shared<gatherer::graphics::WarpShader>( cv::Size(m_windowWidth, m_windowHeight), cv::Point2f(1,1) );
}

void GLWidget::setImage(const cv::Mat &image)
{
     std::unique_lock<std::mutex> lock(m_mutex);
     m_currentFrame = image; 

     // Logging for now...
     if(!(m_counter++ % 100))
	 std::cout << "GLWidget: got image" << image.size() << std::endl;
     update();
}
