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

#ifndef MAZEEDITORVIEW_H
#define MAZEEDITORVIEW_H

#include "mazeview.h"
#include "mazeeditormodel.h"

class MazeEditorView : public MazeView
{
    Q_OBJECT
public:
    MazeEditorView(MazeModelData* modelData_, SvgTheme* theme_, QGraphicsItem* parent = 0);

    MazeEditorModel const& model() const { return model_m; }

    void clear(MazeModelData* modelData_);

protected slots:
    virtual void clickReceived(int x);

private:
    //MazeEditorModel& model() { return model_m; }
    MazeEditorModel model_m;
};

#endif // MAZEEDITORVIEW_H
