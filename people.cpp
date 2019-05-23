#include "datahelper.h"
#include "people.h"

QVector<QString> PeopleProperty::spirit_name ={"毅力", "加速", "瞄准", "防守", "强攻", "友情", "必杀", "疾风", "回避", "潜力", "热血", "情义", "传真", "援助", "怒", "祈祷", "干扰", "狂怒", "爱心"};
int PeopleProperty::spirit_consume_table[19] = {10, 10, 10, 20, 40, 30, 40, 50, 50, 60, 80, 70, 150, 80, 100, 100, 100, 150, 150};

People::People(int id):id(id)
{
    property = DataHelper::getPeopleProperty(id);

    //qDebug() << "people " << id << "property.move = " << property.move;
}
