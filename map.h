#ifndef MAP_H
#define MAP_H

#include <QVector>

#include "maprect.h"
#include "robot.h"

class Map
{
public:
    Map(QString mapfilename);

    int width;
    int height;

    QVector<QVector<MapRect *> > map;
    QVector<QVector<Robot *> > robots;

    //从原版数据格式读取地图
    void loadMapFromTxt(QString filename);
    //从csv矩阵读地图
    void loadMapFromCsv(QString filename);

    void placeRobot_init(QString filename, int player);
    void placePlayerRobot_init(QString filename);
    void placeEnemyRobot_init(QString filename);

    //获取(x,y)格消耗的移动力，考虑敌方机体的影响
    int getMoveConsume(Robot *robot, int x, int y);
    //计算移动范围 返回二维数组表示每个格子是否可移动
    QVector<QVector<int> > calculateMoveRange(Robot * robot);
    //显示可移动范围 不能移动的格子加灰色
    void showMoveRange(Robot * robot);
    QVector<QVector<int> > moveMap; //保存移动范围
    void UnshowMoveRange(); //消除显示可移动范围

    //计算格子距离
    int calcDistance(Robot * robot1, Robot * robot2);

    //武器
    QVector<QVector<int> > AttackMap; //保存攻击范围
    QVector<QVector<int> > calculateAttackRange(Robot * robot, Weapon * weapon);
    void showAttackRange(Robot * robot, Weapon * weapon);

    //在射程范围内返回true
    bool canAttack(Robot * robot, Weapon * weapon, Robot * enemy);

    //移动机器人
    void move(Robot * selectedRobot, int xTo, int yTo);

    //放置一个机器人
    void addRobot(Robot *robot, int xPos, int yPos);
};

#endif // MAP_H
