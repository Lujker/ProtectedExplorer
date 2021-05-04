import QtQuick 2.0
import QtQuick.Controls 2.15

MenuBar {
    background: Rectangle {
        color: "#d8f3fb"
    }

    Menu {
        id: fileMenu
        title: qsTr("File")

        MenuItem {
            text: qsTr("Add one from dump")
            onTriggered: dupmFile.open()
        }
        MenuItem {
            text: qsTr("Add all from dump dir")
            visible: !isClient
            height: isClient ? 0 : 40
            onTriggered: dupmFileDir.open()
        }
        MenuItem {
            text: qsTr("Save all to dmp")
            onTriggered: TreeList.save_to_file(AppSettings.save_dump_dir)
        }
        MenuItem {
            text: qsTr("Exit")
            onTriggered: Qt.quit()
        }
    }
    Menu {
        id: settMenu
        title: qsTr("Settings")
        MenuItem {
            CheckBox {
                text: qsTr("Dark theme")
                onClicked: colors.isDarkTheme = !colors.isDarkTheme
            }
        }
        MenuItem {
            CheckBox {
                text: qsTr("Tooltips")
                onClicked: appWindow.help = !appWindow.help
            }
        }
        MenuItem {
            text: AppSettings.save_dump_dir
            ToolTip.text: "Pick folder for save dump"
            ToolTip.visible: mar.containsMouse ? true : false
            MouseArea {
                id: mar
                anchors.fill: parent
                hoverEnabled: appWindow.help
                onClicked: dupmFolder.open()
            }
        }
        MenuItem {
            text: "Apply and save settings"
            onClicked: AppSettings.save_app_settings()
        }
        MenuItem {
            id: serverIpInfo
            text: "Server IP: " + AppSettings.ip_serv
            background: Rectangle {
                color: Connector.isConnected ? "#c5ece0" : "red"
                border.width: 1
            }
        }
    }
}

/*##^##
Designer {
    D{i:0;autoSize:true;height:480;width:640}
}
##^##*/

