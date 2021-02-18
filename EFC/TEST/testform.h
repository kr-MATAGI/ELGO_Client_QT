#ifndef TESTFORM_H
#define TESTFORM_H

#include <QWidget>

#include "QrMaker/QrMaker.h"

namespace Ui {
class testForm;
}

class testForm : public QWidget
{
    Q_OBJECT

public:
    explicit testForm(QWidget *parent = nullptr);
    ~testForm();

    void QrMakerTest(QPaintEvent *event);

private:
    Ui::testForm *ui;
};

#endif // TESTFORM_H
