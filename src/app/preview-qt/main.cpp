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

#include "mainwindow.h"
#include "glwidget.h"
#include "VideoCapture.h"

#include "graphics/GLWarpShader.h"
#include "graphics/GLTexture.h"
#include "graphics/RenderTexture.h"
#include "graphics/Logger.h"

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

#if defined(Q_OS_IOS)
extern "C" int qtmn(int argc, char** argv)
{
#else
int main(int argc, char **argv)
{
#endif
    try {
        QApplication app(argc, argv);

        QSurfaceFormat fmt;
        fmt.setDepthBufferSize(24);
        if (QCoreApplication::arguments().contains(QStringLiteral("--multisample")))
            fmt.setSamples(4);
        if (QCoreApplication::arguments().contains(QStringLiteral("--coreprofile"))) {
            fmt.setVersion(3, 2);
            fmt.setProfile(QSurfaceFormat::CoreProfile);
        }
        QSurfaceFormat::setDefaultFormat(fmt);

        auto logger = gatherer::graphics::Logger::create("preview-qt");
        logger->info("Start");

#if defined(__ANDROID__)
        const bool synth = true; // Camera not working on Android (yet)
#else
        const bool synth = false;
#endif
        
        VideoCapture capture(0, synth);

        cv::Size size = capture.getSize(0);

#if USE_OGLESGPGPU
		// OGLES_GPGPU currently expects BGRA on OS X 
        std::function<void(cv::Mat &)> conversion = [](cv::Mat &frame)
        {
            cv::cvtColor(frame, frame, cv::COLOR_BGR2BGRA);
        };
        capture.setConversion(conversion); // for ogles_gpgpu + opencv
#endif

        // Should be created in heap, ownership taken by parent widget
        GLWidget *glWidget = new GLWidget; // Note: moved creation output of Window for signal connection:

        glWidget->makeCurrent();
        
#if USE_OGLESGPGPU

#endif
        MainWindow mainWindow(glWidget, capture);
        mainWindow.setFixedSize(size.width, size.height);
        mainWindow.show();

        // ((((((((((((((((( bEGIN )))))))))))))))))

        QThread captureThread;
        captureThread.start();

        capture.moveToThread(&captureThread);

        qRegisterMetaType< cv::Mat >("cv::Mat");
        QObject::connect(&capture, &VideoCapture::started, [](){ qDebug() << "capture started"; });

        glWidget->connect(&capture, SIGNAL(matReady(cv::Mat)), SLOT(setImage(cv::Mat)));
        
        // ((((((((((((((((( eND )))))))))))))))))
        int rc = app.exec();

        captureThread.quit();
        captureThread.wait();

        return rc;
    }
    catch (const std::exception& exc) {
        std::cerr << "Exception catched: " << exc.what() << std::endl;
        return EXIT_FAILURE;
    }
    catch (...) {
        std::cerr << "Unknown exception catched" << std::endl;
        return EXIT_FAILURE;
    }
}

#include "main.moc"
