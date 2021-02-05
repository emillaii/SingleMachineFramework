#include "uioperationimpl.h"

void UIOperationImpl::onUIResponse(QString uuid, QString clickedButton)
{
    m_msgBoxModel.removeMsgBox(uuid);
    emit InstructionExecutionServer::getIns().uiResponse(uuid, clickedButton);
}

void UIOperationImpl::showMessage(QString title, QString content, int icon, QList<QVariant> buttons)
{
    getUIResponse(title, content, icon, buttons);
}

QString UIOperationImpl::getUIResponse(QString title, QString content, int icon, QList<QVariant> buttons)
{
    MsgBoxIcon::Icon ico = static_cast<MsgBoxIcon::Icon>(icon);
    QString uuid = QUuid::createUuid().toString(QUuid::Id128);
    QList<QString> buttonNames;
    for (int i = 0; i < buttons.count(); i++)
    {
        buttonNames.append(buttons[i].toString());
    }
    MsgBoxItem msgBox(uuid, title, ico, content, buttonNames);
    m_msgBoxModel.addMsgBox(msgBox);
    return uuid;
}
