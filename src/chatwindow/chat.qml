import QtQuick 2.7
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.3
import "chatelements/scripts/ChatBase.js" as ChatBase
import "chatelements" as ChatElements

Item {
    id: chatBody
    visible: true

    ColumnLayout {
        id: chatBodyLayout
        anchors.fill: chatBody

        spacing: 0

        Control {
            Layout.alignment: Qt.AlignCenter
            Layout.fillWidth: true
            Layout.minimumHeight: 40
            Layout.preferredHeight:
                chatBody.height / (ChatBase.golden_ratio * 2)
            Layout.maximumHeight: 130

            RowLayout {
                id: usersAndControlLayout

                width:  parent.availableWidth
                height: parent.availableHeight

                spacing: 1

                ChatElements.AvailUsers {
                    id: availableUsers

                    Layout.alignment: Qt.AlignCenter
                    Layout.fillHeight: true
                    Layout.fillWidth:  true
                }

                ChatElements.ChatControls {
                    id: controlButtons

                    Layout.alignment: Qt.AlignTop

                    Layout.minimumHeight: 30
                    Layout.preferredHeight:
                        chatBody.height / (ChatBase.golden_ratio * 7)
                    Layout.maximumHeight: 40

                    Layout.minimumWidth: 30
                    Layout.preferredWidth:
                        chatBody.width / (ChatBase.golden_ratio * 6)
                    Layout.maximumWidth: 40
                }
            }
        }

        ChatElements.MsgsLifeline {
            id: messagesHistory

            Layout.alignment: Qt.AlignCenter
            Layout.fillWidth:  true
            Layout.fillHeight: true
        }

        ChatElements.MsgInputArea {
            id: inputMessageArea

            Layout.alignment: Qt.AlignCenter
            Layout.fillWidth:  true
            Layout.minimumHeight: 30
            Layout.preferredHeight: inputMessageArea.height
            Layout.maximumHeight: 104
        }
    }
}
