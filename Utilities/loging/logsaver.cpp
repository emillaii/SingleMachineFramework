#include "logsaver.h"

LogSaver::LogSaver(LoggerConfig *loggerConfig, QObject *parent) : QObject(parent), loggerConfig(loggerConfig)
{
    defaultCategoryName = QLoggingCategory::defaultCategory()->categoryName();
    addRollbackFile(defaultCategoryName, "silicoolLog");
}

LogSaver::~LogSaver()
{
    foreach (auto rollbackFile, rollbackFiles.values())
    {
        delete rollbackFile;
    }
    rollbackFiles.clear();
}

void LogSaver::onNewLog(QString category, int logLevel, QString msg)
{
    if (!rollbackFiles.contains(category))
    {
        addRollbackFile(category, category);
    }
    rollbackFiles[category]->appendLine(msg);

    if (category != defaultCategoryName)
    {
        rollbackFiles[defaultCategoryName]->appendLine(msg);
    }

    if (logLevel >= LogLevel::Warning)
    {
        if (!rollbackFiles.contains(warningCategory))
        {
            addRollbackFile(warningCategory, warningLogName);
        }
        rollbackFiles[warningCategory]->appendLine(msg);
    }
}

void LogSaver::addRollbackFile(const QString &category, const QString &fileName)
{
    rollbackFiles[category] = new RollbackFile();
    rollbackFiles[category]->init(loggerConfig->logDir(), fileName, loggerConfig->maxFileSize(), loggerConfig->maxFileCount(),
                                  loggerConfig->flushImmediately());
}
