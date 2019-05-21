#ifndef DATAHELPER_H
#define DATAHELPER_H

#include <QFile>
#include <QVector>

#include <qDebug>

#include "config.h"
#include "peopleproperty.h"
#include "robotproperty.h"
#include "weapon.h"
#include "weapon.h"
extern Config * config;

struct EnemyData
{
    int stage;  //第几关
    int round;  //第几回合

    int x;
    int y;

    int order; //我方顺序

    int peopleId;

    int robotId;

    int robotLevel;

    int robotBehavior;  //第几回合开始进攻
};
struct RunTimeRobotData
{
    int x = 0;
    int y = 0;

    int robotId = 0;
    int peopleId = 0;

    int player = 0;
    int level = 0;
    int exp = 0;
    int hp = 0;
    int sprit = 0;

    int robotBehavior = 1;

    int active = 1;
};

struct RobotData
{
    int stage;  //第几关
    int round;  //第几回合

    int x;
    int y;

    int order; //我方顺序

    int peopleId;

    int robotId;
};

struct DataHelper
{
    struct Map
    {
        int width;
        int height;
        QVector<QVector<int>> data;

        QVector<int> operator[](int index) {
            return data[index];
        }

        Map(QString filename)
        {
            QFile file(filename);
            if(!file.open(QIODevice::ReadOnly | QIODevice::Text))
            {
                qDebug() << filename << " open failed";
                exit(-1);
            }
            QTextStream in(&file);

            QString line = in.readLine();
            QStringList t = line.split(",");


            height = QString(t[0]).toInt();
            width = QString(t[1]).toInt();

            data = QVector<QVector<int>>(width+1, QVector<int>(height+1, 0));
            //qDebug() << width << height;

            for (int j = 1; j <= height; ++j)
            {
                QString line = in.readLine();
                QStringList t = line.split(",");

                for (int i = 1; i <= width; ++i)
                {
                    //qDebug() << i << j;
                    data[i][j] = (QString(t[i-1]).toInt());
                }
            }


            file.close();
        }
    };


    static QVector<RobotData> RobotInit(QString filename)
    {
        QFile file(filename);
        if(!file.open(QIODevice::ReadOnly | QIODevice::Text))
        {
            qDebug() << filename << "open failed";
            exit(-1);

        }

        QVector<RobotData> robots;

        QTextStream in(&file);

        QString line = in.readLine();

        while(!in.atEnd())
        {
            QString line = in.readLine();

            QStringList t = line.split(",");

            if (t.size() == 0)
                continue;
            RobotData robotData;
            int i = 0;
            robotData.stage = QString(t[i++]).toInt();
            robotData.round = QString(t[i++]).toInt();
            robotData.x = QString(t[i++]).toInt() + 1;
            robotData.y = QString(t[i++]).toInt() + 1;
            robotData.order = QString(t[i++]).toInt();
            robotData.peopleId = QString(t[i++]).toInt();
            i++;    // people Name
            robotData.robotId = QString(t[i++]).toInt();

            robots.push_back(robotData);
        }

        file.close();

        return robots;
    }



    static QVector<EnemyData> EnemyRobotInit(QString filename)
    {
        QFile file(filename);
        if(!file.open(QIODevice::ReadOnly | QIODevice::Text))
        {
            qDebug() << filename << "open failed";
            exit(-1);

        }

        QVector<EnemyData> enemys;

        QTextStream in(&file);

        QString line = in.readLine();

        while(!in.atEnd())
        {
            QString line = in.readLine();

            QStringList t = line.split(",");

            if (t.size() == 0)
                continue;

            EnemyData enemy;

            int i = 0;
            enemy.stage = QString(t[i++]).toInt();
            enemy.round = QString(t[i++]).toInt();
            enemy.x = QString(t[i++]).toInt() + 1;
            enemy.y = QString(t[i++]).toInt() + 1;
            enemy.peopleId = QString(t[i++]).toInt();
            enemy.robotId = QString(t[i++]).toInt();
            enemy.robotLevel = QString(t[i++]).toInt();
            enemy.robotBehavior = QString(t[i++]).toInt();
            i++;    //people name
            i++;    //robot name
            enemy.robotBehavior = QString(t[i++]).toInt();

            enemys.push_back(enemy);
        }

        file.close();

        return enemys;
    }


    static RobotProperty getRobotProperty(int id)
    {
        RobotProperty property;

        QFile file(config->robot_value_path);
        if(!file.open(QIODevice::ReadOnly | QIODevice::Text))
        {
            qDebug() << config->robot_value_path << "open failed";
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
                property.robotName = QString(t.at(1));
                property.type_original = QString(t[3]).toInt();
                property.type = property.type_original & 0x3;

                property.robot_hp0 = QString(t[19]).toInt();
                property.robot_move0 = QString(t[6]).toInt();
                property.robot_speed0 = QString(t[7]).toInt();
                property.robot_strength0 = QString(t[8]).toInt();
                property.robot_defense0 = QString(t[9]).toInt();

                property.robot_hp_plus = QString(t[18]).toInt();
                property.robot_strength_plus = QString(t[16]).toInt();
                property.robot_defense_plus = QString(t[17]).toInt();
                property.robot_speed_plus = QString(t[15]).toInt();

                property.exp_dievalue = QString(t[13]).toInt();
                property.money = QString(t[10]).toInt();

                property.weapon1id = QString(t[24]).toInt();
                property.weapon2id = QString(t[25]).toInt();

                //机体图标

                property.img_id = QString(t[20]).toInt();

            }
        }


        file.close();

        return property;
    }

    static PeopleProperty getPeopleProperty(int id)
    {
        PeopleProperty property;
        QFile file(config->people_value_path);
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
                property.name = QString(t.at(1));


                property.spirit_total0 = QString(t[2]).toInt();


                property.spirit_increase = QString(t[3]).toInt();

                for (int i = 0; i < 19; ++i)
                {
                    property.spirit_table[i] = QString(t[i+4]).toInt() == 1 ? true : false;
                }

                property.move = QString(t[23]).toInt();
                property.strength = QString(t[24]).toInt();
                property.defense = QString(t[25]).toInt();
                property.speed = QString(t[26]).toInt();
                property.hp = QString(t[27]).toInt();

                property.music_id = QString(t[28]).toInt(0,16);

                break;
            }
        }

        file.close();
        return property;
    }

    static Weapon * getWeapon(int id)
    {
        Weapon * weapon = new Weapon();
        weapon->id = id;

        QFile file(config->weapon_value_path);
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
                weapon->name = QString(t.at(1));

                weapon->range = QString(t[3]).toInt();

                weapon->firepower[0] = QString(t[8]).toInt();
                weapon->firepower[1] = QString(t[7]).toInt();
                weapon->firepower[2] = QString(t[6]).toInt();

                weapon->hitRadio = QString(t[4]).toInt();   //命中率

                if (id == 164)
                {
                    weapon->range = 1;
                    weapon->firepower[0] = 0;
                    weapon->firepower[1] = 0;
                    weapon->firepower[2] = 0;
                    weapon->hitRadio = 100;
                }

                break;
            }
        }

        file.close();

        return weapon;
    }

    static QVector<int> getExpUpdateTable()
    {
        QString filename = config->exp_table_path;
        QFile file(filename);
        if(!file.open(QIODevice::ReadOnly | QIODevice::Text))
        {
            qDebug() << filename << "open failed";
            exit(-1);

        }
        QTextStream in(&file);

        QVector<int> exp_update_table(128, 99999);

        while(!in.atEnd())
        {
            QString line = in.readLine();

            QStringList t = line.split(":");

            int le = QString(t[0]).toInt();
            int value = QString(t[1]).toInt();

            exp_update_table[le] = value;
        }

        file.close();

        return exp_update_table;
    }
};



#endif // DATAHELPER_H
