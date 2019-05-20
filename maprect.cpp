#include "maprect.h"

#include "datahelper.h"
#include "game.h"

#include <QGraphicsSceneMouseEvent>
extern Game * game;
MapRect::MapRect(int kind):Rect()
{
    this->kind = kind;
    QString style = "A";

    setPixmap(QPixmap(DataHelper::pathOfMapRectKind(kind, style)));

    setMoveConsume();
}



void MapRect::setMoveConsume()
{
    static int moveConsumeTable[3][16] = {
        {9999,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
            {9999,1,1,2,3,3,2,2,2,1,1,1,1,1,1,1},
            {9999,2,2,3,4,1,3,3,3,2,2,2,2,2,2,2}
    };

    moveConsume[0] = moveConsumeTable[0][kind];
    moveConsume[1] = moveConsumeTable[1][kind];
    moveConsume[2] = moveConsumeTable[2][kind];

    if (style == "C" && kind == 1)
        moveConsume[2] = 1;
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
        game->scene->remove(text);
        text = 0;
    }
}



void MapRect::mousePressEvent(QGraphicsSceneMouseEvent *event)
{

    if (event->button() == Qt::LeftButton)
    {
        //如果有选中的机器 并且 点击的地图格子可以到达
        if (game->scene->inMoveStatus && game->scene->selectedRobot)
        {
            game->scene->deleteMenu();

            if (game->scene->map->moveMap[x][y] >= 0 || game->scene->inDebugMode)
            {
                game->scene->map->move(game->scene->selectedRobot, x, y);
                game->scene->inMoveStatus  = false;

                game->scene->displayMenu2(game->scene->selectedRobot);

            }
        }
        else if (game->scene->selectedRobot)
        {
            game->scene->robotActionFinished();
        }
        else {
            static QString t[16] = {QString("墙壁"), QString("平地"), QString("平地"), QString("戈壁"), QString("山地"), QString("海"), QString("草地"), QString("森林"), QString("沙漠"), QString("建筑物"), QString("回复站"), QString("基地"), QString("基地"), QString("基地"), QString("基地"), QString("基地")};
            //qDebug() << "mouse Hover mapRect";

            QString s;
            s += "(" + QString::number(this->x) + ", " + QString::number(this->y) + ")\n";
            s += t[kind] + "\n";
            s += QString("\n") + QString("空 ") + QString::number(moveConsume[0]) + "\n";
            s += QString("陆 ") + QString::number(moveConsume[1]) + "\n";
            s += QString("海 ") + QString::number(moveConsume[2]) + "\n";


            game->scene->robotStatus->setString(s);
        }

    }
    else if (event->button() == Qt::RightButton)
    {
        game->scene->cancel();
    }



}
