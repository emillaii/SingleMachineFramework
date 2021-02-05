#include "scserialportimpl.h"

SCSerialPortImpl::~SCSerialPortImpl()
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

void SCSerialPortImpl::initSerialPort() {}

void SCSerialPortImpl::writeData(const QByteArray &data) {}

void SCSerialPortImpl::clearReadBuffer()
{
    readBuffer.clear();
    if (serialPort != nullptr && serialPort->isOpen())
    {
        serialPort->clear(QSerialPort::Input);
    }
}

QByteArray SCSerialPortImpl::readData(const QString &endMark, int timeout)
{
    if (!isInitSerialPort || !serialPort->isOpen())
    {
        throw GeneralError(deviceName, QObject::tr("SerialPort is not opened!"));
    }
    QByteArray response(readBuffer);
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
        readBuffer = response.mid(dataLen);
        return response.mid(0, dataLen);
    }
    else
    {
        return response;
    }
}

QByteArray SCSerialPortImpl::readData(int length, int timeout)
{
    if (!isInitSerialPort || !serialPort->isOpen())
    {
        throw GeneralError(deviceName, QObject::tr("SerialPort is not opened!"));
    }
    QByteArray response(readBuffer);
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
        readBuffer = response.mid(length);
        return response.mid(0, length);
    }
    else
    {
        return response;
    }
}
