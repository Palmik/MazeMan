#ifndef TEXTBUTTONSDIALOG_H
#define TEXTBUTTONSDIALOG_H

#include <QDialog>

namespace Ui {
    class TextButtonsDialog;
}

class TextButtonsDialog : public QDialog {
    Q_OBJECT
public:
    TextButtonsDialog(QString const& text, QString const& acceptButtonText, QString const& rejectButtonText, QWidget* parent = 0);
    ~TextButtonsDialog();

protected:
    void changeEvent(QEvent* e);

private:
    Ui::TextButtonsDialog* ui;
};

#endif // TEXTBUTTONSDIALOG_H
