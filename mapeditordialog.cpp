#include "mapeditordialog.h"
#include "ui_mapeditordialog.h"

MapEditorDialog::MapEditorDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::MapEditorDialog)
{
    ui->setupUi(this);
}

MapEditorDialog::~MapEditorDialog()
{
    delete ui;
}

void MapEditorDialog::changeEvent(QEvent *e)
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

void MapEditorDialog::on_createNewMapButton_clicked()
{
    chosenOption_m = CreateNewMap;
}

void MapEditorDialog::on_editExistingMapButton_clicked()
{
    chosenOption_m = EditExistingMap;
}
