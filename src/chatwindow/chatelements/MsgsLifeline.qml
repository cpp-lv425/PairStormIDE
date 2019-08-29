import QtQuick          2.8
import QtQml.Models     2.12
import PairStormChat    1.0
import QtQuick.Layouts  1.3
import QtQuick.Controls 2.0
import "basicelements"       as BasicElements
import "scripts/ChatBase.js" as ChatBase

Item
{
    id: messagesHistory

    Rectangle
    {
        anchors.fill: parent
        color: ChatBase.backgroundColor(globalTheme)
    }
    ListView
    {
        id: listView

        anchors.fill: parent
        anchors.margins: 0
        spacing:         5

        clip:                        true
        highlightMoveDuration:       300
        highlightFollowsCurrentItem: true
        boundsBehavior:              Flickable.StopAtBounds

        onCountChanged:
        {
            Qt.callLater( listView.positionViewAtEnd )
        }
        onHeightChanged:
        {
            listView.positionViewAtEnd()
        }

        model: messagesModel
    }
    DelegateModel
    {
        id: messagesModel

        model: MessagesModel
        {
            list: messagesList
        }

        delegate: Column
        {
            id: messageAttributesColumn

            readonly property bool adjustRight: model.authorName === globalUserName

            anchors.left:  messageAttributesColumn.adjustRight ?
                               undefined    : parent.left
            anchors.right: messageAttributesColumn.adjustRight ?
                               parent.right : undefined

            Row
            {
                id: messageAttributesRow

                anchors.left:  messageAttributesColumn.adjustRight ?
                                   undefined    : parent.left
                anchors.right: messageAttributesColumn.adjustRight ?
                                   parent.right : undefined

                Control
                {
                    id: leftToothShape

                    property string qml: messageAttributesColumn.adjustRight ?
                                             "" : "basicelements/ToothShapeToLeft.qml"

                    anchors.bottom: messageBody.bottom
                    width:  messageAttributesColumn.adjustRight ? 0 : 10
                    height: messageAttributesColumn.adjustRight ? 0 : 15

                    Loader
                    {
                        anchors.fill: parent
                        Component.onCompleted:
                        {
                            setSource(leftToothShape.qml,
                                      {
                                          itemColor:  messageBody.color,
                                          leftOffset: 16,
                                          maxHeight:  20
                                      })
                        }
                    }
                }

                Rectangle
                {
                    id: messageBody

                    width:
                    {
                        return Math.min(Math.max(messageText.implicitWidth,
                                                 messageAuthor.contentWidth) + 24,
                                        messagesHistory.width - 70)
                    }
                    height: messageText.implicitHeight + 27
                    radius: 10

                    color:
                    {
                        if (model.type === "ordinary")
                        {
                            return messageAttributesColumn.adjustRight                   ?
                                   ChatBase.chatAuthorMessageColor(globalTheme) :
                                   ChatBase.chatUserMessageColor(globalTheme)
                        }
                        if (model.type === "service")
                        {
                            return ChatBase.chatSystemMessageColor(globalTheme)
                        }
                    }

                    TextEdit
                    {
                        id: messageText

                        anchors.fill: parent
                        anchors.margins:   10
                        anchors.topMargin: 23

                        selectByMouse: true
                        readOnly:      true
                        wrapMode:      Label.Wrap

                        text: model.content
                        color:          ChatBase.chatMessageTextColor(globalTheme)
                        selectionColor: Qt.darker(messageBody.color, 1.5)

                        MouseArea
                        {
                            anchors.fill: parent

                            enabled:     false
                            cursorShape: Qt.IBeamCursor
                        }
                    }
                    RowLayout
                    {
                        anchors.fill: parent
                        anchors.left: messageAttributesColumn.adjustRight ?
                                           undefined : messageBody.left
                        anchors.right: messageAttributesColumn.adjustRight ?
                                           messageBody.right : undefined
                        anchors.leftMargin:  10
                        anchors.rightMargin: 15

                        Label
                        {
                            id: messageAuthor

                            Layout.alignment: messageAttributesColumn.adjustRight ?
                                                  Qt.AlignRight : Qt.AlignLeft
                            Layout.fillHeight: true
                            Layout.topMargin: 4

                            Layout.preferredWidth:
                            {
                                return Math.min(messageBody.width - 10,
                                                messageAuthor.implicitWidth)
                            }
                            text:  qsTr("  " + model.authorName)
                            clip:  true
                            elide: Text.ElideRight
                            color: ChatBase.chatMessageTextColor(globalTheme)

                            font.pixelSize: 15
                            font.family:    "Consolas"
                            font.bold:      true

                            Image
                            {
                                width:  13
                                height: 13

                                mipmap:   true
                                fillMode: Image.PreserveAspectFit

                                source:
                                {
                                    if (model.type === "ordinary")
                                    {
                                        return "res/USER.svg"
                                    }
                                    if (model.type === "service")
                                    {
                                        return "res/SYSTEM.svg"
                                    }
                                }
                                sourceSize: Qt.size(width, height)
                            }
                        }
                    }
                }
                Control
                {
                    id: rightToothShape

                    property string qml: messageAttributesColumn.adjustRight ?
                                             "basicelements/ToothShapeToRight.qml" : ""

                    anchors.bottom: messageBody.bottom
                    width:  messageAttributesColumn.adjustRight ? 10 : 0
                    height: messageAttributesColumn.adjustRight ? 15 : 0

                    Loader
                    {
                        anchors.fill: parent
                        Component.onCompleted:
                        {
                            setSource(rightToothShape.qml,
                                      {
                                          itemColor:  messageBody.color,
                                          leftOffset: 16,
                                          maxHeight:  20
                                      })
                        }
                    }
                }
            }
            Label
            {
                id: timestampText

                anchors.left:  messageAttributesColumn.adjustRight ?
                                   undefined    : parent.left
                anchors.right: messageAttributesColumn.adjustRight ?
                                   parent.right : undefined
                anchors.leftMargin:  5
                anchors.rightMargin: 5

                text:  Qt.formatDateTime(model.publicationDateTime, "d MMM hh:mm")
                color: ChatBase.backgroundTextColor(globalTheme)

                font.pixelSize: 15
                font.family:    "Consolas"
                font.bold:      true
            }
        }
    }
}

