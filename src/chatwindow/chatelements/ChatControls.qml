import QtQuick 2.8
import "scripts/ChatBase.js" as ChatBase

Item
{
    Rectangle
    {
        anchors.fill: parent
        color: ChatBase.chatInputBackgroundColor(ChatBase.globalTheme)
    }
}
