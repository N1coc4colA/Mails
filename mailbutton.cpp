#include "mailbutton.h"
#include <QPushButton>
#include <QLabel>
#include <QHBoxLayout>

MailButton::MailButton(QWidget *parent)
    :QWidget(parent)
{
    this->setStyleSheet("border-radius: 5px; background-color: rgba(255, 255, 255, 100); margin-bottom: 1px;");
    this->setFixedHeight(70);
    QPushButton * main = new QPushButton(this);
}

MailButton::~MailButton()
{
}

void MailButton::handleClick()
{
        emit IwasClicked();
}

void MailButton::initialize(QString Message)
{
    QString content = Message;
    connect(this, SIGNAL(IwasClicked()), this, SLOT(handleMessage(content)));
}

void MailButton::handleMessage(QString Message)
{
    emit Processed(QString(Message));
}
