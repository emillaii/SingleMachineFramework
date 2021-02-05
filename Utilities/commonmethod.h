#ifndef COMMONMETHOD_H
#define COMMONMETHOD_H

#include "utilities_global.h"
#include <QCoreApplication>
#include <QDateTime>
#include <QElapsedTimer>
#include <QMetaEnum>
#include <QThread>
#include <QVariantList>
#include <QtCore>

UTILITIESSHARED_EXPORT QString combineString(const QString &seperator, const QList<QString> &stringList);

UTILITIESSHARED_EXPORT QString combineVariant(const QString &seperator, const QList<QVariant> &variantList);

template <typename T>
QString combineNumber(const QString &seperator, const QList<T> &numberList)
{
    QString returnString;
    for (int i = 0; i < numberList.count(); i++)
    {
        returnString += QString::number(numberList[i]);
        if (i != numberList.count() - 1)
        {
            returnString += seperator;
        }
    }
    return returnString;
}

inline QString getCurrentDateTime()
{
    return QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss.zzz");
}

template <typename T>
QVariantList toVariantList(QList<T> source)
{
    QVariantList qv;
    foreach (T item, source)
    {
        qv.append(item);
    }
    return qv;
}

template <typename T>
QList<T> unpackVariantList(QVariantList source)
{
    QList<T> list;
    foreach (const QVariant &qv, source)
    {
        list.append(qv.value<T>());
    }
    return list;
}

class AutoResetBool
{
public:
    AutoResetBool(bool *target, bool resetValue) : target(target), resetValue(resetValue) {}
    ~AutoResetBool()
    {
        *target = resetValue;
    }

private:
    bool *target;
    bool resetValue;
};

class AutoResetProperty
{
public:
    AutoResetProperty(QObject *target, QString property, QVariant value) : target(target), property(property), value(value) {}
    ~AutoResetProperty()
    {
        if (!target->setProperty(property.toUtf8(), value))
        {
            qCritical() << "Set" << QString("%1::%2").arg(target->metaObject()->className()).arg(property) << "to" << value << "failed!";
        }
    }

private:
    QObject *target;
    QString property;
    QVariant value;
};

bool UTILITIESSHARED_EXPORT silicoolWait(int timeout, std::function<bool()> checkFunc, int checkInterval = 20);

int UTILITIESSHARED_EXPORT getMethodIndex(const QMetaObject *metaObj, QMetaMethod::MethodType methodType, const QString &methodName);

extern Qt::HANDLE mainThreadID;
bool UTILITIESSHARED_EXPORT isInMainThread();

#define SILICOOL_DELETE(pointer)                                                                                                                     \
    if (pointer != nullptr)                                                                                                                          \
    {                                                                                                                                                \
        delete pointer;                                                                                                                              \
        pointer = nullptr;                                                                                                                           \
    }

#endif
