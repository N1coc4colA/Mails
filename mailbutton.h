#ifndef MAILBUTTON_H
#define MAILBUTTON_H

#include <QWidget>

class MailButton : public QWidget
{
    Q_OBJECT

public:
    explicit MailButton(QWidget *parent = nullptr);
    ~MailButton();
signals:
          void handleContent();
          void Processed(QString);
          void IwasClicked();
public slots:
          void handleMessage(QString Message);
          void initialize(QString Message);
          void handleClick();
};

#endif // MAILBUTTON_H
