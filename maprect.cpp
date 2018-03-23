#include "maprect.h"
#include "game.h"
#include "map.h"

extern Game * game;

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
        //如果有选中的机器 并且 点击的地图格子可以到达
        if (game->canMoveStatus)
        {
            if (game->selectedRobot && game->map->moveMap[x][y] >= 0)
            {
                game->map->move(game->selectedRobot, x, y);

                game->map->UnshowMoveRange();   //消除不能移动的格子的灰色效果
                //game->selectedRobot = 0;    //设置状态没有选中机器

                game->canMoveStatus = false;

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
