#ifndef PEOPLE_H
#define PEOPLE_H

#include "peopleproperty.h"



class People
{
public:
    People(int id);
    int id;

    int exp = 0;    //当前经验

    PeopleProperty property;

    int spirit; //剩余精神
    int spirit_total; //实际level精神
};

#endif // PEOPLE_H
