/**
 * Copyright (c) 2012-2014 Microsoft Mobile.
 */

#include <QtGui>
#include <QtDeclarative>
#include "applicationactivationlistener.h"
#include "customcamera.h"
#include "histogram.h"
#include "imageanalyzer.h"
#include "performancemeter.h"

#ifdef Q_OS_SYMBIAN
#include "volumekeys.h"
#include "profilingdata.h"
#endif


// Include the camerakeylistener.pri on the project file to handle the
// shutter button on the application and not to start the native
// camera application. Note that handling the camera key requires
// SwEvent capability and it does not work with self sign.
#ifdef CAMERA_KEY_HANDLING
#include "camerakeylistener.h"
#endif



/*!
  \class MyApplication
  \brief Used to catch the GOOM event to prevent the termination of the
         application when the system is low on graphics memory. Sometimes when
         the graphics memory is really low this won't help and the application
         is killed anyway.
*/

const int KGoomMemoryLowEvent = 0x10282DBF;

class MyApplication : public QApplication {
public:
    MyApplication(int argc, char** argv)
        : QApplication(argc, argv)
    {
    }

protected:
#ifdef Q_OS_SYMBIAN
    bool symbianEventFilter(const QSymbianEvent* symbianEvent)
    {
        const TWsEvent *event = symbianEvent->windowServerEvent();

        if (!event) {
            return false;
        }

        switch (event->Type()) {
        case EEventUser: {
            TApaSystemEvent* eventData =
                    reinterpret_cast<TApaSystemEvent*>(event->EventData());

            if ((*eventData) == EApaSystemEventShutdown) {
                eventData++;

                if ((*eventData) == KGoomMemoryLowEvent) {
                    qDebug() << "Handling the GOOM Memory low event.";
                    return true;
                }
            }
            break;
        }
        }

        return false;
    }
#endif
};


Q_DECL_EXPORT int main(int argc, char *argv[])
{
    MyApplication app(argc, argv);

    QDeclarativeView view;

    // Lock to landscape
    view.setAttribute(Qt::WA_LockLandscapeOrientation, true);

    // Resize the root QML element to view size
    view.setResizeMode(QDeclarativeView::SizeRootObjectToView);

    // Performance optimization flags
    view.setAttribute(Qt::WA_OpaquePaintEvent);
    view.setAttribute(Qt::WA_NoSystemBackground);
    view.viewport()->setAttribute(Qt::WA_OpaquePaintEvent);
    view.viewport()->setAttribute(Qt::WA_NoSystemBackground);
    view.setViewportUpdateMode(QGraphicsView::FullViewportUpdate);

    qmlRegisterType<CustomCamera>("CustomElements", 1, 0, "CustomCamera");
    qmlRegisterType<ImageAnalyzer>("CustomElements", 1, 0, "ImageAnalyzer");
    qmlRegisterType<HistogramDisplay>("CustomElements", 1, 0,
                                      "HistogramDisplay");
    qmlRegisterType<PerformanceMeter>("CustomElements", 1, 0,
                                      "PerformanceMeter");
    qmlRegisterType<ApplicationActivationListener>(
                "CustomElements", 1, 0, "ApplicationActivationListener");

#if defined(Q_WS_HARMATTAN)
    view.setSource(QUrl::fromLocalFile("qml/harmattan/Main.qml"));
#elif defined(Q_OS_SYMBIAN)
    view.setSource(QUrl::fromLocalFile("qml/symbian/Main.qml"));

    // Query the total graphics memory
    ProfilingData profilingData;
    if (profilingData.resolveData()
            && profilingData.m_totalMemory <= 32 * 1024 * 1024) {
        // Too little gfx memory for video support, lets hide the
        // capture mode selector so video features cannot be accessed.

        qDebug() << "Hiding video feature as there isn't enough graphics "
                    "memory to support them!";

        QMetaObject::invokeMethod(view.rootObject(), "hideVideoFeature");
    }
#endif

    QObject::connect(view.engine(), SIGNAL(quit()), &app, SLOT(quit()));

    view.setGeometry(QApplication::desktop()->screenGeometry());
    view.showFullScreen();

#ifdef Q_OS_SYMBIAN
    // Handle the HW volume keys.
    new VolumeKeys(&view);

#ifdef CAMERA_KEY_HANDLING
    // Handles the HW camera key press and passes the event to the view.
    new CameraKeyListener(&view);
#endif
#endif

    return app.exec();
}
