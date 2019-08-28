import QtQuick          2.8
import QtQml.Models     2.12
import QtQuick.Layouts  1.3
import QtQuick.Controls 2.0
import "basicelements"       as BasicElements
import "scripts/ChatBase.js" as ChatBase
// TODO: import pair storm chat

Item
{
    id: availableUsers

    Rectangle
    {
        anchors.fill: parent

        color: ChatBase.backgroundColor(ChatBase.globalTheme)

        ListView
        {
            anchors.fill: parent
            anchors.margins:      0
            anchors.topMargin:    3
            anchors.bottomMargin: 7
            spacing:              0

            clip:                        true
            focus:                       true
            interactive:                 true
            highlightFollowsCurrentItem: true
            snapMode:                    ListView.SnapToItem
            boundsBehavior:              Flickable.StopAtBounds

            model: availableUsersModel
        }
    }
    DelegateModel
    {
        id: availableUsersModel

        // TODO: replace model
        model: ListModel {
            //@disable-check M16
            ListElement { userName: "Vasia nickname and something"; isUserConnected: true; }
            //@disable-check M16
            ListElement { userName: "Diana nickname"; isUserConnected: true; }
            //@disable-check M16
            ListElement { userName: "Petia nickname"; isUserConnected: false; }
            //@disable-check M16
            ListElement { userName: "Bohdan nickname"; isUserConnected: false; }
            //@disable-check M16
            ListElement { userName: "Dima nickname"; isUserConnected: false; }
            //@disable-check M16
            ListElement { userName: "Roman nickname"; isUserConnected: false; }
        }

        delegate: Component
        {
            Rectangle
            {
                id: userElement

                width:  parent.width
                height: availableUsers.height / 6

                color: ChatBase.chatUsersBackgroundColor(ChatBase.globalTheme)

                RowLayout
                {
                    anchors.fill: parent

                    Label
                    {
                        Layout.alignment:  Qt.AlignCenter
                        Layout.topMargin:  5
                        Layout.leftMargin: 15

                        Layout.fillHeight: true
                        Layout.fillWidth:  true

                        Layout.preferredWidth: 50
                        text:  qsTr(model.userName)
                        elide: Text.ElideRight
                        clip:  true

                        font.pixelSize: userElement.height /
                                        (ChatBase.golden_ratio)
                        font.family:    "Consolas"

                        color: ChatBase.chatUsersTextColor(ChatBase.globalTheme)
                    }
                    Control
                    {
                        Layout.alignment:    Qt.AlignRight
                        Layout.topMargin:    5
                        Layout.rightMargin:  0
                        Layout.bottomMargin: 5

                        Layout.fillHeight: true

                        Layout.minimumWidth:   40
                        Layout.maximumWidth:   60
                        Layout.preferredWidth: userElement.width /
                                               (ChatBase.golden_ratio * 5)

                        BasicElements.Switch
                        {
                            id: userSharingSwitch

                            anchors.fill:    parent

                            onStateColor:    ChatBase.switchTurnedOnColor(ChatBase.globalTheme)
                            offStateColor:   ChatBase.switchTurnedOffColor(ChatBase.globalTheme)
                            widthToHeight:   ChatBase.golden_ratio
                            preferredWidth:  parent.width
                            preferredHeight: parent.height

                            state: model.isUserConnected?
                                       "on" : "off"

                            onStateChanged:
                            {
                                switch(state)
                                {
                                case "on":
                                    model.isUserConnected = true;
                                    break;
                                case "off":
                                    model.isUserConnected = false;
                                }
                            }
                        }
                    }
                }
                Rectangle
                {
                    anchors.left:   userElement.left
                    anchors.bottom: userElement.bottom
                    anchors.leftMargin: 9

                    width:  userElement.width - 18
                    height: 1
                    radius: 1

                    color: ChatBase.chatUsersSpacerColor(ChatBase.globalTheme)
                }
            }
        }
    }
}
