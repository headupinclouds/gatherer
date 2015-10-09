/**
 * Copyright (c) 2012 Nokia Corporation.
 */

#ifndef ACTIVATIONEVENTFILTER_H
#define ACTIVATIONEVENTFILTER_H

#include <QObject>

class QEvent;

class ActivationEventFilter : public QObject
{
    Q_OBJECT
public:
    explicit ActivationEventFilter(QObject *parent = 0);

signals:
    void applicationActivated();
    void applicationDeactivated();
    
protected:
    bool eventFilter(QObject *object, QEvent *event);
};

#endif // ACTIVATIONEVENTFILTER_H
