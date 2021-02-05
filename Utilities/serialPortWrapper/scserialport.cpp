#include "scserialport.h"

SCSerialPort::~SCSerialPort()
{
    if (serialPort != nullptr)
    {
        if (serialPort->isOpen())
        {
            serialPort->close();
        }
        delete serialPort;
        serialPort = nullptr;
    }
}

void SCSerialPort::initSerialPort()
{
    if (config == nullptr)
    {
        qFatal("Please set config first! %s", deviceName.toUtf8().data());
    }
    if (isInitSerialPort)
    {
        return;
    }
    if (serialPort == nullptr)
    {
        serialPort = new QSerialPort;
    }
    if (serialPort->isOpen())
    {
        serialPort->close();
    }
    serialPort->setPortName(config->portName());
    serialPort->setBaudRate(config->baudrate());
    if (!serialPort->open(QIODevice::ReadWrite))
    {
        throw SilicolAbort(QObject::tr("Open serial port failed! Device: %1 Port: %2, errMsg: %3")
                               .arg(deviceName)
                               .arg(config->portName())
                               .arg(serialPort->error()));
    }
    serialPort->clear();
    serialPort->clearError();
    isInitSerialPort = true;
}

void SCSerialPort::writeData(const QByteArray &data)
{
    if (!isInitSerialPort || !serialPort->isOpen())
    {
        throw GeneralError(deviceName, QObject::tr("SerialPort is not opened!"));
    }
    if(config->writeDataMinInterval() > 0)
    {
        qint64 writeInterval = QDateTime::currentMSecsSinceEpoch() - lastWriteDataTime;
        if(writeInterval < config->writeDataMinInterval())
        {
            QThread::msleep(config->writeDataMinInterval() - writeInterval);
        }
    }
    serialPort->write(data);
    serialPort->flush();
    if (!serialPort->waitForBytesWritten(1000))
    {
        throw GeneralError(deviceName, QObject::tr("Wait for writing data timeout!"));
    }
    lastWriteDataTime = QDateTime::currentMSecsSinceEpoch();
}

void SCSerialPort::clearReadBuffer()
{
    remainedResponse.clear();
    if(serialPort != nullptr && serialPort->isOpen())
    {
        serialPort->clear(QSerialPort::Input);
    }
}

QByteArray SCSerialPort::readData(const QString &endMark, int timeout)
{
    if (!isInitSerialPort || !serialPort->isOpen())
    {
        throw GeneralError(deviceName, QObject::tr("SerialPort is not opened!"));
    }
    QByteArray response(remainedResponse);
    QElapsedTimer timer;
    timer.start();
    while (response.indexOf(endMark) < 0)
    {
        if (timeout > 0 && timer.elapsed() > timeout)
        {
            throw GeneralError(deviceName, QObject::tr("Read data timeout!"));
        }
        if (serialPort->waitForReadyRead(30))
        {
            response += serialPort->readAll();
        }
    }
    int dataLen = response.indexOf(endMark) + endMark.toUtf8().length();
    if (dataLen < response.length())
    {
        remainedResponse = response.mid(dataLen);
        return response.mid(0, dataLen);
    }
    else
    {
        return response;
    }
}

QByteArray SCSerialPort::readData(int length, int timeout)
{
    if (!isInitSerialPort || !serialPort->isOpen())
    {
        throw GeneralError(deviceName, QObject::tr("SerialPort is not opened!"));
    }
    QByteArray response(remainedResponse);
    QElapsedTimer timer;
    timer.start();
    while (response.length() < length)
    {
        if (timeout > 0 && timer.elapsed() > timeout)
        {
            throw GeneralError(deviceName, QObject::tr("Read data timeout!"));
        }
        if (serialPort->waitForReadyRead(30))
        {
            response += serialPort->readAll();
        }
    }
    if (response.length() > length)
    {
        remainedResponse = response.mid(length);
        return response.mid(0, length);
    }
    else
    {
        return response;
    }
}
