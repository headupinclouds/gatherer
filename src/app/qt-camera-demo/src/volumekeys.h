/**
 * Copyright (c) 2012 Nokia Corporation.
 */

#ifndef VOLUMEKEYS_H
#define VOLUMEKEYS_H

#include <QObject>

# include <remconcoreapitargetobserver.h>
# include <remconcoreapitarget.h>
# include <remconinterfaceselector.h>


QT_BEGIN_NAMESPACE
QT_FORWARD_DECLARE_CLASS(QWidget)
QT_END_NAMESPACE

QT_USE_NAMESPACE

class VolumeKeys : public QObject, public MRemConCoreApiTargetObserver
{
    Q_OBJECT

public:
    explicit VolumeKeys(QWidget *parent = 0);
    virtual ~VolumeKeys();
    
protected:
    // from MRemConCoreApiTargetObserver
    void MrccatoCommand(TRemConCoreApiOperationId operationId,
                        TRemConCoreApiButtonAction buttonAct);

protected: // Data
    QWidget *m_widget; // Not owned
    CRemConInterfaceSelector *m_interfaceSelector; // Owned
    CRemConCoreApiTarget *m_coreTarget; // Not owned
};

#endif // VOLUMEKEYS_H
