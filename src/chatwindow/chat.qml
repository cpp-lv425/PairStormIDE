import QtQuick          2.8
import QtQuick.Layouts  1.3
import QtQuick.Controls 2.0
import "chatelements"                     as ChatComponents
import "chatelements/scripts/ChatBase.js" as ChatBase

Item
{
    id: chatBody
    visible: true

    Rectangle
    {
        anchors.fill: chatBody
        color: ChatBase.backgroundColor(globalTheme)
    }
    ColumnLayout
    {
        id: chatBodyLayout

        anchors.fill: chatBody
        spacing: 0

        Control
        {
            Layout.alignment:       Qt.AlignCenter
            Layout.fillWidth:       true
            Layout.minimumHeight:   40
            Layout.maximumHeight:   130
            Layout.preferredHeight: chatBody.height /
                                    (ChatBase.goldenRatio * 2)

            RowLayout
            {
                z: 2
                id: usersAndControlLayout

                width:  parent.availableWidth
                height: parent.availableHeight
                spacing: 1

                ChatComponents.AvailUsers
                {
                    id: availableUsers

                    Layout.alignment:  Qt.AlignCenter
                    Layout.fillWidth:  true
                    Layout.fillHeight: true
                }
                ChatComponents.ChatControls
                {
                    id: controlButtons

                    Layout.alignment:       Qt.AlignTop
                    Layout.minimumWidth:    30
                    Layout.maximumWidth:    40
                    Layout.preferredWidth:  chatBody.width /
                                            (ChatBase.goldenRatio * 6)
                    Layout.minimumHeight:   30
                    Layout.maximumHeight:   40
                    Layout.preferredHeight: chatBody.height /
                                            (ChatBase.goldenRatio * 7)
                }
            }
        }
        ChatComponents.MsgsLifeline
        {
            id: messagesHistory
            z: 1

            Layout.alignment:  Qt.AlignCenter
            Layout.fillWidth:  true
            Layout.fillHeight: true
        }
        ChatComponents.MsgInputArea
        {
            id: inputMessageArea
            z: 2

            Layout.alignment:       Qt.AlignCenter
            Layout.fillWidth:       true
            Layout.minimumHeight:   30
            Layout.maximumHeight:   104
            Layout.preferredHeight: inputMessageArea.height
        }
    }
}
