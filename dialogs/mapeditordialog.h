////////////////////////////////////////////////////////////////////////////
//                                                                        //
//  Copyright 2010 Petr Pilar                                             //
//                                                                        //
//  This file is part of MazeMan.                                         //
//                                                                        //
//  MazeMan is free software: you can redistribute it and/or modify       //
//  it under the terms of the GNU General Public License as published by  //
//  the Free Software Foundation, either version 3 of the License, or     //
//  (at your option) any later version.                                   //
//                                                                        //
//  MazeMan is distributed in the hope that it will be useful,            //
//  but WITHOUT ANY WARRANTY; without even the implied warranty of        //
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         //
//  GNU General Public License for more details.                          //
//                                                                        //
//  You should have received a copy of the GNU General Public License     //
//  along with MazeMan.  If not, see <http://www.gnu.org/licenses/>.      //
//                                                                        //
////////////////////////////////////////////////////////////////////////////

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
