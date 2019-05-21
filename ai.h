#ifndef AI_H
#define AI_H

#include "map.h"
#include "robot.h"



class AI
{
public:
    AI();


    static Robot * GetEnemy(Map * map, Robot * robot);
};

#endif // AI_H
