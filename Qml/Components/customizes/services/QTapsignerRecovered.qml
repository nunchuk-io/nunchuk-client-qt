/**************************************************************************
 * This file is part of the Nunchuk software (https://nunchuk.io/)        *
 * Copyright (C) 2020-2022 Enigmo								          *
 * Copyright (C) 2022 Nunchuk								              *
 *                                                                        *
 * This program is free software; you can redistribute it and/or          *
 * modify it under the terms of the GNU General Public License            *
 * as published by the Free Software Foundation; either version 3         *
 * of the License, or (at your option) any later version.                 *
 *                                                                        *
 * This program is distributed in the hope that it will be useful,        *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of         *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the          *
 * GNU General Public License for more details.                           *
 *                                                                        *
 * You should have received a copy of the GNU General Public License      *
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.  *
 *                                                                        *
 **************************************************************************/
import QtQuick 2.4
import QtQuick.Controls 2.3
import QtGraphicalEffects 1.12
import Qt.labs.platform 1.1
import HMIEVENTS 1.0
import EWARNING 1.0
import NUNCHUCKTYPE 1.0
import DataPool 1.0
import "../../origins"
import "../../customizes"
import "../../customizes/Texts"
import "../../customizes/Buttons"
import "../../../../localization/STR_QML.js" as STR

QOnScreenContent {
    id: _content
    width: popupWidth
    height: popupHeight
    anchors.centerIn: parent
    label.text: ""
    onCloseClicked: closeTo(NUNCHUCKTYPE.SERVICE_TAB)
    signal signalGotIt()
    Item {
        width: 539
        height: 344
        anchors {
            top:parent.top
            topMargin: 36
            left: parent.left
            leftMargin: 36
        }
        Column {
            width: parent.width
            spacing: 24
            Rectangle {
                width: 96;height: 96;
                radius: 48
                color: "#A7F0BA"
                QIcon {
                    iconSize: 60
                    anchors.centerIn: parent
                    source: "qrc:/Images/Images/check-dark.svg"
                }
            }
            QText {
                width: 500
                height: 40
                text: STR.STR_QML_730
                color: "#031F2B"
                font.family: "Lato"
                font.pixelSize: 32
                font.weight: Font.Bold
                horizontalAlignment: Text.AlignLeft
                verticalAlignment: Text.AlignVCenter
            }
            Repeater {
                id: _description
                width: parent.width
                readonly property var content_map: [
                    {width: 500, height: 56, horizontal: Text.AlignLeft,    title:STR.STR_QML_731, icon: ""   },
                    {width: 464, height: 28, horizontal: Text.AlignLeft,    title:STR.STR_QML_732, icon: "qrc:/Images/Images/1.Active.svg"   },
                    {width: 464, height: 28, horizontal: Text.AlignLeft,    title:STR.STR_QML_733, icon: "qrc:/Images/Images/2.Active.svg"   },
                ]
                model: content_map.length
                Rectangle {
                    id: _rect
                    width: _description.content_map[index].width
                    height: _description.content_map[index].height
                    Row {
                        spacing: 8
                        QIcon {
                            iconSize: 24
                            id:_numicon
                            source: _description.content_map[index].icon
                            visible: source != ""
                        }
                        QText {
                            id:_numText
                            width: _rect.width - _numicon.width
                            height: _description.content_map[index].height
                            anchors.verticalCenter: parent.verticalCenter
                            text: _description.content_map[index].title
                            color: "#031F2B"
                            font.family: "Lato"
                            font.pixelSize: 16
                            lineHeightMode: Text.FixedHeight
                            lineHeight: 28
                            horizontalAlignment: _description.content_map[index].horizontal
                            verticalAlignment: Text.AlignVCenter
                        }
                    }
                }
            }
        }
    }

    Rectangle {
        width: 718
        height: 80
        radius: 8
        color: "#FDEBD2"
        anchors {
            horizontalCenter: parent.horizontalCenter
            bottom: parent.bottom
            bottomMargin: 120
        }

        Row {
            anchors.fill: parent
            anchors.margins: 15
            spacing: 11
            QIcon {
                iconSize: 30
                anchors.verticalCenter: parent.verticalCenter
                source: "qrc:/Images/Images/warning-dark.svg"
            }
            QText {
                width: 650
                height: 28
                anchors.verticalCenter: parent.verticalCenter
                text: STR.STR_QML_734
                color: "#031F2B"
                font.family: "Lato"
                font.pixelSize: 16
                horizontalAlignment: Text.AlignLeft
                verticalAlignment: Text.AlignVCenter
            }
        }
    }

    QTextButton {
        width: 73
        height: 48
        label.text: STR.STR_QML_341
        label.font.pixelSize: 16
        type: eTypeE
        anchors {
            right: parent.right
            rightMargin: 36
            bottom: parent.bottom
            bottomMargin: 36
        }
        onButtonClicked: {
            signalGotIt()
        }
    }
}
