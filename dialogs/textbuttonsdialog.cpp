#include "textbuttonsdialog.h"
#include "ui_textbuttonsdialog.h"

TextButtonsDialog::TextButtonsDialog(QString const& text, QString const& acceptButtonText, QString const& rejectButtonText, QWidget* parent) :
    QDialog(parent),
    ui(new Ui::TextButtonsDialog)
{
    ui->setupUi(this);

    ui->textLabel->setText(text);
    ui->acceptButton->setText(acceptButtonText);
    ui->rejectButton->setText(rejectButtonText);

    connect(ui->acceptButton, SIGNAL(clicked()), this, SLOT(accept()));
    connect(ui->rejectButton, SIGNAL(clicked()), this, SLOT(reject()));
}

TextButtonsDialog::~TextButtonsDialog()
{
    delete ui;
}

void TextButtonsDialog::changeEvent(QEvent *e)
{
    QDialog::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}
