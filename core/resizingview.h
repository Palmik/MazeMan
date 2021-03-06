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

#ifndef RESIZINGVIEW_H
#define RESIZINGVIEW_H

#include <QGraphicsView>

class QSvgRenderer;

class ResizingView : public QGraphicsView
{
public:
    ResizingView(QWidget* parent = 0);
    ResizingView(Qt::AspectRatioMode, QWidget* parent = 0);
    ResizingView(QGraphicsScene* scene, Qt::AspectRatioMode = Qt::KeepAspectRatio, QWidget* parent = 0);

    void setAspectRatioMode(Qt::AspectRatioMode aspectRatioMode) { aspectRatioMode_m = aspectRatioMode; fitInView(sceneRect(), aspectRatioMode_m); }

protected:
    //====================================
    // REIMPLEMENTATION OF VIRTUAL METHODS
    void resizeEvent(QResizeEvent *event);

private:
    Qt::AspectRatioMode aspectRatioMode_m;
};

#endif // RESIZINGVIEW_H
