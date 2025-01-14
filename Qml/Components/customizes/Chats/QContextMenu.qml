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
import QtQuick 2.0
import QtQuick.Controls 2.3
import QtQuick.Controls.Styles 1.4
import QtGraphicalEffects 1.12
import "../../origins"
import "../../customizes/Texts"
import "../../customizes/Buttons"

Menu {
    id: optionMenu
    implicitWidth: menuWidth
    implicitHeight: {
        var cnt = 0;
        for(var i=0; i< labels.length; i++){
            var show = visibles.length > i ? visibles[i] : true
            if(show){cnt++}
        }
        return cnt*menuHeight
    }
    property int menuWidth: 250
    property int menuHeight: 48
    property var functions:[]
    property var colors:[]
    property var labels: [""]
    property var icons: [""]
    property var enables: [true]
    property var visibles: [true]
    signal itemClicked(var index)
    background: Rectangle {
        implicitWidth: menuWidth
        implicitHeight: optionMenu.labels.length*menuHeight
        radius: 8
        color: "#FFFFFF"
        layer.enabled: true
        layer.effect: DropShadow {
            horizontalOffset: 3
            verticalOffset: 3
            radius: 8.0
            samples: 17
            color: "#80000000"
        }
    }
    Repeater {
        model: labels
        MenuItem {
            id: delegateMenu
            height: visible ? menuHeight : 0
            text: optionMenu.labels.length > index ? optionMenu.labels[index] : ""
            icon.source: optionMenu.icons.length > index ? optionMenu.icons[index] : ""
            enabled: optionMenu.enables.length > index ? optionMenu.enables[index] : true
            visible: optionMenu.visibles.length > index ? optionMenu.visibles[index] : true
            onTriggered: {
                var cacheEnable = delegateMenu.enabled
                delegateMenu.enabled = false
                itemClicked(index)
                delegateMenu.enabled = cacheEnable
            }
            background: Rectangle {
                implicitWidth: menuWidth
                implicitHeight: menuHeight
                radius: 8
                opacity: delegateMenu.enabled ? 1 : 0.7
                color: {
                    if(delegateMenu.enabled){ return delegateMenu.hovered ? "#F5F5F5": "#FFFFFF" }
                    else{ return "#EAEAEA" }
                }
            }
            contentItem: Item {
                width: menuWidth
                height: menuHeight
                QIcon {
                    id:_icon
                    iconSize: _icon.source == "" ? 0 : 24
                    anchors.left: parent.left
                    anchors.leftMargin: 12
                    anchors.verticalCenter: parent.verticalCenter
                    source: optionMenu.icons.length > index ? optionMenu.icons[index] : ""
                    opacity: delegateMenu.enabled ? 1.0 : 0.7
                }
                QText {
                    text: optionMenu.labels.length > index ? optionMenu.labels[index] : ""
                    color: {
                        if(!delegateMenu.enabled){ return "#595959" }
                        return (optionMenu.colors.length > index ? optionMenu.colors[index] : "#031F2B")
                    }
                    width: paintedWidth
                    anchors.left: _icon.right
                    anchors.leftMargin: _icon.source == "" ? 0 : 11
                    font.family: "Lato"
                    font.weight: Font.Normal
                    font.pixelSize: 16
                    anchors.verticalCenter: parent.verticalCenter
                }
            }
        }
    }
}
