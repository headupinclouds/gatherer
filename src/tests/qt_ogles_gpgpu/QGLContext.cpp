#include "QGLContext.h"
#include <QDebug>

QGLContext::QGLContext()
{
    QSurfaceFormat format;

#if defined(Q_OS_OSX)
    format.setRenderableType(QSurfaceFormat::OpenGL);
    format.setProfile(QSurfaceFormat::CompatibilityProfile);
    format.setVersion(2, 1);
    format.setDepthBufferSize(24);
    format.setStencilBufferSize(8);
#endif

    m_surface = std::make_shared<QOffscreenSurface>();
    m_surface->setFormat(format);
    m_surface->create();
    
    m_context = std::make_shared<QOpenGLContext>();
    m_context->setFormat(format);
    
    if(!m_context->create())
    {
        qDebug()<<Q_FUNC_INFO<<"QOpenGLContext create";
    }
    
    if(!m_context->makeCurrent(m_surface.get()))
    {
        qDebug()<<Q_FUNC_INFO<<"Shit happens with QOpenGLContext makeCurrent";
    }
}


