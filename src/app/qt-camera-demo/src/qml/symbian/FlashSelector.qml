/**
 * Copyright (c) 2012-2014 Microsoft Mobile.
 */

import QtQuick 1.1
import com.nokia.symbian 1.1
import CustomElements 1.0
import "Constants.js" as Constants


CustomButton {
    id: flashSelector

    property variant model
    property int currentMode

    signal requestFlashMode(int flashMode)

    /*!
      Resolves the path to icon file corresponding to given flash mode.
    */
    function resolveIconPathFromMode(mode) {
        switch (mode) {
        case CustomCamera.FlashOn:
            return "../images/flash_on.svg";
        case CustomCamera.FlashOff:
            return "../images/flash_off.svg";
        case CustomCamera.FlashRedEyeReduction:
            return "../images/flash_red_eye.svg";
        case CustomCamera.FlashAuto:
            return "../images/flash_auto.svg";
        }

        return "";
    }

    iconSource: resolveIconPathFromMode(currentMode)

    onClicked: {
        var index = 0;
        var found = false;

        for (index=0; index<model.length; index++) {
            if (model[index] === currentMode) {
                found = true;
                break;
            }
        }

        if (!found) {
            return;
        }

        index++;

        if (index > (model.length-1)) {
            index = 0;
        }

        requestFlashMode(model[index]);
    }
}
