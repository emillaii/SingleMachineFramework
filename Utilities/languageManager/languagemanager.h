#ifndef LANGUAGEMANAGER_H
#define LANGUAGEMANAGER_H

#include "../utilities_global.h"
#include "languageconfig.h"
#include <QGuiApplication>
#include <QObject>
#include <QQmlApplicationEngine>
#include <QTranslator>

class UTILITIESSHARED_EXPORT LanguageManager : public QObject
{
    Q_OBJECT

public:
    LanguageManager(LanguageConfig *languageConfig, QGuiApplication &app, QQmlApplicationEngine &engine);

private slots:
    void onLanguageChanged(int language);

private:
    LanguageConfig *languageConfig;
    QGuiApplication &app;
    QQmlApplicationEngine &engine;
    QList<QTranslator *> installedTranslators;
};

#endif    // LANGUAGEMANAGER_H
