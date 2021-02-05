//
//
//  Generated by StarUML(tm) C++ Add-In
//
//  @ Project : Untitled
//  @ File Name : LogServer.h
//  @ Date : 2020/1/7
//  @ Author :
//
//

#if !defined(LOGSERVER_H)
#define LOGSERVER_H

#include "../utilities_global.h"
#include "LogCollector.h"
#include "configManager/configarray.h"
#include "configManager/configfile.h"
#include "configManager/configobject.h"
#include "rollbackfile.h"

class LogToFileConfig : public ConfigObject
{
    Q_OBJECT

    Q_PROPERTY(QString logDir READ logDir WRITE setLogDir NOTIFY logDirChanged)
    Q_PROPERTY(int maxFileSize READ maxFileSize WRITE setMaxFileSize NOTIFY maxFileSizeChanged)
    Q_PROPERTY(int maxFileCount READ maxFileCount WRITE setMaxFileCount NOTIFY maxFileCountChanged)
    Q_PROPERTY(bool flushImmediately READ flushImmediately WRITE setFlushImmediately NOTIFY flushImmediatelyChanged)

public:
    LogToFileConfig(QObject *parent = nullptr) : ConfigObject(parent)
    {
        disableTranslate();
        init();
    }

    QString logDir() const
    {
        return m_logDir;
    }

    int maxFileSize() const
    {
        return m_maxFileSize;
    }

    int maxFileCount() const
    {
        return m_maxFileCount;
    }

    bool flushImmediately() const
    {
        return m_flushImmediately;
    }

public slots:
    void setLogDir(QString logDir)
    {
        if (m_logDir == logDir)
            return;

        m_logDir = logDir;
        emit logDirChanged(m_logDir);
    }

    void setMaxFileSize(int maxFileSize)
    {
        if (m_maxFileSize == maxFileSize)
            return;

        m_maxFileSize = maxFileSize;
        emit maxFileSizeChanged(m_maxFileSize);
    }

    void setMaxFileCount(int maxFileCount)
    {
        if (m_maxFileCount == maxFileCount)
            return;

        m_maxFileCount = maxFileCount;
        emit maxFileCountChanged(m_maxFileCount);
    }

    void setFlushImmediately(bool flushImmediately)
    {
        if (m_flushImmediately == flushImmediately)
            return;

        m_flushImmediately = flushImmediately;
        emit flushImmediatelyChanged(m_flushImmediately);
    }

signals:
    void logDirChanged(QString logDir);

    void maxFileSizeChanged(int maxFileSize);

    void maxFileCountChanged(int maxFileCount);

    void flushImmediatelyChanged(bool flushImmediately);

private:
    QString m_logDir = "C:\\SilicoolLog";
    int m_maxFileSize = 1024 * 1024 * 3;
    int m_maxFileCount = 100;
    bool m_flushImmediately = true;
};

class UTILITIESSHARED_EXPORT LogToFileHandler : public LogCollector
{
public:
    LogToFileHandler(bool outputToConsole);

    void init();

    ~LogToFileHandler() override;

    // LogCollector interface
protected:
    virtual void onUnCategorizedLog(int logLevel, QString &msg) override;

    virtual void onCategorizedLog(QString &category, int logLevel, QString &msg) override;

private:
    void addRollbackFile(const QString &category, const QString &fileName)
    {
        rollbackFiles[category] = new RollbackFile();
        rollbackFiles[category]->init(logToFileConfig->logDir(), fileName, logToFileConfig->maxFileSize(), logToFileConfig->maxFileCount(),
                                      logToFileConfig->flushImmediately());
    }

private:
    bool outputToConsole;
    ConsoleOutputer consoleOutputer;
    const QString warningCategory = "warningCategory";
    const QString warningLogName = "warning";
    LogToFileConfig *logToFileConfig;
    ConfigFile *logToFileConfigFile;
    ConfigArray *addressToBeListenedConfig;
    ConfigFile *addressToBeListenedConfigFile;
    QMap<QString, RollbackFile *> rollbackFiles;
};

#endif    // LOGSERVER_H
