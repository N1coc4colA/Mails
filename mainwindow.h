#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <DMainWindow>
#include "smtp.h"
#include <DBlurEffectWidget>
#include <QHBoxLayout>
#include <QFrame>
#include <QPropertyAnimation>
#include <DTitlebar>
#include <QListWidget>

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
    void ChilkatSample(void);
    void startUnlockBundleofChilkat();
    void MessageFallBack(const char *, int);
    void makeClean();
    void showSMTPArea();
    void popupSettingsDialog();
    void runCheck();
    void loadSettings();
    void saveSettings();
    void on_runTest_clicked();

signals:
    void transfer(QString);
    void askForRename(int i);
    void askForNumber(int i);
    void replyForRenameAsk(QString magik);
    void replyForNumberAsk(int k);
    void initCheck(QString magik);
    void startConnect();
    void MessageSender(const char *, int);

public:
    Ui::MainWindow *ui;
    QStringList files;
    QString port;
    QString uname;
    QString paswd;
    QString server;
    QVBoxLayout *internLayout;
    QHBoxLayout *mainLayout;
    QPropertyAnimation *showSMTP;
    QPushButton *sSMTP;
};

#endif // MAINWINDOW_H
