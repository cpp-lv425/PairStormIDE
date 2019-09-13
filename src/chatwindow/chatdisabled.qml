import QtQuick          2.8
import QtQuick.Controls 2.0
import "chatelements/scripts/ChatBase.js" as ChatBase

Item
{
    id: chatBody
    visible: true

    readonly property string userLoginHint: "Unable to use chat"

    Rectangle
    {
        anchors.fill: chatBody
        color: ChatBase.backgroundColor(globalTheme)

        Column
        {
            anchors.centerIn: parent

            Control
            {
                anchors.horizontalCenter: parent.horizontalCenter
                width:  warningText.implicitWidth + 2
                height: 30

                Label
                {
                    id: warningText

                    anchors.centerIn: parent

                    text:  chatBody.userLoginHint
                    color: ChatBase.backgroundTextColor(globalTheme)

                    font.pixelSize: 23
                    font.family:    "Arial"
                    font.bold:      true

                    wrapMode: Label.Wrap
                }
            }

            Image
            {
                id: sorryUserImage

                width:  200
                height: 250

                mipmap:   true
                fillMode: Image.PreserveAspectFit

                source: "chatelements/res/SORRY.svg"
                sourceSize: Qt.size(width, height)
            }
        }
    }
}
