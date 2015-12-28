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

#include <cassert> // assert

#include <QGuiApplication>
#include <QQuickView>
#include <QQuickItem>
#include <QCamera>

#include "VideoFilter.hpp"
#include "InfoFilter.hpp"

#include <iostream>

#if defined(Q_OS_IOS)
extern "C" int qtmn(int argc, char** argv)
{
#else
int main(int argc, char **argv)
{
#endif
#ifdef Q_OS_WIN // avoid ANGLE on Windows
    QCoreApplication::setAttribute(Qt::AA_UseDesktopOpenGL);
#endif
    QGuiApplication app(argc, argv);
    
    qmlRegisterType<VideoFilter>("qmlvideofilter.test", 1, 0, "VideoFilter");
    qmlRegisterType<InfoFilter>("qmlvideofilter.test", 1, 0, "InfoFilter");
    
    QQuickView view;
    view.setSource(QUrl("qrc:///main.qml"));
    view.setResizeMode( QQuickView::SizeRootObjectToView );
    
    //QVideoSurfaceFormat
    
#if defined(Q_OS_IOS)
    // Default camera on iOS is not setting good parameters by default
    QQuickItem* root = view.rootObject();
    
    QObject* qmlCamera = root->findChild<QObject*>("CameraObject");
    assert(qmlCamera != nullptr);
    
    QCamera* camera = qvariant_cast<QCamera*>(qmlCamera->property("mediaObject"));
    assert(camera != nullptr);
    
    QObject * qmlVideoOutput = root->findChild<QObject*>("VideoOutput");
    assert(qmlVideoOutput);

#if 0
    QCameraViewfinderSettings viewfinderFoundSetting;
    assert(viewfinderFoundSetting.isNull());
    
    // Format_ARGB32 == 1
    // Format_NV12 == 22
    auto viewfinderSettings = camera->supportedViewfinderSettings();
    for (auto i: viewfinderSettings) {
        bool good = true;
        
        if (i.pixelFormat() != QVideoFrame::Format_ARGB32) {
            good = false;
        }
        if (i.resolution().height() > view.height()) {
            good = false;
        }
        if (i.resolution().width() > view.width()) {
            good = false;
        }
        if (good) {
            // Get last setting if several RGB available.
            // Goes from lower resolution to higher (verify?)
            viewfinderFoundSetting = i;
        }
    }
    assert(viewfinderFoundSetting.pixelFormat() == QVideoFrame::Format_ARGB32);
    camera->setViewfinderSettings(viewfinderFoundSetting);
#endif
    // Try the highest resolution ARGB32 format:
    QVideoFrame::PixelFormat desiredFormat = QVideoFrame::Format_ARGB32;
    auto viewfinderSettings = camera->supportedViewfinderSettings();
    std::pair<int, QCameraViewfinderSettings> best;
    for (auto i: viewfinderSettings)
    {
        if (i.pixelFormat() == desiredFormat)
        {
            int area = (i.resolution().height() * i.resolution().width());
            if(area > best.first)
            {
                best = { area, i };
            }
        }
    }
    assert(!best.second.isNull());
    assert(best.second.pixelFormat() == desiredFormat);
    camera->setViewfinderSettings(best.second);
#endif
    
    //view.show();
    view.showFullScreen();
    
    return app.exec();
}
