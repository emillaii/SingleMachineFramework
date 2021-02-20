#include "languagemanager.h"

LanguageManager::LanguageManager(LanguageConfig *languageConfig, QGuiApplication &app, QQmlApplicationEngine &engine)
    : languageConfig(languageConfig), app(app), engine(engine)
{
    connect(languageConfig, &LanguageConfig::languageChanged, this, &LanguageManager::onLanguageChanged);
}

void LanguageManager::onLanguageChanged(int language)
{
    QString suffix;
    if (language == LanguageConfig::Chinese)
    {
        suffix = "chinese.qm";
    }
    else if (language == LanguageConfig::English)
    {
        suffix = "english.qm";
    }
    else
    {
        qCritical("Unknown language: %d", language);
        return;
    }

    foreach (auto translator, installedTranslators)
    {
        app.removeTranslator(translator);
    }
    installedTranslators.clear();

    for (int i = 0; i < languageConfig->languagePackageNames()->count(); i++)
    {
        QString langPackName = QString(":/languagePackage/%1_%2").arg(languageConfig->languagePackageNames()->at(i).toString()).arg(suffix);
        QTranslator *translator = new QTranslator();
        if (translator->load(langPackName))
        {
            app.installTranslator(translator);
            installedTranslators.append(translator);
        }
        else
        {
            qCritical() << "Load language package failed!" << langPackName;
        }
    }
    engine.retranslate();
}
