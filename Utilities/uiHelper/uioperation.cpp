#include "uioperation.h"

UIOperation::UIOperation()
{
    connect(this, &UIOperation::addMsgBoxReq, &m_msgBoxModel, &MsgBoxModel::onAddMsgBox);
}

void UIOperation::setContextProperty(QQmlEngine &engine)
{
    engine.rootContext()->setContextProperty("$msgBoxModel", &m_msgBoxModel);
    engine.rootContext()->setContextProperty("$uiOperation", this);
}

void UIOperation::setGrabMouseItem(QQuickItem *item)
{
    grabMouseItem = item;
}

void UIOperation::showMessage(QString title, QString content, MsgBoxIcon::Icon icon, QList<QString> buttons)
{
    getLogger(icon) << QString("showMessage, title: %1, content: %2").arg(title).arg(content);

    QString uuid = QUuid::createUuid().toString(QUuid::Id128);
    emit addMsgBoxReq(uuid, title, content, icon, buttons, {});
}

void UIOperation::showMessage(QString title, QString content, MsgBoxIcon::Icon icon, QString button)
{
    showMessage(title, content, icon, button.split(BtnSeperator));
}

QString UIOperation::getUIResponse(QString title, QString content, MsgBoxIcon::Icon icon, QList<QString> buttons)
{
    auto logger = getLogger(icon);
    logger << QString("getUIResponse, title: %1, content: %2, buttons: %3").arg(title).arg(content).arg(combineString("|", buttons));

    QString uuid = QUuid::createUuid().toString(QUuid::Id128);
    auto response = new UIResponse();
    {
        QMutexLocker t(&locker);
        uiRsp[uuid] = response;
    }
    emit addMsgBoxReq(uuid, title, content, icon, buttons, {});

    resetMouse();
    silicoolWait(
        -1, [&response] { return response->gotResponse; }, 50);

    {
        QMutexLocker t(&locker);
        uiRsp.remove(uuid);
    }
    QString clickedButton = response->clickedButton;
    delete response;

    logger << QString("getUIResponse, user clicked button: %1").arg(clickedButton);
    return clickedButton;
}

QString UIOperation::getUIResponse(QString title, QString content, MsgBoxIcon::Icon icon, QString button)
{
    return getUIResponse(title, content, icon, button.split(BtnSeperator));
}

void UIOperation::showTip(QString tipMsg)
{
    showMessage("Tip", tipMsg, MsgBoxIcon::Information, OkBtn);
}

void UIOperation::showError(QString errMsg)
{
    showMessage("Error", errMsg, MsgBoxIcon::Error, OkBtn);
}

bool UIOperation::yesNoConfirm(QString content)
{
    auto rsp = getUIResponse("Confirm", content, MsgBoxIcon::Question, YesNoBtns);
    return rsp == YesBtn;
}

bool UIOperation::okCancelConfirm(QString content)
{
    auto rsp = getUIResponse("Confirm", content, MsgBoxIcon::Question, OkCancelBtns);
    return rsp == OkBtn;
}

void UIOperation::onUIResponse(QString uuid, QString clickedButton)
{
    m_msgBoxModel.removeMsgBox(uuid);

    QMutexLocker t(&locker);
    if (uiRsp.contains(uuid))
    {
        auto rsp = uiRsp[uuid];
        rsp->clickedButton = clickedButton;
        rsp->gotResponse = true;
    }
}

QDebug UIOperation::getLogger(MsgBoxIcon::Icon icon)
{
    switch (icon)
    {
        case MsgBoxIcon::Error:
            return qCritical();
        case MsgBoxIcon::Warning:
            return qWarning();
        default:
            return qInfo();
    }
}

void UIOperation::resetMouse()
{
    if (isInMainThread() && grabMouseItem)
    {
        grabMouseItem->grabMouse();
        grabMouseItem->ungrabMouse();
    }
}
