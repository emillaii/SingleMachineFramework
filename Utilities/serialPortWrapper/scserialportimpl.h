#ifndef SCSERIALPORTIMPL_H
#define SCSERIALPORTIMPL_H

#include "configManager/configobject.h"
#include "highpricisiontimer.h"
#include <QSerialPort>

class UTILITIESSHARED_EXPORT SerialPortConfig : public ConfigObject
{
    Q_OBJECT

    Q_PROPERTY(QString portName READ portName WRITE setPortName NOTIFY portNameChanged)
    Q_PROPERTY(int baudrate READ baudrate WRITE setBaudrate NOTIFY baudrateChanged)
    Q_PROPERTY(double writeDataMinInterval READ writeDataMinInterval WRITE setWriteDataMinInterval NOTIFY writeDataMinIntervalChanged)

public:
    Q_INVOKABLE SerialPortConfig(QObject *parent = nullptr) : ConfigObject(parent)
    {
        QVariantList comNames;
        for (int i = 1; i < 30; i++)
        {
            comNames.append(QString("COM%1").arg(i));
        }
        setOptionalProperty("portName", comNames);

        QVariantList baudrates;
        baudrates << 9600 << 19200 << 38400 << 115200 << 230400 << 460800 << 921600;
        setOptionalProperty("baudrate", baudrates);

        init();
    }

    QString portName() const
    {
        return m_portName;
    }

    int baudrate() const
    {
        return m_baudrate;
    }

    double writeDataMinInterval() const
    {
        return m_writeDataMinInterval;
    }

public slots:
    void setPortName(QString portName)
    {
        if (m_portName == portName)
            return;

        m_portName = portName;
        emit portNameChanged(m_portName);
    }

    void setBaudrate(int baudrate)
    {
        if (m_baudrate == baudrate)
            return;

        m_baudrate = baudrate;
        emit baudrateChanged(m_baudrate);
    }

    void setWriteDataMinInterval(double writeDataMinInterval)
    {
        if (qFuzzyCompare(m_writeDataMinInterval, writeDataMinInterval))
            return;

        m_writeDataMinInterval = writeDataMinInterval;
        emit writeDataMinIntervalChanged(m_writeDataMinInterval);
    }

signals:
    void portNameChanged(QString portName);

    void baudrateChanged(int baudrate);

    void writeDataMinIntervalChanged(double writeDataMinInterval);

private:
    QString m_portName;
    int m_baudrate;
    double m_writeDataMinInterval = -1;
};

class SCSerialPortImpl : public QObject
{
    Q_OBJECT

public:
    explicit SCSerialPortImpl(const QString &deviceName, SerialPortConfig *config) : deviceName(deviceName), config(config) {}

    virtual ~SCSerialPortImpl();

    void initSerialPort();

    void writeData(const QByteArray &data);

    void clearReadBuffer();

    QByteArray readData(const QString &endMark, int timeout = 10000);

    QByteArray readData(int length, int timeout = 10000);

public slots:
    void onInitRep()
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
    void onWriteDataRep(const QByteArray &data)
    {
        if (timer.elapsedMs() < config->writeDataMinInterval())
        {
            timer.uSleep((timer.elapsedMs() - config->writeDataMinInterval()) * 1000);
        }
        serialPort->write(data);
        serialPort->flush();
        timer.start();
    }
    void onReadyForRead()
    {
        readBuffer += serialPort->readAll();
    }

private:
    QByteArray readBuffer;
    HighPrecisionTimer timer;
    bool isInitSerialPort = false;
    QString deviceName;
    SerialPortConfig *config = nullptr;
    QSerialPort *serialPort = nullptr;
};

#endif    // SCSERIALPORTIMPL_H
