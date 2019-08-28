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
        color: ChatBase.backgroundColor(ChatBase.globalTheme)
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
                                    (ChatBase.golden_ratio * 2)

            RowLayout
            {
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
                                            (ChatBase.golden_ratio * 6)
                    Layout.minimumHeight:   30
                    Layout.maximumHeight:   40
                    Layout.preferredHeight: chatBody.height /
                                            (ChatBase.golden_ratio * 7)
                }
            }
        }
        ChatComponents.MsgsLifeline
        {
            id: messagesHistory

            Layout.alignment:  Qt.AlignCenter
            Layout.fillWidth:  true
            Layout.fillHeight: true
        }
        ChatComponents.MsgInputArea
        {
            id: inputMessageArea

            Layout.alignment:       Qt.AlignCenter
            Layout.fillWidth:       true
            Layout.minimumHeight:   30
            Layout.maximumHeight:   104
            Layout.preferredHeight: inputMessageArea.height
        }
    }
}
