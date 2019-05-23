#ifndef PEOPLEPROPERTY_H
#define PEOPLEPROPERTY_H

#include <QString>



struct PeopleProperty
{
    QString name;

    int spirit_total0; //精神


    int spirit_increase;    //成长方式

    int level;  //水平


    //人物对机体的增益
    int move;   //机动
    int strength;   //强度
    int defense;    //防卫
    int speed;  //速度
    int hp; //hp

    int music_id;

    bool spirit_table[19];
    static QVector<QString> spirit_name;
    static int spirit_consume_table[19];
};

#endif // PEOPLEPROPERTY_H
