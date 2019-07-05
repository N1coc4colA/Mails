#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <DMainWindow>
#include "smtp.h"
#include <DBlurEffectWidget>
#include <QtWidgets/QHBoxLayout>
#include <QFrame>

DWIDGET_USE_NAMESPACE

namespace Ui {
class MainWindow;
}

class MainWindow : public QWidget
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void sendMail();
    void mailSent();
    void browse();
    void showSettings();

public:
    Ui::MainWindow *ui;
    QStringList files;
    QString port;
    QString uname;
    QString paswd;
    QString server;
    QVBoxLayout *internLayout;
    QHBoxLayout *mainLayout;
};

#endif // MAINWINDOW_H
