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
#include "core/clickablesvgitem.h"
#include "core/svgtheme.h"
#include <QDebug>


MazeEditorModel::MazeEditorModel(MazeModelData* data_) :
    data_m(data_)
{
    validateMapIfNeeded();

    previousPlayerPosition_m = data()->playerPosition();
    previousEnemyPosition_m = data()->enemyPosition();
    previousPortalPosition_m = data()->portalPosition();

    assert((freeTilesCount() >= 8) && "Not enough free tiles");
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
            if (isNotOccupiedTile(point)) {
                return point;
            }
        }
    }

    return QPoint(0, 0);
}

bool MazeEditorModel::isNotOccupiedTile(QPoint point) const
{
    return (!data()->isObstacleAt(point) && (data()->playerPosition() != point) && (data()->enemyPosition() != point) && (data()->portalPosition() != point) && (previousPlayerPosition_m != point) && (previousEnemyPosition_m != point) && (previousPortalPosition_m != point));
}

void MazeEditorModel::validateMapIfNeeded()
{
    placeObstaclesAtBorders();

    data_m->setObstacleAt(data()->playerPosition(), false);
    data_m->setObstacleAt(data()->enemyPosition(), false);
    data_m->setObstacleAt(data()->portalPosition(), false);

    if (!isValidMap()) {
        if ((data()->playerPosition() == data()->enemyPosition())  ||
            (data()->playerPosition() == data()->portalPosition()) ||
            (data()->enemyPosition() == data()->portalPosition())) {
            data_m->setPlayerPosition(firstFreeNonOccupiedTile());
            data_m->setEnemyPosition(firstFreeNonOccupiedTile());
            data_m->setPortalPosition(firstFreeNonOccupiedTile());
        }
    }
}

bool MazeEditorModel::isValidMap() const
{
    // Enough free tiles
    if (freeTilesCount() < 8) {
        return false;
    }

    // Obstacles at borders
    for (uint y = 0; y < data()->height(); ++y) {
        for (uint x = 0; x < data()->width(); ++x) {
            if (isBorderAt(QPoint(x, y))) {
                if (!data()->isObstacleAt(QPoint(x, y))) {
                    return false;
                }
            }
        }
    }

    // Player, Enemy and Portal positions do not overlap
    if ((data()->playerPosition() == data()->enemyPosition())  ||
        (data()->playerPosition() == data()->portalPosition()) ||
        (data()->enemyPosition() == data()->portalPosition())) {
        return false;
    }

    // Player, Enemy and Portal positions are not where obstacles are
    if ((data()->isObstacleAt(data()->playerPosition())) ||
        (data()->isObstacleAt(data()->enemyPosition()))  ||
        (data()->isObstacleAt(data()->portalPosition()))) {
        return false;
    }

    return true;
}

void MazeEditorModel::setPlayerPosition(QPoint point)
{
    if (!isBorderAt(point) && (data()->playerPosition() != point)) {
        previousPlayerPosition_m = data()->playerPosition();

        if (point == data()->enemyPosition()) {
            setEnemyPosition(previousEnemyPosition_m);
        }
        else if (point == data()->portalPosition()) {
            setPortalPosition(previousPortalPosition_m);
        }

        data_m->setObstacleAt(point, false);
        data_m->setPlayerPosition(point);
    }

    if (point == previousPortalPosition_m) {
        previousPortalPosition_m = firstFreeNonOccupiedTile();
    }
    else if (point == previousEnemyPosition_m) {
        previousEnemyPosition_m = firstFreeNonOccupiedTile();
    }

    validateMapIfNeeded();
}

void MazeEditorModel::setEnemyPosition(QPoint point)
{
    if (!isBorderAt(point) && (data()->enemyPosition() != point)) {
        previousEnemyPosition_m = data()->enemyPosition();

        if (point == data()->playerPosition()) {
            setPlayerPosition(previousPlayerPosition_m);
        }
        else if (point == data()->portalPosition()) {
            setPortalPosition(previousPortalPosition_m);
        }

        data_m->setObstacleAt(point, false);
        data_m->setEnemyPosition(point);
    }

    if (point == previousPortalPosition_m) {
        previousPortalPosition_m = firstFreeNonOccupiedTile();
    }
    else if (point == previousPlayerPosition_m) {
        previousPlayerPosition_m = firstFreeNonOccupiedTile();
    }

    validateMapIfNeeded();
}

void MazeEditorModel::setPortalPosition(QPoint point)
{
    if (!isBorderAt(point) && (data()->portalPosition() != point)) {
        previousPortalPosition_m = data()->portalPosition();

        if (point == data()->playerPosition()) {
            setPlayerPosition(previousPlayerPosition_m);
        }
        else if (point == data()->enemyPosition()) {
            setEnemyPosition(previousEnemyPosition_m);
        }

        data_m->setObstacleAt(point, false);
        data_m->setPortalPosition(point);
    }

    if (point == previousEnemyPosition_m) {
        previousEnemyPosition_m = firstFreeNonOccupiedTile();
    }
    else if (point == previousPlayerPosition_m) {
        previousPlayerPosition_m = firstFreeNonOccupiedTile();
    }

    validateMapIfNeeded();
}

void MazeEditorModel::setObstacleAt(QPoint point, bool obstacle)
{
    // You can not change border tiles
    if (isBorderAt(point) || ((freeTilesCount() <= 8) && obstacle)) {
        return;
    }

    if (obstacle) {
        if (point == data()->playerPosition()) {
            setPlayerPosition(previousPlayerPosition_m);
        }
        else if (point == data()->enemyPosition()) {
            setEnemyPosition(previousEnemyPosition_m);
        }
        else if (point == data()->portalPosition()) {
            setPortalPosition(previousPortalPosition_m);
        }
    }

    data_m->setObstacleAt(point, obstacle);

    if (point == previousPortalPosition_m) {
        previousPortalPosition_m = firstFreeNonOccupiedTile();
    }
    else if (point == previousEnemyPosition_m) {
        previousEnemyPosition_m = firstFreeNonOccupiedTile();
    }
    else if (point == previousPlayerPosition_m) {
        previousPlayerPosition_m = firstFreeNonOccupiedTile();
    }

    validateMapIfNeeded();
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
