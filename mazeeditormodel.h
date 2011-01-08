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

#ifndef MAZEEDITORMODEL_H
#define MAZEEDITORMODEL_H

#include <QPoint>
#include <QVector>
#include <QString>
#include <iostream>
#include <cassert>
#include "mazemodeldata.h"
typedef unsigned int uint;

class MazeEditorModel
{
public:
    MazeEditorModel(MazeModelData* data);

    MazeModelData* data() const { return data_m; }

    virtual void setObstacleAt(QPoint p, bool obstacle);
    virtual void setPlayerPosition(QPoint point);
    virtual void setEnemyPosition(QPoint point);
    virtual void setPortalPosition(QPoint point);
    void toggleObstacleAt(QPoint p);
    QPoint firstFreeNonOccupiedTile() const;

    QPoint previousPortalPosition() { return previousPortalPosition_m; }

private:
    void validateMapIfNeeded();
    bool isValidMap() const;
    void placeObstaclesAtBorders();
    bool isBorderAt(QPoint p) const;
    bool isNotOccupiedTile(QPoint p) const;
    uint freeTilesCount() const;

    QPoint previousPlayerPosition_m;
    QPoint previousEnemyPosition_m;
    QPoint previousPortalPosition_m;

    MazeModelData* data_m;
};

#endif // MAZEEDITORMODEL_H
