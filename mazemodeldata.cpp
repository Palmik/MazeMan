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

#include "mazemodeldata.h"

MazeModelData::MazeModelData(std::istream &in) :
    matrix_m(0, false), width_m(0), height_m(0), playerPosition_m(0, 0), enemyPosition_m(0, 0), portalPosition_m(0, 0), mapName_m("Some Name")
{
    load(in);
}

MazeModelData::MazeModelData(uint width, uint height, const QString& mapName, bool obstacles) :
    matrix_m(width * height, obstacles), width_m(width), height_m(height), playerPosition_m(0, 0), enemyPosition_m(0, 0), portalPosition_m(0, 0), mapName_m(mapName)
{
}

void MazeModelData::load(std::istream& in)
{
    matrix_m.clear();
    playerPosition_m = QPoint(0, 0);
    enemyPosition_m = QPoint(0, 0);
    portalPosition_m = QPoint(0, 0);
    width_m = 0;
    height_m = 0;

    std::string buffer;
    std::getline(in, buffer);
    setMapName(QString::fromStdString(buffer));

    while (std::getline(in, buffer) && in.good()) {
        std::size_t length = buffer.length();
        width_m = length;
        if ((length > 0) && (buffer.at(0) == '#')) {
            for (size_t i = 0; i < length; ++i) {
                matrix_m.push_back((buffer.at(i) == '#'));
                switch (buffer.at(i)) {
                case 'P' :
                    playerPosition_m.setX(i);
                    playerPosition_m.setY(height_m);
                    break;
                case 'E' :
                    enemyPosition_m.setX(i);
                    enemyPosition_m.setY(height_m);
                    break;
                case '@' :
                    portalPosition_m.setX(i);
                    portalPosition_m.setY(height_m);
                default:
                    break;
                }
            }
            ++height_m;
        }
    }
}

void MazeModelData::save(std::ostream& out) const
{
    out << mapName().toStdString();

    std::size_t size = matrix_m.size();
    for (std::size_t i = 0; i < size; ++i) {
        if ((i % width()) == 0) {
            out << std::endl;
        }

        if (translate(enemyPosition()) == i) {
            out << 'E';
        }
        else if (translate(playerPosition()) == i) {
            out << 'P';
        }
        else if (translate(portalPosition()) == i) {
            out << '@';
        }
        else if (isObstacleAt(translate(i))) {
            out << '#';
        }
        else {
            out << ' ';
        }
    }

    out << std::endl;
}
