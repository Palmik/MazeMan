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
