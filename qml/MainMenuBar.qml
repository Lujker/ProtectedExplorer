import QtQuick 2.0
import QtQuick.Controls 2.15

MenuBar {
    background: Rectangle {
        color: "#d8f3fb"
    }

    Menu {
        id: fileMenu
        title: qsTr("File")
        height: parent.height
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
                text: qsTr("Tooltips")
                onClicked: mainWindow.help = !mainWindow.help
            }
        }
        MenuItem {
            text: "Apply and save settings"
            onClicked: AppSettings.save_app_settings()
        }
    }
}

/*##^##
Designer {
    D{i:0;autoSize:true;height:480;width:640}
}
##^##*/

