/**
 * Copyright (c) 2012 Nokia Corporation.
 */

#ifndef CAMERAKEYLISTENER_H
#define CAMERAKEYLISTENER_H

#include <QObject>

QT_BEGIN_NAMESPACE
QT_FORWARD_DECLARE_CLASS(QWidget)
QT_END_NAMESPACE

QT_USE_NAMESPACE

class CameraKeyListener : public QObject
{
    Q_OBJECT
public:
    explicit CameraKeyListener(QWidget *parent = 0);
    virtual ~CameraKeyListener();
    
private:
    QWidget *m_widget;
};

#endif // CAMERAKEYLISTENER_H
