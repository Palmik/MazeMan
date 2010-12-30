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

#include "mazeview.h"
#include "mazemodeldata.h"
#include <QSignalMapper>
#include <QDebug>
#include <QCursor>
#include <QKeyEvent>
#include <cassert>
#include "core/clickablesvgitem.h"
#include "core/svgtheme.h"

MazeView::MazeView(SvgTheme* theme_, QGraphicsItem* parent) :
    QGraphicsSvgItem(parent), player_m(0), enemy_m(0), portal_m(0), theme_m(theme_), modelData_m(0), clickMapper_m(new QSignalMapper())
{
}

MazeView::MazeView(MazeModelData* modelData_, SvgTheme* theme_, QGraphicsItem* parent) :
    QGraphicsSvgItem(parent), player_m(0), enemy_m(0), portal_m(0), theme_m(theme_), modelData_m(modelData_), clickMapper_m(new QSignalMapper())
{
    setFlags(flags() ^ QGraphicsItem::ItemIsFocusable);
    reload(modelData());
}

void MazeView::clickReceived(int x)
{
    Q_UNUSED(x);
    setFocus();
    qDebug() << "Clicked (MazeView)";
}

void MazeView::updatePlayerPosition()
{
    playerItem()->setPos(tiles_m.at(modelData()->translate(modelData()->playerPosition()))->pos());
}

void MazeView::updateEnemyPosition()
{
    enemyItem()->setPos(tiles_m.at(modelData()->translate(modelData()->enemyPosition()))->pos());
}

void MazeView::updatePortalPosition()
{
    portalItem()->setPos(tiles_m.at(modelData()->translate(modelData()->portalPosition()))->pos());
}

void MazeView::updateTilesPositions()
{
    if (tiles_m.empty()) {
        return;
    }
    uint padding = 0;
    QSizeF tileSize = tiles_m.at(0)->boundingRect().size();

    for (uint y = 0; y < modelData()->height(); ++y) {
        for (uint x = 0; x < modelData()->width(); ++x) {
            tiles_m.at(modelData()->translate(QPoint(x, y)))->setPos(padding + (x) * (tileSize.width()), padding + y*(tileSize.height()));
        }
    }
}

void MazeView::updateTileGraphicsAt(QPoint p)
{
    assert(modelData()->isInBounds(p) && "Out of bounds attempt to update tile graphics");
    tiles_m.at(modelData()->translate(p))->setElementId((modelData()->isObstacleAt(p)) ? "tile_wall" : "tile_free");
}

void MazeView::reload(MazeModelData* modelData_)
{
    modelData_m = modelData_;
    qDeleteAll(tiles_m);
    tiles_m.clear();
    clickMapper_m->disconnect();

    delete player_m;
    delete enemy_m;
    delete portal_m;

    player_m = new QGraphicsSvgItem(this);
    enemy_m = new QGraphicsSvgItem(this);
    portal_m = new QGraphicsSvgItem(this);

    player_m->setSharedRenderer(theme()->renderer());
    player_m->setElementId("player");

    enemy_m->setSharedRenderer(theme()->renderer());
    enemy_m->setElementId("enemy1");

    portal_m->setSharedRenderer(theme()->renderer());
    portal_m->setElementId("portal");

    for (uint y = 0; y < modelData()->height(); ++y) {
        for (uint x = 0; x < modelData()->width(); ++x) {
            ClickableSvgItem* newTile = new ClickableSvgItem(this);
            newTile->setSharedRenderer(theme()->renderer());
            newTile->setElementId((modelData()->isObstacleAt(QPoint(x, y))) ? "tile_wall" : "tile_free");

            tiles_m.push_back(newTile);
            connect(newTile, SIGNAL(clicked(Qt::MouseButtons)), clickMapper_m, SLOT(map()));
            clickMapper_m->setMapping(newTile, modelData()->translate(QPoint(x, y)));
        }
    }

    connect(clickMapper_m, SIGNAL(mapped(int)), this, SLOT(clickReceived(int)));

    updateTilesPositions();
    updateEnemyPosition();
    updatePlayerPosition();
    updatePortalPosition();

    setFocus();
}

void MazeView::reload()
{
    reload(modelData());
}
