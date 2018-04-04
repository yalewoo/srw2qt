#ifndef ROBOT_H
#define ROBOT_H

#include <QGraphicsPixmapItem>

#include <QGraphicsSceneMouseEvent>

#include "point.h"
#include "weapon.h"

#include <QHoverEvent>

class People;

class Robot : public QGraphicsPixmapItem
{
public:
    Robot(int id, People * pilot, int level2, QGraphicsItem * parent = 0);

    //event
    void mousePressEvent(QGraphicsSceneMouseEvent * event);

    void hoverEnterEvent(QGraphicsSceneHoverEvent *event);

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);


    int player; //表示属于玩家(0)还是电脑(1)

    bool active = true;
    void setNotActive();
    void setActive();


    People * pilot = 0;

    static int exp_update_table[128];

    void get_exp_update_table(QString filename);
    int exp;    //当前经验
    int exp_dievalue;  //本机死亡带来的固定经验
    int money;  //金钱
    int level = 1;

    //基本属性
    int id;
    QString robotName;  //机体名
    int type_original;//原始类型
    int type;   //类型 0=空，1=陆，2=海
    int robot_move;   //机动

    //等级1时数据
    int robot_hp0; //Hp
    int robot_strength0;   //强度
    int robot_defense0;    //防卫
    int robot_speed0;  //速度
    //成长方式
    int robot_hp_plus; //Hp
    int robot_strength_plus;   //强度
    int robot_defense_plus;    //防卫
    int robot_speed_plus;  //速度

    //当前level数据
    int robot_hp; //Hp
    int robot_strength;   //强度
    int robot_defense;    //防卫
    int robot_speed;  //速度
    void calcLevelValue();

    //驾驶员加成最终数据
    int hp_total;   //总hp
    int hp; //现有Hp
    int move;   //机动
    int strength;   //强度
    int defense;    //防卫
    int speed;  //速度


    //武器
    Weapon * weapon1;
    Weapon * weapon2;
    Robot *  canAttack1(){return canAttack(weapon1);}
    Robot *  canAttack2(){return canAttack(weapon2);}
    Robot *  canAttack(Weapon * weapon);

    //位置
    int x;
    int y;
    void setxy(int xPos, int yPos);
    void setxy(Point pos);

    //methods
    void getAttributes();



};

#endif // ROBOT_H
