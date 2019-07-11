#include "mailbutton.h"

#include <QLabel>

MailButton::MailButton(QWidget *parent)
    :QWidget(parent)
{
    this->setStyleSheet("border-radius: 5px; background-color: rgba(255, 255, 255, 100); margin-bottom: 1px;");
    this->setFixedHeight(70);
    state = 0;
}

void MailButton::mouseReleaseEvent(QMouseEvent*)
{
       state = 1;
       update();
       emit clicked();
       isClicked = true;

}

void MailButton::paintEvent(QPaintEvent*)
{
}

MailButton::~MailButton()
{
}

void MailButton::handleMessage(QString Message)
{
    emit Processed(QString(Message));
}
