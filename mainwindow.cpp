#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QAuthenticator>
#include <QDesktopServices>
#include <QDesktopWidget>
#include <QDir>
#include <QDockWidget>
#include <QFileDialog>
#include <QHeaderView>
#include <QItemSelectionModel>
#include <QKeyEvent>
#include <QMenuBar>
#include <QMessageBox>
#include <QProgressBar>
#include <QRegularExpression>
#include <QScopedPointer>
#include <QScrollBar>
#include <QSplitter>
#include <QSslError>
#include <QSslKey>
#include <QStackedWidget>
#include <QStatusBar>
#include <QTextDocument>
#include <QToolBar>
#include <QToolButton>
#include <QToolTip>
#include <QUrl>
#include <QWheelEvent>
#include <DBlurEffectWidget>
#include <DMainWindow>
#include <smtp.h>
#include <DTitlebar>
#include <DPlatformWindowHandle>
#include <dwindowclosebutton.h>
#include <dwindowminbutton.h>
#include <QFileDialog>
#include <DWindowManagerHelper>
#include <dplatformwindowhandle.h>


DWIDGET_USE_NAMESPACE

MainWindow::MainWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MainWindow)
{   
    ui->setupUi(this);

    ui->bar->setBlendMode(DBlurEffectWidget::BehindWindowBlend);
    ui->bar->setMaskColor(QColor(0, 0, 0, 90));
    ui->settings->setBlendMode(DBlurEffectWidget::BehindWindowBlend);
    ui->settings->setMaskColor(QColor(0, 0, 0, 90));
    ui->m_blurEffect->setBlendMode(DBlurEffectWidget::BehindWindowBlend);
    ui->m_blurEffect->setMaskColor(QColor(235, 235, 235, 255));

    ui->settings->setVisible(false);
    ui->settings->setFixedWidth(300);

    ui->msg->setStyleSheet("background-color: white;background-image: url(:/msg_bg.jpg);background-repeat: no-repeat; background-position: bottom right; border-width: 2.5px; border-radius: 3px; border-color: grey;");

    connect(ui->sendBtn, SIGNAL(clicked()),this, SLOT(sendMail()));
    connect(ui->browseBtn, SIGNAL(clicked()), this, SLOT(browse()));
    connect(ui->setBtn, SIGNAL(clicked()), this, SLOT(showSettings()));
    connect(ui->closeSettings, SIGNAL(clicked()), this, SLOT(showSettings()));

    QString adress = ui->uname->text();
    ui->label_14->setText(adress);
    ui->label_21->setText(adress);

    QString ssmtp = ui->server->text();
    ui->lSMTP1->setText(ssmtp);
    ui->lSMTP2->setText(ssmtp);

    QString psmtp = ui->port->text();
    ui->pSMTP1->setText(psmtp);
    ui->pSMTP2->setText(psmtp);

    DPlatformWindowHandle handle(this);
    handle.setWindowRadius(4);

    DTitlebar *titleb = new DTitlebar(this);

    ui->m_blurEffect->move(300, 40);

    mainLayout = new QHBoxLayout;
    internLayout = new QVBoxLayout;

    internLayout->setSpacing(0);
    internLayout->setContentsMargins(0, 0, 0, 0);
    internLayout->addWidget(titleb);
    internLayout->addWidget(ui->m_blurEffect);

    mainLayout->addWidget(ui->bar);
    mainLayout->addWidget(ui->settings);
    mainLayout->addLayout(internLayout);
    mainLayout->setSpacing(0);
    mainLayout->setMargin(0);

    this->setLayout(mainLayout);

}

void MainWindow::browse()
{
    files.clear();

    QFileDialog dialog(this);
    dialog.setDirectory(QDir::homePath());
    dialog.setFileMode(QFileDialog::ExistingFiles);

    if (dialog.exec())
        files = dialog.selectedFiles();

    QString fileListString;
    foreach(QString file, files)
        fileListString.append( "\"" + QFileInfo(file).fileName() + "\" " );

    ui->file->setText( fileListString );

}

void MainWindow::sendMail()
{
    Smtp* smtp = new Smtp(ui->uname->text(), ui->paswd->text(), ui->server->text(), ui->port->text().toInt());
    connect(smtp, SIGNAL(status(QString)), this, SLOT(mailSent(QString)));

    if( !files.isEmpty() )
        smtp->sendMail(ui->uname->text(), ui->rcpt->text() , ui->subject->text(),ui->msg->toPlainText(), files );
    else
        smtp->sendMail(ui->uname->text(), ui->rcpt->text() , ui->subject->text(),ui->msg->toPlainText());
    ui->file->clear();
    ui->rcpt->clear();
    ui->subject->clear();
    ui->msg->clear();
}

void MainWindow::mailSent()
{
}

void MainWindow::showSettings()
{
    if (ui->settings->isVisible() == true)
    {
        ui->settings->setVisible(false);
        ui->bar->setVisible(true);
        QString adress = ui->uname->text();
        ui->label_14->setText(adress);
        ui->label_21->setText(adress);

        QString ssmtp = ui->server->text();
        ui->lSMTP1->setText(ssmtp);
        ui->lSMTP2->setText(ssmtp);

        QString psmtp = ui->port->text();
        ui->pSMTP1->setText(psmtp);
        ui->pSMTP2->setText(psmtp);
    } else {
        ui->settings->setVisible(true);
        ui->bar->setVisible(false);
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}
