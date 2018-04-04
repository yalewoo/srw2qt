#include "maprect.h"
#include "game.h"
#include "map.h"

extern Game * game;

#include <QDebug>


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

    setPixmap(QPixmap(game->workDir + "res/images/map32/" + style + gAImgPaths[kind]));

    setMoveConsume();
}

static int moveConsumeTable[3][16] = {
    {9999,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
        {9999,1,1,2,3,3,2,2,2,1,1,1,1,1,1,1},
        {9999,2,2,3,4,1,3,3,3,2,2,2,2,2,2,2}
};
void MapRect::setMoveConsume()
{
    moveConsume[0] = moveConsumeTable[0][kind];
    moveConsume[1] = moveConsumeTable[1][kind];
    moveConsume[2] = moveConsumeTable[2][kind];

    if (style == "C" && kind == 1)
        moveConsume[2] = 1;
}

void MapRect::setxy(int xPos, int yPos)
{
    x = xPos;
    y = yPos;

    setPos(32*x, 32*y);
}

void MapRect::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
    {
        static QString t[16] = {QString("墙壁"), QString("平地"), QString("平地"), QString("戈壁"), QString("山地"), QString("海"), QString("草地"), QString("森林"), QString("沙漠"), QString("建筑物"), QString("回复站"), QString("基地"), QString("基地"), QString("基地"), QString("基地"), QString("基地")};
        //qDebug() << "mouse Hover mapRect";

        QString s;
        s += t[kind] + "\n";
        s += QString("\n") + QString("空 ") + QString::number(moveConsume[0]) + "\n";
        s += QString("陆 ") + QString::number(moveConsume[1]) + "\n";
        s += QString("海 ") + QString::number(moveConsume[2]) + "\n";


        game->board->setString(s);

        //如果有选中的机器 并且 点击的地图格子可以到达
        if (game->canMoveStatus)
        {
            if (game->selectedRobot && game->map->moveMap[x][y] >= 0)
            {
                game->map->move(game->selectedRobot, x, y);

                game->displayMenu2(game->selectedRobot);

            }
        }
    }
    else if (event->button() == Qt::RightButton)
    {
        game->cancel();
    }




}



void MapRect::showString(QString textToShow)
{
    text = new QGraphicsTextItem(textToShow, this);
    //game->scene->addItem(text);
}

void MapRect::UnshowString()
{
    if (text)
    {
        game->scene->removeItem(text);
        text = 0;
    }
}
