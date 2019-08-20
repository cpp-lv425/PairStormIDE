import QtQuick 2.7
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.3
import QtQml.Models 2.12

import "scripts/ChatBase.js" as ChatBase

Item {
    id: inputMessageArea

    width:  parent.width
    height: messageInput.implicitHeight + 8
    onHeightChanged: {console.log("text heigh has been changned to " + height) }

    Rectangle {
        anchors.fill: parent
        //color: "steelblue"
        color: "#263745"
    }

    RowLayout {
        anchors.fill: parent

        Rectangle {
            id: messageInputField

            //Layout.fillHeight: true
            //height: messageInput.implicitHeight
            Layout.alignment: Qt.AlignCenter
            Layout.leftMargin: 5
            Layout.rightMargin: 10

            Layout.preferredHeight: messageInput.implicitHeight
            Layout.maximumHeight:   inputMessageArea.height - 8
            Layout.fillWidth: true
            radius: 4

            color: "white"
            border.width: 2
            border.color: "snow"


            TextInput {
                id: messageInput
                anchors.fill: parent
                anchors.centerIn: parent
                clip: true
                wrapMode: TextInput.WordWrap

                //text: qsTr("message")
                //overwriteMode: true

                color: "darkgrey"
                selectedTextColor: "black"
                selectionColor:    "silver"

                topPadding: 3
                bottomPadding: 3
                leftPadding:  10
                rightPadding: 10
                font.pixelSize: 20
                font.family: "consolas"

                onAccepted: {
                    if (messageInput.text === "")
                    {
                        console.log("message is empty")
                        return
                    }
                    messageInput.clear()
                    console.log("send message: " + messageInput.text)
                }

                property string placeholderText: "Type message..."

                Text {
                    text: messageInput.placeholderText
                    topPadding: 3
                    bottomPadding: 3
                    leftPadding:  10
                    rightPadding: 10
                    font.pixelSize: 20
                    font.family: "consolas"
                    color: "#aaa"
                    visible: !messageInput.text && !messageInput.activeFocus // <----------- ;-)
                }
            }
/*
            MouseArea {
                anchors.fill: messageInput
                cursorShape: Qt.IBeamCursor
            }*/
        }

        Rectangle {
            id: sendButton

            //Layout.fillHeight: true
            Layout.maximumHeight: 24
            Layout.minimumHeight: 24
            Layout.minimumWidth: 24
            Layout.maximumWidth: 24
            Layout.alignment: Qt.AlignBottom
            Layout.rightMargin: 4
            Layout.bottomMargin: 7

            color: "transparent"

            Image {
                anchors.fill: sendButton
                source: "res/SEND.png"
                fillMode: Image.PreserveAspectFit
                //sourceSize.width:  300
                //sourceSize.height: 300
            }

            MouseArea {
                anchors.fill: parent
                hoverEnabled: true

                cursorShape: Qt.PointingHandCursor
                onClicked: {
                    if (messageInput.text === "")
                    {
                        console.log("message is empty")
                        return
                    }
                    messageInput.clear()
                    console.log("send message: " + messageInput.text)
                }
            }
        }
    }
}
