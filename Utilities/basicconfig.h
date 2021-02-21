#ifndef BASICCONFIG_H
#define BASICCONFIG_H

#include "languageManager/languageconfig.h"
#include "utilities_global.h"

class UTILITIESSHARED_EXPORT BasicConfig : public ConfigObject
{
    Q_OBJECT

public:
    enum ChartTheme
    {
        ChartThemeLight = 0,
        ChartThemeBlueCerulean,
        ChartThemeDark,
        ChartThemeBrownSand,
        ChartThemeBlueNcs,
        ChartThemeHighContrast,
        ChartThemeBlueIcy,
        ChartThemeQt
    };
    Q_ENUM(ChartTheme)

    Q_PROPERTY(LanguageConfig *languageConfig READ languageConfig)
    Q_PROPERTY(QString skin READ skin WRITE setSkin NOTIFY skinChanged)
    Q_PROPERTY(QString dutType READ dutType WRITE setDutType NOTIFY dutTypeChanged)
    Q_PROPERTY(QString lastInputUser READ lastInputUser WRITE setLastInputUser NOTIFY lastInputUserChanged)
    Q_PROPERTY(ChartTheme chartTheme READ chartTheme WRITE setChartTheme NOTIFY chartThemeChanged)

public:
    BasicConfig(QObject *parent = nullptr) : ConfigObject(parent)
    {
        m_languageConfig = new LanguageConfig(this);
        init();
    }

    LanguageConfig *languageConfig() const
    {
        return m_languageConfig;
    }

    QString dutType() const
    {
        return m_dutType;
    }

    QString skin() const
    {
        return m_skin;
    }

    QString lastInputUser() const
    {
        return m_lastInputUser;
    }

    ChartTheme chartTheme() const
    {
        return m_chartTheme;
    }

public slots:
    void setDutType(QString dutType)
    {
        if (m_dutType == dutType)
            return;

        m_dutType = dutType;
        emit dutTypeChanged(m_dutType);
    }

    void setSkin(QString skin)
    {
        if (m_skin == skin)
            return;

        m_skin = skin;
        emit skinChanged(m_skin);
    }

    void setLastInputUser(QString lastInputUser)
    {
        if (m_lastInputUser == lastInputUser)
            return;

        m_lastInputUser = lastInputUser;
        emit lastInputUserChanged(m_lastInputUser);
    }

    void setChartTheme(ChartTheme chartTheme)
    {
        if (m_chartTheme == chartTheme)
            return;

        m_chartTheme = chartTheme;
        emit chartThemeChanged(m_chartTheme);
    }

signals:
    void dutTypeChanged(QString dutType);

    void skinChanged(QString skin);

    void lastInputUserChanged(QString lastInputUser);

    void chartThemeChanged(ChartTheme chartTheme);

private:
    LanguageConfig *m_languageConfig;
    QString m_dutType = "DefaultDUT";
    QString m_skin;
    QString m_lastInputUser;
    ChartTheme m_chartTheme{ ChartThemeLight };
};

#endif    // BASICCONFIG_H
