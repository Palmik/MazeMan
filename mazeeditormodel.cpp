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

#include "mazeeditormodel.h"
#include <QSignalMapper>
#include <QDebug>
#include "core/clickablesvgitem.h"
#include "core/svgtheme.h"


MazeEditorModel::MazeEditorModel(MazeModelData* data_) :
    data_m(data_)
{
    placeObstaclesAtBorders();
    data_m->setPlayerPosition(firstFreeNonOccupiedTile());
    data_m->setEnemyPosition(firstFreeNonOccupiedTile());
    data_m->setPortalPosition(firstFreeNonOccupiedTile());
    assert((freeTilesCount() > 4) && "Not enought free tiles");
}

void MazeEditorModel::toggleObstacleAt(QPoint p)
{
    data_m->setObstacleAt(p, !data()->isObstacleAt(p));
}

QPoint MazeEditorModel::firstFreeNonOccupiedTile() const
{
    for (uint y = 0; y < data()->height(); ++y) {
        for (uint x = 0; x < data()->width(); ++x) {
            QPoint point(x, y);
            if (!data()->isObstacleAt(point) && (data()->playerPosition() != point) && (data()->enemyPosition() != point) && (data()->portalPosition() != point)) {
                return point;
            }
        }
    }

    return QPoint(0, 0);
}

void MazeEditorModel::setPlayerPosition(QPoint point)
{
    data_m->setPlayerPosition(point);
}

void MazeEditorModel::setEnemyPosition(QPoint point)
{
    data_m->setEnemyPosition(point);
}

void MazeEditorModel::setPortalPosition(QPoint point)
{
    data_m->setPortalPosition(point);
}

void MazeEditorModel::setObstacleAt(QPoint p, bool obstacle)
{
    // You can not change border tiles
    if (isBorderAt(p) || (freeTilesCount() <= 4)) {
        return;
    }

    data_m->setObstacleAt(p, obstacle);
}

uint MazeEditorModel::freeTilesCount() const
{
    uint counter = 0;
    for (uint y = 0; y < data()->height(); ++y) {
        for (uint x = 0; x < data()->width(); ++x) {
            if (!data()->isObstacleAt(QPoint(x, y))) {
                ++counter;
            }
        }
    }
    return counter;
}

bool MazeEditorModel::isBorderAt(QPoint p) const
{
    return ((p.x() % (data()->width() - 1)) == 0) || ((p.y() % (data()->height() - 1)) == 0);
}

void MazeEditorModel::placeObstaclesAtBorders()
{
    for (uint y = 0; y < data()->height(); ++y) {
        for (uint x = 0; x < data()->width(); ++x) {
            if (isBorderAt(QPoint(x, y))) {
                data_m->setObstacleAt(QPoint(x, y), true);
            }
        }
    }
}
