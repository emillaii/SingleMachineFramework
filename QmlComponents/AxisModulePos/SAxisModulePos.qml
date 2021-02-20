import QtQuick 2.0
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12
import "../ConfigManager"
import ConfigElementType 1.0

GroupBox{
    property string moduleName: ""
    property string posName: ""

    RowLayout{
        ConfigRow{
            id: posViewer
            configName: "pos"
            txtWidth: 90
        }

        ModulePosEvent{
            id: btns
        }
    }
    Component.onCompleted: {
        if(title == ""){
            title = moduleName + "_" + posName
        }

        var modulePosModel = motionConfigManager.sAxisModulePos(moduleName, posName)
        if(modulePosModel === null){
            posViewer.visible = false
            btns.visible = false
            console.error("Undefined module pos! module: " + moduleName + " pos: " + posName)
        }else{
            posViewer.configModel = modulePosModel
            btns.configObj = modulePosModel
            posViewer.init(ConfigElementType.Double)
        }
    }
}

