import QtQuick 2.10
import QtQuick.Controls 2.3
import QtQuick.Dialogs 1.3
import QtQuick.Window 2.10
import QtGraphicalEffects 1.0 // DropShadow

import components.common 1.0
import "pages"

ApplicationWindow {
    id: window
    visible: true
    width: 1280
    height: 720
    //visibility: Window.FullScreen
    title: qsTr("YUVPlayer")

    header: ToolBar {
        id: menu

        background: Rectangle {
            implicitWidth: 100
            implicitHeight: 50
            border.color: "#999"

            gradient: Gradient {
                GradientStop { position: 0 ; color: "#fff" }
                GradientStop { position: 1 ; color: "#eee" }
            }
        }

        Row {
            anchors.fill: parent
            spacing: 5

            ToolButton {
                Image {
                    id: newFileImage
                    source: "images/newFile.ico"
                    asynchronous:true
                    fillMode: Image.PreserveAspectFit
                    anchors.fill: parent
                }
                anchors.verticalCenter: parent.verticalCenter
                onClicked: fileDialog.open();
            }

            ToolButton {
                Image {
                    id: aboutImage
                    source: "images/about.ico"
                    asynchronous:true
                    fillMode: Image.PreserveAspectFit
                    anchors.fill: parent
                }
                anchors.verticalCenter: parent.verticalCenter
                onClicked: aboutBox.open();
            }

            ToolButton {
                Image {
                    id: exitImage
                    source: "images/exit.ico"
                    asynchronous:true
                    fillMode: Image.PreserveAspectFit
                    anchors.fill: parent
                }
                anchors.verticalCenter: parent.verticalCenter
                onClicked: {
                    Qt.quit();
                }
            }
        }

        Clock {
            id: clock
            anchors.right: parent.right
            anchors.rightMargin: 3
            anchors.verticalCenter: parent.verticalCenter

            gradient: Gradient {
                GradientStop { position: 0; color: "#fff" }
                GradientStop { position: 1; color: "#eee" }
            }
        }
    }

    MessageDialog {
        id: aboutBox
        title: "About"
        text: "
               This is YUVPlayer\n
               Version: 0.1
               Date:2022/12/06"
        icon: StandardIcon.Information
    }

    FileDialog {
        id: fileDialog
        title: "Please choose a file"
        folder: shortcuts.home
        visible: false
        selectFolder: true
    }

    // Page: YUVPlayer
    YUVPlayerPage {}
}
