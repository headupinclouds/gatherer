/**
 * Copyright (c) 2012-2014 Microsoft Mobile.
 */

import QtQuick 1.1
import com.nokia.symbian 1.1
import "Constants.js" as Constants

ButtonRow {
    id: isoSelector

    property variant model

    signal isoSelected(string isoValue)

    onModelChanged: {
        for (var i=0; i<children.length; i++) {
            children[i].destroy();
        }

        for (i=0; i<model.length; i++) {
            var button = buttonComponent.createObject(isoSelector);
            button.iso = model[i];
            button.iconSource = "../images/iso_" + model[i] + ".svg";
            button.width = Constants.BUTTON_ROW_BUTTON_WIDTH;
            button.height = Constants.BUTTON_ROW_BUTTON_HEIGHT;
        }

        width = Constants.BUTTON_ROW_BUTTON_WIDTH * model.length;
    }

    Component {
        id: buttonComponent

        Button {
            property string iso

            onClicked: isoSelector.isoSelected(iso);
        }
    }
}
