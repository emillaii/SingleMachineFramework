#include "samethreadinstructionexecutor.h"

SameThreadInstructionExecutor::SameThreadInstructionExecutor(QObject *parent) : QThread(parent)
{
    start();
}

SameThreadInstructionExecutor::~SameThreadInstructionExecutor()
{
    kill();
}

void SameThreadInstructionExecutor::kill()
{
    isKill = true;
    hasInstruction.wakeAll();
    wait();
}

QVariant SameThreadInstructionExecutor::runIns(const QString &cmd, const QVariantList &args, bool waitDone)
{
    QString uuid = executeIns(cmd, args, waitDone);
    if (waitDone)
    {
        return getInsResult(uuid);
    }
    else
    {
        return QVariant();
    }
}

void SameThreadInstructionExecutor::run()
{
    ThreadInstruction threadIns;
    while (!isKill)
    {
        {
            QMutexLocker tmpLocker(&locker);
            if (insQueue.isEmpty())
            {
                hasInstruction.wait(&locker);
            }
        }
        if (isKill)
        {
            return;
        }

        {
            QMutexLocker tmpLocker(&locker);
            threadIns = insQueue.dequeue();
        }
        Instruction ins(this, threadIns.cmd, threadIns.args);
        try
        {
            QVariant result = ins.execute();
            if (threadIns.waitDone)
            {
                insExeResults[threadIns.uuid].result = result;
            }
        }
        catch (SilicoolException &se)
        {
            if (threadIns.waitDone)
            {
                insExeResults[threadIns.uuid].errMsg = se.what();
            }
            else
            {
                qCritical() << se.what();
            }
        }
        if (threadIns.waitDone)
        {
            insExeResults[threadIns.uuid].isExecutionDone = true;
        }
    }
}

QString SameThreadInstructionExecutor::executeIns(const QString &cmd, const QVariantList &args, bool waitDone)
{
    QString uuid = QUuid::createUuid().toString(QUuid::Id128);
    if (waitDone)
    {
        insExeResults[uuid] = InstructionExecuteResult();
    }
    {
        QMutexLocker tmpLocker(&locker);
        insQueue.append(ThreadInstruction(uuid, cmd, args, waitDone));
    }
    hasInstruction.wakeAll();
    return uuid;
}

QVariant SameThreadInstructionExecutor::getInsResult(const QString &uuid)
{
    if (insExeResults.contains(uuid))
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
    else
    {
        throw SilicolAbort(tr("Instruction with uuid %1 was not watched!").arg(uuid), EX_LOCATION);
    }
}
