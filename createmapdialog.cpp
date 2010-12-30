#include "createmapdialog.h"
#include "ui_createmapdialog.h"

CreateMapDialog::CreateMapDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CreateMapDialog)
{
    ui->setupUi(this);
    connect(ui->buttonBox, SIGNAL(accepted()), this, SLOT(accept()));
    connect(ui->buttonBox, SIGNAL(rejected()), this, SLOT(reject()));
}

CreateMapDialog::~CreateMapDialog()
{
    delete ui;
}

int CreateMapDialog::enteredWidth()
{
    return ui->widthSpinBox->value();
}

int CreateMapDialog::enteredHeight()
{
    return ui->heightSpinBox->value();
}

QString CreateMapDialog::enteredName()
{
    return ui->mapName->text();
}

void CreateMapDialog::changeEvent(QEvent *e)
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
