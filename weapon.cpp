#include "weapon.h"

Weapon::Weapon(int id): id(id)
{
    getAttribute();
}

void Weapon::getAttribute()
{
    if (id == 1)
    {
        name = QString("光剑");
        range = 1;  //射程

        firepower[0] = 110;
        firepower[1] = 120;
        firepower[2] = 120;

        hitRadio = 110;   //命中率
    }
    else if (id == 2)
    {
        name = QString("导弹");
        range = 4;  //射程

        firepower[0] = 0;
        firepower[1] = 130;
        firepower[2] = 140;

        hitRadio = 85;   //命中率
    }
}
