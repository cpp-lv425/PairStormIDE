import QtQuick 2.8
import "scripts/ChatBase.js" as ChatBase

Item
{
    id: controlButtons

    Image
    {
        id: chatSettingsImage

        anchors.centerIn: parent
        width:  parent.width  / ChatBase.goldenRatio
        height: parent.height / ChatBase.goldenRatio

        mipmap:   true
        fillMode: Image.PreserveAspectFit

        source: "res/CHATCTRL.svg"
        sourceSize: Qt.size(width, height)
    }
    MouseArea
    {
        anchors.fill: chatSettingsImage

        enabled:     false
        cursorShape: Qt.PointingHandCursor
    }
}
