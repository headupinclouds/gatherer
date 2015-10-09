/**
 * Copyright (c) 2012-2014 Microsoft Mobile.
 */

import QtQuick 1.1
import com.nokia.symbian 1.1
import "Constants.js" as Constants

Rectangle {
    id: infoView

    color: "black"

    Flickable {
        id: flickable

        anchors {
            fill: parent
            leftMargin: 5
            rightMargin: 53
        }

        contentWidth: width
        contentHeight: label.height

        Label {
            id: label

            anchors.centerIn: parent

            width: flickable.width - 15
            font.pixelSize: platformStyle.fontSizeLarge
            wrapMode: Text.WordWrap
            text: "<h1>Qt Camera Demo 1.2.1</h1>" +
                  "<p>This application demonstrates the use of Qt Mobility " +
                  "Qt camera API.</p>" +
                  "<p>This example application is hosted in " +
                  "<a href=\"http://projects.developer.nokia.com/camerademo\">" +
                  "Nokia Developer Projects</a>.</p>" +
                  "<p>The Qt Camera Demo application has capability to set " +
                  "properties of the camera, show view finder image and " +
                  "capture still images and video. The application also " +
                  "analyses the view finder image in real time and shows a " +
                  "histogram window about the image. The over-exposed areas " +
                  "are marked in the view finder to demonstrate the pixel " +
                  "manipulation techniques in real-time images.</p>" +
                  "<p>The video feature is disabled on devices with 32 MB of " +
                  "graphics memory or less. The supported devices include " +
                  "Nokia 603, Nokia 700, Nokia 701 and Nokia 808 " +
                  "PureView.</p>"

            onLinkActivated: Qt.openUrlExternally(link)
        }
    }

    ScrollDecorator {
        flickableItem: flickable
    }
}
