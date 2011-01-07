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

#ifndef STARTUPDIALOG_H
#define STARTUPDIALOG_H

#include <QDialog>

namespace Ui {
    class StartupDialog;
}

class StartupDialog : public QDialog {
    Q_OBJECT
public:
    StartupDialog(bool showQuit = true, QWidget *parent = 0);
    ~StartupDialog();

    void showQuit();
    void hideQuit();

signals:
    void openCampaignRequested();
    void openMapRequested();
    void createMapRequested();
    void quitRequested();

protected:
    void changeEvent(QEvent *e);

private:
    Ui::StartupDialog *ui;
};

#endif // STARTUPDIALOG_H
