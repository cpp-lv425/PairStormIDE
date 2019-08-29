import QtQuick         2.8
import QtQuick.Layouts 1.3
import "scripts/ChatBase.js" as ChatBase

Item
{
    id: messageInputArea

    width:  parent.width
    height: messageInput.implicitHeight + 8

    readonly property string messageInputHint: "Message..."

    Rectangle
    {
        anchors.fill: parent
        color: ChatBase.chatInputBackgroundColor(globalTheme)
    }
    RowLayout
    {
        anchors.fill: parent

        Rectangle
        {
            id: messageInputField

            Layout.alignment:   Qt.AlignCenter
            Layout.leftMargin:  5
            Layout.rightMargin: 10

            Layout.fillWidth: true

            Layout.maximumHeight:   messageInputArea.height - 8
            Layout.preferredHeight: messageInput.implicitHeight
            radius: 3

            color:        Qt.darker(ChatBase.chatInputBackgroundColor(globalTheme), 1.1)
            border.color: Qt.darker(ChatBase.chatInputBackgroundColor(globalTheme), 1.05)

            border.width: 1

            TextInput
            {
                id: messageInput

                anchors.fill: parent

                topPadding:    3
                leftPadding:   10
                rightPadding:  10
                bottomPadding: 3

                selectByMouse: true
                clip:          true
                renderType:    Text.QtRendering
                wrapMode:      TextInput.WordWrap

                color:             ChatBase.chatInputTextColor(globalTheme)
                selectionColor:    ChatBase.chatInputSelectionBackColor(globalTheme)
                selectedTextColor: ChatBase.chatInputSelectionTextColor(globalTheme)

                font.pixelSize: 20
                font.family:    "Consolas"

                onAccepted:
                {
                    if (messageInput.text === "")
                    {
                        return
                    }
                    messagesList.appendMessage(messageInput.text)
                    messageInput.clear()
                }
                Text
                {
                    text: messageInputArea.messageInputHint

                    topPadding:    3
                    leftPadding:   10
                    rightPadding:  10
                    bottomPadding: 3

                    visible: !messageInput.text && !messageInput.activeFocus

                    font.pixelSize: 20
                    font.family:    "Consolas"

                    color: ChatBase.chatInputTextHintColor(globalTheme)
                }
            }
            MouseArea
            {
                anchors.fill: messageInput

                enabled:     false
                cursorShape: Qt.IBeamCursor
            }
        }
        Rectangle
        {
            id: sendButton

            Layout.alignment: Qt.AlignBottom

            Layout.maximumHeight: 24
            Layout.minimumHeight: 24
            Layout.minimumWidth:  24
            Layout.maximumWidth:  24

            Layout.rightMargin:   4
            Layout.bottomMargin:  7

            color: "transparent"

            Image
            {
                anchors.fill: sendButton

                mipmap:     true
                fillMode:   Image.PreserveAspectFit

                source:     "res/SEND.svg"
                sourceSize: Qt.size(width, height)
            }
            MouseArea
            {
                anchors.fill: parent

                hoverEnabled: true
                cursorShape:  Qt.PointingHandCursor

                onClicked:
                {
                    if (messageInput.text === "")
                    {
                        return
                    }
                    messagesList.appendMessage(messageInput.text)
                    messageInput.clear()
                }
            }
        }
    }
}
