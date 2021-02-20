#include "Logger.h"

Logger::Logger()
{
    qmsgTypeToLevel[QtDebugMsg] = LogLevel::Debug;
    qmsgTypeToLevel[QtInfoMsg] = LogLevel::Info;
    qmsgTypeToLevel[QtWarningMsg] = LogLevel::Warning;
    qmsgTypeToLevel[QtCriticalMsg] = LogLevel::Critical;
    qmsgTypeToLevel[QtFatalMsg] = LogLevel::Fatal;

    loggerConfig = new LoggerConfig;
    loggerConfigFile = new ConfigFile("LoggerConfig", loggerConfig, "./config/loggerConfig.json", false);
    loggerConfigFile->populate(true);

    logSaver = new LogSaver(loggerConfig);
    logPublisher = new LogPublisher();
    logSaver->moveToThread(&saveLogThd);
    saveLogThd.start();

    connect(&saveLogThd, &QThread::finished, logSaver, &LogSaver::deleteLater);
    connect(loggerConfig, &LoggerConfig::logLevelChanged, this, &Logger::onLogLevelChanged);

    connect(this, &Logger::newLog, logSaver, &LogSaver::onNewLog);
    connect(this, &Logger::newLog, logPublisher, &LogPublisher::onNewLog);

    qSetMessagePattern("%{function}");
    qInstallMessageHandler(qlogMessageHandler);

    registerCategory(QLoggingCategory::defaultCategory());
}

void Logger::setContextProperty(QQmlEngine &engine)
{
    engine.rootContext()->setContextProperty("$logPublisher", logPublisher);
    engine.rootContext()->setContextProperty("$loggerConfig", loggerConfig);
}

void Logger::registerCategory(QLoggingCategory *category)
{
    logCategorys.append(category);
    setLogEnable(category, loggerConfig->logLevel());
}

void Logger::qlogMessageHandler(QtMsgType msgType, const QMessageLogContext &context, const QString &msg)
{
    auto ins = getIns();

    int logLevel = ins->qmsgTypeToLevel[msgType];
    auto souceCodeFileName = QFileInfo(context.file).fileName();
    QString location = QString("%1:%2(%3)").arg(souceCodeFileName).arg(context.line).arg(qFormatLogMessage(msgType, context, msg));
    QString formatedMsg = QString("%1 [%2] [%3] %4: %5")
                              .arg(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss.zzz"))
                              .arg(context.category)
                              .arg(LogLevel::getIns().toString(logLevel))
                              .arg(location)
                              .arg(msg);

    emit ins->newLog(context.category, logLevel, formatedMsg);
    if (ins->loggerConfig->outputLogToConsole())
    {
        ins->consoleOutputer.outputLogToConsole(logLevel, formatedMsg);
    }
}

Logger::~Logger()
{
    qSetMessagePattern("%{if-category}%{category}: %{endif}%{message}");
    qInstallMessageHandler(nullptr);
    saveLogThd.quit();
    saveLogThd.wait();
}

void Logger::onLogLevelChanged(int logLevel)
{
    foreach (auto category, logCategorys)
    {
        setLogEnable(category, logLevel);
    }
}

void Logger::setLogEnable(QLoggingCategory *category, int logLevel)
{
    setLogEnable(category, QtDebugMsg, logLevel);
    setLogEnable(category, QtInfoMsg, logLevel);
    setLogEnable(category, QtWarningMsg, logLevel);
    setLogEnable(category, QtCriticalMsg, logLevel);
}
