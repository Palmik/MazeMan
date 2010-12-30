#ifndef CREATEMAPDIALOG_H
#define CREATEMAPDIALOG_H

#include <QDialog>

namespace Ui {
    class CreateMapDialog;
}

class CreateMapDialog : public QDialog {
    Q_OBJECT
public:
    CreateMapDialog(QWidget *parent = 0);
    ~CreateMapDialog();

    int enteredWidth();
    int enteredHeight();
    QString enteredName();

protected:
    void changeEvent(QEvent *e);

private:
    Ui::CreateMapDialog *ui;
};

#endif // CREATEMAPDIALOG_H
