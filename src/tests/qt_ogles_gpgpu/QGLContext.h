#include <QApplication>
#include <QDesktopWidget>
#include <QSurfaceFormat>
#include <QtGlobal> // Q_OS_IOS

#include <QOffscreenSurface>
#include <QOpenGLContext>

#include <memory>

class QGLContext
{
public:
    QGLContext();
protected:
    std::shared_ptr<QOffscreenSurface> m_surface;
    std::shared_ptr<QOpenGLContext> m_context;
};
        
