import QtQuick 2.0
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.0

Rectangle{
    color: "transparent"
    property string category: ""

    border.width: 1

    function setLogModel(model)
    {
        logView.model = model
    }

    Label{
        id: templateLbl
        visible: false
    }

    ListView{
        id: logView
        x: 2
        y: 2
        width: parent.width - 4
        height: parent.height - 4

        clip: true

        delegate: Text {
            width: parent.width - 3
            color: {
                if(logLevel < 2){
                    return templateLbl.color
                }else if(logLevel == 2){
                    return  "orangered"
                }else{
                    return "red"
                }
            }
            text: logStr
            wrapMode: Text.Wrap
            lineHeight: 1.2
            lineHeightMode: Text.ProportionalHeight
        }

        MouseArea{
            z: 1
            anchors.fill: parent
            onDoubleClicked: {
                logView.model.onClearLog()
            }
        }

        ScrollBar.vertical: ScrollBar {
            id: scrollBar
            z: 2
            width: 15
        }

        MouseArea{
            id: logViewMouseArea
            z: 3
            anchors.fill: parent
            hoverEnabled: true
            onPressed: {
                mouse.accepted = false
            }
        }
    }

    Timer {
        id: scrollTimer
        repeat: true
        interval: 200
        onTriggered: {
            if(!logViewMouseArea.containsMouse)
            {
                logView.positionViewAtEnd()
            }
        }
    }

    Component.onCompleted: {
        scrollTimer.start()
    }
}
