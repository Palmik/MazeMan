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

#ifndef MAZEGAMEMODEL_H
#define MAZEGAMEMODEL_H

#include <QPoint>
#include <QVector>
#include <QString>
#include <iostream>
#include <cassert>
#include "mazemodeldata.h"
typedef unsigned int uint;

class MazeGameModel
{
public:
    MazeGameModel(MazeModelData* data);

    enum Move {
        Up,
        Down,
        Left,
        Right,
        UpLeft,
        UpRight,
        DownLeft,
        DownRight,
        None
    };

    MazeModelData* data() const { return data_m; }

    bool movePlayer(Move move);
    bool moveEnemy(Move move);

    bool isEnemyOnMove() const { return enemyOnMove_m; }
    bool isPlayerOnMove() const { return !isEnemyOnMove(); }
    bool isValidEnemyMove(Move m) const;
    bool isValidPlayerMove(Move m) const;
    bool isGameOver() const;
    bool hasPlayerWon() const;

    QVector<Move> possiblePlayerMoves() const;
    QVector<Move> possibleEnemyMoves() const;

private:
    QPoint movePoint(QPoint p, Move move) const;

    QVector<bool> matrix_m;
    QVector<Move> availablePlayerMoves_m;
    QVector<Move> availableEnemyMoves_m;

    bool playerWon_m;
    bool enemyOnMove_m;

    MazeModelData* data_m;
};

#endif // MAZEGAMEMODEL_H
