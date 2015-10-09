/**
 * Copyright (c) 2012-2014 Microsoft Mobile.
 */

import QtQuick 1.1
import com.nokia.symbian 1.1
import CustomElements 1.0
import "Constants.js" as Constants

Item {
    id: settingsView

    property CustomCamera camera
    property string lastCamera
    property bool showHistogram: true

    signal infoPressed()

    /*!
      Starts the primary camera.
    */
    function startLastCamera() {
        camera.start(lastCamera);
    }

    Component.onCompleted: {
        // Hide the possibility to choose the primary / secondary camera
        // if there only exists primary camera.
        if (camera.availableDevices.length < 2)
            cameraDevices.visible = false;

        lastCamera = camera.availableDevices[0];
    }

    Rectangle {
        anchors {
            top: parent.top
            left: parent.left
            bottom: parent.bottom
        }

        width: Constants.SETTINGS_LABEL_WIDTH

        opacity: Constants.SETTINGS_LABEL_PANEL_OPACITY
        color: "black"
    }

    Flickable {
        id: flickable

        anchors {
            fill: parent
            leftMargin: 10
            rightMargin: 78
        }

        contentWidth: width
        contentHeight: settingsColumn.height

        Column {
            id: settingsColumn

            spacing: Constants.SETTINGS_VERTICAL_SPACING

            Item {
                width: 100
                height: 5
            }

            Row {
                Item {
                    width: Constants.SETTINGS_LABEL_WIDTH + 20
                    height: 2
                }

                Label {
                    text: "Settings"
                    verticalAlignment: Text.AlignVCenter

                    style: Text.Outline
                    styleColor: "black"

                    width: 320

                    font.pixelSize: 34
                    font.bold: true
                }

                Button {
                    id: infoViewButton

                    opacity: Constants.CONTROL_OPACITY
                    iconSource: "../images/info-white.png"

                    onClicked: { settingsView.infoPressed(); }
                }
            }

            Item {
                width: 100
                height: 3
            }

            Row {
                id: cameraDevices

                Label {
                    width: Constants.SETTINGS_LABEL_WIDTH
                    height: primaryCameraButton.height

                    opacity: Constants.CONTROL_OPACITY

                    text: "Camera\nselection"
                    verticalAlignment: Text.AlignVCenter
                    wrapMode: Text.WordWrap
                }

                ButtonRow {
                    id: cameraModeSelector

                    Button {
                        id: primaryCameraButton

                        width: Constants.BUTTON_ROW_BUTTON_TEXT_WIDTH
                        height: Constants.BUTTON_ROW_BUTTON_HEIGHT

                        opacity: Constants.CONTROL_OPACITY
                        text: "Primary"
                        onClicked: {
                            if (camera.currentDevice ===
                                    camera.availableDevices[0])
                                return;

                            camera.start(camera.availableDevices[0]);
                            settingsView.lastCamera = camera.availableDevices[0];
                        }
                    }

                    Button {
                        id: secondaryCameraButton

                        width: Constants.BUTTON_ROW_BUTTON_TEXT_WIDTH
                        height: Constants.BUTTON_ROW_BUTTON_HEIGHT

                        opacity: Constants.CONTROL_OPACITY
                        text: "Secondary"
                        onClicked: {
                            if (camera.currentDevice ===
                                    camera.availableDevices[1])
                                return;

                            camera.start(camera.availableDevices[1]);
                            settingsView.lastCamera = camera.availableDevices[1];
                        }
                    }
                }
            }

            Row {
                visible: camera.captureMode === CustomCamera.CaptureVideo &&
                         camera.videoRecorderState !== CustomCamera.RecordingState &&
                         camera.videoResolutions.length > 1

                onVisibleChanged: {
                    if (visible) {
                        videoResolutionSelector.updateSelection();
                    }
                }

                Label {
                    width: Constants.SETTINGS_LABEL_WIDTH
                    height: videoResolutionSelector.height
                    opacity: Constants.CONTROL_OPACITY

                    text: "Video\nresolution"
                    verticalAlignment: Text.AlignVCenter
                    wrapMode: Text.WordWrap
                }

                VideoResolutionSelector {
                    id: videoResolutionSelector

                    opacity: Constants.CONTROL_OPACITY
                    model: camera.videoResolutions

                    onRequestVideoResolution: {
                        camera.videoResolution = resolution;
                    }

                    currentResolution: camera.videoResolution
                }
            }

            Row {
                visible: !camera.frontCamera

                Label {
                    width: Constants.SETTINGS_LABEL_WIDTH
                    height: contrastSlider.height
                    opacity: Constants.CONTROL_OPACITY

                    text: "Contrast"
                    verticalAlignment: Text.AlignVCenter
                }

                Slider {
                    id: contrastSlider

                    width: Constants.SETTINGS_SLIDER_WIDTH
                    opacity: Constants.CONTROL_OPACITY

                    valueIndicatorVisible: true
                    valueIndicatorText: value.toFixed(0)

                    minimumValue: -100
                    maximumValue: 100
                    value: camera.contrast
                    onValueChanged: { camera.contrast = value; }
                }
            }

            Row {
                visible: !camera.frontCamera

                Label {
                    width: Constants.SETTINGS_LABEL_WIDTH
                    height: sharpeningSlider.height

                    opacity: Constants.CONTROL_OPACITY
                    text: "Sharpening"
                    verticalAlignment: Text.AlignVCenter
                }

                Slider {
                    id: sharpeningSlider

                    width: Constants.SETTINGS_SLIDER_WIDTH

                    opacity: Constants.CONTROL_OPACITY

                    valueIndicatorVisible: true
                    valueIndicatorText: value.toFixed(0)

                    minimumValue: 0
                    maximumValue: 100
                    value: camera.sharpeningLevel
                    onValueChanged: { camera.sharpeningLevel = value; }
                }
            }

            Row {
                visible: !camera.frontCamera

                Label {
                    width: Constants.SETTINGS_LABEL_WIDTH
                    height: exposureCompensationSlider.height

                    opacity: Constants.CONTROL_OPACITY
                    text: "Exposure +/-"
                    verticalAlignment: Text.AlignVCenter
                }

                Slider {
                    id: exposureCompensationSlider

                    width: Constants.SETTINGS_SLIDER_WIDTH

                    opacity: Constants.CONTROL_OPACITY

                    minimumValue: -2
                    maximumValue: 2
                    stepSize: 1/3

                    valueIndicatorText: value.toFixed(1)
                    valueIndicatorVisible: true
                    value: 0

                    onValueChanged: { camera.exposureCompensation = value; }
                }
            }

            Row {
                visible: exposureModeSelector.model.length > 1

                Label {
                    width: Constants.SETTINGS_LABEL_WIDTH
                    height: exposureModeSelector.height
                    opacity: Constants.CONTROL_OPACITY

                    text: "Exposure\nmodes"
                    verticalAlignment: Text.AlignVCenter
                    wrapMode: Text.WordWrap
                }

                ExposureModeSelector {
                    id: exposureModeSelector

                    opacity: Constants.CONTROL_OPACITY
                    model: camera.supportedExposureModes

                    onExposureModeSelected: {
                        camera.exposureMode = exposureMode;
                    }
                }
            }

            Row {
                visible: whiteBalanceSelector.model.length > 1

                Label {
                    width: Constants.SETTINGS_LABEL_WIDTH
                    height: whiteBalanceSelector.height
                    opacity: Constants.CONTROL_OPACITY

                    text: "White balance"
                    verticalAlignment: Text.AlignVCenter
                }

                WhiteBalanceSelector {
                    id: whiteBalanceSelector

                    opacity: Constants.CONTROL_OPACITY
                    model: camera.supportedWhiteBalanceModes

                    onWhiteBalanceSelected: {
                        camera.whiteBalanceMode = whiteBalanceMode;
                    }
                }
            }

            Row {
                visible: camera.captureMode !== CustomCamera.CaptureVideo &&
                         isoSelector.model.length > 1

                Label {
                    width: Constants.SETTINGS_LABEL_WIDTH
                    height: isoSelector.height

                    opacity: Constants.CONTROL_OPACITY
                    text: "ISO value"
                    verticalAlignment: Text.AlignVCenter
                }

                IsoSelector {
                    id: isoSelector

                    height: whiteBalanceSelector.height

                    opacity: Constants.CONTROL_OPACITY
                    model: camera.supportedIsoValues
                    onIsoSelected: { camera.isoValue = isoValue; }
                }
            }

            Row {
                visible: camera.captureMode === CustomCamera.CaptureVideo

                onVisibleChanged: {
                    if (visible) {
                        fpsSelector.updateSelection();
                    }
                }

                Label {
                    width: Constants.SETTINGS_LABEL_WIDTH
                    height: fpsSelector.height

                    opacity: Constants.CONTROL_OPACITY
                    text: "Frames per\nsecond"
                    verticalAlignment: Text.AlignVCenter
                    wrapMode: Text.WordWrap
                }

                FPSSelector {
                    id: fpsSelector

                    opacity: Constants.CONTROL_OPACITY
                    model: camera.frameRates

                    currentFPS: camera.frameRate
                    onRequestFPS: { camera.frameRate = fps; }
                }
            }

            Row {
                Label {
                    width: Constants.SETTINGS_LABEL_WIDTH
                    height: showHistogramButtonRow.height

                    opacity: Constants.CONTROL_OPACITY
                    text: "Display\nhistogram"
                    verticalAlignment: Text.AlignVCenter
                    wrapMode: Text.WordWrap
                }

                ButtonRow {
                    id: showHistogramButtonRow

                    Button {
                        width: Constants.BUTTON_ROW_BUTTON_WIDTH
                        height: Constants.BUTTON_ROW_BUTTON_HEIGHT

                        opacity: Constants.CONTROL_OPACITY
                        text: "Yes"
                        onClicked: { settingsView.showHistogram = true; }
                    }

                    Button {
                        width: Constants.BUTTON_ROW_BUTTON_WIDTH
                        height: Constants.BUTTON_ROW_BUTTON_HEIGHT

                        opacity: Constants.CONTROL_OPACITY
                        text: "No"
                        onClicked: { settingsView.showHistogram = false; }
                    }
                }
            }

            Item {
                width: 100
                height: 10
            }
        }
    }

    ScrollDecorator {
        flickableItem: flickable
    }
}
