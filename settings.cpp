#include "settings.h"

#include "dthememanager.h"
#include <DSettings>
#include <DSettingsGroup>
#include <DSettingsWidgetFactory>
#include <DSettingsOption>
#include <QStyleFactory>
#include <QFontDatabase>
#include <QApplication>
#include <QComboBox>
#include <QDebug>
#include <QDir>
#include <QStandardPaths>

Settings::Settings(QWidget *parent)
        : QObject(parent)
{
    DBlurEffectWidget *test = new DBlurEffectWidget;
    test->setBlendMode(DBlurEffectWidget::BlendMode::BehindWindowBlend);
    test->setFixedSize(200, 200);
}
Settings::~Settings()
{
}
