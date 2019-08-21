import QtQuick 2.8
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.3
import QtQml.Models 2.12
import QtGraphicalEffects 1.12

import "scripts/ChatBase.js" as ChatBase
import "basicelements" as BasicElements
import AvailableUsers 1.0


Item {
    id: availableUsers

    Rectangle {
        anchors.fill: parent

        antialiasing: true
        color: "steelblue"

        ListView {
            anchors.fill: parent
            anchors.margins: 0
            anchors.topMargin: 3
            anchors.bottomMargin: 7
            spacing: 0
            clip: true

            highlightFollowsCurrentItem: true
            snapMode: ListView.SnapToItem

            focus: true
            interactive: true
            //flickableDirection: Flickable.VerticalFlick
            boundsBehavior: Flickable.StopAtBounds

            model: availableUsersModel
        }
    }

    DelegateModel {
        id: availableUsersModel
        model: AvailableUsersModel {
            //usersList: AvailableUsersList
        }


        /*
        model: ListModel {
            //@disable-check M16
            ListElement { name: "Vasia nickname and something"; connected: true; }
            //@disable-check M16
            ListElement { name: "Diana nickname"; connected: true; }
            //@disable-check M16
            ListElement { name: "Petia nickname"; connected: false; }
            //@disable-check M16
            ListElement { name: "Bohdan nickname"; online: true; connected: false; }
            //@disable-check M16
            ListElement { name: "Dima nickname"; online: true; connected: false; }
            //@disable-check M16
            ListElement { name: "Roman nickname"; online: true; connected: false; }
        }*/

        delegate: Component {
            Rectangle {
                id: userElement

                width:  parent.width
                height: availableUsers.height / 6
                /*
                height: {
                    var maximumElementsNum = availableUsersModel.count
                    var possibleElementsNum = 1
                    while (possibleElementsNum < maximumElementsNum)
                    {
                        if (availableUsers.height / possibleElementsNum < 25)
                        {
                            return availableUsers.height / possibleElementsNum
                        }
                        possibleElementsNum += 1
                    }
                    return 25
                }*/

                //border.width: height * 0.03
                //border.color:"white"

                color: "lightgrey"
                gradient: Gradient.SolidStone

                RowLayout {
                    anchors.fill: parent

                    Label {
                        Layout.alignment: Qt.AlignCenter
                        Layout.leftMargin: 10
                        Layout.topMargin: 5

                        Layout.fillHeight: true
                        Layout.fillWidth: true

                        text: qsTr(model.name)
                        font.pixelSize: userElement.height / (ChatBase.golden_ratio)
                        font.family: "consolas"

                        color: "lightgrey"

                        clip: true
                        Layout.preferredWidth: 50
                        elide: Text.ElideRight
                    }

                    Control {
                        Layout.alignment: Qt.AlignRight
                        Layout.rightMargin: 10
                        Layout.topMargin: 5
                        Layout.bottomMargin: 5

                        Layout.fillHeight: true
                        Layout.minimumWidth: 40
                        Layout.preferredWidth: userElement.width / (ChatBase.golden_ratio * 5)
                        Layout.maximumWidth: 60

                        BasicElements.Switch {
                            id: userSwitch
                            anchors.fill: parent
                            anchors.centerIn: parent

                            state: model.connected? "on" : "off"

                            preferredHeight: parent.height
                            preferredWidth:  parent.width
                            widthToHeight:   ChatBase.golden_ratio//1.618034
                            onStateChanged: {
                                console.log("" + model.name + " has been turned " + state)
                                if (state === "on")
                                {
                                    model.connected = true
                                }
                                if (state === "off")
                                {
                                    model.connected = false
                                }

                                //model.connected = !model.connected
                                //AvailableUsersList.connectToUserOnClick()
                            }
                        }
                    }
                }
            }
        }
    }
}
