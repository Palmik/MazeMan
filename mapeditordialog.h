#ifndef MAPEDITORDIALOG_H
#define MAPEDITORDIALOG_H

#include <QDialog>

namespace Ui {
    class MapEditorDialog;
}

class MapEditorDialog : public QDialog {
    Q_OBJECT
public:
    MapEditorDialog(QWidget *parent = 0);
    ~MapEditorDialog();

    enum Option {
        CreateNewMap,
        EditExistingMap
    };

    Option chosenOption() { return chosenOption_m; }

protected:
    void changeEvent(QEvent *e);

private:
    Ui::MapEditorDialog *ui;

    Option chosenOption_m;

private slots:
    void on_editExistingMapButton_clicked();
    void on_createNewMapButton_clicked();
};

#endif // MAPEDITORDIALOG_H
