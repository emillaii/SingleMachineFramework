import QtQuick 2.0
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12
import "../ConfigManager"
import ConfigElementType 1.0

GroupBox{
    property string vcmName: ""
    property string softlandingPosName: ""

    property var __softlandingPosModel: null

    ColumnLayout{
        id: layout

        RowLayout{
            ConfigRow{
                id: sfVel
                configName: "velocity"
            }
            ConfigRow{
                id: sfForce
                configName: "force"
            }
        }
        RowLayout{
            ConfigRow{
                id: sfHoldTime
                configName: "measureHeightHoldTime"
            }
            ConfigRow{
                id: sfTargetPos
                configName: "targetPos"
            }
        }
        Button{
            text: qsTr("MeasureHeight")
            onClicked: {
                __softlandingPosModel.handleEvent("MeasureHeight")
            }
        }
    }
    Component.onCompleted: {
        if(title == ""){
            title = vcmName + "_" + softlandingPosName
        }

        var softlandingPosModel = motionConfigManager.softLandingPos(vcmName, softlandingPosName)
        if(softlandingPosModel === null){
            layout.visible = false
            console.error("Undefined softlanding pos! VCM: " + vcmName + " softlanding pos: " + softlandingPosName)
        }else{
            sfVel.configModel = softlandingPosModel
            sfForce.configModel = softlandingPosModel
            sfHoldTime.configModel = softlandingPosModel
            sfTargetPos.configModel = softlandingPosModel
            __softlandingPosModel = softlandingPosModel
            sfVel.autoInit()
            sfForce.autoInit()
            sfHoldTime.autoInit()
            sfTargetPos.autoInit()
        }
    }
}

