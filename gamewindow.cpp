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

#include "gamewindow.h"
#include "ui_gamewindow.h"

#include <QGraphicsSvgItem>
#include <QDebug>
#include <QFileDialog>
#include <fstream>
#include "core/svgresizingview.h"
#include "mazeeditorview.h"
#include "mazegameview.h"
#include "core/svgtheme.h"

GameWindow::GameWindow(SvgTheme* theme_, QWidget* parent) :
    QMainWindow(parent),
    ui(new Ui::GameWindow),
    currentCampaign_m(0),
    theme_m(theme_),
    gameView_m(0),
    editorView_m(0),
    modelData_m(0),
    mainMenu_m(new StartupDialog()),
    campaignInProgress_m(false),
    editorMode_m(false),
    testGameInProgress_m(false)
{
    ui->setupUi(this);

    QGraphicsScene* scene = new QGraphicsScene(this);
    programView()->setScene(scene);

    ui->actionSave->setVisible(false);
    //ui->actionStartTestGame->setVisible(false);
    //ui->actionStopTestGame->setVisible(false);

    connect(mainMenu(), SIGNAL(openMap()), this, SLOT(openNewMap()));
    connect(mainMenu(), SIGNAL(openCampaign()), this, SLOT(openNewCampaign()));
    connect(mainMenu(), SIGNAL(quit()), this, SIGNAL(quit()));
    connect(mainMenu(), SIGNAL(createMap()), this, SLOT(createMap()));

    connect(ui->actionQuit, SIGNAL(triggered()), this, SIGNAL(quit()));
    connect(ui->actionStartMap, SIGNAL(triggered()), this, SLOT(openNewMap()));
    connect(ui->actionStartCampaign, SIGNAL(triggered()), this, SLOT(openNewCampaign()));
    connect(ui->actionSave, SIGNAL(triggered()), this, SLOT(saveCurrentMap()));
    connect(ui->actionCreateMap, SIGNAL(triggered()), this, SLOT(createMap()));

    programView()->setAspectRatioMode(Qt::KeepAspectRatio);
    programView()->setBackgroundRenderer(theme()->renderer());
    programView()->setBackgroundElementId("background");
    programView()->setCacheMode(QGraphicsView::CacheBackground);
    //gameView()->setViewportUpdateMode(QGraphicsView::FullViewportUpdate);

    this->show();
    mainMenu()->exec();
}

GameWindow::~GameWindow()
{
    delete ui;
    delete gameView_m;
}

void GameWindow::openNewMap()
{
    QString mapFileName = QFileDialog::getOpenFileName(this, tr("Open Map File"), "./", tr("Maps (*.map)"));
    if (!mapFileName.isEmpty()) {
        forfeitCampaign();
        startMap(mapFileName);
    }
}

void GameWindow::openNewCampaign()
{
    QString campaignDirectoryName = QFileDialog::getExistingDirectory(this, tr("Open Campaign Directory"), "./", QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
    if (!campaignDirectoryName.isEmpty()) {
        startCampaign(campaignDirectoryName);
    }
}

void GameWindow::startCampaign(const QString& campaignDirectory)
{
    forfeitCampaign();
    currentCampaign_m = new Campaign(campaignDirectory);
    startMap(currentCampaign_m->nextMap());
}

void GameWindow::createMap()
{
    ui->actionSave->setVisible(true);

    delete modelData_m;
    modelData_m = new MazeModelData(30, 30, "Some Name");

    editorMode_m = true;

    if (gameView()) {
        gameView()->hide();
    }

    if (!editorView()) {
        editorView_m = new MazeEditorView(modelData(), theme());
        programView()->scene()->addItem(editorView());
    }
    else {
        editorView()->clear(modelData());
    }

    editorView()->show();
}

void GameWindow::startMap(const QString& mapPath)
{
    std::ifstream mapFile(mapPath.toStdString().c_str());
    if (mapFile.is_open() && mapFile.good()) {
        editorMode_m = false;

        if (editorView()) {
            editorView()->hide();
        }

        if (!modelData()) {
            modelData_m = new MazeModelData(mapFile);
        }
        else {
            modelData()->load(mapFile);
        }

        if (!gameView()) {
            gameView_m = new MazeGameView(modelData(), theme());

            connect(gameView(), SIGNAL(playerWon()), this, SLOT(handlePlayerWon()));
            connect(gameView(), SIGNAL(playerLost()), this, SLOT(handlePlayerLost()));
            programView()->scene()->addItem(gameView());
        }
        else {
            gameView()->reloadModel(modelData());
        }
        ui->actionSave->setVisible(true);
        gameView()->show();
    }    
}

void GameWindow::forfeitCampaign()
{
    campaignInProgress_m = false;
    delete currentCampaign_m;
    currentCampaign_m = 0;
}

void GameWindow::handlePlayerWon()
{
    if (campaignInProgress_m) {
        QString nextMapPath = currentCampaign_m->nextMap();
        if (nextMapPath.isEmpty()) {
            handlePlayerWonCampaign();
        }
        else {
            startMap(nextMapPath);
        }
    }
    else {
        mainMenu()->exec();
    }
    ui->actionSave->setVisible(false);
}

void GameWindow::handlePlayerLost()
{
    if (campaignInProgress_m) {
        handlePlayerLostCampaign();
    }
    else {
        mainMenu()->exec();
    }
    ui->actionSave->setVisible(false);
}

void GameWindow::handlePlayerLostCampaign()
{
    mainMenu()->exec();
}

void GameWindow::handlePlayerWonCampaign()
{
    mainMenu()->exec();
}

SvgResizingView* GameWindow::programView()
{
    return ui->gameView;
}

void GameWindow::saveCurrentMap()
{
    QString saveFileName = QFileDialog::getSaveFileName(this, tr("Save Current Map"), "./", tr("Maps (*.map)"));
    std::ofstream saveFile(saveFileName.toStdString().c_str());

    if (saveFile.is_open() && saveFile.good()) {
        if (editorMode_m) {
            editorView()->model().data()->save(saveFile);
        }
        else {
            modelData()->save(saveFile);
        }
    }

    saveFile.close();
}
void GameWindow::on_actionStartTestGame_triggered()
{
    if (editorMode_m && !testGameInProgress_m) {
        editorView()->hide();
        if (!gameView()) {
            gameView_m = new MazeGameView(editorView()->model().data(), theme());

            connect(gameView(), SIGNAL(playerWon()), this, SLOT(handlePlayerWon()));
            connect(gameView(), SIGNAL(playerLost()), this, SLOT(handlePlayerLost()));
            programView()->scene()->addItem(gameView());
        }
        else {
            gameView()->reloadModel(editorView()->model().data());
        }
        ui->actionSave->setVisible(true);
        gameView()->show();
        gameView()->setFocus();
    }
}

void GameWindow::on_actionFullscreen_triggered()
{
    (isFullScreen()) ? showNormal() : showFullScreen();
}

void GameWindow::changeEvent(QEvent* e)
{
    QMainWindow::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}
