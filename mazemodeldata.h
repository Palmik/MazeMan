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

#ifndef MAZEMODELDATA_H
#define MAZEMODELDATA_H

#include <QPoint>
#include <QDebug>
#include <QVector>
#include <QString>
#include <iostream>
#include <cassert>
typedef unsigned int uint;

class MazeModelData
{
public:
    MazeModelData(std::istream& in);
    MazeModelData(uint width, uint height, QString const& mapName, bool obstacles = false);
    MazeModelData();

    void reload(std::istream& in);
    void reload(uint width, uint height, QString const& mapName, bool obstacles = false);

    void save(std::ostream& out) const;

    bool isEmpty() const { return ((width() * height()) == 0); }

    QPoint translate(uint n) const { return QPoint(n % width_m, n / height_m); }
    uint translate(QPoint p) const { return (p.y() * width_m) + p.x(); }

    uint width() const { return width_m; }
    uint height() const { return height_m; }
    QPoint playerPosition() const { return playerPosition_m; }
    QPoint enemyPosition() const { return enemyPosition_m; }
    QPoint portalPosition() const { return portalPosition_m; }
    QString const& mapName() const { return mapName_m; }

    bool isObstacleAt(QPoint p) const { return (!isInBounds(p)) ? false : matrix_m.at(translate(p)); }
    bool isInBounds(QPoint p) const { return !(((uint)p.x() >= width_m) || ((uint)p.y() >= height_m) || (p.x() < 0) || (p.y() < 0)); }

    virtual void setObstacleAt(QPoint p, bool obstacle) { assert(isInBounds(p) && "Attempt to set obstacle out of bounds"); matrix_m[(translate(p))] = obstacle; }
    void setPlayerPosition(QPoint p) { assert(isInBounds(p) && "Attempt to set player pos. out of bounds"); playerPosition_m = p; }
    void setEnemyPosition(QPoint p) { assert(isInBounds(p) && "Attempt to set enemy pos. out of bounds"); enemyPosition_m = p; }
    void setPortalPosition(QPoint p) { assert(isInBounds(p) && "Attempt to set portal pos. out of bounds"); portalPosition_m = p; }
    void setMapName(QString const& name) { mapName_m = name; }

private:
    QVector<bool> matrix_m;

    uint width_m;
    uint height_m;
    QPoint playerPosition_m;
    QPoint enemyPosition_m;
    QPoint portalPosition_m;
    QString mapName_m;
};

#endif // MAZEMODELDATA_H
