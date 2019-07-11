#include "beta.h"
#include <ddialog.h>
#include <QVBoxLayout>
#include <QLabel>
#include <QTranslator>
#include <QApplication>

DWIDGET_USE_NAMESPACE

beta::beta(DDialog *parent) :
    DDialog(parent)
{
    QPixmap pm(":/beta.jpg");

    QLabel *logoLabel = new QLabel();
    logoLabel->setContentsMargins(0, 0, 0, 0);
    logoLabel->setStyleSheet(QStringLiteral("background-image: url(:/beta.png);background-repeat: no-repeat; background-position: center;"));
    logoLabel->setFixedSize(55, 55);

    QLabel *msg = new QLabel(QString(tr("C'est une bêta, certaines choses peuvent ne pas marcher.")));
    QLabel *excuses = new QLabel(QString(tr("Désolé pour les inconvénients occasionés.")));

    QVBoxLayout *message = new QVBoxLayout;
    message->addSpacing(6);
    message->addWidget(msg);
    message->addWidget(excuses);
    message->addSpacing(6);

    QHBoxLayout *mainLayout = new QHBoxLayout;
    mainLayout->addWidget(logoLabel);
    mainLayout->addSpacing(15);
    mainLayout->addLayout(message);

    QWidget  *mainContent = new QWidget(this);
    mainContent->setLayout(mainLayout);
    this->addContent(mainContent);
    mainContent->move(40, 0);
    this->setFocus();
}

beta::~beta()
{
}
