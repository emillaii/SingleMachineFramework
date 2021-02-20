#ifndef CONSOLEOUTPUTER_H
#define CONSOLEOUTPUTER_H

#include "loglevel.h"
#include <QMutex>
#include <QMutexLocker>
#include <QTextCodec>
#include <Windows.h>
#include <iostream>

class ConsoleOutputer
{
private:
    enum ConsoleColor
    {
        White,
        Yellow,
        Red
    };

public:
    void outputLogToConsole(int logLevel, const QString &msg);

private:
    void setConsoleColor(unsigned short forecolor = 4, unsigned short backgroudcolor = 0)
    {
        HANDLE hCon = GetStdHandle(STD_OUTPUT_HANDLE);
        SetConsoleTextAttribute(hCon, forecolor | backgroudcolor);
    }

private:
    int lastConsoleColor = White;
    QMutex locker;
};

#endif    // CONSOLEOUTPUTER_H
