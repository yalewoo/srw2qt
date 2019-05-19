#ifndef ROBOT_H
#define ROBOT_H

#include "datahelper.h"
#include "people.h"
#include "rect.h"
#include "robotproperty.h"
#include "weapon.h"



class Robot : public Rect
{
public:
    // 全局变量
    static QVector<int> exp_update_table;    // 经验
    static QVector<RobotData> robots_init;    // 每一关的我方机器人
    static QVector<EnemyData> enemys_init;    // 每一关的敌方机器人

    // ==========基础属性，这些属性不会变化=======
    int id; // 唯一标识一个机体
    RobotProperty property;

    // 机体五维，包含驾驶员加成之后的最终数据
    int hp_total;   //总hp
    int move;   //机动
    int strength;   //强度
    int defense;    //防卫
    int speed;  //速度

    // =========可能会变化的机器人属性==========
    // 拥有的武器
    Weapon * weapon1 = nullptr;
    Weapon * weapon2 = nullptr;

    // 属于我方机器人还是敌方
    int player = 0; //0 玩家 1敌人

    // 机器人驾驶员
    People * pilot = nullptr;

    // 等级和经验
    int exp;    //当前经验
    int level = 1;  //当前等级

    int robotBehavior;  // 第几回合开始进攻

    //==============游戏中用到的变量===================
    bool active = true; //机体是否行动过
    int hp; //现有Hp








    Robot(int id, int player=0);



    void setImage();
    void setPilot(int peopleId);


    void setNotActive();
    void setActive();

    Robot *  canAttack1(){return canAttack(weapon1);}
    Robot *  canAttack2(){return canAttack(weapon2);}
    Robot *  canAttack(Weapon * weapon);

    //event
    void hoverEnterEvent(QGraphicsSceneHoverEvent *event);
    void mousePressEvent(QGraphicsSceneMouseEvent * event);

    // 绘制时考虑是否移动过
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

};

#endif // ROBOT_H
