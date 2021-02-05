#include "loghelper.h"

SILICOOL_CREATE_LOGGING_CATEGORY(timerCate, "Timer")

SCTimer::SCTimer(const QString &msg, bool showLog) : m_msg(msg), m_showLog(showLog)
{
    timer.start();
}

SCTimer::SCTimer(const QString &msg, const QLoggingCategory &logCate, bool showLog)
    : m_msg(msg), m_logCate(&logCate), m_showLog(showLog)
{
    timer.start();
}

SCTimer::~SCTimer()
{
    if (m_showLog)
    {
        if (m_logCate == nullptr)
        {
            qCDebug(timerCate()) << QString("%1 Timecost: %2ms").arg(m_msg).arg(timer.elapsedMs());
        }
        else
        {
            qCDebug((*m_logCate)) << QString("%1 Timecost: %2ms").arg(m_msg).arg(timer.elapsedMs());
        }
    }
}
