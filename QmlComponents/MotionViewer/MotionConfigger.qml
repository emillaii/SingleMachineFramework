import QtQuick 2.0
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12
import "../CustomizedUIElement"
import "../ConfigManager"

Item {
    property alias lblTip: lblTip
    property var maxListHeight: 700

    Component{
        id: diConfigger
        ArrayEditor{
            titleVisible: false
            configModel: diConfigs
            listMaxHeight: maxListHeight
            arrayItemAddRemovable: false
            Component.onCompleted: {
                init()
            }
        }
    }

    Component{
        id: doConfigger
        ArrayEditor{
            titleVisible: false
            configModel: doConfigs
            listMaxHeight: maxListHeight
            arrayItemAddRemovable: false
            Component.onCompleted: {
                init()
            }
        }
    }

    Component{
        id: cylConfigger
        ArrayEditor{
            titleVisible: false
            configModel: cylConfigs
            listMaxHeight: maxListHeight
            arrayItemAddRemovable: false
            Component.onCompleted: {
                init()
            }
        }
    }

    Component{
        id: vacuumConfigger
        ArrayEditor{
            titleVisible: false
            configModel: vacuumConfigs
            listMaxHeight: maxListHeight
            arrayItemAddRemovable: false
            Component.onCompleted: {
                init()
            }
        }
    }

    Component{
        id: axisConfigger
        ArrayEditor{
            titleVisible: false
            configModel: axisConfigs
            listMaxHeight: maxListHeight
            arrayItemAddRemovable: false
            Component.onCompleted: {
                init()
            }
        }
    }

    Component{
        id: vcmConfigger
        ArrayEditor{
            titleVisible: false
            configModel: vcmConfigs
            listMaxHeight: maxListHeight
            listMaxWidth: parent.parent.width
            arrayItemAddRemovable: false
            Component.onCompleted: {
                init()
            }
        }
    }

    Component{
        id: extendedAxisConfigger
        ArrayEditor{
            titleVisible: false
            configModel: extendedAxisConfigs
            listMaxHeight: maxListHeight
            listMaxWidth: parent.parent.width
            arrayItemAddRemovable: false
            Component.onCompleted: {
                init()
            }
        }
    }

    Component{
        id: extendedAxis2Configger
        ArrayEditor{
            titleVisible: false
            configModel: extendedAxis2Configs
            listMaxHeight: maxListHeight
            listMaxWidth: parent.parent.width
            arrayItemAddRemovable: false
            Component.onCompleted: {
                init()
            }
        }
    }

    Component{
        id: extendedVcmConfigger
        ArrayEditor{
            titleVisible: false
            configModel: extendedVcmConfigs
            listMaxHeight: maxListHeight
            listMaxWidth: parent.parent.width
            arrayItemAddRemovable: false
            Component.onCompleted: {
                init()
            }
        }
    }

    Component{
        id: sAxisModuleConfigger
        ArrayEditor{
            titleVisible: false
            configModel: singleAxisModuleConfigs
            listMaxHeight: maxListHeight
            arrayItemAddRemovable: false
            Component.onCompleted: {
                init()
            }
        }
    }

    Component{
        id: xyModuleConfigger
        ArrayEditor{
            titleVisible: false
            configModel: xyModuleConfigs
            listMaxHeight: maxListHeight
            arrayItemAddRemovable: false
            Component.onCompleted: {
                init()
            }
        }
    }

    Component{
        id: xyzModuleConfigger
        ArrayEditor{
            titleVisible: false
            configModel: xyzModuleConfigs
            listMaxHeight: maxListHeight
            arrayItemAddRemovable: false
            Component.onCompleted: {
                init()
            }
        }
    }

    Component{
        id: softLandingConfigger
        ArrayEditor{
            titleVisible: false
            configModel: softLandingConfigs
            listMaxHeight: maxListHeight
            arrayItemAddRemovable: false
            Component.onCompleted: {
                init()
            }
        }
    }

    Component{
        id: xyzrDebuggerConfigger
        ArrayEditor{
            titleVisible: false
            configModel: xyzrDebuggerConfigs
            listMaxHeight: maxListHeight
            arrayItemAddRemovable: false
            Component.onCompleted: {
                init()
            }
        }
    }

    ColumnLayout{
        anchors.fill: parent
        Label{
            id: lblTip
        }
        MyTabView{
            Layout.fillHeight: true
            Layout.fillWidth: true
            trContext: "MotionConfigger"
            id: viewer
        }
    }

    Component.onCompleted: {
        viewer.addTab(qsTr("DI"), diConfigger)
        viewer.addTab(qsTr("DO"), doConfigger)
        viewer.addTab(qsTr("Vacuum"), vacuumConfigger)
        viewer.addTab(qsTr("Cyl"), cylConfigger)
        viewer.addTab(qsTr("Axis"), axisConfigger)
        viewer.addTab(qsTr("SingleAxis"), sAxisModuleConfigger)
        viewer.addTab(qsTr("XyModule"), xyModuleConfigger)
        viewer.addTab(qsTr("XyzModule"), xyzModuleConfigger)
        viewer.addTab(qsTr("VCM"), vcmConfigger)
        if(extendedAxisConfigs !== null && extendedAxisConfigs.count() > 0){
            viewer.addTab(qsTr("ExAxis"), extendedAxisConfigger)
        }
        if(extendedAxis2Configs !== null && extendedAxis2Configs.count() > 0){
            viewer.addTab(qsTr("ExAxis2"), extendedAxis2Configger)
        }
        if(extendedVcmConfigs !== null && extendedVcmConfigs.count() > 0){
            viewer.addTab(qsTr("ExVCM"), extendedVcmConfigger)
        }
        viewer.addTab(qsTr("SoftLanding"), softLandingConfigger)
        viewer.addTab(qsTr("XYZRDbg"), xyzrDebuggerConfigger)
    }
}
