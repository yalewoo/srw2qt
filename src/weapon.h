#ifndef WEAPON_H
#define WEAPON_H

#include <QString>

struct Weapon
{
    int id;

    QString name;
    int range;  //射程

    int firepower[3];   //火力 对海陆空

    int hitRadio;   //命中率

    Weapon(int id = 0):id(id){ }
};

#endif // WEAPON_H
