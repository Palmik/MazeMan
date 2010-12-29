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

#ifndef SVGRESIZINGVIEW_H
#define SVGRESIZINGVIEW_H

#include "resizingview.h"

class QSvgRenderer;

class SvgResizingView : public ResizingView
{
public:
    SvgResizingView(QWidget* parent = 0);
    SvgResizingView(Qt::AspectRatioMode = Qt::KeepAspectRatio, QWidget* parent = 0);
    SvgResizingView(QGraphicsScene* scene, Qt::AspectRatioMode = Qt::KeepAspectRatio, QWidget* parent = 0);
    SvgResizingView(QSvgRenderer* backgroundRenderer, Qt::AspectRatioMode = Qt::KeepAspectRatio, QWidget* parent = 0);
    SvgResizingView(QSvgRenderer* backgroundRenderer, QGraphicsScene* scene, Qt::AspectRatioMode = Qt::KeepAspectRatio, QWidget* parent = 0);

    //==================
    // SETTERS & GETTERS
    void setBackgroundRenderer(QSvgRenderer* renderer);
    void setBackgroundElementId(const QString& id);
    QSvgRenderer* backgroundRenderer() const;
    QString backgroundElementId() const;

    //===================================
    // REIMPLEMENTATION OF VIRTUAL METHOD
    virtual void drawBackground(QPainter* painter, const QRectF& rect);

private:
    QSvgRenderer* backgroundRenderer_m;
    QString       backgroundElementId_m;
};

inline
QSvgRenderer* SvgResizingView::backgroundRenderer() const
{
    return backgroundRenderer_m;
}

inline
QString SvgResizingView::backgroundElementId() const
{
    return backgroundElementId_m;
}

#endif // SVGRESIZINGVIEW_H
