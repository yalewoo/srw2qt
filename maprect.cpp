#include "maprect.h"

#include <QPixmap>

#include <QDebug>
#include <QString>

static QString gAImgPaths[16] = {
    "_00.png",
    "_01.png",
    "_02.png",
    "_03.png",
    "_04.png",
    "_05.png",
    "_06.png",
    "_07.png",
    "_08.png",
    "_09.png",
    "_0A.png",
    "_0B.png",
    "_0C.png",
    "_0D.png",
    "_0E.png",
    "_0F.png"
};

MapRect::MapRect(int kindOfMap, QGraphicsItem *parent) : QGraphicsPixmapItem(parent)
{
    style = 'A';
    kind = kindOfMap;

    setPixmap(QPixmap("../srw2QT/res/images/map32/" + style + gAImgPaths[kind]));

    setMoveConsume();
}

static int moveConsumeTable[3][16] = {
    {999,2,2,3,4,1,3,3,3,2,2,2,2,2,2,2},
    {999,1,1,2,3,3,2,2,2,1,1,1,1,1,1,1},
    {999,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1}
};
void MapRect::setMoveConsume()
{
    moveConsume[0] = moveConsumeTable[0][kind];
    moveConsume[1] = moveConsumeTable[1][kind];
    moveConsume[2] = moveConsumeTable[2][kind];

    if (style == "C" && kind == 1)
        moveConsume[0] = 1;
}
