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

#include "mazegamemodel.h"

MazeGameModel::MazeGameModel(MazeModelData* data) :
    enemyOnMove_m(false), data_m(data)
{
    availablePlayerMoves_m.clear();
    availableEnemyMoves_m.clear();

    availablePlayerMoves_m.push_back(Up);
    availablePlayerMoves_m.push_back(Down);
    availablePlayerMoves_m.push_back(Left);
    availablePlayerMoves_m.push_back(Right);

    availableEnemyMoves_m.push_back(Up);
    availableEnemyMoves_m.push_back(Down);
    availableEnemyMoves_m.push_back(Left);
    availableEnemyMoves_m.push_back(Right);
    availableEnemyMoves_m.push_back(UpLeft);
    availableEnemyMoves_m.push_back(DownRight);
    availableEnemyMoves_m.push_back(DownLeft);
    availableEnemyMoves_m.push_back(UpRight);
    availableEnemyMoves_m.push_back(None);
}

bool MazeGameModel::moveEnemy(Move move)
{
    bool success = isEnemyOnMove() && isValidEnemyMove(move);

    if (success) {
        QPoint newPos = movePoint(data()->enemyPosition(), move);
        enemyOnMove_m = !enemyOnMove_m;
        data()->setEnemyPosition(newPos);
    }

    return success;
}

bool MazeGameModel::movePlayer(Move move)
{
    bool success = isPlayerOnMove() && isValidPlayerMove(move);

    if (success) {
        QPoint newPos = movePoint(data()->playerPosition(), move);
        enemyOnMove_m = !enemyOnMove_m;
        data()->setPlayerPosition(newPos);
    }

    return success;
}

bool MazeGameModel::isValidEnemyMove(Move m) const
{
    return (!data()->isObstacleAt(movePoint(data()->enemyPosition(), m)) && (data()->portalPosition() != movePoint(data()->enemyPosition(), m)) && availableEnemyMoves_m.contains(m));
}

bool MazeGameModel::isValidPlayerMove(Move m) const
{
    return (!data()->isObstacleAt(movePoint(data()->playerPosition(), m)) && availablePlayerMoves_m.contains(m));
}

QVector<MazeGameModel::Move> MazeGameModel::possiblePlayerMoves() const
{
    QVector<Move> possibleMoves;

    foreach(Move m, availablePlayerMoves_m) {
        if (isValidPlayerMove(m)) {
            possibleMoves.push_back(m);
        }
    }

    return possibleMoves;
}

QVector<MazeGameModel::Move> MazeGameModel::possibleEnemyMoves() const
{
    QVector<Move> possibleMoves;

    foreach(Move m, availableEnemyMoves_m) {
        if (isValidEnemyMove(m)) {
            possibleMoves.push_back(m);
        }
    }

    return possibleMoves;
}

bool MazeGameModel::isGameOver() const
{
    return ((data()->enemyPosition() == data()->playerPosition()) || (data()->portalPosition() == data()->playerPosition()));
}

bool MazeGameModel::hasPlayerWon() const
{
    return ((data()->portalPosition() == data()->playerPosition()));
}

QPoint MazeGameModel::movePoint(QPoint p, Move move) const
{
    QPoint targetPos(p);
    switch (move) {
    case Up:
        targetPos = QPoint(p.x(), p.y() - 1);
        break;
    case Down:
        targetPos = QPoint(p.x(), p.y() + 1);
        break;
    case Left:
        targetPos = QPoint(p.x() - 1, p.y());
        break;
    case Right:
        targetPos = QPoint(p.x() + 1, p.y());
        break;
    case UpLeft:
        targetPos = QPoint(p.x() - 1, p.y() - 1);
        break;
    case UpRight:
        targetPos = QPoint(p.x() + 1, p.y() - 1);
        break;
    case DownLeft:
        targetPos = QPoint(p.x() - 1, p.y() + 1);
        break;
    case DownRight:
        targetPos = QPoint(p.x() + 1, p.y() + 1);
        break;
    case None:
        targetPos = p;
        break;
    }

    return targetPos;
}
