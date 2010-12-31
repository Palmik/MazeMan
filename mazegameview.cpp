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

#include "mazegameview.h"
#include <QDebug>
#include <QCursor>
#include <QKeyEvent>
#include "core/clickablesvgitem.h"
#include "core/svgtheme.h"

MazeGameView::MazeGameView(MazeModelData* modelData_, SvgTheme* theme_, QGraphicsItem* parent) :
    MazeView(modelData_, theme_, parent), model_m(modelData_)
{
}

void MazeGameView::keyPressEvent(QKeyEvent* event)
{
    QGraphicsSvgItem::keyPressEvent(event);
    event->accept();
    MazeGameModel::Move move;
    switch (event->key()) {
    case Qt::Key_Up :
        //qDebug() << "U";
        move = MazeGameModel::Up;
        break;
    case Qt::Key_Down :
        //qDebug() << "D";
        move = MazeGameModel::Down;
        break;
    case Qt::Key_Left :
        //qDebug() << "L";
        move = MazeGameModel::Left;
        break;
    case Qt::Key_Right :
        //qDebug() << "R";
        move = MazeGameModel::Right;
        break;
    default:
        return;
        break;
    }

    //qDebug() << model().data()->width();
    //qDebug() << model().data()->height();
    //qDebug() << model().data()->playerPosition().x() << model().data()->playerPosition().y();
    //qDebug() << model().data()->isObstacleAt(QPoint(model().data()->playerPosition().x() + 1, model().data()->playerPosition().y()));

    handlePlayerMove(move);
}

void MazeGameView::clickReceived(int x)
{
    QPoint clickedPos(model().data()->translate(x));

    qDebug() << "Clicked " << clickedPos.x() << clickedPos.y() << " (" << x << ") (MazeGameView)";
    qDebug() << "Player pos " << model().data()->playerPosition().x() << model().data()->playerPosition().y();

    int diffX = model().data()->playerPosition().x() - clickedPos.x();
    int diffY = model().data()->playerPosition().y() - clickedPos.y();

    qDebug() << diffX << diffY;

    bool validClick = false;

    MazeGameModel::Move move;

    if (diffX == -1 && diffY == 0) {
        qDebug() << "R";
        move = MazeGameModel::Right;
        validClick = true;
    }
    else if (diffX == 1 && diffY == 0) {
        qDebug() << "L";
        move = MazeGameModel::Left;
        validClick = true;
    }
    else if (diffX == 0 && diffY == -1) {
        qDebug() << "D";
        move = MazeGameModel::Down;
        validClick = true;
    }
    else if (diffX == 0 && diffY == 1) {
        qDebug() << "U";
        move = MazeGameModel::Up;
        validClick = true;
    }

    if (false) {
        handlePlayerMove(move);
    }

    MazeView::clickReceived(x);
    setFocus();
}

void MazeGameView::handlePlayerMove(MazeGameModel::Move move)
{
    if (model().movePlayer(move)) {
        //qDebug() << "wut?";
        updatePlayerPosition();
        model().moveEnemy(solveEnemyMove());
        updateEnemyPosition();
    }

    if (model().isGameOver()) {
        //qDebug() << "Game Over";
        if (model().hasPlayerWon()) {
            //qDebug() << "Player Won";
            emit playerWon();
        }
        else {
            //qDebug() << "Player Lost";
            emit playerLost();
        }
    }
}

MazeGameModel::Move MazeGameView::solveEnemyMove() const
{
    int diffX = model().data()->enemyPosition().x() - model().data()->playerPosition().x();
    int diffY = model().data()->enemyPosition().y() - model().data()->playerPosition().y();

    bool needLeft = diffX > 0;
    bool needRight = diffX < 0;
    bool needUp = diffY > 0;
    bool needDown = diffY < 0;

    QVector<MazeGameModel::Move> possibleMoves = model().possibleEnemyMoves();
    QVector<MazeGameModel::Move> neededMoves;

    if (needUp) {
        if (needLeft) {
            if (possibleMoves.contains(MazeGameModel::UpLeft)) {
                neededMoves.push_back(MazeGameModel::UpLeft);
            }
            else {
                neededMoves.push_back(MazeGameModel::Up);
                neededMoves.push_back(MazeGameModel::Left);
            }
        }
        else if (needRight) {
            if (possibleMoves.contains(MazeGameModel::UpRight)) {
                neededMoves.push_back(MazeGameModel::UpRight);
            }
            else {
                neededMoves.push_back(MazeGameModel::Up);
                neededMoves.push_back(MazeGameModel::Right);
            }
        }
        else {
            neededMoves.push_back(MazeGameModel::Up);
        }
    }

    if (needDown) {
        if (needLeft) {
            if (possibleMoves.contains(MazeGameModel::DownLeft)) {
                neededMoves.push_back(MazeGameModel::DownLeft);
            }
            else {
                neededMoves.push_back(MazeGameModel::Down);
                neededMoves.push_back(MazeGameModel::Left);
            }
        }
        else if (needRight) {
            if (possibleMoves.contains(MazeGameModel::DownRight)) {
                neededMoves.push_back(MazeGameModel::DownRight);
            }
            else {
                neededMoves.push_back(MazeGameModel::Down);
                neededMoves.push_back(MazeGameModel::Right);
            }
        }
        else {
            neededMoves.push_back(MazeGameModel::Down);
        }
    }

    if (needLeft && !(needUp || needDown)) {
        neededMoves.push_back(MazeGameModel::Left);
    }

    if (needRight && !(needUp || needDown)) {
        neededMoves.push_back(MazeGameModel::Right);
    }

    QVector<MazeGameModel::Move> viableMoves;

    foreach(MazeGameModel::Move m, neededMoves) {
        if (possibleMoves.contains(m)) {
            viableMoves.push_back(m);
        }
    }

    if (viableMoves.empty()) {
        return MazeGameModel::None;
    }
    else {
        uint moveIndex = randFromRange(0, viableMoves.size() - 1);
        return viableMoves.at(moveIndex);
    }
}
