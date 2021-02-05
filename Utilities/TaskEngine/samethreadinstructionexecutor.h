#ifndef SAMETHREADINSTRUCTIONEXECUTOR_H
#define SAMETHREADINSTRUCTIONEXECUTOR_H

#include "errorHandling/silicolerror.h"
#include "instruction.h"
#include "instructionexecutionresult.h"
#include "utilities_global.h"
#include <QMap>
#include <QMutex>
#include <QMutexLocker>
#include <QObject>
#include <QQueue>
#include <QThread>
#include <QUuid>
#include <QWaitCondition>

struct ThreadInstruction
{
public:
    ThreadInstruction() {}
    ThreadInstruction(const QString &uuid, const QString &cmd, const QVariantList &args, bool waitDone)
        : uuid(uuid), cmd(cmd), args(args), waitDone(waitDone)
    {
    }

    QString uuid;
    QString cmd;
    QVariantList args;
    bool waitDone;
};

///
/// \brief The SameThreadInstructionExecutor class
/// Can execute instruction in the same thread
///
class UTILITIESSHARED_EXPORT SameThreadInstructionExecutor : public QThread
{
    Q_OBJECT
public:
    explicit SameThreadInstructionExecutor(QObject *parent = nullptr);

    virtual ~SameThreadInstructionExecutor() override;

    void kill();

    template <typename T>
    T runIns(const QString &cmd, const QVariantList &args = QVariantList())
    {
        return runIns(cmd, args, true).value<T>();
    }

    QVariant runIns(const QString &cmd, const QVariantList &args = QVariantList(), bool waitDone = true);

    // QThread interface
protected:
    virtual void run() override;

private:
    QString executeIns(const QString &cmd, const QVariantList &args = QVariantList(), bool waitDone = true);

    QVariant getInsResult(const QString &uuid);

private:
    QQueue<ThreadInstruction> insQueue;
    QMap<QString, InstructionExecuteResult> insExeResults;
    QMutex locker;
    QWaitCondition hasInstruction;
    bool isKill = false;
};

#endif    // SAMETHREADINSTRUCTIONEXECUTOR_H
