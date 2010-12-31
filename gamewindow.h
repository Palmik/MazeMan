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

#ifndef GAMEWINDOW_H
#define GAMEWINDOW_H

#include <QMainWindow>
#include "campaign.h"
#include "mazegamemodel.h"
#include "startupdialog.h"

class SvgResizingView;
class MazeEditorView;
class MazeGameView;
class SvgTheme;

namespace Ui {
    class GameWindow;
}

class GameWindow : public QMainWindow {
    Q_OBJECT

public:
    GameWindow(SvgTheme* theme, QWidget* parent = 0);
    ~GameWindow();

    void start();

signals:
    void quitRequested();

protected:
    void changeEvent(QEvent* e);

private:
    Ui::GameWindow* ui;

private slots:
    void startCampaign(const QString& campaignDirectory);
    void startMap(const QString& mapPath);
    void createMap();
    void stopCampaign();
    void openNewMap();
    void openNewCampaign();
    void handlePlayerLost();
    void handlePlayerWon();
    void handlePlayerWonCampaign();
    void handlePlayerLostCampaign();
    void saveCurrentMap();
    void on_actionFullscreen_triggered();
    void startTestGame();
    void stopTestGame();
    //void on_actionStopTestGame_triggered();

private:
    void switchToGameView();
    void switchToEditorView();
    bool campaignInProgress() { return campaignInProgress_m; }
    SvgResizingView* programView();
    SvgTheme* theme() const { return theme_m; }
    MazeGameView* gameView();
    MazeEditorView* editorView();
    MazeModelData* modelData();
    StartupDialog* mainMenu() const { return mainMenu_m; }

    Campaign* currentCampaign_m;
    SvgTheme* theme_m;
    MazeGameView* gameView_m;
    MazeEditorView* editorView_m;
    MazeModelData* modelData_m;
    StartupDialog* mainMenu_m;
    bool campaignInProgress_m;
    bool editorMode_m;
    bool testGameInProgress_m;
};

#endif // GAMEWINDOW_H
