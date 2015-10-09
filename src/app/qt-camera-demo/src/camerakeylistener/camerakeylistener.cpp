/**
 * Copyright (c) 2012-2014 Microsoft Mobile.
 */

#include "camerakeylistener.h"

#include <QWidget>       // WId
#include <QDebug>
#include <eikon.hrh>     // EKeyCamera
#include <coecntrl.h>    // CCoeControl (WId)
#include <w32std.h>      // RWindowbase, RWindowGroup, RWsSession

/*!
  \class CameraKeyListener
  \brief Handles the camera key press and passes the event to the associated
         widget. Use of the listener requires the SwEvent capability.
*/


/*!
  Constructor, captures the HW camera (shutter) button events and directs them
  to the given widget. Note that the widget must be visible before this
  constructor is called.
*/
CameraKeyListener::CameraKeyListener(QWidget *widget)
    : QObject(widget),
      m_widget(widget)
{
    if (!m_widget) {
        return;
    }

    // Get view's native Symbian window
    WId windowId = 0;

    if (m_widget->internalWinId()) {
        windowId = m_widget->internalWinId();
    }
    else if (m_widget->parentWidget() && m_widget->effectiveWinId()) {
        windowId = m_widget->effectiveWinId();
    }

    RWindowBase *window =
            windowId ? static_cast<RWindowBase*>(windowId->DrawableWindow())
                     : 0;

    // Get hold of the window group
    TInt wGroupId = window ? window->WindowGroupId() : 0;
    if (!wGroupId) {
        return;
    }

    RWsSession &wsSession = CCoeEnv::Static()->WsSession();
    TUint wGroupHandle = wsSession.GetWindowGroupHandle(wGroupId);

    if (wGroupHandle) {
        RWindowGroup wGroup(wsSession);
        wGroup.Construct(wGroupHandle);

        TBool captureOK = ETrue;

        // Capture camera key, in Symbian Belle the key up and downs must
        // be also handled in order to prevent the native camera application
        // to start.
        captureOK = captureOK && (wGroup.CaptureKey(EKeyCamera, 0, 0, 100) > 0);
        captureOK = captureOK && (wGroup.CaptureKeyUpAndDowns(EStdKeyDevice7, 0, 0, 100) > 0);

        if (!captureOK)
            qWarning() << "Unable to register for camera capture key events, "
                       << "SwEvent capability may be missing";
    }
}


/*!
  Destructor.
*/
CameraKeyListener::~CameraKeyListener()
{
    if (!m_widget)
        return;

    // Get view's native Symbian window
    WId windowId = 0;

    if (m_widget->internalWinId())
        windowId = m_widget->internalWinId();
    else if (m_widget->parentWidget() && m_widget->effectiveWinId())
        windowId = m_widget->effectiveWinId();

    RWindowBase *window =
            windowId ? static_cast<RWindowBase*>(windowId->DrawableWindow())
                     : 0;

    // Get hold of the window group
    TInt wGroupId = window ? window->WindowGroupId() : 0;

    if (!wGroupId)
        return;

    RWsSession &wsSession = CCoeEnv::Static()->WsSession();
    TUint wGroupHandle = wsSession.GetWindowGroupHandle(wGroupId);

    if (wGroupHandle) {
        RWindowGroup wGroup(wsSession);
        wGroup.Construct(wGroupHandle);
        wGroup.CancelCaptureKey(EKeyCamera);
        wGroup.CancelCaptureKeyUpAndDowns(EKeyCamera);
    }
}
