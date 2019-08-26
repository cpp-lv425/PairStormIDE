import QtQuick 2.8
import QtQuick.Controls 2.0

Item {
    id: chatBody
    visible: true
    Rectangle {
        anchors.fill: chatBody
        color: {
            if (globalTheme === "white")
            {
                return "#bde1ff"
            }
            if (globalTheme === "blue")
            {
                return "#2c8dde"
            }
            if (globalTheme === "dark")
            {
                return "#41607a"
            }
        }

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
                    color: {
                        if (globalTheme === "white")
                        {
                            return "#6b3636"
                        }
                        if (globalTheme === "blue")
                        {
                            return "#6b3636"
                        }
                        if (globalTheme === "dark")
                        {
                            return "#fad2d2"
                        }
                    }
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
