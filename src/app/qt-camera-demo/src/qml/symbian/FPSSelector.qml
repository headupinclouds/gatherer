/**
 * Copyright (c) 2012-2014 Microsoft Mobile.
 */

import QtQuick 1.1
import com.nokia.symbian 1.1
import CustomElements 1.0
import "Constants.js" as Constants

ButtonRow {
    id: fpsSelector

    property variant model
    property int currentFPS

    signal requestFPS(int fps)

    function updateSelection() {
        for (var i=0; i<children.length; i++) {
            if (children[i].fps === currentFPS) {
                fpsSelector.checkedButton = children[i];
                break;
            }
        }
    }

    onModelChanged: {
        for (var i=0; i<children.length; i++) {
            children[i].destroy();
        }

        for (i=0; i<model.length; i++) {
            var button = buttonComponent.createObject(fpsSelector);
            button.fps = model[i];
            button.text = model[i];
            button.width = Constants.BUTTON_ROW_BUTTON_WIDTH
            button.height = Constants.BUTTON_ROW_BUTTON_HEIGHT
        }

        width = Constants.BUTTON_ROW_BUTTON_WIDTH * model.length
    }

    Component {
        id: buttonComponent

        Button {
            property int fps

            onClicked: { fpsSelector.requestFPS(fps); }
        }
    }
}

