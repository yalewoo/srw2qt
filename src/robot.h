#ifndef ROBOT_H
#define ROBOT_H

#include "DataReader/datahelper.h"
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
    int m_id; // 唯一标识一个机体
    RobotProperty m_property;

    // 机体五维，包含驾驶员加成之后的最终数据
    int m_hp_total;   //总hp
    int m_move;   //机动
    int m_strength;   //强度
    int m_defense;    //防卫
    int m_speed;  //速度

    // 实际五维，精神加成之后
    int t_hp_total();   //总hp
    int t_move(){   //机动
        if (m_spirit[7]) return m_move + 5;
        if (m_spirit[1]) return m_move + 3;
        return m_move;
    }
    int t_strength();   //强度
    int t_defense();    //防卫
    int t_speed();  //速度

    // =========可能会变化的机器人属性==========

    // 变形
    QVector<int> canTransform();
    void ChangeId(int targetId);

    // 拥有的武器
    Weapon * m_weapon1 = nullptr;
    Weapon * m_weapon2 = nullptr;

    // 属于我方机器人还是敌方
    int m_player = 0; //0 玩家 1敌人

    // 机器人驾驶员
    People * m_pilot = nullptr;

    // 等级和经验
    int m_level = 1;  //当前等级
    static int getLevelPropertyPlus(int plusType, int m_level);
    void gotExp(int exp);
    void updateLevel(); // 升级 更新属性数据

    int m_robotBehavior;  // 第几回合开始进攻

    //==============游戏中用到的变量===================
    bool m_active = true; //机体是否行动过
    int m_hp; //现有Hp

    //母舰搭载
    QVector<Robot *> m_passengers; // 母舰记录搭载的所有机体
    bool m_inMainShip = false;    // 其他机器记录自己在母舰中
    void AddPassenger(Robot * passenger);
    void DeletePassenger(Robot * passenger);

    // AI计算临时变量
    bool m_tmp_ai_weapon1;
    bool m_tmp_ai_weapon2;


    bool m_shouldPaintUsingActive = true;


    bool m_spirit[19] = {false};
    void clearSpirit(){
        for (int i = 0; i < 19; ++i)
        {
            m_spirit[i] = false;
        }
    }


    Robot(int id, int player=0);
    ~Robot();




    void setImage();
    void setPilot(int peopleId);


    void setNotActive();
    void setActive();

    Robot *  canAttack1(){return canAttack(m_weapon1);}
    Robot *  canAttack2(){return canAttack(m_weapon2);}
    Robot *  canAttack(Weapon * weapon);

    //event
    void hoverEnterEvent(QGraphicsSceneHoverEvent *event);
    void mousePressEvent(QGraphicsSceneMouseEvent * event);

    // 绘制时考虑是否移动过
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

};

#endif // ROBOT_H
