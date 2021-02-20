#ifndef LOGSAVER_H
#define LOGSAVER_H

#include "LoggerConfig.h"
#include "loglevel.h"
#include "rollbackfile.h"
#include <QObject>

class LogSaver : public QObject
{
    Q_OBJECT

public:
    explicit LogSaver(LoggerConfig *loggerConfig, QObject *parent = nullptr);

    ~LogSaver() override;

public slots:
    void onNewLog(QString category, int logLevel, QString msg);

private:
    void addRollbackFile(const QString &category, const QString &fileName);

private:
    const QString warningCategory = "warningCategory";
    const QString warningLogName = "warning";
    QString defaultCategoryName;
    LoggerConfig *loggerConfig;
    QMap<QString, RollbackFile *> rollbackFiles;
};

#endif    // LOGSAVER_H
