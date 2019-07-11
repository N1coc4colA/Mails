#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "chilkat/include/CkImap.h"
#include "mailbutton.h"
#include "settings.h"

#include <CkGlobal.h>
#include <CkEmail.h>
#include <CkMessageSet.h>
#include <CkEmailBundle.h>

#include <string>
#include <iostream>
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
#include <QListWidgetItem>
#include <sstream>
#include <QStyleFactory>
#include <DSettingsDialog>
#include <DThemeManager>
#include <DSettingsWidgetFactory>

DWIDGET_USE_NAMESPACE

MainWindow::MainWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MainWindow)
{   
    startUnlockBundleofChilkat();

    ui->setupUi(this);

    loop = 0;

    ui->bar->setBlendMode(DBlurEffectWidget::BehindWindowBlend);
    ui->bar->setMaskColor(QColor(0, 0, 0, 90));
    ui->settings->setBlendMode(DBlurEffectWidget::BehindWindowBlend);
    ui->settings->setMaskColor(QColor(0, 0, 0, 90));
    ui->MailShower->setVisible(false);
    ui->settings->setVisible(false);
    ui->settings->setFixedWidth(300);
    ui->m_blurEffect->setVisible(true);

    ui->listView->horizontalScrollBar()->setDisabled(true);
    ui->listView->verticalScrollBar()->setStyleSheet("border: 0px;background: grey;margin: 0px 0px 0 0px;");

    ui->msg->setStyleSheet("background-color: white;background-image: url(:/msg_bg.jpg);background-repeat: no-repeat; background-position: bottom right; border-width: 2.5px; border-radius: 3px; border-color: grey;");

    connect(ui->browseBtn, SIGNAL(clicked()), this, SLOT(browse()));
    connect(ui->setBtn, SIGNAL(clicked()), this, SLOT(showSettings()));
    connect(ui->closeSettings, SIGNAL(clicked()), this, SLOT(showSettings()));
    connect(ui->runTest, SIGNAL(clicked()), this, SLOT(runCheck()));
    connect(this, SIGNAL(initCheck(QString)), this, SLOT(ChilkatSample(QString)));

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

    QMenu *m_menu = new QMenu;
    titleb->setMenu(m_menu);
    QAction *settingAction(new QAction(tr("Settings"), this));
    m_menu->addAction(settingAction);
    m_menu->setStyle(QStyleFactory::create("dlight"));

    connect(settingAction, &QAction::triggered, this, &MainWindow::popupSettingsDialog);

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

    expand = new QPropertyAnimation(ui->listView, "geometry");
    expand->setDuration(1000);
    expand->setStartValue(ui->listView->geometry());
    expand->setEndValue(QRect(10, 220, ui->bar->width() - 20, ui->bar->height() - 110 - 10));
    expand->start();

    QWidget *buttonsArea = new QWidget(titleb);
    buttonsArea->setFixedHeight(40);

    QHBoxLayout *buttons = new QHBoxLayout;

    QPushButton *nSMTP = new QPushButton();
    QPushButton *sSMTP = new QPushButton();

        connect(nSMTP, SIGNAL(clicked()), this, SLOT(sendMail()));
        sSMTP->setStyleSheet("border-image: url(:/send.png) 0 0 0 0 stretch stretch;");
        sSMTP->setFixedSize(32, 32);
        buttons->addWidget(sSMTP);
        buttons->addSpacing(4);
        connect(nSMTP, SIGNAL(clicked()), this, SLOT(showSMTPArea()));
        nSMTP->setStyleSheet("border-image: url(:/new.png) 0 0 0 0 stretch stretch;");
        nSMTP->setFixedSize(32, 32);
        buttons->addWidget(nSMTP);
        buttons->addSpacing(4);
    buttons->setMargin(4);
    buttons->addStretch();
    buttonsArea->setLayout(buttons);
}

void MainWindow::loadSettings()
{
    QSettings settings("TEDMOL", "Mails");
    QCoreApplication::setOrganizationName("TEDMOL");
    QCoreApplication::setOrganizationDomain("n1coc4cola@gmail.com");
    QCoreApplication::setApplicationName("Mail");
    settings.beginGroup("Internal");
    settings.setValue("Position", this->geometry());
}

void MainWindow::saveSettings()
{

}

void MainWindow::popupSettingsDialog()
{
    DSettingsDialog *dialog = new DSettingsDialog(this);
    Settings *m_settings = new Settings;

    dialog->exec();
    delete dialog;
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
    connect(smtp, SIGNAL(mailSent()), this, SLOT(mailSent()));
}

void MainWindow::mailSent()
{
    makeClean();
}

void MainWindow::makeClean()
{
    ui->file->clear();
    ui->rcpt->clear();
    ui->subject->clear();
    ui->msg->clear();
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

        expand = new QPropertyAnimation(ui->widget_4, "geometry");
        expand->setDuration(700);
        expand->setStartValue(ui->widget_4->geometry());
        expand->setEndValue(QRect(10, 270, ui->bar->width() - 20, 130));
        expand->start();

        expand = new QPropertyAnimation(ui->widget_5, "geometry");
        expand->setDuration(700);
        expand->setStartValue(ui->widget_5->geometry());
        expand->setEndValue(QRect(10, 190, ui->bar->width() - 20, 70));
        expand->start();
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::startUnlockBundleofChilkat()
{
    CkGlobal glob;
    bool success = glob.UnlockBundle("Anything for 30-day trial");
    if (success != true) {
        std::cout << glob.lastErrorText() << "\r\n";
        return;
    }

    int status = glob.get_UnlockStatus();
    if (status == 2) {
        std::cout << "Unlocked using purchased unlock code." << "\r\n";
    }

    else {
        std::cout << "Unlocked in trial mode." << "\r\n";
    }

    // The LastErrorText can be examined in the success case to see if it was unlocked in
    // trial more, or with a purchased unlock code.
    std::cout << glob.lastErrorText() << "\r\n";
}

void MainWindow::ChilkatSample(QString magik)
    {
    CkImap imap;
    qDebug() << "Running IMAP";

    // This example requires the Chilkat API to have been previously unlocked.
    // See Global Unlock Sample for sample code.

    // Connect to an IMAP server.
    // Use TLS
    imap.put_Ssl(true);
    imap.put_Port(993);
    bool success = imap.Connect(ui->lineEdit_3->text().toStdString().c_str());
    if (success != true) {
        std::cout << imap.lastErrorText() << "\r\n";
        return;
        qDebug() << "Connection to IMAP successfull";
    }

    // Login
    success = imap.Login(ui->uname->text().toStdString().c_str(),ui->paswd->text().toStdString().c_str());
    if (success != true) {
        std::cout << imap.lastErrorText() << "\r\n";
        return;
        qDebug() << "Login successfull";
    }

    // Select an IMAP mailbox
    success = imap.SelectMailbox("Inbox");
    if (success != true) {
        std::cout << imap.lastErrorText() << "\r\n";
        return;
        qDebug() << "Opening Mailbox (inbox) was successfull";
    }

    // Once the mailbox is selected, the NumMessages property
    // will contain the number of messages in the mailbox.
    // You may loop from 1 to NumMessages to
    // fetch each message by sequence number.

    // We can choose to fetch UIDs or sequence numbers.
    bool fetchUids = true;
    // Get the message IDs of all the emails in the mailbox
    CkMessageSet *messageSet = imap.Search("ALL",fetchUids);
    if (imap.get_LastMethodSuccess() == false) {
        std::cout << imap.lastErrorText() << "\r\n";
        return;
    }

    // Fetch the emails into a bundle object:
    CkEmailBundle *bundle = imap.FetchBundle(*messageSet);
    if (imap.get_LastMethodSuccess() == false) {
        delete messageSet;
        std::cout << imap.lastErrorText() << "\r\n";
        return;
    }

    // Loop over the bundle and display the FROM and SUBJECT of each.
    int i = 0;
    int numEmails = bundle->get_MessageCount();
    while (i < numEmails) {
        CkEmail *email = bundle->GetEmail(i);

        MailButton *mail  = new MailButton(this);
        mail->setStyleSheet("border-radius: 5px; background-color: rgba(0, 0, 0, 0); margin-bottom: 1px;");
        mail->setFixedHeight(70);
        mail->setFixedWidth(ui->listView->width() - 15);

        QVBoxLayout *mLayout = new QVBoxLayout;

        std::cout << email->ck_from() << "\r\n";
        QLabel *FROM = new QLabel;
        FROM->setText(email->ck_from());
        FROM->setStyleSheet("font-size: 15px; color: white; background: #00000000;");
        FROM->setMargin(0);
        FROM->setFixedHeight(30);

        std::cout << email->subject() << "\r\n";
        QLabel *SUBJECT = new QLabel;
        SUBJECT->setText(email->subject());
        SUBJECT->setStyleSheet("font-size: 14px; color: white; background: #00000000;");
        SUBJECT->setMargin(0);
        SUBJECT->setFixedHeight(20);

        std::cout << "--" << "\r\n";

        mLayout->addWidget(FROM);
        mLayout->addWidget(SUBJECT);
        mail->setLayout(mLayout);

        QListWidgetItem *item = new QListWidgetItem;
        item->setSizeHint(mail->sizeHint());

        emit transfer(QString(email->body()));
        emit askForRename(i);

        connect(this, SIGNAL(askForRename(int)), this, SLOT(handleRenameAsk(int)));
        connect(this, SIGNAL(transfer(QString)), mail, SLOT(handleMessage(QString)));
        connect(mail, SIGNAL(Processed(QString)), this, SLOT(MessageFallBack(QString)));

        mail->setObjectName(magik);

        ui->listView->addItem(item);
        ui->listView->setItemWidget(item, mail);

        delete email;
        i = i + 1;
    }

    // Disconnect from the IMAP server.
    success = imap.Disconnect();
    qDebug() << "The server connection was close, all the code was read properly, and as expected, if no error was print.";
}

void MainWindow::handleRenameAsk(int i)
{
    emit replyForRenameAsk(QString("mail" + QString::number(i)));
}

void MainWindow::runCheck()
{
    emit initCheck(QString("mail0"));
}

void MainWindow::MessageFallBack(QString Message)
{
    ui->MailShower->setVisible(true);
    ui->MailCont->setPlainText(Message);
    ui->m_blurEffect->setVisible(false);
    internLayout->removeWidget(ui->m_blurEffect);
    internLayout->addWidget(ui->MailShower);
    this->setLayout(mainLayout);
}

void MainWindow::showSMTPArea()
{
    ui->MailShower->setVisible(false);
    ui->m_blurEffect->setVisible(true);
    internLayout->removeWidget(ui->MailShower);
    internLayout->addWidget(ui->m_blurEffect);
    this->setLayout(mainLayout);
}
