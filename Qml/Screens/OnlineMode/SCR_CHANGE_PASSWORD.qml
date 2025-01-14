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
import HMIEVENTS 1.0
import EWARNING 1.0
import DRACO_CODE 1.0
import "../../Components/origins"
import "../../Components/customizes"
import "../../Components/customizes/Texts"
import "../../Components/customizes/Buttons"
import "../../../localization/STR_QML.js" as STR

QScreen {
    QOnScreenContent {
        width: popupWidth
        height: popupHeight
        anchors.centerIn: parent
        label.text: STR.STR_QML_516
        onCloseClicked: {
            QMLHandle.sendEvent(EVT.EVT_ONS_CLOSE_REQUEST, EVT.STATE_ID_SCR_CHANGE_PASSWORD)
        }
        Column{
            anchors{
                top: parent.top
                topMargin: 100
                left: parent.left
                leftMargin: 36
            }
            spacing: 24
            Rectangle {
                width: 539
                height: 80
                color: "#EAEAEA"
                radius: 8
                Row{
                    anchors.centerIn: parent
                    width: 515
                    height: 56
                    spacing: 8
                    QIcon {
                        iconSize: 36
                        anchors.verticalCenter: parent.verticalCenter
                        source: "qrc:/Images/Images/info-60px.svg"
                    }
                    QText {
                        width: 471
                        height: 56
                        text: STR.STR_QML_400.arg(Draco.emailRequested)
                        font.family: "Lato"
                        font.pixelSize: 16
                        color: "#031F2B"
                        wrapMode: Text.WordWrap
                        verticalAlignment: Text.AlignVCenter
                        horizontalAlignment: Text.AlignLeft
                        anchors.verticalCenter: parent.verticalCenter
                    }
                }
            }
            Column{
                spacing: 16
                QTextInputBoxTypeB {
                    id: temporary
                    label: STR.STR_QML_401
                    isPassword: true
                    boxWidth: 539
                    boxHeight: 48
                    isValid: true
                    onTextInputtedChanged: {
                        if(!temporary.isValid){
                            temporary.isValid = true
                            temporary.errorText = ""
                        }
                        temporary.showError = false;
                    }
                }
                QTextInputBoxTypeB {
                    id: new_pass
                    label: STR.STR_QML_402
                    isPassword: true
                    boxWidth: 539
                    boxHeight: 48
                    isValid: true
                    onTextInputtedChanged: {
                        if(!new_pass.isValid){
                            new_pass.isValid = true
                            new_pass.errorText = ""
                        }
                        new_pass.showError = false;
                    }
                }
                QTextInputBoxTypeB {
                    id: confirm_pass
                    label: STR.STR_QML_403
                    isPassword: true
                    boxWidth: 539
                    boxHeight: 48
                    isValid: true
                    onTextInputtedChanged: {
                        if(!confirm_pass.isValid){
                            confirm_pass.isValid = true
                            confirm_pass.errorText = ""
                        }
                        confirm_pass.showError = false;
                    }
                }
                QText {
                    id: commonError
                    width: 539
                    height: 48
                    font.family: "Lato"
                    font.pixelSize: 16
                    color: "#CF4018"
                    visible: false
                    wrapMode: Text.WordWrap
                }
            }
        }
        QButtonTextLink {
            height: 24
            label: STR.STR_QML_059
            anchors {
                left: parent.left
                leftMargin: 40
                bottom: parent.bottom
                bottomMargin: 40
            }
            onButtonClicked: {
                QMLHandle.sendEvent(EVT.EVT_CHANGE_PASSWORD_BACK)
            }
        }

        QTextButton {
            width: 200
            height: 48
            label.text: STR.STR_QML_405
            label.font.pixelSize: 16
            type: eTypeE
            anchors {
                right: parent.right
                rightMargin: 40
                bottom: parent.bottom
                bottomMargin: 32
            }
            onButtonClicked: {
                if(new_pass.textInputted === temporary.textInputted){
                    new_pass.isValid = false
                    temporary.isValid = false
                    if(new_pass.textInputted === ""){
                        new_pass.errorText = STR.STR_QML_396
                        temporary.errorText = STR.STR_QML_396
                    }
                    else{
                        temporary.errorText = STR.STR_QML_406
                    }
                }
                else{
                    var requestBody = {
                        "oldPassword" : temporary.textInputted,
                        "newPassword" : new_pass.textInputted,
                    };
                    QMLHandle.sendEvent(EVT.EVT_CHANGE_PASSWORD_REQUEST, requestBody)
                }
            }
        }
    }
    function processingChangePasswordResult(https_code, error_code, error_msg){
        if(https_code === DRACO_CODE.SUCCESSFULL && error_code === DRACO_CODE.RESPONSE_OK){
            QMLHandle.sendEvent(EVT.EVT_ONS_CLOSE_REQUEST, EVT.STATE_ID_SCR_CHANGE_PASSWORD)
        }
        else{
            commonError.visible = true
            commonError.text = qsTr("https_code[%1] error_code[%2] error_msg[%3], %4").arg(https_code).arg(error_code).arg(error_msg).arg(STR.STR_QML_407)
        }
    }
    Connections {
        target: Draco
        onChangePasswordResult: {
            processingChangePasswordResult(https_code, error_code, error_msg)
        }
    }
}
