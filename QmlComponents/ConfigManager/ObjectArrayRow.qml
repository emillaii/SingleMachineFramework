import QtQuick 2.0
import QtQuick.Controls 2.5
import QtQuick.Layouts 1.3
import ConfigElementType 1.0

BasicArrayRow{
    id: configNodes

    property bool arrayItemAddRemovable: true

    function init(configNames)
    {
        for(var i in configNames){
            var isSimpleType = true
            var elementType = configModel.getConfigType(configNames[i])
            var component = null
            if(elementType === ConfigElementType.Bool){
                component = Qt.createComponent("SilicolCheckBox.qml")
            }else if(elementType === ConfigElementType.Int ||
                     elementType === ConfigElementType.Double ||
                     elementType === ConfigElementType.Other){
                if(configModel.isOptional(configNames[i])){
                    component = Qt.createComponent("SilicolComboBox.qml")
                }else{
                    component = Qt.createComponent("SilicolTextField.qml")
                }
            }else{
                isSimpleType = false
                component = Qt.createComponent("SilicolConfigButton.qml")
            }

            if(component !== null){
                var obj = component.createObject(configNodes)
                if(obj === null){
                    console.error(component.errorString())
                    continue
                }
                obj.dataModel = configModel
                obj.identity = configNames[i]
                if(!isSimpleType){
                    obj.arrayItemAddRemovable = arrayItemAddRemovable
                }

                obj.init()
            }else{
                console.log("Unknown type: " + elementType)
                continue
            }
        }

        connRowIndexChanged.target = rowModel
    }

    function getItemsWidth()
    {
        var itemsWidth = []
        for(var i in children){
            itemsWidth.push(children[i].width)
        }
        return itemsWidth
    }

    function getItemsSpacing()
    {
        return spacing
    }
}
