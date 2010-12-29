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
