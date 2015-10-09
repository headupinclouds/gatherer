/**
 * Copyright (c) 2012-2014 Microsoft Mobile.
 */

#include "activationeventfilter.h"
#include <QEvent>
#include <QDebug>

/*!
  \class ActivationEventFilter
  \brief Detects ApplicationActivate and ApplicationDeactivate events and
         signals them forward.
*/


/*!
  Constructor.
*/
ActivationEventFilter::ActivationEventFilter(QObject *parent)
 : QObject(parent)
{
}


/*!
  Detects ApplicationActivate and ApplicationDeactive events and emits those
  as signals.
*/
bool ActivationEventFilter::eventFilter(QObject *object, QEvent *event)
{
    if (event->type() == QEvent::ApplicationActivate) {
        emit applicationActivated();
    }
    else if (event->type() == QEvent::ApplicationDeactivate) {
        emit applicationDeactivated();
    }

    return QObject::eventFilter(object, event);
}
