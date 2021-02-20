import QtQuick 2.0
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12
import "../CustomizedUIElement"

Rectangle {
    color: "transparent"
    border.width: 1
    width: 800
    height: 200

    function setLogLevel(level){

    }

    QtObject{
        id: self
        property var logViewPages: []
    }

    Rectangle{
        id: loglevelSelector
        anchors.left: parent.left
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        anchors.margins: 3

        border.width: 1
        width: 80
        color: "transparent"

        ColumnLayout{
            anchors.fill: parent

            RadioButton{
                id: rdbLevel0
                text: qsTr("Debug")
                onClicked: {
                    $loggerConfig.setLogLevel(0)
                }
            }
            RadioButton{
                id: rdbLevel1
                text: qsTr("Info")
                onClicked: {
                    $loggerConfig.setLogLevel(1)
                }
            }
            RadioButton{
                id: rdbLevel2
                text: qsTr("Warn")
                onClicked: {
                    $loggerConfig.setLogLevel(2)
                }
            }
        }
    }

    MyTabView{
        id: logViewers

        anchors.left: loglevelSelector.right
        anchors.top: parent.top
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        anchors.margins: 1
    }

    Component{
        id: logViewComponent

        LogView{
            Component.onCompleted: {
                category = parent.title
                setLogModel($logPublisher.getLogModel(category))
                self.logViewPages[category] = this
            }
        }
    }

    Connections{
        target: $logPublisher
        onNewLogModelAttached:{
            logViewers.addTab(category, logViewComponent)
        }
    }

    Component.onCompleted: {
        var defaultCategoryName = $logPublisher.getDefaultCategoryName()
        logViewers.addTab(defaultCategoryName, logViewComponent)
        var warnCategoryName = $logPublisher.getWarnCategoryName()
        logViewers.addTab(warnCategoryName, logViewComponent)
        if($loggerConfig.logLevel === 0){
            rdbLevel0.checked = true
        }else if($loggerConfig.logLevel === 1){
            rdbLevel1.checked = true
        }else{
            rdbLevel2.checked = true
        }
    }
}
