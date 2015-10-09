# Copyright (c) 2012-2014 Microsoft Mobile.

QT += declarative

CONFIG += mobility
MOBILITY += multimedia

VERSION = 1.2.1

TARGET = QtCameraDemo

HEADERS += \
    applicationactivationlistener.h \
    activationeventfilter.h \
    customcamera.h \
    histogram.h \
    imageanalyzer.h \
    performancemeter.h \
    videosurface.h \
    profilingdata.h

SOURCES += \
    applicationactivationlistener.cpp \
    activationeventfilter.cpp \
    main.cpp \
    customcamera.cpp \
    histogram.cpp \
    imageanalyzer.cpp \
    performancemeter.cpp \
    videosurface.cpp

OTHER_FILES += \
    qml/symbian/*.qml \
    qml/symbian/*.js

symbian {
    # Uncomment the following line to add the camera key handling. Note that
    # the camera key handling requires SwEvent capability and the application
    # cannot be self signed anymore. The application must be Symbian Signed at
    # https://www.symbiansigned.com.
    #
    #include(camerakeylistener/camerakeylistener.pri)

    HEADERS += volumekeys.h
    SOURCES += volumekeys.cpp

    # To handle volume keys
    LIBS += -lremconcoreapi -lremconinterfacebase

    # To retrieve the amount of graphics memory.
    LIBS += -llibegl

    TARGET.CAPABILITY += \
        UserEnvironment   # To access the camera

    TARGET.EPOCHEAPSIZE = 0x20000 0x9000000

    CONFIG(debug, debug|release) {
        MMP_RULES += DEBUGGABLE_UDEBONLY
    }

    ICON = icons/camerademo.svg

    # For enabling hardware floats
    MMP_RULES += "OPTION gcce -march=armv6"
    MMP_RULES += "OPTION gcce -mfpu=vfp"
    MMP_RULES += "OPTION gcce -mfloat-abi=softfp"
    MMP_RULES += "OPTION gcce -marm"

    # Deploy the symbian qml files
    qmlfiles.sources = qml/symbian qml/images
    qmlfiles.path = qml

    DEPLOYMENT += qmlfiles
}

simulator {
    CONFIG(debug, debug|release) {
        system(mkdir ..\\camerademo-build-simulator-Simulator_Qt_for_MinGW_4_4__Qt_SDK__Debug\\qml)
        system(mkdir ..\\camerademo-build-simulator-Simulator_Qt_for_MinGW_4_4__Qt_SDK__Debug\\qml\\symbian)
        system(mkdir ..\\camerademo-build-simulator-Simulator_Qt_for_MinGW_4_4__Qt_SDK__Debug\\qml\\images)
        system(copy .\\qml\\symbian\\* ..\\camerademo-build-simulator-Simulator_Qt_for_MinGW_4_4__Qt_SDK__Debug\\qml\\symbian)
        system(copy .\\qml\\images\\* ..\\camerademo-build-simulator-Simulator_Qt_for_MinGW_4_4__Qt_SDK__Debug\\qml\\images)
    }
    else {
        system(mkdir ..\\camerademo-build-simulator-Simulator_Qt_for_MinGW_4_4__Qt_SDK__Release\\qml)
        system(mkdir ..\\camerademo-build-simulator-Simulator_Qt_for_MinGW_4_4__Qt_SDK__Release\\qml\\symbian)
        system(mkdir ..\\camerademo-build-simulator-Simulator_Qt_for_MinGW_4_4__Qt_SDK__Release\\qml\\images)
        system(copy .\\qml\\symbian\\* ..\\camerademo-build-simulator-Simulator_Qt_for_MinGW_4_4__Qt_SDK__Release\\qml\\symbian)
        system(copy .\\qml\\images\\* ..\\camerademo-build-simulator-Simulator_Qt_for_MinGW_4_4__Qt_SDK__Release\\qml\\images)
    }
}
