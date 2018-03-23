#ifndef MAP_H
#define MAP_H

#include <QVector>

#include "maprect.h"
#include "robot.h"

class Map
{
public:
    Map(int width, int height);

    int width;
    int height;

    QVector<QVector<MapRect *> > map;
    QVector<QVector<Robot *> > robots;
    void loadMapFromTxt(QString filename);

    //获取(x,y)格消耗的移动力，考虑敌方机体的影响
    int getMoveConsume(Robot *robot, int x, int y);
    //计算移动范围 返回二维数组表示每个格子是否可移动
    QVector<QVector<int> > calculateMoveRange(Robot * robot);
    //显示可移动范围 不能移动的格子加灰色
    void showMoveRange(Robot * robot);
    QVector<QVector<int> > moveMap; //保存移动范围
    void UnshowMoveRange(); //消除显示可移动范围

    //武器
    QVector<QVector<int> > AttackMap; //保存攻击范围
    QVector<QVector<int> > calculateAttackRange(Robot * robot, Weapon * weapon);
    void showAttackRange(Robot * robot, Weapon * weapon);

    //移动机器人
    void move(Robot * selectedRobot, int xTo, int yTo);

    //放置一个机器人
    void addRobot(int xPos, int yPos, int id, int player = 0);
};

#endif // MAP_H
