/**
 * Copyright (c) 2012-2014 Microsoft Mobile.
 */

import QtQuick 1.1
import QtMobility.feedback 1.1

Item {
    id: button

    property alias enabled: mouseArea.enabled
    property alias iconSource: icon.source
    property alias text: text.text

    property bool checkable: false
    property bool checked: false

    signal clicked();
    signal pressed();
    signal released();

    width: 50; height: 50

    Image {
        id: buttonImage

        property bool pressed: false

        anchors.fill: parent

        smooth: true

        source: {
            if (button.checkable) {
                if (buttonImage.pressed) {
                    if (button.checked)
                        return "../images/button_bg.svg";
                    else
                        return "../images/button_pressed_bg.svg";
                }
                else {
                    if (button.checked)
                        return "../images/button_pressed_bg.svg";
                    else
                        return "../images/button_bg.svg";
                }
            }
            else {
                if (buttonImage.pressed)
                    return "../images/button_pressed_bg.svg";
                else
                    return "../images/button_bg.svg";
            }
        }
    }

    Image {
        id: icon

        anchors {
            fill: parent
            margins: 4
        }

        smooth: true
    }

    Text {
        id: text

        anchors.centerIn: parent

        color: "white"
        opacity: 0.8

        font.pixelSize: 20
    }

    ThemeEffect { id: pressEffect; effect: ThemeEffect.BasicButton }
    ThemeEffect { id: clickEffect; effect: ThemeEffect.PositiveTacticon }

    MouseArea {
        id: mouseArea

        anchors {
            fill: parent
            margins: -5
        }

        onPressed: {
            buttonImage.pressed = true;

            pressEffect.play();
            button.pressed();
        }

        onReleased: {
            buttonImage.pressed = false;

            button.released();
        }

        onCanceled: {
            buttonImage.pressed = true;
        }

        onClicked: {
            if (button.checkable) {
                button.checked = !button.checked;
            }

            clickEffect.play();
            button.clicked();
        }
    }
}
