#ifndef BATTLE_H
#define BATTLE_H

#include "robot.h"
#include "weapon.h"

class Battle
{
public:
    Battle(Robot *robot, Weapon *weapon, Robot *enemy);
};

#endif // BATTLE_H
