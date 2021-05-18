﻿import QtQuick 2.0
import QtQuick.Controls 2.15

MenuBar {
    background: Rectangle {
        color: "skyblue"
    }

    Menu {
        id: fileMenu
        title: qsTr("Файл")
        height: parent.height
        MenuItem {
            text: qsTr("Выход")
            onTriggered: Qt.quit()
        }
    }
    Menu {
        id: settMenu
        title: qsTr("Параметры")
        MenuItem {
            CheckBox {
                text: qsTr("Подсказки")
                onClicked: mainWindow.help = !mainWindow.help
            }
        }
        MenuItem {
            text: "Сохранить настройки"
            onClicked: AppSettings.save_app_settings()
        }
    }
}

/*##^##
Designer {
    D{i:0;autoSize:true;height:480;width:640}
}
##^##*/

