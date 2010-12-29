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

#include "mazeeditorview.h"
#include "mazegameview.h"

MazeEditorView::MazeEditorView(MazeModelData* modelData_, SvgTheme* theme_, QGraphicsItem* parent) :
    MazeView(theme_, parent), model_m(modelData_)
{
    reloadModel(model().data());
}

void MazeEditorView::clear(MazeModelData* modelData_)
{
    model_m = MazeEditorModel(modelData_);

    reloadModel(model().data());
}

void MazeEditorView::clickReceived(int x)
{
    QPoint clickedPos(model().data()->translate(x));

    if ((model().data()->playerPosition() == clickedPos)) {
        model_m.setObstacleAt(clickedPos, false);
        model_m.setPlayerPosition(model().firstFreeNonOccupiedTile());
        model_m.setEnemyPosition(clickedPos);
    }
    else if ((model().data()->enemyPosition() == clickedPos)) {
        model_m.setObstacleAt(clickedPos, false);
        model_m.setEnemyPosition(model().firstFreeNonOccupiedTile());
        model_m.setPortalPosition(clickedPos);
    }
    else if ((model().data()->portalPosition() == clickedPos)) {
        model_m.setObstacleAt(clickedPos, false);
        model_m.setPortalPosition(model().firstFreeNonOccupiedTile());
    }
    else if (!model().data()->isObstacleAt(clickedPos)) {
        model_m.setObstacleAt(clickedPos, true);
    }
    else if (model().data()->isObstacleAt(clickedPos)) {
        model_m.setObstacleAt(clickedPos, false);
        model_m.setPlayerPosition(clickedPos);
    }

    updatePlayerPosition();
    updateEnemyPosition();
    updatePortalPosition();
    updateTileGraphicsAt(clickedPos);

    MazeView::clickReceived(x);
}
