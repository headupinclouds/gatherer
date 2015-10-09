/**
 * Copyright (c) 2012-2014 Microsoft Mobile.
 */

#include "applicationactivationlistener.h"
#include <QCoreApplication>
#include "activationeventfilter.h"


/*!
  \class ApplicationActivationListener
  \brief Detects if the application is on the foreground or background by
         listening the ActivationChanged events.
*/


/*!
  Constructor.
*/
ApplicationActivationListener::ApplicationActivationListener(
        QDeclarativeItem *parent)
    : QDeclarativeItem(parent)
{
    QCoreApplication *application = QCoreApplication::instance();

    ActivationEventFilter *filter = new ActivationEventFilter(this);

    connect(filter, SIGNAL(applicationActivated()),
            this, SLOT(handleApplicationActivated()));
    connect(filter, SIGNAL(applicationDeactivated()),
            this, SLOT(handleApplicationDeactivated()));

    application->installEventFilter(filter);
}


/*!
  Handles the ApplicationActivated event.
*/
void ApplicationActivationListener::handleApplicationActivated()
{
    emit applicationActivated();
}


/*!
  Handles the ApplicationDeactivated event.
*/
void ApplicationActivationListener::handleApplicationDeactivated()
{
    emit applicationDeactivated();
}
