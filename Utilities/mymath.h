#ifndef MYMATH_H
#define MYMATH_H

#include "utilities_global.h"
#include <QPointF>
#include <QVector>

template <typename T>
T myAve(QList<T> &data)
{
    if (data.length() == 0)
    {
        return 0;
    }
    T sum = 0;
    for (int i = 0; i < data.length(); i++)
    {
        sum += data[i];
    }
    return sum / data.length();
}

template <typename T>
T myMax(QList<T> &data)
{
    if (data.length() == 0)
    {
        qFatal("Can not get the max value for the reason that data length is zero!");
    }
    T maxData = data[0];
    for (int i = 1; i < data.length(); i++)
    {
        if (data[i] > maxData)
        {
            maxData = data[i];
        }
    }
    return maxData;
}

template <typename T>
T myMin(QList<T> &data)
{
    if (data.length() == 0)
    {
        qFatal("Can not get the min value for the reason that data length is zero!");
    }
    T minData = data[0];
    for (int i = 1; i < data.length(); i++)
    {
        if (data[i] < minData)
        {
            minData = data[i];
        }
    }
    return minData;
}

bool UTILITIESSHARED_EXPORT fitCircle(const QVector<QPointF> &points, QPointF &center, double &radius);

bool UTILITIESSHARED_EXPORT fitLine(const QVector<QPointF> &points, double &a, double &b, double &c);

#endif    // MYMATH_H
