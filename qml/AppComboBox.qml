import QtQuick 2.0
import QtQuick.Controls 2.15

ComboBox {
    id: dirComboBox
    height: 25
    model: ["Файловая система", "Сетевые папки"]
    anchors.topMargin: 5
    anchors.leftMargin: 10
}
