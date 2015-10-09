/**
 * Copyright (c) 2012-2014 Microsoft Mobile.
 */

import QtQuick 1.1
import com.nokia.symbian 1.1
import CustomElements 1.0
import "Constants.js" as Constants


ButtonRow {
    id: whiteBalanceSelector

    property variant model

    signal whiteBalanceSelected(int whiteBalanceMode)

    /*!
      Resolves the path to icon file corresponding to given white balance mode.
    */
    function resolveIconPathFromMode(mode) {
        switch (mode) {
        case CustomCamera.WhiteBalanceAuto:
            return "../images/wb_auto.svg";
        case CustomCamera.WhiteBalanceSunlight:
            return "../images/wb_sunlight.svg";
        case CustomCamera.WhiteBalanceCloudy:
            return "../images/wb_cloudy.svg";
        case CustomCamera.WhiteBalanceTungsten:
            return "../images/wb_tungsten.svg";
        case CustomCamera.WhiteBalanceFluorescent:
            return "../images/wb_fluorescent.svg";
        case CustomCamera.WhiteBalanceFlash:
        case CustomCamera.WhiteBalanceIncandescent:
        case CustomCamera.WhiteBalanceManual:
        case CustomCamera.WhiteBalanceShade:
        case CustomCamera.WhiteBalanceSunset:
        case CustomCamera.WhiteBalanceVendor:
            break;
        }

        return "";
    }

    onModelChanged: {
        for (var i=0; i<children.length; i++) {
            children[i].destroy();
        }

        for (i=0; i<model.length; i++) {
            var button = buttonComponent.createObject(whiteBalanceSelector);
            button.whiteBalanceMode = model[i];
            button.iconSource = resolveIconPathFromMode(model[i]);
            button.width = Constants.BUTTON_ROW_BUTTON_WIDTH;
            button.height = Constants.BUTTON_ROW_BUTTON_HEIGHT;
        }

        width = Constants.BUTTON_ROW_BUTTON_WIDTH * model.length;
    }

    Component {
        id: buttonComponent

        Button {
            property int whiteBalanceMode

            onClicked: {
                whiteBalanceSelector.whiteBalanceSelected(whiteBalanceMode);
            }
        }
    }
}
