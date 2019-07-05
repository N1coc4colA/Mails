#ifndef BETA_H
#define BETA_H

#include <QWidget>
#include <ddialog.h>

DWIDGET_BEGIN_NAMESPACE

class beta : public DDialog
{
    Q_OBJECT

public:
    explicit beta(DDialog *parent = nullptr);
    ~beta();
};

DWIDGET_END_NAMESPACE

#endif // BETA_H
