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
#include "core/svgtheme.h"
#include "mazeeditorview.h"
#include "mazegameview.h"
#include "mapeditordialog.h"
#include "createmapdialog.h"

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

    connect(mainMenu(), SIGNAL(openMapRequested()), this, SLOT(openNewMap()));
    connect(mainMenu(), SIGNAL(openCampaignRequested()), this, SLOT(openNewCampaign()));
    connect(mainMenu(), SIGNAL(quitRequested()), this, SIGNAL(quitRequested()));
    connect(mainMenu(), SIGNAL(quitRequested()), this, SLOT(close()));
    connect(mainMenu(), SIGNAL(createMapRequested()), this, SLOT(createMap()));

    connect(ui->actionQuit, SIGNAL(triggered()), this, SIGNAL(quitRequested()));
    connect(ui->actionStartMap, SIGNAL(triggered()), this, SLOT(openNewMap()));
    connect(ui->actionStartCampaign, SIGNAL(triggered()), this, SLOT(openNewCampaign()));
    connect(ui->actionSave, SIGNAL(triggered()), this, SLOT(saveCurrentMap()));
    connect(ui->actionCreateMap, SIGNAL(triggered()), this, SLOT(createMap()));
    connect(ui->actionStartTestGame, SIGNAL(triggered()), this, SLOT(startTestGame()));
    connect(ui->actionStopTestGame, SIGNAL(triggered()), this, SLOT(stopTestGame()));

    programView()->setAspectRatioMode(Qt::KeepAspectRatio);
    programView()->setBackgroundRenderer(theme()->renderer());
    programView()->setBackgroundElementId("background");
    programView()->setCacheMode(QGraphicsView::CacheBackground);
}

GameWindow::~GameWindow()
{
    delete ui;
    delete modelData_m;
}

void GameWindow::start()
{
    show();
    mainMenu()->exec();
}

void GameWindow::openNewMap()
{
    QString mapFileName = QFileDialog::getOpenFileName(this, tr("Open Map File"), "./", tr("Maps (*.map)"));
    if (!mapFileName.isEmpty()) {
        stopCampaign();
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
    stopCampaign();
    campaignInProgress_m = true;
    currentCampaign_m = new Campaign(campaignDirectory);
    startMap(currentCampaign_m->nextMap());
}

void GameWindow::createMap()
{
    MapEditorDialog dialog;
    if (dialog.exec()) {

        switch (dialog.chosenOption()) {
        case MapEditorDialog::CreateNewMap : {
                CreateMapDialog createDialog;
                if (dialog.exec()) {
                    editorMode_m = true;
                    modelData()->reload(createDialog.enteredWidth(), createDialog.enteredHeight(), createDialog.enteredName(), false);
                }
            }
            break;
        case MapEditorDialog::EditExistingMap : {
                QString mapFileName = QFileDialog::getOpenFileName(this, tr("Open Map File"), "./", tr("Maps (*.map)"));
                if (!mapFileName.isEmpty()) {
                    std::ifstream mapFile(mapFileName.toStdString().c_str());
                    if (mapFile.is_open() && mapFile.good()) {
                        editorMode_m = true;
                        modelData()->reload(mapFile);
                    }
                }
            }
            break;
        }

        if (editorMode_m) {
            editorView()->reload(modelData());
            switchToEditorView();
        }
    }
}

void GameWindow::startMap(const QString& mapPath)
{
    std::ifstream mapFile(mapPath.toStdString().c_str());
    if (mapFile.is_open() && mapFile.good()) {
        editorMode_m = false;
        modelData()->reload(mapFile);
        gameView()->reload(modelData());
        switchToGameView();
    }    
}

void GameWindow::stopCampaign()
{
    campaignInProgress_m = false;
    delete currentCampaign_m;
    currentCampaign_m = 0;
}

void GameWindow::handlePlayerWon()
{
    if (!editorMode_m) {
        if (campaignInProgress()) {
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
    }
    else {
        stopTestGame();
    }
}

void GameWindow::handlePlayerLost()
{
    if (!editorMode_m) {
        if (campaignInProgress()) {
            handlePlayerLostCampaign();
        }
        else {
            mainMenu()->exec();
        }
    }
    else {
        stopTestGame();
    }
}

void GameWindow::saveCurrentMap()
{
    QString saveFileName = QFileDialog::getSaveFileName(this, tr("Save Current Map"), "./", tr("Maps (*.map)"));
    std::ofstream file(saveFileName.toStdString().c_str());

    if (file.is_open() && file.good()) {
        modelData()->save(file);
    }

    file.close();
}

void GameWindow::startTestGame()
{
    if (editorMode_m) {
        qDebug() << "Starting test game";
        gameView()->reload(modelData());
        switchToGameView();
    }
}

void GameWindow::stopTestGame()
{
    if (editorMode_m) {
        qDebug() << "Stoping test game";
        editorView()->reload(modelData());
        switchToEditorView();
    }
}

void GameWindow::switchToEditorView()
{
    if (gameView_m) {
        gameView()->hide();
    }
    editorView()->show();
    editorView()->setFocus();
    programView()->scene()->setSceneRect(programView()->scene()->itemsBoundingRect());
    programView()->setSceneRect(programView()->scene()->itemsBoundingRect());
}

void GameWindow::switchToGameView()
{
    if (editorView_m) {
        editorView()->hide();
    }
    gameView()->show();
    gameView()->setFocus();
    programView()->scene()->setSceneRect(programView()->scene()->itemsBoundingRect());
    programView()->setSceneRect(programView()->scene()->itemsBoundingRect());
}

MazeModelData* GameWindow::modelData()
{
    if (!modelData_m) {
        modelData_m = new MazeModelData();
    }

    return modelData_m;
}

MazeEditorView* GameWindow::editorView()
{
    if (!editorView_m) {
        editorView_m = new MazeEditorView(modelData(), theme());
        editorView_m->setParent(this);
        programView()->scene()->addItem(editorView());
    }

    return editorView_m;
}

MazeGameView* GameWindow::gameView()
{
    if (!gameView_m) {
        gameView_m = new MazeGameView(modelData(), theme());
        gameView_m->setParent(this);
        connect(gameView(), SIGNAL(playerWon()), this, SLOT(handlePlayerWon()));
        connect(gameView(), SIGNAL(playerLost()), this, SLOT(handlePlayerLost()));
        programView()->scene()->addItem(gameView());
    }

    return gameView_m;
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
