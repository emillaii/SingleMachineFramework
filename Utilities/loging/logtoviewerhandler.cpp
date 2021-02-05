#include "logtoviewerhandler.h"

LogToViewerHandler::LogToViewerHandler() : msgPublisher("!@#$%")
{
    levelToColor[0] = "normalColor";
    levelToColor[1] = "normalColor";
    levelToColor[2] = "warnColor";
    levelToColor[3] = "errorColor";
    levelToColor[4] = "errorColor";

    logToViewerConfig = new LogToViewerConfig();
    logToViewerConfigFile
        = new ConfigFile("LogToViewerConfig", logToViewerConfig, "./config/logConfig/logToViewerConfig.json", false);
    addressToBeListenedConfig = new ConfigArray(ConfigElementInfo::Other);
    addressToBeListenedConfigFile = new ConfigFile("AddressToBeListened", addressToBeListenedConfig,
                                                   "./config/logConfig/addressToBeListened.json", false);

    connect(this, &LogToViewerHandler::appendCategorySignal, this, &LogToViewerHandler::appendCategory,
            Qt::BlockingQueuedConnection);
    appendCategory(defaultCategoryName);
}

void LogToViewerHandler::init()
{
    logToViewerConfigFile->populate(true);
    addressToBeListenedConfigFile->populate(true);

    LogCollector::init(addressToBeListenedConfig);
    detectLogBufferTimerID = startTimer(DetectLogBufferInterval);
    try
    {
        msgPublisher.bind(logToViewerConfig->msgPublishAddr());
    }
    catch (std::exception &ex)
    {
        qFatal(ex.what());
    }
}

LogToViewerHandler::~LogToViewerHandler()
{
    logSubscriber.stopListenMessage();
    if (detectLogBufferTimerID != -1)
    {
        killTimer(detectLogBufferTimerID);
        detectLogBufferTimerID = -1;
    }
    foreach (auto logBuffer, logBuffers.values())
    {
        delete logBuffer;
    }
    foreach (auto logModel, logModels.values())
    {
        delete logModel;
    }
}

void LogToViewerHandler::setLogLevel(QString category, int level)
{
    QJsonObject jsonMsg;
    jsonMsg["msgName"] = "setLogLevel";
    jsonMsg["category"] = category;
    jsonMsg["level"] = level;
    msgPublisher.sendJson(jsonMsg);
}

void LogToViewerHandler::updateLogLevel(QString category)
{
    QJsonObject jsonMsg;
    jsonMsg["msgName"] = "getLogLevel";
    jsonMsg["category"] = category;
    msgPublisher.sendJson(jsonMsg);
}

QObject *LogToViewerHandler::getLogModel(QString category)
{
    if (logModels.contains(category))
    {
        return logModels[category];
    }
    else
    {
        qCritical() << "Did not find category:" << category;
        return nullptr;
    }
}

void LogToViewerHandler::onUnCategorizedLog(int logLevel, QString &msg)
{
    logBuffers[defaultCategoryName]->appendLog(new LogItem{ levelToColor[logLevel], msg });
    if (logLevel >= LogLevel::Warning)
    {
        if (!logBuffers.contains(warnLogName))
        {
            emit appendCategorySignal(warnLogName);
            emit newLogModelAttached(warnLogName);
        }
        logBuffers[warnLogName]->appendLog(new LogItem{ levelToColor[logLevel], msg });
    }
}

void LogToViewerHandler::onCategorizedLog(QString &category, int logLevel, QString &msg)
{
    if (!logBuffers.contains(category))
    {
        emit appendCategorySignal(category);
        emit newLogModelAttached(category);
    }
    logBuffers[category]->appendLog(new LogItem{ levelToColor[logLevel], msg });
}

void LogToViewerHandler::onMessage(const QJsonObject &jsonObj)
{
    if (jsonObj["msgName"].toString() == "getLogLevelRsp")
    {
        QString category = jsonObj["category"].toString();
        int logLevel = jsonObj["logLevel"].toInt();
        emit logLevelPublished(category, logLevel);
    }
}

void LogToViewerHandler::timerEvent(QTimerEvent *event)
{
    if (event->timerId() == detectLogBufferTimerID)
    {
        emit detectLogBuffer();
    }
}

void LogToViewerHandler::appendCategory(QString category)
{
    auto logBuffer = new LogBuffer;
    auto logModel = new LogModel;
    connect(this, &LogToViewerHandler::detectLogBuffer, logBuffer, &LogBuffer::onDetectBuffer);
    connect(logBuffer, &LogBuffer::logBufferChanged, logModel, &LogModel::onLogBufferChanged);
    logBuffers[category] = logBuffer;
    logModels[category] = logModel;
}
