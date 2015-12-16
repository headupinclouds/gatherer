/****************************************************************************
**
** Copyright (C) 2015 The Qt Company Ltd.
** Copyright (C) 2015 Ruslan Baratov
** Contact: http://www.qt.io/licensing/
**
** This file is part of the examples of the Qt Multimedia module.
**
** $QT_BEGIN_LICENSE:LGPL21$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see http://www.qt.io/terms-conditions. For further
** information use the contact form at http://www.qt.io/contact-us.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 2.1 or version 3 as published by the Free
** Software Foundation and appearing in the file LICENSE.LGPLv21 and
** LICENSE.LGPLv3 included in the packaging of this file. Please review the
** following information to ensure the GNU Lesser General Public License
** requirements will be met: https://www.gnu.org/licenses/lgpl.html and
** http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** As a special exception, The Qt Company gives you certain additional
** rights. These rights are described in The Qt Company LGPL Exception
** version 1.1, included in the file LGPL_EXCEPTION.txt in this package.
**
** $QT_END_LICENSE$
**
****************************************************************************/

import QtQuick 2.0
import QtMultimedia 5.5
import qmlvideofilter.test 1.0 // VideoFilter, InfoFilter

Item {
  width: 1024
  height: 768

  Camera {
    id: camera
    objectName: "CameraObject"
  }

  VideoOutput {
    id: output
    source: camera
    filters: [ infofilter, videofilter ]
    anchors.fill: parent
  }

  VideoFilter {
    id: videofilter
    // Animate a property which is passed to filter.
    SequentialAnimation on factor {
      loops: Animation.Infinite
      NumberAnimation {
        from: 1
        to: 20
        duration: 6000
      }
      NumberAnimation {
        from: 20
        to: 1
        duration: 3000
      }
    }
  }

  InfoFilter {
    // This filter does not change the image.
    // Instead, it provides some results calculated from the frame.
    id: infofilter
    onFinished: {
      infoResolution.v = result.frameResolution.width + "x" + result.frameResolution.height;
      infoFrameType.v = result.handleType;
      infoPixelFormat.v = result.pixelFormat;
    }
  }

  Column {
    Text {
      font.pointSize: 20
      color: "green"
      text: "Transformed with VideoFilter on GPU\nClick to disable and enable the emboss filter"
    }
    Text {
      font.pointSize: 12
      color: "green"
      text: "Emboss factor " + Math.round(videofilter.factor)
      visible: videofilter.active
    }
    Text {
      id: infoResolution
      font.pointSize: 12
      color: "green"
      property string v
      text: "Input resolution: " + v
    }
    Text {
      id: infoFrameType
      font.pointSize: 12
      color: "blue"
      property string v
      text: "Input frame type: " + v
    }
    Text {
      id: infoPixelFormat
      font.pointSize: 12
      color: "red"
      property string v
      text: v ? "Pixel format: " + v : ""
    }
  }

  MouseArea {
    anchors.fill: parent
    onClicked: videofilter.active = !videofilter.active
  }
}
