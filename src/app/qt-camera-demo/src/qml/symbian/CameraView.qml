/**
 * Copyright (c) 2012-2014 Microsoft Mobile.
 */

import QtQuick 1.1
import com.nokia.symbian 1.1
import CustomElements 1.0
import "Constants.js" as Constants

Item {
    id: cameraView

    property CustomCamera camera

    property bool showHistogram: true

    property alias imageAnalyzer: imageAnalyzer

    signal requestCameraView()

    /*!
      Indicates the lock status on the reticule.
    */
    function indicateLockStatus(status) {
        if (status === "failed")
            reticule.source = "../images/crosshair_frame_red.svg";
        else if (status === "locked")
            reticule.source = "../images/crosshair_frame_green.svg";
    }

    /*!
      Sets the reticule color back to white.
    */
    function resetReticuleColor() {
        reticule.source = "../images/crosshair_frame_white.svg"
    }

    /*!
      Performs the auto focus.
    */
    function focusToCoordinate(x, y) {
        // Shutter button can be pressed while in settings or info view
        // and we should switch to the camera view.
        cameraView.requestCameraView();

        resetReticuleColor();

        camera.focusToCoordinate(x, y);
    }

    /*!
      Requests camera to capture a image.
    */
    function captureImage() {
        camera.captureImage();
    }

    focus: true

    Keys.onPressed: {
        if (event.key === Qt.Key_VolumeUp) {
            // Zoom down
            zoomSlider.value -= 1;
            event.accepted = true;
        }
        else if (event.key === Qt.Key_VolumeDown) {
            // Zoom up
            zoomSlider.value += 1;
            event.accepted = true;
        }
        else if (event.key === 17825824 && !event.isAutoRepeat) {
            // Shutter pressed
            if (camera.captureMode === CustomCamera.CaptureStillImage) {
                cameraView.captureImage();
            }
            event.accepted = true;
        }
        else if (event.key === 17825825 && !event.isAutoRepeat) {
            // Shutter pressed halfway
            cameraView.focusToCoordinate(0, 0);
            event.accepted = true;
        }
    }

    MouseArea {
        id: focusArea

        anchors.fill: parent

        onClicked: { cameraView.focusToCoordinate(mouse.x, mouse.y); }
    }

    Image {
        id: reticule

        anchors.centerIn: parent

        source: "../images/crosshair_frame_white.svg"

        Behavior on opacity { PropertyAnimation { duration: 250 } }

        Image {
            id: waterLevelReticule

            anchors.centerIn: parent

            source: "../images/crosshair_white.svg"
        }
    }

    /*
    PerformanceMeter {
        id: performanceMeter

        anchors {
            top: parent.top
            right: parent.center
        }

        width: 128; height: 64
    }
    */

    Slider {
        id: zoomSlider

        anchors {
            top: parent.top; topMargin: 5
            bottom: parent.bottom; bottomMargin: 5
            left: parent.left; leftMargin: 3
        }

        opacity: Constants.CONTROL_OPACITY
        orientation: Qt.Vertical
        inverted: true
        value: camera.digitalZoom
        minimumValue: 1
        maximumValue: camera.maximumDigitalZoom
        onValueChanged: { camera.zoomTo(1, value); }
    }

    Label {
        id: recLabel

        anchors {
            verticalCenter: recDuration.verticalCenter
            left: parent.left; leftMargin: 110
        }

        text: "Rec"
        style: Text.Outline
        styleColor: "black"
        visible: camera.videoRecorderState === CustomCamera.RecordingState
    }

    Image {
        id: recIndicator

        anchors {
            left: recLabel.right; leftMargin: 2
            verticalCenter: recLabel.verticalCenter
        }

        width: 20
        height: 20

        source: "../images/rec.svg"
        visible: camera.videoRecorderState === CustomCamera.RecordingState
    }

    Label {
        id: recDuration

        anchors {
            verticalCenter: settingsButton.verticalCenter
            left: recIndicator.right; leftMargin: 30
        }

        text: camera.videoDuration

        style: Text.Outline
        styleColor: "black"
        visible: camera.videoRecorderState === CustomCamera.RecordingState
    }

    CustomButton {
        id: settingsButton

        anchors {
            top: parent.top; topMargin: 5
            horizontalCenter: parent.horizontalCenter
        }

        opacity: Constants.CONTROL_OPACITY
        iconSource: "../images/settings.svg"

        onClicked: { main.state = "SettingsView"; }
    }

    FlashSelector {
        id: flashSelector

        anchors {
            left: settingsButton.right; leftMargin: 90
            top: parent.top; topMargin: 5
        }

        visible: camera.captureMode === CustomCamera.CaptureStillImage

        opacity: Constants.CONTROL_OPACITY
        model: camera.supportedFlashModes
        currentMode: camera.flashMode

        onRequestFlashMode: { camera.flashMode = flashMode; }
    }

    MuteSelector {
        id: muteSelector

        anchors.centerIn: flashSelector

        enabled: camera.videoRecorderState !== CustomCamera.RecordingState
        visible: camera.captureMode === CustomCamera.CaptureVideo

        opacity: Constants.CONTROL_OPACITY

        onMutedChanged: { camera.muted = muted; }
    }

    ImageAnalyzer { id: imageAnalyzer }

    HistogramDisplay {
        id: histogramDisplay

        anchors {
            left: zoomSlider.right; leftMargin: 15
            bottom: parent.bottom; bottomMargin: 10
        }

        width: 100
        height: 50

        visible: cameraView.showHistogram

        imageAnalyzer: imageAnalyzer
    }
}
