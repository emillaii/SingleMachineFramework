#ifndef LOGPUBLISHER_H
#define LOGPUBLISHER_H

#include "loglevel.h"
#include "logmodel.h"
#include "utilities_global.h"
#include <QEvent>
#include <QLoggingCategory>
#include <QObject>

class UTILITIESSHARED_EXPORT LogPublisher : public QObject
{
    Q_OBJECT

public:
    LogPublisher();

    ~LogPublisher() override;

    Q_INVOKABLE QObject *getLogModel(QString category);

    Q_INVOKABLE QString getDefaultCategoryName() const
    {
        return defaultCategoryName;
    }

    Q_INVOKABLE QString getWarnCategoryName() const
    {
        return warnLogName;
    }

public slots:
    void onNewLog(QString category, int logLevel, QString msg);

    // QObject interface
protected:
    virtual void timerEvent(QTimerEvent *event) override;

private:
    void appendCategory(QString category)
    {
        logModels[category] = new LogModel();
    }

signals:
    void newLogModelAttached(QString category);
    void logLevelPublished(QString category, int logLevel);

private:
    const QString warnLogName = "Warn";
    QString defaultCategoryName;
    int detectLogBufferTimerID = -1;
    const int DetectLogBufferInterval = 180;
    QMap<QString, LogModel *> logModels;
};

#endif    // LOGPUBLISHER_H
