/**
 * Copyright (c) 2012-2014 Microsoft Mobile.
 */

import QtQuick 1.1
import com.nokia.symbian 1.1
import CustomElements 1.0
import "Constants.js" as Constants

ButtonRow {
    id: videoResolutionSelector

    property variant model
    property string currentResolution

    signal requestVideoResolution(string resolution)

    function updateSelection() {
        for (var i=0; i<children.length; i++) {
            if (children[i].resolution === currentResolution) {
                videoResolutionSelector.checkedButton = children[i];
                break;
            }
        }
    }

    onModelChanged: {
        for (var i=0; i<children.length; i++) {
            children[i].destroy();
        }

        for (i=0; i<model.length; i++) {
            var button = buttonComponent.createObject(videoResolutionSelector);
            button.resolution = model[i];
            button.text = model[i];
            button.width = Constants.SETTINGS_SLIDER_WIDTH / 3
            button.height = Constants.BUTTON_ROW_BUTTON_HEIGHT
        }

        width = Constants.SETTINGS_SLIDER_WIDTH / 3 * model.length
    }

    Component {
        id: buttonComponent

        Button {
            property string resolution

            onClicked: {
                videoResolutionSelector.requestVideoResolution(resolution);
            }
        }
    }
}
