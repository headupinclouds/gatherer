/**
 * Copyright (c) 2012-2014 Microsoft Mobile.
 */

import QtQuick 1.1
import com.nokia.symbian 1.1
import CustomElements 1.0
import "Constants.js" as Constants


ButtonRow {
    id: exposureModeSelector

    property variant model

    signal exposureModeSelected(int exposureMode)

    /*!
      Resolves the path to icon file corresponding to given exposure mode.
    */
    function resolveIconPathFromMode(mode) {
        switch (mode) {
        case CustomCamera.ExposureAuto:
            return "../images/exposure_auto.svg";
        case CustomCamera.ExposureNight:
            return "../images/exposure_night.svg";
        case CustomCamera.ExposureBacklight:
            return "../images/exposure_backlight.svg";
        case CustomCamera.ExposureSports:
            return "../images/exposure_sport.svg";
        case CustomCamera.ExposureManual:
        case CustomCamera.ExposurePortrait:
        case CustomCamera.ExposureSpotlight:
        case CustomCamera.ExposureSnow:
        case CustomCamera.ExposureBeach:
        case CustomCamera.ExposureLargeAperture:
        case CustomCamera.ExposureSmallAperture:
        case CustomCamera.ExposureModeVendor:
            break;
        }

        return "";
    }

    onModelChanged: {
        for (var i=0; i<children.length; i++) {
            children[i].destroy();
        }

        for (i=0; i<model.length; i++) {
            var button = buttonComponent.createObject(exposureModeSelector);
            button.exposureMode = model[i];
            button.iconSource = resolveIconPathFromMode(model[i]);
            button.width = Constants.BUTTON_ROW_BUTTON_WIDTH;
            button.height = Constants.BUTTON_ROW_BUTTON_HEIGHT;
        }

        width = Constants.BUTTON_ROW_BUTTON_WIDTH * model.length;
    }

    Component {
        id: buttonComponent

        Button {
            property int exposureMode

            onClicked: exposureModeSelector.exposureModeSelected(exposureMode)
        }
    }
}
