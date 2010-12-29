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

#ifndef MAZEGAMEVIEW_H
#define MAZEGAMEVIEW_H

#include "mazeview.h"
#include "mazegamemodel.h"

inline uint randFromRange(uint lowerBound, uint upperBound)
{
    if (lowerBound > upperBound) {
        return 0;
    }

    if (lowerBound == upperBound) {
        return upperBound;
    }

    srand(time(NULL));
    return ((rand() % (upperBound - lowerBound + 1)) + lowerBound);
}


class MazeGameView : public MazeView
{
    Q_OBJECT
public:
    MazeGameView(MazeModelData* modelData_, SvgTheme* theme_, QGraphicsItem* parent = 0);

protected:
    //====================================
    // REIMPLEMENTATION OF VIRTUAL METHODS
    virtual void keyPressEvent(QKeyEvent* event);

signals:
    void playerWon();
    void playerLost();

protected slots:
    virtual void clickReceived(int x);

private:
    void handlePlayerMove(MazeGameModel::Move);
    MazeGameModel::Move solveEnemyMove() const;
    MazeGameModel& model() { return model_m; }
    MazeGameModel const& model() const { return model_m; }

    MazeGameModel model_m;
};

#endif // MAZEGAMEVIEW_H
