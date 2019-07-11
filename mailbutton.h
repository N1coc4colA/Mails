#ifndef MAILBUTTON_H
#define MAILBUTTON_H

#include <QWidget>

class MailButton : public QWidget
{
    Q_OBJECT

public:
    explicit MailButton(QWidget *parent = nullptr);
    ~MailButton();
          bool isClicked;
signals:
          void handleContent();
          void Processed(QString);
public slots:
          void handleMessage(QString Message);
protected:
          void mouseReleaseEvent(QMouseEvent*);
          void paintEvent(QPaintEvent*);

private:
           bool state;

    signals:
           void clicked();
           void release();
           void pressed();
};

#endif // MAILBUTTON_H
