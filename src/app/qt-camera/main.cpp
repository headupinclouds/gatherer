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

#include "camera.h"

#include <QtWidgets>

// Enable this for potential fix by @pretyman: https://github.com/ruslo/hunter/issues/253
#define STATIC_BUILD 0

// Undefined symbols for architecture x86_64:
//   "qt_static_plugin_QtQuick2Plugin()", referenced from:
//       _main in main.cpp.o
//       __GLOBAL__sub_I_main.cpp in main.cpp.o
//   "qt_static_plugin_CoreAudioPlugin()", referenced from:
//       __GLOBAL__sub_I_main.cpp in main.cpp.o
//   "qt_static_plugin_AVFServicePlugin()", referenced from:
//       __GLOBAL__sub_I_main.cpp in main.cpp.o
//   "qt_static_plugin_QtQuick2WindowPlugin()", referenced from:
//       _main in main.cpp.o
//       __GLOBAL__sub_I_main.cpp in main.cpp.o
//   "qt_static_plugin_QtQuickLayoutsPlugin()", referenced from:
//       _main in main.cpp.o
//       __GLOBAL__sub_I_main.cpp in main.cpp.o
//   "qt_static_plugin_AudioCaptureServicePlugin()", referenced from:
//       __GLOBAL__sub_I_main.cpp in main.cpp.o
//   "qt_static_plugin_AVFMediaPlayerServicePlugin()", referenced from:
//       __GLOBAL__sub_I_main.cpp in main.cpp.o
//   "qt_static_plugin_QMultimediaDeclarativeModule()", referenced from:
//       _main in main.cpp.o
//       __GLOBAL__sub_I_main.cpp in main.cpp.o
// ld: symbol(s) not found for architecture x86_64x


#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QQmlEngine>
#include <QAudio>
#include <QQmlExtensionPlugin>
#include <QtQml/qqml.h>
#include <QtQml>
#if STATIC_BUILD
#include <QtPlugin>
Q_IMPORT_PLUGIN(QCocoaIntegrationPlugin)
Q_IMPORT_PLUGIN(QtQuick2Plugin)
Q_IMPORT_PLUGIN(QtQuick2WindowPlugin)
Q_IMPORT_PLUGIN(QMultimediaDeclarativeModule)
Q_IMPORT_PLUGIN(QtQuickLayoutsPlugin)
Q_IMPORT_PLUGIN(AVFServicePlugin) // Camera
Q_IMPORT_PLUGIN(AVFMediaPlayerServicePlugin)
Q_IMPORT_PLUGIN(AudioCaptureServicePlugin) // Record
Q_IMPORT_PLUGIN(CoreAudioPlugin)
#endif // STATIC_BUILD

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

#if STATIC_BUILD
	// registration because of https://bugreports.qt.io/browse/QTBUG-38661
	qRegisterMetaType<QAudio::Error>();
	qRegisterMetaType<QAudio::State>();
	qRegisterMetaType<QAudio::Mode>();
	qobject_cast<QQmlExtensionPlugin*>(qt_static_plugin_QtQuick2Plugin().instance())->registerTypes("QtQuick");
	qobject_cast<QQmlExtensionPlugin*>(qt_static_plugin_QMultimediaDeclarativeModule().instance())->registerTypes("QtMultimedia");
	qobject_cast<QQmlExtensionPlugin*>(qt_static_plugin_QtQuick2WindowPlugin().instance())->registerTypes("QtQuick.Window");
	qobject_cast<QQmlExtensionPlugin*>(qt_static_plugin_QtQuickLayoutsPlugin().instance())->registerTypes("QtQuick.Layouts");
#endif // STATIC_BUILD

    Camera camera;
    camera.show();

    return app.exec();
};
