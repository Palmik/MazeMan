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

#ifndef MAZEVIEW_H
#define MAZEVIEW_H

#include <QGraphicsSvgItem>
#include <QVector>
#include <ctime>
#include "mazemodeldata.h"

class ClickableSvgItem;
class QSignalMapper;
class SvgTheme;

class MazeView : public QGraphicsSvgItem
{
    Q_OBJECT
public:
    MazeView(SvgTheme* theme, QGraphicsItem* parent = 0);
    MazeView(MazeModelData* modelData, SvgTheme* theme, QGraphicsItem* parent = 0);
    virtual ~MazeView() {}

public slots:
    virtual void reload(MazeModelData* modelData);
    void reload();

protected slots:
    virtual void clickReceived(int x);

protected:
    void updateTilesPositions();
    void updatePlayerPosition();
    void updateEnemyPosition();
    void updatePortalPosition();
    void updateTileGraphicsAt(QPoint p);
    SvgTheme* theme() const { return theme_m; }
    QGraphicsSvgItem* playerItem() const { return player_m; }
    QGraphicsSvgItem* enemyItem() const { return enemy_m; }
    QGraphicsSvgItem* portalItem() const { return portal_m; }

private:
    MazeModelData* modelData() const { return modelData_m; }

    QVector<ClickableSvgItem*> tiles_m;
    QGraphicsSvgItem* player_m;
    QGraphicsSvgItem* enemy_m;
    QGraphicsSvgItem* portal_m;
    SvgTheme* theme_m;

    MazeModelData* modelData_m;
    QSignalMapper* clickMapper_m;
};

#endif // MAZEVIEW_H
