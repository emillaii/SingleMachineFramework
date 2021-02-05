#ifndef LOGTOVIEWERHANDLER_H
#define LOGTOVIEWERHANDLER_H

#include "../utilities_global.h"
#include "../zmqWrapper/publisher.h"
#include "LogBuffer.h"
#include "LogCollector.h"
#include "configManager/configarray.h"
#include "configManager/configfile.h"
#include "configManager/configobject.h"
#include "logmodel.h"

class LogToViewerConfig : public ConfigObject
{
    Q_OBJECT

    Q_PROPERTY(QString msgPublishAddr READ msgPublishAddr WRITE setMsgPublishAddr NOTIFY msgPublishAddrChanged)

public:
    LogToViewerConfig(QObject *parent = nullptr) : ConfigObject(parent)
    {
        disableTranslate();
        init();
    }

    QString msgPublishAddr() const
    {
        return m_msgPublishAddr;
    }

public slots:
    void setMsgPublishAddr(QString msgPublishAddr)
    {
        if (m_msgPublishAddr == msgPublishAddr)
            return;

        m_msgPublishAddr = msgPublishAddr;
        emit msgPublishAddrChanged(m_msgPublishAddr);
    }

signals:
    void msgPublishAddrChanged(QString msgPublishAddr);

private:
    QString m_msgPublishAddr = "tcp://127.0.0.1:60001";
};

class UTILITIESSHARED_EXPORT LogToViewerHandler : public QObject, public LogCollector
{
    Q_OBJECT
public:
    LogToViewerHandler();

    void init();

    ~LogToViewerHandler() override;

    Q_INVOKABLE void setLogLevel(QString category, int level);

    Q_INVOKABLE void updateLogLevel(QString category);

    Q_INVOKABLE QObject *getLogModel(QString category);

    Q_INVOKABLE QString getDefaultCategoryName() const
    {
        return defaultCategoryName;
    }

    // LogCollector interface
protected:
    virtual void onUnCategorizedLog(int logLevel, QString &msg) override;

    virtual void onCategorizedLog(QString &category, int logLevel, QString &msg) override;

    virtual void onMessage(const QJsonObject &jsonObj) override;

    // QObject interface
protected:
    virtual void timerEvent(QTimerEvent *event) override;

private slots:
    void appendCategory(QString category);

signals:
    void appendCategorySignal(QString category);
    void detectLogBuffer();
    void newLogModelAttached(QString category);
    void logLevelPublished(QString category, int logLevel);

private:
    const QString warnLogName = "Warn";
    int detectLogBufferTimerID = -1;
    const int DetectLogBufferInterval = 180;
    QMap<int, QString> levelToColor;
    Publisher msgPublisher;
    QMap<QString, LogBuffer *> logBuffers;
    QMap<QString, LogModel *> logModels;
    LogToViewerConfig *logToViewerConfig;
    ConfigFile *logToViewerConfigFile;
    ConfigArray *addressToBeListenedConfig;
    ConfigFile *addressToBeListenedConfigFile;
};

#endif    // LOGTOVIEWERHANDLER_H
