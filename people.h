#ifndef PEOPLE_H
#define PEOPLE_H

#include "robot.h"

class People
{
public:
    People(int id, int player);

    int player; //表示属于玩家(0)还是电脑(1)

    int id;
    QString name;

    int spirit; //剩余精神
    int spirit_total0; //精神
    int spirit_total; //实际level精神


    int spirit_increase;    //成长方式

    int level;  //水平
    int exp = 0;    //经验值


    //人物对机体的增益
    int move;   //机动
    int strength;   //强度
    int defense;    //防卫
    int speed;  //速度
    int hp; //hp


    bool spirit_table[19];
    static QString spirit_name[19];



};

#endif // PEOPLE_H
