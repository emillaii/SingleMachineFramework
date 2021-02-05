#include "objectlivedthreadinstructionexecutor.h"

ObjectLivedThreadInstructionExecutor::ObjectLivedThreadInstructionExecutor(QObject *parent) : QObject(parent)
{
    connect(this, &ObjectLivedThreadInstructionExecutor::executeCmdReq, this,
            &ObjectLivedThreadInstructionExecutor::onExecuteCmdReq, Qt::AutoConnection);
}

QVariant ObjectLivedThreadInstructionExecutor::runIns(const QString &cmd, const QVariantList &args, bool waitDone)
{
    QString uuid;
    if (waitDone)
    {
        uuid = QUuid::createUuid().toString(QUuid::Id128);
        insExeResults[uuid] = InstructionExecuteResult();
    }
    emit executeCmdReq(uuid, cmd, args, waitDone);
    if (waitDone)
    {
        return getInsResult(uuid);
    }
    else
    {
        return QVariant();
    }
}

QVariant ObjectLivedThreadInstructionExecutor::getInsResult(QString uuid)
{
    silicoolWait(
        -1, [&] { return insExeResults[uuid].isExecutionDone; }, 10);
    if (!insExeResults[uuid].errMsg.isEmpty())
    {
        throw SilicolAbort(insExeResults[uuid].errMsg);
    }
    else
    {
        return insExeResults[uuid].result;
    }
}

void ObjectLivedThreadInstructionExecutor::onExecuteCmdReq(QString uuid, QString cmd, QVariantList args, bool waitDone)
{
    Instruction ins(this, cmd, args);
    try
    {
        QVariant result = ins.execute();
        if (waitDone)
        {
            insExeResults[uuid].result = result;
        }
    }
    catch (SilicoolException &se)
    {
        if (waitDone)
        {
            insExeResults[uuid].errMsg = se.what();
        }
        else
        {
            qCritical() << se.what();
        }
    }
    if (waitDone)
    {
        insExeResults[uuid].isExecutionDone = true;
    }
}
