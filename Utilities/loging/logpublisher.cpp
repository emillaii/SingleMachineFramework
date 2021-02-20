#include "logpublisher.h"

LogPublisher::LogPublisher()
{
    defaultCategoryName = QLoggingCategory::defaultCategory()->categoryName();
    appendCategory(defaultCategoryName);
    appendCategory(warnLogName);
    detectLogBufferTimerID = startTimer(DetectLogBufferInterval);
}

LogPublisher::~LogPublisher()
{
    if (detectLogBufferTimerID != -1)
    {
        killTimer(detectLogBufferTimerID);
        detectLogBufferTimerID = -1;
    }
    foreach (auto logModel, logModels.values())
    {
        delete logModel;
    }
    logModels.clear();
}

QObject *LogPublisher::getLogModel(QString category)
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

void LogPublisher::onNewLog(QString category, int logLevel, QString msg)
{
    if (!logModels.contains(category))
    {
        appendCategory(category);
        emit newLogModelAttached(category);
    }
    logModels[category]->append(logLevel, msg);

    if (category != defaultCategoryName)
    {
        logModels[defaultCategoryName]->append(logLevel, msg);
    }
    if (logLevel >= LogLevel::Warning)
    {
        logModels[warnLogName]->append(logLevel, msg);
    }
}

void LogPublisher::timerEvent(QTimerEvent *event)
{
    if (event->timerId() == detectLogBufferTimerID)
    {
        foreach (auto logModel, logModels.values())
        {
            logModel->flush();
        }
    }
}
