#ifndef AI_H
#define AI_H

#include "map.h"
#include "robot.h"



class AI
{
public:
    AI();


    static Robot * GetEnemy(Map * map, Robot * robot);

    static QVector<Robot *> GetAllAttackTargetRobots(Map *map, Robot *robot);

    // 决定机体动作顺序
    static QVector<Robot *> GetRobotList(Map * map);
};

#endif // AI_H
