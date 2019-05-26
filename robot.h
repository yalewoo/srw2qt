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

    // 实际五维，精神加成之后
    int t_hp_total();   //总hp
    int t_move(){   //机动
        if (spirit[7]) return move + 5;
        if (spirit[1]) return move + 3;
        return move;
    }
    int t_strength();   //强度
    int t_defense();    //防卫
    int t_speed();  //速度

    // =========可能会变化的机器人属性==========

    // 变形
    QVector<int> canTransform();
    void ChangeId(int targetId);

    // 拥有的武器
    Weapon * weapon1 = nullptr;
    Weapon * weapon2 = nullptr;

    // 属于我方机器人还是敌方
    int player = 0; //0 玩家 1敌人

    // 机器人驾驶员
    People * pilot = nullptr;

    // 等级和经验
    int level = 1;  //当前等级
    static int getLevelPropertyPlus(int plusType, int level);
    void gotExp(int exp);
    void updateLevel(); // 升级 更新属性数据

    int robotBehavior;  // 第几回合开始进攻

    //==============游戏中用到的变量===================
    bool active = true; //机体是否行动过
    int hp; //现有Hp

    QVector<Robot *> passengers;
    bool inMainShip = false;    // 处于搭载状态

    // AI计算临时变量
    bool tmp_ai_weapon1;
    bool tmp_ai_weapon2;


    bool shouldPaintUsingActive = true;


    bool spirit[19] = {false};
    void clearSpirit(){
        for (int i = 0; i < 19; ++i)
        {
            spirit[i] = false;
        }
    }


    Robot(int id, int player=0);
    ~Robot();




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
