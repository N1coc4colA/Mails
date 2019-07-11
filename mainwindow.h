#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <DMainWindow>
#include "smtp.h"
#include <DBlurEffectWidget>
#include <QtWidgets/QHBoxLayout>
#include <QFrame>
#include <QPropertyAnimation>
#include <DTitlebar>

DWIDGET_USE_NAMESPACE

typedef struct
{
} MyStruct;

Q_DECLARE_METATYPE(MyStruct);

namespace Ui {
class MainWindow;
}

class MainWindow : public QWidget
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    bool loop;

private slots:
    void sendMail();
    void mailSent();
    void browse();
    void showSettings();
    void ChilkatSample(QString magik);
    void startUnlockBundleofChilkat();
    void MessageFallBack(QString Message);
    void makeClean();
    void showSMTPArea();
    void popupSettingsDialog();
    void handleRenameAsk(int i);
    void runCheck();
    void loadSettings();
    void saveSettings();

    signals:
    void transfer(QString);
    void askForRename(int i);
    void replyForRenameAsk(QString magik);
    void initCheck(QString magik);

public:
    Ui::MainWindow *ui;
    QStringList files;
    QString port;
    QString uname;
    QString paswd;
    QString server;
    QVBoxLayout *internLayout;
    QHBoxLayout *mainLayout;
    QPropertyAnimation *expand;
    QPropertyAnimation *showSMTP;
};

#endif // MAINWINDOW_H
