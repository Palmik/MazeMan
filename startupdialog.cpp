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

#include "startupdialog.h"
#include "ui_startupdialog.h"

StartupDialog::StartupDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::StartupDialog)
{
    ui->setupUi(this);

    connect(ui->openMap, SIGNAL(clicked()), this, SLOT(accept()));
    connect(ui->openCampaign, SIGNAL(clicked()), this, SLOT(accept()));
    connect(ui->createMap, SIGNAL(clicked()), this, SLOT(accept()));
    connect(ui->quit, SIGNAL(clicked()), this, SLOT(accept()));

    connect(ui->openMap, SIGNAL(clicked()), this, SIGNAL(openMap()));
    connect(ui->openCampaign, SIGNAL(clicked()), this, SIGNAL(openCampaign()));
    connect(ui->createMap, SIGNAL(clicked()), this, SIGNAL(createMap()));
    connect(ui->quit, SIGNAL(clicked()), this, SIGNAL(quit()));
}

StartupDialog::~StartupDialog()
{
    delete ui;
}

void StartupDialog::changeEvent(QEvent *e)
{
    QWidget::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}
