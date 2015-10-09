/**
 * Copyright (c) 2012-2014 Microsoft Mobile.
 */

#include "volumekeys.h"

#include <QtGui>


/*!
  \class VolumeKeys
  \brief Handles the volume key presses.
*/


/*!
  Constructor.
*/
VolumeKeys::VolumeKeys(QWidget *parent)
    : QObject(parent),
      m_widget(parent),
      m_interfaceSelector(0),
      m_coreTarget(0)
{
    QT_TRAP_THROWING(
        m_interfaceSelector = CRemConInterfaceSelector::NewL();
        m_coreTarget = CRemConCoreApiTarget::NewL(*m_interfaceSelector, *this);
        m_interfaceSelector->OpenTargetL();
    );
}


/*!
  Destructor.
*/
VolumeKeys::~VolumeKeys()
{
    delete m_interfaceSelector;
    m_interfaceSelector = 0;

    // owned by interfaceselector
    m_coreTarget = 0;
}


/*!
  From MRemConCoreApiTargetObserver.

  Checks if volume up/down keys are pressed and posts QKeyEvents to the
  m_widget.
*/
void VolumeKeys::MrccatoCommand(TRemConCoreApiOperationId operationId,
                                TRemConCoreApiButtonAction buttonAct)
{
    switch (operationId) {
    case ERemConCoreApiVolumeUp:
        if ((buttonAct == ERemConCoreApiButtonClick) ||
            (buttonAct == ERemConCoreApiButtonPress) ||
            (buttonAct == ERemConCoreApiButtonRelease)) {
            QKeyEvent *keyEvent = new QKeyEvent(QEvent::KeyPress,
                                                Qt::Key_VolumeUp,
                                                Qt::NoModifier);

            qApp->postEvent(m_widget, keyEvent);
        }
        break;
    case ERemConCoreApiVolumeDown:
        if ((buttonAct == ERemConCoreApiButtonClick) ||
            (buttonAct == ERemConCoreApiButtonPress) ||
            (buttonAct == ERemConCoreApiButtonRelease)) {
            QKeyEvent *keyEvent = new QKeyEvent(QEvent::KeyPress,
                                                Qt::Key_VolumeDown,
                                                Qt::NoModifier);

            qApp->postEvent(m_widget, keyEvent);
        }
        break;
    }
}
