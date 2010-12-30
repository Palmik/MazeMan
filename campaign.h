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

#ifndef CAMPAIGN_H
#define CAMPAIGN_H

#include <QStringList>
#include <QDir>

class Campaign
{
public:
    Campaign(const QString& directoryPath)
    {
        directory_m = new QDir(directoryPath);
        maps_m = directory_m->entryList(QStringList("*.map"));
    }

    ~Campaign()
    {
        delete directory_m;
    }

    QString name() { return directory_m->dirName(); }
    QString nextMap() { if (!maps_m.isEmpty()) return directory_m->absoluteFilePath(maps_m.takeFirst()); else return QString(); }

private:
    QStringList maps_m;
    QDir* directory_m;
};

#endif // CAMPAIGN_H
