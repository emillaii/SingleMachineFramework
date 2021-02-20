#ifndef LOGGERCONFIG_H
#define LOGGERCONFIG_H

#include "../configManager/configfile.h"
#include "../utilities_global.h"

class UTILITIESSHARED_EXPORT LoggerConfig : public ConfigObject
{
    Q_OBJECT

    Q_PROPERTY(QString logDir READ logDir WRITE setLogDir NOTIFY logDirChanged)
    Q_PROPERTY(int logLevel READ logLevel WRITE setLogLevel NOTIFY logLevelChanged)
    Q_PROPERTY(int maxFileSize READ maxFileSize WRITE setMaxFileSize NOTIFY maxFileSizeChanged)
    Q_PROPERTY(int maxFileCount READ maxFileCount WRITE setMaxFileCount NOTIFY maxFileCountChanged)
    Q_PROPERTY(bool flushImmediately READ flushImmediately WRITE setFlushImmediately NOTIFY flushImmediatelyChanged)
    Q_PROPERTY(bool outputLogToConsole READ outputLogToConsole WRITE setOutputLogToConsole NOTIFY outputLogToConsoleChanged)

public:
    LoggerConfig()
    {
        disableTranslate();
        init();
    }

    QString logDir() const
    {
        return m_logDir;
    }

    int logLevel() const
    {
        return m_logLevel;
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

    bool outputLogToConsole() const
    {
        return m_outputLogToConsole;
    }

public slots:

    void setLogDir(QString logDir)
    {
        if (m_logDir == logDir)
            return;

        m_logDir = logDir;
        emit logDirChanged(m_logDir);
    }

    void setLogLevel(int logLevel)
    {
        if (m_logLevel == logLevel)
            return;

        m_logLevel = logLevel;
        emit logLevelChanged(m_logLevel);
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

    void setOutputLogToConsole(bool outputLogToConsole)
    {
        if (m_outputLogToConsole == outputLogToConsole)
            return;

        m_outputLogToConsole = outputLogToConsole;
        emit outputLogToConsoleChanged(m_outputLogToConsole);
    }

signals:
    void logDirChanged(QString logDir);

    void logLevelChanged(int logLevel);

    void maxFileSizeChanged(int maxFileSize);

    void maxFileCountChanged(int maxFileCount);

    void flushImmediatelyChanged(bool flushImmediately);

    void outputLogToConsoleChanged(bool outputLogToConsole);

private:
    QString m_logDir = "C:/SilicoolLog";
    int m_logLevel = 0;
    int m_maxFileSize = 1024 * 1024 * 5;
    int m_maxFileCount = 50;
    bool m_flushImmediately = true;
    bool m_outputLogToConsole = true;
};

#endif    // LOGGERCONFIG_H
