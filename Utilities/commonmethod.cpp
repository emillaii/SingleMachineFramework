#include "commonmethod.h"

QString combineString(const QString &seperator, const QList<QString> &stringList)
{
    QString returnString;
    for (int i = 0; i < stringList.count(); i++)
    {
        returnString += stringList[i];
        if (i != stringList.count() - 1)
        {
            returnString += seperator;
        }
    }
    return returnString;
}

QString combineVariant(const QString &seperator, const QList<QVariant> &variantList)
{
    QString returnString;
    for (int i = 0; i < variantList.count(); i++)
    {
        returnString += variantList[i].toString();
        if (i != variantList.count() - 1)
        {
            returnString += seperator;
        }
    }
    return returnString;
}

bool silicoolWait(int timeout, std::function<bool()> checkFunc, int checkInterval)
{
    bool _isInMainThread = isInMainThread();
    QElapsedTimer timer;
    timer.start();
    while (true)
    {
        if (checkFunc())
        {
            return true;
        }
        if (timeout != -1 && timer.elapsed() > timeout)
        {
            return false;
        }
        if (_isInMainThread)
        {
            QCoreApplication::processEvents(QEventLoop::AllEvents, checkInterval);
        }
        else
        {
            QThread::msleep(checkInterval);
        }
    }
}

int UTILITIESSHARED_EXPORT getMethodIndex(const QMetaObject *metaObj, QMetaMethod::MethodType methodType, const QString &methodName)
{
    for (int i = 0; i < metaObj->methodCount(); i++)
    {
        QMetaMethod metaMethod = metaObj->method(i);
        if (metaMethod.methodType() == methodType && metaMethod.name() == methodName)
        {
            return i;
        }
    }
    return -1;
}

Qt::HANDLE mainThreadID = QThread::currentThreadId();

bool isInMainThread()
{
    return QThread::currentThreadId() == mainThreadID;
}
