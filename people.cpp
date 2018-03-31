#include "people.h"

#include <QFile>
#include "game.h"

extern Game * game;

#include <QDebug>


QString spirit_name[19] = {QString("毅力"), QString("加速"), QString("瞄准"), QString("防守"), QString("强攻"), QString("友情"), QString("必杀"), QString("疾风"), QString("回避"), QString("潜力"), QString("热血"), QString("情义"), QString("传真"), QString("援助"), QString("怒"), QString("祈祷"), QString("干扰"), QString("狂怒"), QString("爱心")};

People::People(int id2, int player2) : id(id2), player(player2)
{

    QFile file(game->people_value_path);
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


            spirit_total0 = QString(t[2]).toInt();


            spirit_increase = QString(t[3]).toInt();

            for (int i = 0; i < 19; ++i)
            {
                spirit_table[i] = QString(t[i+4]).toInt() == 1 ? true : false;
            }

            move = QString(t[23]).toInt();
            strength = QString(t[24]).toInt();
            defense = QString(t[25]).toInt();
            speed = QString(t[26]).toInt();
            hp = QString(t[27]).toInt();

            break;
        }
    }

    file.close();

}
