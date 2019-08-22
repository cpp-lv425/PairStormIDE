import QtQuick 2.7
import QtQuick.Controls 2.0
import QtQuick 2.8
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.3
import QtQml.Models 2.12
import "basicelements" as BasicElements
import PairStormChat 1.0

Item {
    id: messagesHistory

    Rectangle {
        anchors.fill: parent
        color: "#5a8db8"

        //gradient: Gradient.SolidStone
        /*

        Image {
            anchors.fill: parent
            source: "res/BACKGROUND.jpg"
        }*/
    }

    ListView {
        id: listView

        anchors.fill: parent

        anchors.margins: 0

        spacing: 1
        clip: true

        highlightFollowsCurrentItem: true

        model: messagesModel

        boundsBehavior: Flickable.StopAtBounds
        highlightMoveDuration: 300
        onCountChanged: {
            Qt.callLater( listView.positionViewAtEnd )
        }
        onHeightChanged: {
            listView.positionViewAtEnd()
        }
    }

    DelegateModel {
        id: messagesModel

        model: MessagesModel {
            list: messagesList
        }

        delegate: Column {
            id: messageAttributesColumn

            readonly property bool adjustRight: model.authorName === globalUserName
            anchors.right: messageAttributesColumn.adjustRight ?
                               parent.right : undefined
            anchors.left:  messageAttributesColumn.adjustRight ?
                               undefined    : parent.left

            spacing: 3

            Row {
                id: messageAttributesRow

                anchors.right: messageAttributesColumn.adjustRight ?
                                   parent.right : undefined
                anchors.left:  messageAttributesColumn.adjustRight ?
                                   undefined    : parent.left

                spacing: 0


                Control {
                    id: leftToothShape

                    anchors.bottom: messageBody.bottom
                    width:  messageAttributesColumn.adjustRight ? 0 : 10
                    height: messageAttributesColumn.adjustRight ? 0 : 15
                    property string qml: messageAttributesColumn.adjustRight ?
                                             "" : "basicelements/ToothShapeToLeft.qml"

                    Loader {
                        anchors.fill: parent
                        Component.onCompleted: {
                            setSource(leftToothShape.qml,
                                      {
                                          itemColor: "#263745",
                                          leftOffset: 16,
                                          maxHeight: 20
                                      })
                        }
                    }
                }

                Rectangle {
                    id: messageBody

                    width: { return Math.min(Math.max(messageText.implicitWidth, messageAuthor.contentWidth) + 24,
                                    messagesHistory.width - 70) }

                    height: messageText.implicitHeight + 27
                    color: "#263745"

                    border.width: 0
                    radius: 14

                    TextEdit {
                        id: messageText

                        readOnly: true
                        selectByMouse: true

                        anchors.fill: parent
                        anchors.margins: 10
                        anchors.topMargin: 23
                        wrapMode: Label.Wrap

                        text: model.content
                        color: "white"

                        MouseArea {
                            anchors.fill: parent
                            enabled: false

                            cursorShape: Qt.IBeamCursor
                        }
                    }

                    Label {
                        id: messageAuthor

                        anchors.top: parent.top
                        anchors.topMargin: 4


                        anchors.right: messageAttributesColumn.adjustRight?
                                           messageBody.right : undefined
                        anchors.left: messageAttributesColumn.adjustRight?
                                           undefined : messageBody.left

                        anchors.leftMargin: 10
                        anchors.rightMargin: 10

                        Image {
                            width:  13
                            height: 13
                            source: "res/USER.png"
                        }

                        text: qsTr("  " + model.authorName)
                        font.bold: true
                        font.pixelSize: 15
                        font.family: "consolas"
                        color: "white"
                    }
                }

                Control {
                    id: rightToothShape

                    anchors.bottom: messageBody.bottom
                    width:  messageAttributesColumn.adjustRight ? 10 : 0
                    height: messageAttributesColumn.adjustRight ? 15 : 0
                    property string qml: messageAttributesColumn.adjustRight ?
                                             "basicelements/ToothShapeToRight.qml" : ""

                    Loader {
                        anchors.fill: parent
                        Component.onCompleted: {
                            setSource(rightToothShape.qml,
                                      {
                                          itemColor: "#263745",
                                          leftOffset: 16,
                                          maxHeight: 20
                                      })
                        }
                    }
                }
            }
            Label {
                id: timestampText

                anchors.right: messageAttributesColumn.adjustRight ?
                                   parent.right : undefined
                anchors.left:  messageAttributesColumn.adjustRight ?
                                   undefined    : parent.left

                text: Qt.formatDateTime(model.publicationDateTime, "d MMM hh:mm")
                color: "white"
            }
        }
    }
}

