import QtQuick 2.0
import QtQuick.Controls 1.4
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.3

Page {
    property alias listModel: fileList.model
    signal pressToElement(int index)
    signal openFolder(int index)
    TableView {

        id: fileList
        anchors.fill: parent

        //        onDoubleClicked: {
        //            openFolder(index)
        //        }
        TableViewColumn {
            role: "name"
            title: "Filename"
            width: 200
        }
        TableViewColumn {
            role: "size"
            title: "Size"
            width: 50
        }
        TableViewColumn {
            role: "date"
            title: "Date"
            width: 50
        }

        //          Item {
        //              id: treeListDelegate
        //              height: 36
        //              width: fileList.width
        //              Rectangle {
        //                  id: rect
        //                  property bool isHovered: false
        //                  color: isHovered ? "#7ddfc1" : "#c5ece0"
        //                  radius: 12
        //                  anchors.fill: parent
        //                  border.color: "black"
        //                  anchors.topMargin: 8
        //                  anchors.leftMargin: 16
        //                  anchors.rightMargin: 16
        //                  Menu {
        //                      id: myContextMenu
        //                      MenuItem {
        //                          text: "Add element"
        //                          onTriggered: {
        //                              //                                TreeList.addNewTree()
        //                          }
        //                      }
        //                      MenuItem {
        //                          text: "Delete element"
        //                          onTriggered: {
        //                              //                                TreeList.erase_elem(index)
        //                          }
        //                      }
        //                  }
        //                  MouseArea {
        //                      anchors.fill: parent
        //                      acceptedButtons: Qt.LeftButton | Qt.RightButton
        //                      hoverEnabled: true
        //                      onHoveredChanged: {
        //                          rect.isHovered = !rect.isHovered
        //                      }
        //                      onClicked: {
        //                          if (mouse.button == Qt.RightButton) {
        //                              myContextMenu.popup()
        //                          } else {
        //                              pressToElement(index)
        //                          }
        //                      }
        //                      onDoubleClicked: {
        //                          if (mouse.button == Qt.RightButton) {
        //                              myContextMenu.popup()
        //                          } else {
        //                              openFolder(index)
        //                          }
        //                      }
        //                  }
        //                  //                    Rectangle {
        //                  //                        id: icon
        //                  //                        width: rect.width / 5
        //                  //                        anchors.left: parent.left
        //                  //                        Image {
        //                  //                            id: iconOpenImage
        //                  //                            opacity: rect.isHovered ? 1 : 0.2
        //                  //                            //                            source: image
        //                  //                        }
        //                  //                    }
        //                  //                    Rectangle {
        //                  //                        id: nameTextItem
        //                  //                        width: rect.width / 5
        //                  //                        anchors.left: icon.right
        //                  //                        anchors.leftMargin: 10
        //                  //                        anchors.top: rect.top
        //                  //                        anchors.topMargin: 5
        //                  //                        Text {
        //                  //                            id: nameText
        //                  //                            width: parent.width
        //                  //                            text: name
        //                  //                            verticalAlignment: Text.AlignBottom
        //                  //                            horizontalAlignment: Text.AlignHCenter
        //                  //                            font.pointSize: 12
        //                  //                        }
        //                  //                    }
        //                  //                    Rectangle {
        //                  //                        id: sizeTextItem
        //                  //                        width: rect.width / 5
        //                  //                        anchors.left: nameTextItem.right
        //                  //                        anchors.leftMargin: 10
        //                  //                        anchors.top: rect.top
        //                  //                        anchors.topMargin: 5
        //                  //                        Text {
        //                  //                            id: sizeText
        //                  //                            width: parent.width
        //                  //                            opacity: rect.isHovered ? 1 : 0.2
        //                  //                            visible: {
        //                  //                                if (size == "") {
        //                  //                                    return false
        //                  //                                } else {
        //                  //                                    return true
        //                  //                                }
        //                  //                            }
        //                  //                            text: size
        //                  //                            verticalAlignment: Text.AlignBottom
        //                  //                            horizontalAlignment: Text.AlignHCenter
        //                  //                            font.pointSize: 12
        //                  //                        }
        //                  //                    }
        //                  //                    Rectangle {
        //                  //                        id: dateTextItem
        //                  //                        width: rect.width / 5
        //                  //                        anchors.left: sizeTextItem.right
        //                  //                        anchors.leftMargin: 34
        //                  //                        anchors.top: rect.top
        //                  //                        anchors.topMargin: 5
        //                  //                        Text {
        //                  //                            id: dateText
        //                  //                            width: parent.width
        //                  //                            opacity: rect.isHovered ? 1 : 0.2
        //                  //                            text: date
        //                  //                            verticalAlignment: Text.AlignBottom
        //                  //                            horizontalAlignment: Text.AlignHCenter
        //                  //                            font.pointSize: 12
        //                  //                        }
        //                  //                    }
    }
}
