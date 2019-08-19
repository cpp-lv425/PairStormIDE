import QtQuick 2.8
import QtQuick.Templates 2.1 as T

Item {
    id: switchItem
    property int  preferredWidth:  10
    property int  preferredHeight: 10
    property real widthToHeight:   1.6

    readonly property real bodyWidth:
    {
        if (switchItem.preferredHeight > switchItem.preferredWidth / widthToHeight)
        {
            return switchItem.preferredWidth
        }
        else
        {
            return switchItem.preferredHeight * widthToHeight
        }
    }
    readonly property real bodyHeight:
    {
        if (switchItem.preferredHeight < switchItem.preferredWidth / widthToHeight)
        {
            return switchItem.preferredHeight
        }
        else
        {
            return switchItem.preferredWidth / widthToHeight
        }
    }
    readonly property color gray: "#b2b1b1"
    readonly property color lightGray: "#dddddd"
    readonly property color light: "#ffffff"
    readonly property color blue: "#2d548b"
    property color mainColor: "#24d292"
    readonly property color dark: "#222222"
    readonly property color mainColorDarker: Qt.darker(mainColor, 1.5)

    states: [
        State { name: "off" },
        State { name: "on"  }
    ]

    T.Switch {
        id: switchControl

        anchors.centerIn: parent
        implicitWidth:   switchItem.bodyWidth
        implicitHeight:  switchItem.bodyHeight

        indicator: Rectangle {
            id: switchHandle

            anchors.verticalCenter: switchControl.verticalCenter
            implicitWidth:  switchItem.bodyWidth
            implicitHeight: switchItem.bodyHeight * 0.9
            radius:         implicitHeight * 3.38

            color: switchItem.light
            border.width: 1
            border.color: switchItem.lightGray

            Rectangle {
                id: switchHaft

                anchors.verticalCenter: switchHandle.verticalCenter
                width:  switchItem.bodyHeight * 1.1
                height: width
                radius: width * 3.38

                color: switchItem.light
                border.width: 2
                border.color: switchItem.gray
            }

            states: [
                State {
                    name: "off"
                    when: !switchControl.checked && !switchControl.down
                },
                State {
                    name: "on"
                    when: switchControl.checked && !switchControl.down

                    PropertyChanges {
                        target: switchHandle
                        color: switchItem.mainColor
                        border.color: switchItem.mainColor
                    }

                    PropertyChanges {
                        target: switchHaft
                        x: parent.width - width * 0.9
                    }
                },
                State {
                    name: "off_down"
                    when: !switchControl.checked && switchControl.down

                    PropertyChanges {
                        target: switchHaft
                        color: switchItem.light
                    }
                },
                State {
                    name: "on_down"
                    extend: "off_down"
                    when: switchControl.checked && switchControl.down

                    PropertyChanges {
                        target: switchHaft
                        x: parent.width - width * 0.9
                        color: switchItem.light
                    }
                    PropertyChanges {
                        target: switchHandle
                        color: switchItem.mainColorDarker
                        border.color: switchItem.mainColorDarker
                    }
                }
            ]

            onStateChanged: {
                if (state === "on")
                    switchItem.state = "on"
                if (state === "off")
                    switchItem.state = "off"
            }
        }
    }
}
