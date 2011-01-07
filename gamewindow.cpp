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
#include <QLabel>
#include <QFileDialog>
#include <fstream>
#include "core/svgresizingview.h"
#include "core/svgtheme.h"
#include "mazeeditorview.h"
#include "mazegameview.h"
#include "dialogs/mapeditordialog.h"
#include "dialogs/createmapdialog.h"
#include "dialogs/startupdialog.h"
#include "dialogs/textbuttonsdialog.h"

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

    ui->actionStartTestGame->setVisible(false);
    ui->actionStopTestGame->setVisible(false);
    ui->actionSave->setVisible(false);

    connect(mainMenu(), SIGNAL(openMapRequested()), this, SLOT(openNewMap()));
    connect(mainMenu(), SIGNAL(openCampaignRequested()), this, SLOT(openNewCampaign()));
    connect(mainMenu(), SIGNAL(quitRequested()), this, SIGNAL(quitRequested()));
    connect(mainMenu(), SIGNAL(quitRequested()), this, SLOT(close()));
    connect(mainMenu(), SIGNAL(createMapRequested()), this, SLOT(startMapEditor()));

    connect(ui->actionQuit, SIGNAL(triggered()), this, SIGNAL(quitRequested()));
    connect(ui->actionStartMap, SIGNAL(triggered()), this, SLOT(openNewMap()));
    connect(ui->actionStartCampaign, SIGNAL(triggered()), this, SLOT(openNewCampaign()));
    connect(ui->actionSave, SIGNAL(triggered()), this, SLOT(saveCurrentMap()));
    connect(ui->actionCreateMap, SIGNAL(triggered()), this, SLOT(createNewMap()));
    connect(ui->actionEditMap, SIGNAL(triggered()), this, SLOT(editExistingMap()));
    connect(ui->actionStartTestGame, SIGNAL(triggered()), this, SLOT(startTestGame()));
    connect(ui->actionStopTestGame, SIGNAL(triggered()), this, SLOT(stopTestGame()));
    connect(ui->actionHelp, SIGNAL(triggered()), this, SLOT(showHelpDialog()));

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
    mainMenu()->hideQuit();
    mainMenu()->exec();
    mainMenu()->showQuit();
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

void GameWindow::startMapEditor()
{
    MapEditorDialog dialog;
    if (dialog.exec()) {
        switch (dialog.chosenOption()) {
        case MapEditorDialog::CreateNewMap :
            createNewMap();
            break;
        case MapEditorDialog::EditExistingMap :
            editExistingMap();
            break;
        }
    }
}

bool GameWindow::createNewMap()
{
    bool success = false;

    CreateMapDialog createDialog;
    if (createDialog.exec()) {
        success = true;
        modelData()->reload(createDialog.enteredWidth(), createDialog.enteredHeight(), createDialog.enteredName(), false);
    }

    if (success) {
        enterEditorMode();
    }

    return success;
}

bool GameWindow::editExistingMap()
{
    bool success = false;

    QString mapFileName = QFileDialog::getOpenFileName(this, tr("Open Map File"), "./", tr("Maps (*.map)"));
    if (!mapFileName.isEmpty()) {
        std::ifstream mapFile(mapFileName.toStdString().c_str());
        if (mapFile.is_open() && mapFile.good()) {
            success = true;
            modelData()->reload(mapFile);
        }
    }

    if (success) {
        enterEditorMode();
    }

    return success;
}

void GameWindow::startMap(const QString& mapPath)
{
    std::ifstream mapFile(mapPath.toStdString().c_str());
    if (mapFile.is_open() && mapFile.good()) {
        currentMapPath_m = mapPath;
        modelData()->reload(mapFile);
        gameView()->reload(modelData());
        enterGameMode();
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
    if (!isInEditorMode()) {
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
    if (!isInEditorMode()) {
        if (campaignInProgress()) {
            if (retryLastMap()) {
                startMap(currentMapPath_m);
            }
            else {
                handlePlayerLostCampaign();
            }
        }
        else {
            if (retryLastMap()) {
                startMap(currentMapPath_m);
            }
            else {
                mainMenu()->exec();
            }
        }
    }
    else {
        stopTestGame();
    }
}

bool GameWindow::retryLastMap()
{
    TextButtonsDialog retryDialog("You have lost. Want to try again?", "Yes", "No");
    return retryDialog.exec();
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
    if (isInEditorMode()) {
        //qDebug() << "Starting test game";
        ui->actionStartTestGame->setVisible(false);
        ui->actionStopTestGame->setVisible(true);
        gameView()->reload(modelData());
        switchToGameView();
    }
}

void GameWindow::stopTestGame()
{
    if (isInEditorMode()) {
        //qDebug() << "Stoping test game";
        ui->actionStartTestGame->setVisible(true);
        ui->actionStopTestGame->setVisible(false);
        editorView()->reload(modelData());
        switchToEditorView();
    }
}

void GameWindow::enterEditorMode()
{
    editorMode_m = true;
    ui->actionStartTestGame->setVisible(true);
    ui->actionStopTestGame->setVisible(false);
    ui->actionSave->setVisible(true);
    switchToEditorView();
}

void GameWindow::enterGameMode()
{
    editorMode_m = false;
    ui->actionStartTestGame->setVisible(false);
    ui->actionStopTestGame->setVisible(false);
    ui->actionSave->setVisible(true);
    switchToGameView();
}

void GameWindow::switchToEditorView()
{
    if (gameView_m) {
        gameView()->hide();
        if (gameView()->scene()) {
            programView()->scene()->removeItem(gameView());
        }
    }

    if (!editorView()->scene()) {
        programView()->scene()->addItem(editorView());
    }

    editorView()->reload(modelData());
    editorView()->show();
    editorView()->setFocus();
    programView()->setSceneRect(programView()->scene()->itemsBoundingRect());
    programView()->fitInView(programView()->sceneRect(), Qt::KeepAspectRatio);
}

void GameWindow::switchToGameView()
{
    if (editorView_m) {
        editorView()->hide();
        if (editorView()->scene()) {
            programView()->scene()->removeItem(editorView());
        }
    }

    if (!gameView()->scene()) {
        programView()->scene()->addItem(gameView());
    }

    gameView()->reload(modelData());
    gameView()->show();
    gameView()->setFocus();
    programView()->setSceneRect(programView()->scene()->itemsBoundingRect());
    programView()->fitInView(programView()->sceneRect(), Qt::KeepAspectRatio);
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

void GameWindow::showHelpDialog()
{

    QDialog helpDialog(this);
    QLabel* helpText = new QLabel(&helpDialog);
    helpText->setTextFormat(Qt::RichText);
    helpText->setText(tr("<h3>The Help</h3> \
                         <ul> \
                         <li> \
                         <h4>The Game</h4> \
                             <ul> \
                                 <li>Start game by clicking \'<em>Game -> Start Game</em>\' and selecting map file.</li> \
                                 <li>Start campaign by clicking \'<em>Game -> Start Campaign</em>\' and selecting folder with map files.</li> \
                                 <li>Controls: \
                                     <ul> \
                                         <li>The arrows</li> \
                                         <li>The mouse (by cliking on the nearby tiles)</li> \
                                     </ul> \
                                 </li> \
                             </ul> \
                         </li> \
                         <li> \
                         <h4>The Editor</h4> \
                             <ul> \
                                 <li>Edit exiting map by clicking \'<em>Editor -> Edit Map</em>\' and selecting map file.</li> \
                                 <li>Create new map by clicking \'<em>Editor -> Create Map</em>\' and entering the map name and dimensions.</li> \
                                 <li>You can test the map you are editing by clicking \'<em>Editor -> Test Map</em>\'.</li> \
                             </ul> \
                         </li> \
                         </ul>"));

    QVBoxLayout* l = new QVBoxLayout(&helpDialog);
    l->addWidget(helpText);

    helpDialog.setLayout(l);

    helpDialog.exec();
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
