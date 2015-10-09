/**
 * Copyright (c) 2012-2014 Microsoft Mobile.
 */

import QtQuick 1.1
import QtMobility.feedback 1.1

Image {
    id: captureModeSelector

    signal requestModeChange()

    property bool stillMode: true

    source: "../images/capture_mode_background.svg"

    Image {
        id: stillModeIndicator

        anchors {
            bottom: parent.bottom
            horizontalCenter: parent.horizontalCenter
        }

        source: "../images/capture_mode_camera_on.svg"
        opacity: captureModeSelector.stillMode ? 1 : 0

        Behavior on opacity { PropertyAnimation { duration: 200 } }
    }

    Image {
        id: videoModeIndicator

        anchors {
            top: parent.top
            horizontalCenter: parent.horizontalCenter
        }

        source: "../images/capture_mode_video_on.svg"
        opacity: captureModeSelector.stillMode ? 0 : 1

        Behavior on opacity { PropertyAnimation { duration: 200 } }
    }

    ThemeEffect { id: pressEffect; effect: ThemeEffect.BasicButton }
    ThemeEffect { id: clickEffect; effect: ThemeEffect.PositiveTacticon }

    MouseArea {
        anchors.fill: parent

        onPressed: { pressEffect.play(); }

        onClicked: {
            clickEffect.play();
            captureModeSelector.requestModeChange();
        }
    }
}
