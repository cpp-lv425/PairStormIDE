import QtQuick           2.8
import QtQuick.Templates 2.1 as QmlTemplates

Item
{
    id: switchItem

    // default values for some switch properties
    property color onStateColor:    "green"
    property color offStateColor:   "lightgrey"
    property real  widthToHeight:   1.6
    property int   preferredWidth:  10
    property int   preferredHeight: 10

    states:
    [
        State { name: "off" },
        State { name: "on"  }
    ]

    // readonly resolved properties
    readonly property color offStateBorderColor: Qt.darker(offStateColor, 1.1)
    readonly property color pressedStateColor:   Qt.darker(onStateColor,  1.5)
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

    QmlTemplates.Switch
    {
        id: switchControl

        anchors.centerIn: parent
        implicitWidth:    switchItem.bodyWidth
        implicitHeight:   switchItem.bodyHeight

        down:    false
        checked: switchItem.state == "on"

        indicator: Rectangle
        {
            id: switchHandle

            anchors.verticalCenter: switchControl.verticalCenter

            implicitWidth:  switchItem.bodyWidth
            implicitHeight: switchItem.bodyHeight * 0.9
            radius:         implicitHeight * 3.38

            color:        switchItem.offStateColor
            border.color: switchItem.offStateBorderColor
            border.width: 1

            Rectangle
            {
                id: switchHaft

                anchors.verticalCenter: switchHandle.verticalCenter

                width:  switchItem.bodyHeight * 1.01
                height: width
                radius: width * 3.38

                color:        switchItem.offStateColor
                border.color: switchHandle.border.color
                border.width: 1
            }

            states:
            [
                State
                {
                    name: "off"
                    when: !switchControl.checked && !switchControl.down
                },
                State
                {
                    name: "on"
                    when: switchControl.checked && !switchControl.down

                    PropertyChanges
                    {
                        target: switchHandle

                        color:        switchItem.onStateColor
                        border.color: switchItem.onStateColor
                    }

                    PropertyChanges
                    {
                        target: switchHaft

                        x: parent.width - width * 0.9
                    }
                },
                State
                {
                    name: "off_down"
                    when: !switchControl.checked && switchControl.down

                    PropertyChanges
                    {
                        target: switchHaft

                        color: switchItem.offStateColor
                    }
                },
                State
                {
                    name: "on_down"
                    extend: "off_down"
                    when: switchControl.checked && switchControl.down

                    PropertyChanges
                    {
                        target: switchHaft

                        x:     parent.width - width * 0.9
                        color: switchItem.offStateColor
                    }
                    PropertyChanges
                    {
                        target: switchHandle

                        color:        switchItem.pressedStateColor
                        border.color: switchItem.pressedStateColor
                    }
                }
            ]

            onStateChanged:
            {
                if (state === "on")
                {
                    switchItem.state = "on"
                }
                if (state === "off")
                {
                    switchItem.state = "off"
                }
            }
        }
    }
}
