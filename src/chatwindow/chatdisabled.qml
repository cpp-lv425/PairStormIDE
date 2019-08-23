import QtQuick 2.8
import QtQuick.Controls 2.0

Item {
    id: chatBody
    visible: true
    Rectangle {
        anchors.fill: chatBody
        color: "#5a8db8"

        Column {
            anchors.centerIn: parent
            //width: unsatisfiedUserImage.width
            //spacing: 10
            Control {
                anchors.horizontalCenter: parent.horizontalCenter
                //anchors.bottom: unsatisfiedUserImage.top
                width: warningText.implicitWidth + 2

                height: 30
                Label {
                    id: warningText
                    text: qsTr("Log in to use chat")
                    font.pixelSize: 23
                    font.family: "arial"
                    font.bold: true
                    wrapMode: Label.Wrap
                }

            }

            Image {
                id: unsatisfiedUserImage
                //anchors.verticalCenter: parent.verticalCenter

                width: 200
                height: 300

                mipmap: true

                sourceSize: Qt.size(width, height)

                fillMode: Image.PreserveAspectFit
                source: "chatelements/res/SORRY.svg"
            }
        }
    }
}
