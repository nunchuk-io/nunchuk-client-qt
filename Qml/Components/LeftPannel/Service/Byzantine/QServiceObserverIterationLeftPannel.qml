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
import QtQuick 2.12
import "../../../origins"
import "../../../customizes"
import "../../../customizes/Chats"
import "../../../customizes/Texts"
import "../../../customizes/Buttons"
import "../../../../../localization/STR_QML.js" as STR

Item {
    Item {
        width: 289
        height: 232
        anchors.centerIn: parent
        Column {
            anchors.fill: parent
            spacing: 16
            Rectangle {
                width: 96
                height: 96
                anchors.horizontalCenter: parent.horizontalCenter
                color: "#F5F5F5"
                radius: 48
                QIcon {
                    iconSize: 60
                    anchors.centerIn: parent
                    source: "qrc:/Images/Images/services-dark.svg"
                }
            }
            QText {
                width: parent.width
                anchors.horizontalCenter: parent.horizontalCenter
                text: STR.STR_QML_1022
                color: "#FFFFFF"
                font.family: "Lato"
                font.pixelSize: 16
                font.weight: Font.Normal
                wrapMode: Text.WordWrap
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
            }
            QIconTextButton {
                width: 289
                height: 48
                type: eTypeG
                label: STR.STR_QML_701
                anchors.horizontalCenter: parent.horizontalCenter
                icons: ["contact-support-dark.svg", "contact-support-light.svg", "contact-support-dark.svg", "contact-support-light.svg"]
                fontPixelSize: 16
                enabled: ClientController.isMatrixLoggedIn && ClientController.readySupport
                onButtonClicked: {
                    QMLHandle.sendEvent(EVT.EVT_SERVICE_SUPPORT_REQUEST)
                }
            }
        }
    }
}
