#ifndef SETTINGS_H
#define SETTINGS_H

#include <DtkWidgets>
#include <qsettingbackend.h>
#include <QSettings>
#include <QPointer>

DWIDGET_USE_NAMESPACE
DCORE_USE_NAMESPACE
DTK_USE_NAMESPACE

class Settings : public QObject
{
    Q_OBJECT

public:
    Settings(QWidget *parent = 0);
    ~Settings();

    void dtkThemeWorkaround(QWidget *parent, const QString &theme);
    static QWidget *createFontComBoBoxHandle(QObject *obj);

    int defaultFontSize = 12;
    int maxFontSize = 50;
    int minFontSize = 8;

    DSettings *settings;

signals:
    void adjustFont(QString name);
    void adjustFontSize(int fontSize);
    void adjustTabSpaceNumber(int number);
    void adjustWordWrap(bool enable);
    void themeChanged(const QString &theme);

private:
    void updateAllKeysWithKeymap(QString keymap);
    void copyCustomizeKeysFromKeymap(QString keymap);

private:
    Dtk::Core::QSettingBackend *m_backend;

    QString m_configPath;
    bool m_userChangeKey = false;
};

#endif // SETTINGS_H
