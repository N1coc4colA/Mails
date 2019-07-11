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
    a.setOrganizationName(QString(QObject::tr("EDMOL (The Enhance Design Movement Of Linux)")));
    a.setApplicationName("Mails");
    a.setApplicationVersion("3.β");
    a.setAttribute(Qt::AA_UseHighDpiPixmaps);
    a.setProductIcon(QIcon("logo.png"));
    a.setApplicationDisplayName(QObject::tr("Mails"));
    a.setApplicationDescription(QString(QObject::tr("Cette application est une bêta de services mail pour dde-desktop (Deepin).")));
    const QString acknowledgementLink = "https://n1coc4cola.github.io/";
    a.setApplicationAcknowledgementPage(acknowledgementLink);
    a.setWindowIcon(QIcon(":/logo.png"));

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
