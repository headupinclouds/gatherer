/**
 * Copyright (c) 2012-2014 Microsoft Mobile.
 */

import QtQuick 1.1
import com.nokia.symbian 1.1
import CustomElements 1.0
import "Constants.js" as Constants

Item {
    id: main

    /*!
      Hides the video feature. In other words hides the capture mode selector.
      This function gets called if the phone doesn't have enough graphics
      memory to support video recording.
    */
    function hideVideoFeature() {
        captureModeSelector.visible = false;
    }

    anchors.fill: parent

    Component.onCompleted: {
        main.state = "CameraView";
    }

    ApplicationActivationListener {
        onApplicationActivated: {
            settingsView.startLastCamera();
        }
    }

    CustomCamera {
        id: camera

        anchors.fill: parent

        imageAnalyzer: cameraView.imageAnalyzer

        onLockFailed: { cameraView.indicateLockStatus("failed"); }
        onLocked: { cameraView.indicateLockStatus("locked"); }

        onImageSaved: {
            // Release resources as the gallery will require lots of them!
            camera.stop();

            Qt.openUrlExternally("file:///" + fileName);
        }
    }

    CameraView {
        id: cameraView

        anchors.fill: parent

        // Pass the CustomCamera object for easier access.
        camera: camera

        showHistogram: settingsView.showHistogram

        opacity: 0
        onRequestCameraView: { main.state = "CameraView"; }
    }

    SettingsView {
        id: settingsView

        anchors.fill: cameraView

        // Pass the CustomCamera object for easier access.
        camera: camera
        opacity: 0

        onInfoPressed: {
            main.state = "InfoView";
        }
    }

    InfoView {
        id: infoView

        anchors.fill: parent

        opacity: 0
    }

    CaptureModeSelector {
        id: captureModeSelector

        anchors {
            top: parent.top
            horizontalCenter: shootButton.horizontalCenter
        }

        opacity: Constants.CONTROL_OPACITY
        stillMode: camera.captureMode === CustomCamera.CaptureStillImage

        onRequestModeChange: {
            if (stillMode) {
                camera.captureMode = CustomCamera.CaptureVideo;
            }
            else {
                camera.captureMode = CustomCamera.CaptureStillImage;
            }
        }
    }

    CustomButton  {
        id: shootButton

        anchors {
            right: parent.right; rightMargin: 5
            verticalCenter: parent.verticalCenter
        }

        // Shoot button will be the biggest button
        width: 70
        height: 70

        opacity: Constants.CONTROL_OPACITY

        iconSource: {
            if (camera.captureMode === CustomCamera.CaptureStillImage) {
                return "../images/shooting.svg";
            }
            else {
                if (camera.videoRecorderState === CustomCamera.RecordingState) {
                    return "../images/stop.svg";
                }
                else {
                    return "../images/rec.svg";
                }
            }
        }

        onClicked: {
            if (camera.captureMode === CustomCamera.CaptureStillImage) {
                cameraView.captureImage();
            }
            else if (camera.captureMode === CustomCamera.CaptureVideo) {
                switch (camera.videoRecorderState) {
                case CustomCamera.RecordingState:
                    camera.endRecording();
                    shootButton.checked = false;
                    break;
                case CustomCamera.PausedState:
                case CustomCamera.StoppedState:
                    camera.beginRecording();
                    shootButton.checked = true;
                    break;
                }
            }
        }
    }

    ToolButton {
        id: backButton

        anchors {
            horizontalCenter: shootButton.horizontalCenter
            bottom: parent.bottom; bottomMargin: 5
        }

        opacity: Constants.CONTROL_OPACITY
        iconSource: "toolbar-back"

        onClicked: {
            if (main.state === "SettingsView") {
                main.state = "CameraView";
            }
            else if (main.state === "InfoView") {
                main.state = "SettingsView";
            }
            else {
                Qt.quit();
            }
        }
    }

    states: [
        State {
            name: "CameraView"
            PropertyChanges { target: cameraView; opacity: 1 }
        },
        State {
            name: "SettingsView"
            PropertyChanges { target: settingsView; opacity: 1 }
        },
        State {
            name: "InfoView"
            PropertyChanges { target: infoView; opacity: 1 }
            PropertyChanges { target: captureModeSelector; opacity: 0 }
            PropertyChanges { target: shootButton; opacity: 0 }
        }
    ]

    transitions: [
        Transition {
            PropertyAnimation { property: "opacity"; duration: 200 }
        }
    ]
}
