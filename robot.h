#ifndef ROBOT_H
#define ROBOT_H

#include <QGraphicsPixmapItem>

#include <QGraphicsSceneMouseEvent>

#include "point.h"
#include "weapon.h"

class Robot : public QGraphicsPixmapItem
{
public:
    Robot(int id, int player = 0, QGraphicsItem * parent = 0);

    //event
    void mousePressEvent(QGraphicsSceneMouseEvent * event);

    int player; //表示属于玩家(0)还是电脑(1)

    //基本属性
    int id;
    QString robotName;  //机体名
    QString driverName;//驾驶员

    int hp; //现有Hp
    int hp_total; //总Hp

    int type;   //类型 0=海，1=陆，2=空
    int move;   //机动

    int spirit; //剩余精神
    int spirit_total; //精神
    int strength;   //强度
    int defense;    //防卫
    int speed;  //速度

    //武器
    Weapon * weapon1;
    Weapon * weapon2;
    bool canAttack1(){return canAttack(weapon1);}
    bool canAttack2(){return canAttack(weapon2);}
    bool canAttack(Weapon * weapon);

    //位置
    int x;
    int y;
    void setxy(int xPos, int yPos);
    void setxy(Point pos);

    //methods
    void Attributes();


};

#endif // ROBOT_H
