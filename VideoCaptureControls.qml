/****************************************************************************
**
** Copyright (C) 2015 The Qt Company Ltd.
** Contact: http://www.qt.io/licensing/
**
** This file is part of the examples of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:BSD$
** You may use this file under the terms of the BSD license as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**   * Neither the name of The Qt Company Ltd nor the names of its
**     contributors may be used to endorse or promote products derived
**     from this software without specific prior written permission.
**
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
**
** $QT_END_LICENSE$
**
****************************************************************************/

import QtQuick 2.0
import QtMultimedia 5.4

FocusScope {
    property Camera camera
    property bool previewAvailable : false

    property int buttonsPanelWidth: buttonPaneShadow.width

    signal previewSelected
    signal photoModeSelected
    id : captureControls

    Rectangle {
        id: buttonPaneShadow
        width: bottomColumn.width + 16
        height: parent.height
        anchors.top: parent.top
        anchors.right: parent.right
        color: Qt.rgba(0.08, 0.08, 0.08, 1)

        Column {
            anchors {
                right: parent.right
                top: parent.top
                margins: 8
            }

            id: buttonsColumn
            spacing: 8

            Image{
                id : currentFrame
                source: "image://updatedframeprovider/yellow"
                cache: false
                asynchronous: false //true
            }

            Timer {
                interval: 1000
                repeat: true
                running: true
                onTriggered: { currentFrame.source = ""; currentFrame.source = "image://updatedframeprovider/red" }
            }
            Timer {
                interval: 1500
                repeat: true
                running: true
                onTriggered: { currentFrame.source = ""; currentFrame.source = "image://updatedframeprovider/yellow" }
            }

            CameraButton {
                text: "Demo 2 (Toggle)"
                visible: camera.cameraStatus == Camera.ActiveStatus
                onClicked: (cameraUI.state == "VideoPreview")? cameraUI.state = "VideoCapture": cameraUI.state = "VideoPreview"
            }
        }

        Column {
            anchors {
                bottom: parent.bottom
                right: parent.right
                margins: 8
            }

            id: bottomColumn
            spacing: 8

            CameraListButton {
                model: QtMultimedia.availableCameras
                onValueChanged: captureControls.camera.deviceId = value
            }

            CameraButton {
                text: "Switch to Demo 1"
                onClicked: captureControls.photoModeSelected()
            }

            CameraButton {
                id: quitButton
                text: "Quit"
                onClicked: Qt.quit()
            }
        }
    }


    ZoomControl {
        x : 0
        y : 0
        width : 100
        height: parent.height

        currentZoom: camera.digitalZoom
        maximumZoom: Math.min(4.0, camera.maximumDigitalZoom)
        onZoomTo: camera.setDigitalZoom(value)
    }
}
