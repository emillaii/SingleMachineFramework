#ifndef UIOPERATION_H
#define UIOPERATION_H

#include "../commonmethod.h"
#include "../utilities_global.h"
#include "btnnamedefinition.h"
#include "msgboxmodel.h"
#include <QDebug>
#include <QObject>
#include <QQmlContext>
#include <QQmlEngine>
#include <QQuickItem>
#include <QThread>
#include <QUuid>

class UIResponse
{
public:
    QString clickedButton;
    bool gotResponse = false;
};

///
/// \brief The UIOperation class
/// UI操作类，不阻塞UI
/// 可以同时调用showMessage
/// 不同线程可以同时调用getUIResponse
/// 非主线程，调用getUIResponse后，必须等函数返回才会处理其他事件
/// 主线程调用getUIResponse后，可以处理其他事件，因此若前一次getUIResponse未返回结果，仍然可以再次调用getUIResponse
///
class UTILITIESSHARED_EXPORT UIOperation : public QObject
{
    Q_OBJECT

private:
    UIOperation();

public:
    static UIOperation *getIns()
    {
        static UIOperation instance;
        return &instance;
    }

    void setContextProperty(QQmlEngine &engine);

    Q_INVOKABLE void setGrabMouseItem(QQuickItem *item);

    Q_INVOKABLE void showMessage(QString title, QString content, MsgBoxIcon::Icon icon, QList<QString> buttons);
    Q_INVOKABLE void showMessage(QString title, QString content, MsgBoxIcon::Icon icon, QString button);
    Q_INVOKABLE QString getUIResponse(QString title, QString content, MsgBoxIcon::Icon icon, QList<QString> buttons);
    Q_INVOKABLE QString getUIResponse(QString title, QString content, MsgBoxIcon::Icon icon, QString button);

    Q_INVOKABLE void showTip(QString tipMsg);
    Q_INVOKABLE void showError(QString errMsg);
    Q_INVOKABLE bool yesNoConfirm(QString content);
    Q_INVOKABLE bool okCancelConfirm(QString content);

signals:
    void addMsgBoxReq(QString uuid, QString title, QString content, int icon, QList<QString> buttons, QPrivateSignal);

public slots:
    void onUIResponse(QString uuid, QString clickedButton);

private:
    QDebug getLogger(MsgBoxIcon::Icon icon);
    void resetMouse();

private:
    QMap<QString, UIResponse *> uiRsp;
    MsgBoxModel m_msgBoxModel;
    QQuickItem *grabMouseItem = nullptr;
    QMutex locker;
};

#endif    // UIOPERATION_H
