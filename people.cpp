#include "datahelper.h"
#include "people.h"

People::People(int id):id(id)
{
    property = DataHelper::getPeopleProperty(id);

    //qDebug() << "people " << id << "property.move = " << property.move;
}
