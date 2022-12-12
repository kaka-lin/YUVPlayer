import QtQuick 2.10
import QtQuick.Controls 2.3
import QtQuick.Dialogs 1.3
import QtQuick.Layouts 1.3
import QtMultimedia 5.9
import QtQuick.Window 2.10

import components.common 1.0

Rectangle {
    id: root
    anchors.fill: parent
    // anchors.margins: 3

    Row {
        id: grid
        spacing: 5

        // row 1
        Rectangle {
            id: showArea
            width: (root.width - grid.spacing) / 3 * 2
            height: root.height
            color: "black"

            VideoOutput {
                id: videoOutput
                source: player
                anchors.fill: parent
                focus : visible // to receive focus and capture key events when visible
                autoOrientation: true
            }
        }

        Rectangle {
            id: settingArea
            width: (root.width - grid.spacing) / 3 * 1
            height: root.height
            color: "#f5f5dc"

            Column {
                anchors {
                    horizontalCenter: parent.horizontalCenter
                    fill: parent
                    margins: 10
                }

                spacing: 20

                GroupBox {
                    title: "Video Source: "

                    Row {
                        spacing: 10

                        RadioButton {
                            id: fileCheck
                            text: qsTr("File")
                            checked: true
                        }

                        RadioButton {
                            id: cameraCheck
                            text: qsTr("Camera")
                        }
                    }
                }

                Row {
                    spacing: 10
                    visible: fileCheck.checked

                    Button {
                        id: fileBtn;
                        width: settingArea.width / 6
                        height: 30
                        text: qsTr("File")
                        onClicked: fileDialog2.open();
                    }

                    TextField {
                        id: fileFiled
                        width: settingArea.width / 1.5
                        height: 30

                        selectByMouse: true
                    }

                    FileDialog {
                        id: fileDialog2
                        title: "Please choose a file"
                        folder: shortcuts.pictures
                        visible: false
                        nameFilters: [ "Image files (*.yuv *.png *.jpg)", "All files (*)" ]

                        onAccepted: {
                            console.log("You chose: " + fileDialog2.fileUrls);
                            var path = fileDialog2.fileUrl.toString();
                            // remove prefixed "file:///" -> /
                            path = path.replace(/^(file:\/{2})|(qrc:\/{2})|(http:\/{2})/,"");
                            // unescape html codes like '%23' for '#'
                            var cleanPath = decodeURIComponent(path);
                            fileFiled.text = cleanPath;
                        }
                    }
                }

                Row {
                    spacing: 10
                    visible: cameraCheck.checked

                    Label {
                        text: "Camera: "
                        anchors.verticalCenter: camera.verticalCenter
                    }

                    CameraCombo {
                        id: camera
                        anchors.verticalCenter: parent.verticalCenter
                    }
                }

                Row {
                    spacing: 10

                    Label {
                        text: "Width: "
                        anchors.verticalCenter: widthFiled.verticalCenter
                    }

                    TextField {
                        id: widthFiled
                        width: settingArea.width / 3
                        height: 30
                        text: fileCheck.checked ? "1280" : "640"

                        selectByMouse: true

                        onTextChanged: {

                        }
                    }

                    Label {
                        text: "Height: "
                        anchors.verticalCenter: heightFiled.verticalCenter
                    }

                    TextField {
                        id: heightFiled
                        width: settingArea.width / 3
                        height: 30
                        text: fileCheck.checked ? "720" : "480"

                        selectByMouse: true
                    }
                }

                Row {
                    spacing: 10

                    Label {
                        text: "Framerate: "
                        anchors.verticalCenter: framerateFiled.verticalCenter
                    }

                    TextField {
                        id: framerateFiled
                        width: settingArea.width / 3
                        height: 30
                        text: "30"

                        selectByMouse: true
                    }
                }

                Row {
                    spacing: 10

                    Label {
                        text: "YUV Format: "
                        anchors.verticalCenter: yuvFiled.verticalCenter
                    }

                    ComboBox {
                        id: yuvFiled
                        width: settingArea.width / 2
                        height: 30
                        currentIndex: fileCheck.checked ? 3 : 2
                        model: ListModel {
                            id: yuvComboBox
                            ListElement { text: "I420 (YU12, YUV420P)" }
                            ListElement { text: "YV12 (YUV420P)" }
                            ListElement { text: "YUY2 (YUYV)" }
                            ListElement { text: "UYVY (Y422)" }
                            ListElement { text: "YVYU" }
                            ListElement { text: "NV12 (YUV420SP)" }
                            ListElement { text: "NV21 (YUV420SP)" }
                            ListElement { text: "NV16 (YUV422SP)" }
                            ListElement { text: "NV61 (YUV422SP)" }
                        }
                        onCurrentIndexChanged: {
                            // console.debug(yuvComboBox.get(currentIndex).text);
                        }
                    }
                }

                Row {
                    spacing: 10

                    Button {
                        id: playBtn;
                        width: (settingArea.width - 40) / 3;
                        height: 60;
                        text: qsTr("Play")
                        onClicked: {
                            var mode = "";
                            var file = "";

                            if (fileCheck.checked && fileFiled.text == "") {
                                console.log("Please choose a file")
                            } else if (cameraCheck.checked && camera.currentText == " ") {
                                console.log("Please choose a camera")
                            } else {
                                mode = fileCheck.checked ? "file" : "camera"
                                file = fileCheck.checked ? fileFiled.text : camera.currentText
                                playBtn.enabled = false;
                                pauseBtn.enabled = true;
                                stopBtn.enabled = true;
                                var format = yuvFiled.currentText.split(' ')[0];

                                var config = {
                                    'mode': mode,
                                    'file': file,
                                    'width': widthFiled.text,
                                    'height': heightFiled.text,
                                    'framerate': framerateFiled.text,
                                    'format': format
                                };

                                player.play(config);
                            }
                        }
                    }

                    Button {
                        id: pauseBtn;
                        width: playBtn.width;
                        height: playBtn.height;
                        text: qsTr("Pause")
                        enabled: false

                        onClicked: {
                            pauseBtn.text = pauseBtn.text == qsTr("Pause") ? qsTr("Continue") : qsTr("Pause")
                            player.pause();
                        }
                    }

                    Button {
                        id: stopBtn;
                        width: playBtn.width;
                        height: playBtn.height;
                        text: qsTr("Stop")
                        enabled: false

                        onClicked: {
                            stopBtn.enabled = false;
                            pauseBtn.enabled = false;
                            playBtn.enabled = true;
                            player.stop();
                        }
                    }
                }
            }
        }
    }

    Connections {
        target: player

        onThreadFinishSig: {
            stopBtn.enabled = false;
            pauseBtn.enabled = false;
            playBtn.enabled = true;
        }
    }
}
