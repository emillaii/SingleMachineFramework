#include "instructionexecutionclient.h"

void InstructionExecutionClient::init(const QString &serverAddress)
{
    this->serverAddress = serverAddress;
    if (!isInMainThread())
    {
        qFatal("InstructionExecutionClient have to be inited in main thread!");
    }
    if (!isInit)
    {
        node = new QRemoteObjectNode();
        node->connectToNode(serverAddress);
        ieReplica = node->acquire<InstructionExecutorReplica>();
        connect(ieReplica, &InstructionExecutorReplica::instructionExecuteDone, this,
                &InstructionExecutionClient::onInstructionExecuteDone, Qt::QueuedConnection);
        isInit = true;
    }
}

QVariant InstructionExecutionClient::runInstruction(const QString &targetObjName,
                                                    const QString &cmd,
                                                    const QVariantList &args,
                                                    bool async)
{
    QString uuid = executeInstruction(targetObjName, cmd, args, true, async);
    return getInstructionExecutionResult(uuid);
}

QString InstructionExecutionClient::executeInstruction(
    const QString &targetObjName, const QString &cmd, const QVariantList &args, bool returnResult, bool async)
{
    if (!isInit)
    {
        qFatal("InstructionExecutionClient is not inited!");
    }
    while (true)
    {
        if (!silicoolWait(
                3000, [this] { return ieReplica->isReplicaValid(); }, 300))
        {
            qCritical() << "InstructionExecutionClient replica is invalid! Please check the connection to source:"
                        << serverAddress;
        }
        else
        {
            break;
        }
    }

    if (isInMainThread())
    {
        return onExecuteInstructionReq(targetObjName, cmd, args, returnResult, async);
    }
    else
    {
        if (returnResult)
        {
            connect(this, &InstructionExecutionClient::executeInstructionReq, this,
                    &InstructionExecutionClient::onExecuteInstructionReq, Qt::BlockingQueuedConnection);
        }
        else
        {
            connect(this, &InstructionExecutionClient::executeInstructionReq, this,
                    &InstructionExecutionClient::onExecuteInstructionReq, Qt::QueuedConnection);
        }
        QString uuid = emit executeInstructionReq(targetObjName, cmd, args, returnResult, async);
        disconnect(this, &InstructionExecutionClient::executeInstructionReq, this,
                   &InstructionExecutionClient::onExecuteInstructionReq);
        return uuid;
    }
}

QVariant InstructionExecutionClient::getInstructionExecutionResult(const QString &uuid)
{
    if (!instructionExecuteResults.contains(uuid))
    {
        throw SilicolAbort(tr("Instruction with uuid %1 was not watched!").arg(uuid), EX_LOCATION);
    }
    silicoolWait(
        -1, [&] { return instructionExecuteResults[uuid].isExecutionDone; }, 10);
    InstructionExecuteResult ier = instructionExecuteResults[uuid];
    instructionExecuteResults.remove(uuid);
    if (!ier.errMsg.isEmpty())
    {
        throw SilicolAbort(ier.errMsg);
    }
    return ier.result;
}

InstructionExecutorReplica *InstructionExecutionClient::getIeReplica() const
{
    if (!isInit)
    {
        qFatal("InstructionExecutionClient is not inited!");
    }
    return ieReplica;
}

QString InstructionExecutionClient::onExecuteInstructionReq(
    QString targetObjName, QString cmd, QVariantList args, bool returnResult, bool async)
{
    auto rsp = ieReplica->executeInstruction(targetObjName, cmd, args, returnResult, async);
    if (returnResult)
    {
        if (!rsp.waitForFinished())
        {
            qCCritical(ieCate()) << tr("Get InstructionExecutionServer response timeout! targetObj: %1, cmd: %2")
                                        .arg(targetObjName)
                                        .arg(cmd);
        }
        QString uuid = rsp.returnValue();
        instructionExecuteResults[uuid] = InstructionExecuteResult();
        return uuid;
    }
    return "";
}

void InstructionExecutionClient::onInstructionExecuteDone(QString uuid, QString errMsg, QVariant result)
{
    if (silicoolWait(
            100, [&] { return instructionExecuteResults.contains(uuid); }, 10))
    {
        instructionExecuteResults[uuid].errMsg = std::move(errMsg);
        instructionExecuteResults[uuid].result = std::move(result);
        instructionExecuteResults[uuid].isExecutionDone = true;
    }
}
