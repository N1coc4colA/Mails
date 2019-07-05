#include "mainwindow.h"
#include "beta.h"
#include <DMainWindow>
#include <DApplication>
#include <DMainWindow>
#include <QMainWindow>
#include <DAboutDialog>
#include <QString>
#include <QTranslator>
#include <QLibraryInfo>

int main(int argc, char *argv[])
{
    DApplication::loadDXcbPlugin();
    DApplication a(argc, argv);

    a.setProductName("Mails");
    a.setOrganizationName(QString(QObject::tr("TISP Way of Life(The It's Sweety Project) by χ (Chi)")));
    a.setApplicationName("Mails");
    a.setApplicationVersion("1.β");
    a.setAttribute(Qt::AA_UseHighDpiPixmaps);
    a.setProductIcon(QIcon("logo.png"));
    a.setApplicationDisplayName(QObject::tr("Mails"));
    a.setApplicationDescription(QString(QObject::tr("Cette application est une bêta de services mail pour dde-desktop (Deepin).")));
    const QString acknowledgementLink = "https://github.com/N1coc4colA/";
    a.setApplicationAcknowledgementPage(acknowledgementLink);

    QTranslator qtTranslator;

    qtTranslator.load("qt_" + QLocale::system().name(),
            QLibraryInfo::location(QLibraryInfo::TranslationsPath));
    a.installTranslator(&qtTranslator);

    QTranslator myappTranslator;
    myappTranslator.load("translation/mails_" + QLocale::system().name());
    a.installTranslator(&myappTranslator);

    beta bta;

    MainWindow k;
    k.show();

    bta.show();
    bta.resize(300, 60);

    return a.exec();
}
