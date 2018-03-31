#include "weapon.h"

#include <QDebug>
#include <QFile>

#include "game.h"
extern Game * game;

Weapon::Weapon(int id): id(id)
{
    getAttribute();
}

void Weapon::getAttribute()
{
    QFile file(game->weapon_value_path);
    if(!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        qDebug() << "open people.csv failed";
        exit(-1);

    }
    QTextStream in(&file);

    while(!in.atEnd())
    {
        QString line = in.readLine();

        QStringList t = line.split(",");

        QString thisid = t.at(0);
        int int_id = thisid.toInt();
        if (id == int_id)
        {
            name = QString(t.at(1));

            range = QString(t[3]).toInt();

            firepower[0] = QString(t[8]).toInt();
            firepower[1] = QString(t[7]).toInt();
            firepower[2] = QString(t[6]).toInt();

            hitRadio = QString(t[4]).toInt();   //命中率

            break;
        }
    }

    file.close();




}
