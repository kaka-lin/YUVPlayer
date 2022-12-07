import QtQuick 2.10
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.3
import QtMultimedia 5.9

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
                source: player
                anchors.fill: parent
                focus : visible // to receive focus and capture key events when visible
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

                    }

                    Label {
                        text: "Height: "
                        anchors.verticalCenter: heightFiled.verticalCenter
                    }

                    TextField {
                        id: heightFiled
                        width: settingArea.width / 3
                        height: 30
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
                        width: settingArea.width / 3
                        height: 30
                        currentIndex: 5
                        model: ListModel {
                            id: yuvComboBox
                            ListElement { text: "YUV444" }
                            ListElement { text: "YUV422(I422)" }
                            ListElement { text: "YUV420(I420)" }
                            ListElement { text: "YUYV(YUY2)" }
                            ListElement { text: "YVYU" }
                            ListElement { text: "UYVY" }
                            ListElement { text: "YV12" }
                            ListElement { text: "YV16" }
                            ListElement { text: "NV12" }
                            ListElement { text: "NV21" }
                        }
                        onCurrentIndexChanged: {
                            console.debug(yuvComboBox.get(currentIndex).text);
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
                            console.log(settingArea.width / 6);
                            player.play();
                        }
                    }

                    Button {
                        id: pauseBtn;
                        width: playBtn.width;
                        height: playBtn.height;
                        text: qsTr("Pause")
                        onClicked: {
                            player.pause();
                        }
                    }

                    Button {
                        id: stopBtn;
                        width: playBtn.width;
                        height: playBtn.height;
                        text: qsTr("Stop")
                        onClicked: {
                            player.stop();
                        }
                    }
                }
            }
        }
    }
}
