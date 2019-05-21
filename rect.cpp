#include "rect.h"

#include "config.h"
extern Config * config;
#include "game.h"
extern Game * game;
#include <QDebug>

Rect::Rect(QGraphicsPixmapItem * parent) : QGraphicsPixmapItem(parent)
{
    setAcceptHoverEvents(true);
}

void Rect::setxy(int xPos, int yPos)
{
    x = xPos;
    y = yPos;

    int rectWidth = config->rectWidth;
    setPos(rectWidth*x, rectWidth*y);
}

void Rect::hoverEnterEvent(QGraphicsSceneHoverEvent *event)
{
    //qDebug() << "rect hovered" << x << y;
//    if (!game->scene->map->select_effect)
//    {
//        game->scene->map->select_effect = new QGraphicsPixmapItem();
//        QGraphicsPixmapItem * effect = game->scene->map->select_effect;
//        effect->setPixmap(QPixmap(config->selected_arrow));
//        effect->setPos(x * 32, y * 32);
//        game->scene->add(effect);
//    }
//    else {
//        game->scene->map->select_effect->setPos(x * 32, y * 32);
//    }
}

void Rect::hoverLeaveEvent(QGraphicsSceneHoverEvent *event)
{
    //qDebug() << "rect hovered leave" << x << y;
}
