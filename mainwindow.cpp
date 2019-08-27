#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "chilkat/include/CkImap.h"
#include "mailbutton.h"
#include "settings.h"
#include "dtitlebar.h"
#include "beta.h"

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
    ui->bar->setMaskColor(QColor(0, 0, 0, 100));
    ui->bar->setRadius(60);
    ui->bar->setRadius(400);
    ui->bar->setFixedWidth(300);
    ui->bar->setRadius(90);
    ui->settings->setBlendMode(DBlurEffectWidget::BehindWindowBlend);
    ui->settings->setMaskColor(QColor(0, 0, 0, 100));
    ui->MailShower->setVisible(false);
    ui->settings->setVisible(false);
    ui->settings->setFixedWidth(300);
    ui->m_blurEffect->setVisible(true);

    ui->listWidget->horizontalScrollBar()->setDisabled(true);
    ui->listWidget->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->listWidget->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->listWidget->verticalScrollBar()->setStyleSheet("border: 0px;margin: 0px 0px 0 0px;");

    ui->msg->setStyleSheet("background-color: white;background-image: url(:/msg_bg.jpg);background-repeat: no-repeat; background-position: bottom right; border-width: 2.5px; border-radius: 3px; border-color: grey;");

    connect(ui->browseBtn, SIGNAL(clicked()), this, SLOT(browse()));
    connect(ui->setBtn, SIGNAL(clicked()), this, SLOT(showSettings()));
    connect(ui->closeSettings, SIGNAL(clicked()), this, SLOT(showSettings()));
    connect(ui->runTest, SIGNAL(clicked()), this, SLOT(ChilkatSample()));

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
    titleb->setTitle("");

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

    QWidget *buttonsArea = new QWidget(titleb);
    buttonsArea->setFixedHeight(40);

    QHBoxLayout *buttons = new QHBoxLayout;

    QPushButton *nSMTP = new QPushButton();
    sSMTP = new QPushButton();

    connect(sSMTP, SIGNAL(clicked()), this, SLOT(sendMail()));
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

    ui->listView_2->setSpacing(5);

    qDebug() << "Every thing is setup.";
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

void MainWindow::ChilkatSample(void)
    {
    CkImap imap;
    qDebug() << "Running IMAP";

    // This example requires the Chilkat API to have been previously unlocked.
    // See Global Unlock Sample for sample code.

    // Connect to an IMAP server.
    // Use TLS
    imap.put_Ssl(true);
    imap.put_Port(ui->lineEdit_4->text().toInt());
    qDebug() << "Connection to IMAP";
    bool success = imap.Connect(ui->lineEdit_3->text().toStdString().c_str());
    if (success != true) {
        std::cout << imap.lastErrorText() << "\r\n";
        return;
    }

    // Login
    success = imap.Login(ui->uname->text().toStdString().c_str(),ui->paswd->text().toStdString().c_str());
    qDebug() << "Connection to account";
    if (success != true) {
        std::cout << imap.lastErrorText() << "\r\n";
        return;
    }

    // Select an IMAP mailbox
    qDebug() << "Connection to the Mailbox 'INBOX'";
    const char *BOX = "Inbox";
    success = imap.SelectMailbox(BOX);
    if (success != true) {
        std::cout << imap.lastErrorText() << "\r\n";
        return;
    }

    // Once the mailbox is selected, the NumMessages property
    // will contain the number of messages in the mailbox.
    // You may loop from 1 to NumMessages to
    // fetch each message by sequence number.

    // We can choose to fetch UIDs or sequence numbers.
    bool fetchUids = true;

    // Get the message IDs of all the emails in the mailbox
    qDebug() << "Getting data about emails";
    CkMessageSet *messageSet = imap.Search("ALL",fetchUids);
    if (imap.get_LastMethodSuccess() == false) {
        std::cout << imap.lastErrorText() << "\r\n";
        return;
    }

    // Fetch the emails into a bundle object:
    qDebug() << "Fetching emails";
    CkEmailBundle *bundle = 0;
    bundle = imap.FetchBundle(*messageSet);
    if (imap.get_LastMethodSuccess() == false) {
        delete messageSet;
        std::cout << imap.lastErrorText() << "\r\n";
        return;
    }

    qDebug() << "Setting settings";
    int i = 0;
    qDebug() << "Getting number of messages";

    int numEmails = bundle->get_MessageCount();
    // Creating the objects of the fetched mails, this way slow the app during downloading

    qDebug() <<"A hard step...";
    QVector < QPushButton *  > pEdits( numEmails, nullptr );
    pEdits.clear();
    qDebug() <<"Starting Downloading data";

    while (i < numEmails) {
        CkEmail *email = bundle->GetEmail(i);

        qDebug() <<"Downloading Email";

        QWidget *trytouse = new QWidget;
        trytouse->setFixedHeight(70);
        trytouse->setFixedWidth(285);
        trytouse->setStyleSheet("background: rgba(255, 255, 255, 20);");

        QPushButton *mail  = new QPushButton();
        mail->setFixedHeight(70);
        mail->setFixedWidth(285);
        mail->setText(QString(QString(email->fromName()) + "\r\n" + email->subject()));

        qDebug() <<"try connecting a certain slot.";

        pEdits[i] = mail;
        //
        QString FROM = QString(email->fromAddress());
        QString SUBJECT = QString(email->subject());
        QString CONTENT = QString(email->body());
        connect(pEdits[i], &QPushButton::clicked, [=] (){
            MessageFallBack(BOX, i);
        });

        QListWidgetItem *item = new QListWidgetItem();
        item->setSizeHint(QSize(mail->width(), mail->height()));

        ui->listWidget->addItem(item);
        ui->listWidget->setItemWidget(item, trytouse);

        mail->setFlat(true);
        mail->setStyleSheet("* { background-color: rgba(0,125,0,0) }");
        QVBoxLayout *LL = new QVBoxLayout;
        LL->addWidget(mail);
        LL->setMargin(0);
        LL->setSpacing(0);
        trytouse->setLayout(LL);

        delete email;
        i = i + 1;
    }

    // Disconnect from the IMAP server.
    success = imap.Disconnect();
    qDebug() << "The server connection was close, all the code was read properly, and as expected, if no error was print.";
}

void MainWindow::runCheck()
{
    emit initCheck(QString("mail0"));
}

void MainWindow::MessageFallBack(const char *BOX, int i)
{
    if (ui->settings->isVisible() == true)
    {
    } else {
        ui->settings->setVisible(true);
        ui->bar->setVisible(false);
    }

    CkImap imap;
    qDebug() << "Running IMAP";

    imap.put_Ssl(true);
    imap.put_Port(ui->lineEdit_4->text().toInt());
    qDebug() << "Connection to IMAP";
    bool success = imap.Connect(ui->lineEdit_3->text().toStdString().c_str());
    if (success != true) {
        std::cout << imap.lastErrorText() << "\r\n";
        return;
    }

    success = imap.Login(ui->uname->text().toStdString().c_str(),ui->paswd->text().toStdString().c_str());
    qDebug() << "Connection to account";
    if (success != true) {
        std::cout << imap.lastErrorText() << "\r\n";
        return;
    }

    qDebug() << "Connection to the Mailbox";
    success = imap.SelectMailbox(BOX);
    if (success != true) {
        std::cout << imap.lastErrorText() << "\r\n";
        return;
    }

    bool fetchUids = true;

    qDebug() << "Getting data about emails";
    CkMessageSet *messageSet = imap.Search("ALL",fetchUids);
    if (imap.get_LastMethodSuccess() == false) {
        std::cout << imap.lastErrorText() << "\r\n";
        return;
    }

    qDebug() << "Fetching emails";
    CkEmailBundle *bundle = 0;
    bundle = imap.FetchBundle(*messageSet);
    if (imap.get_LastMethodSuccess() == false) {
        delete messageSet;
        std::cout << imap.lastErrorText() << "\r\n";
        return;
    }

    CkEmail *email = bundle->GetEmail(i);
    QString FROM = QString(email->fromAddress());
    QString SUBJECT = QString(email->subject());
    QString CONTENT = QString(email->body());

    ui->MailShower->setVisible(true);
    ui->MailCont->clear();
    ui->FROM->setText(FROM);
    ui->SUBJECT->setText(SUBJECT);
    ui->MailCont->setPlainText(CONTENT);
    ui->m_blurEffect->setVisible(false);
    ui->settings->setVisible(false);
    ui->bar->setVisible(true);
    sSMTP->setVisible(false);
    internLayout->addWidget(ui->MailShower);
    this->setLayout(mainLayout);
}

void MainWindow::showSMTPArea()
{
    ui->MailShower->setVisible(false);
    ui->m_blurEffect->setVisible(true);
    sSMTP->setVisible(true);
    internLayout->removeWidget(ui->MailShower);
    internLayout->addWidget(ui->m_blurEffect);
    this->setLayout(mainLayout);
}

void MainWindow::on_runTest_clicked()
{
    qDebug() <<"Running IMAP button was clicked.";
}
