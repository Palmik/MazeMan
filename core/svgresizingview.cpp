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

#include "svgresizingview.h"
#include <QSvgRenderer>
#include <QPainter>

SvgResizingView::SvgResizingView(QWidget* parent)
    : ResizingView(parent), backgroundRenderer_m(0)
{
}


SvgResizingView::SvgResizingView(Qt::AspectRatioMode aspectRatioMode, QWidget* parent)
    : ResizingView(aspectRatioMode, parent), backgroundRenderer_m(0)
{
}

SvgResizingView::SvgResizingView(QGraphicsScene* scene, Qt::AspectRatioMode aspectRatioMode, QWidget* parent)
    : ResizingView(scene, aspectRatioMode, parent), backgroundRenderer_m(0)
{
}

SvgResizingView::SvgResizingView(QSvgRenderer* backgroundRenderer, Qt::AspectRatioMode aspectRatioMode, QWidget* parent)
    : ResizingView(aspectRatioMode, parent), backgroundRenderer_m(backgroundRenderer)
{
}

SvgResizingView::SvgResizingView(QSvgRenderer* backgroundRenderer, QGraphicsScene* scene, Qt::AspectRatioMode aspectRatioMode, QWidget* parent)
    : ResizingView(scene, aspectRatioMode, parent), backgroundRenderer_m(backgroundRenderer)
{
}

void SvgResizingView::setBackgroundRenderer(QSvgRenderer* renderer)
{
    backgroundRenderer_m = renderer;
}

void SvgResizingView::setBackgroundElementId(const QString& id)
{
    backgroundElementId_m = id;
}

void SvgResizingView::drawBackground(QPainter* painter, const QRectF& rect)
{
    if ((backgroundRenderer() == 0) || !(backgroundRenderer()->isValid()))
        return;

    painter->setRenderHint(QPainter::Antialiasing, false);

    if (backgroundElementId().isEmpty())
        backgroundRenderer()->render(painter, rect);
    else
        backgroundRenderer()->render(painter, backgroundElementId(), rect);
}
