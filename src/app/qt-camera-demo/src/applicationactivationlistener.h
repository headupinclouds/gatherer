/**
 * Copyright (c) 2012 Nokia Corporation.
 */

#ifndef APPLICATIONACTIVATIONLISTENER_H
#define APPLICATIONACTIVATIONLISTENER_H

#include <QDeclarativeItem>

class ApplicationActivationListener : public QDeclarativeItem
{
    Q_OBJECT
public:
    explicit ApplicationActivationListener(QDeclarativeItem *parent = 0);
    
signals:
    void applicationActivated();
    void applicationDeactivated();

protected slots:
    void handleApplicationActivated();
    void handleApplicationDeactivated();
};

QML_DECLARE_TYPE(ApplicationActivationListener)

#endif // APPLICATIONACTIVATIONLISTENER_H
