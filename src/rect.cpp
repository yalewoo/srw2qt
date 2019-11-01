#include "rect.h"

#include "datareader/config.h"
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
    m_x = xPos;
    m_y = yPos;

    int rectWidth = config->rectWidth;
    setPos(rectWidth*m_x, rectWidth*m_y);
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

void Rect::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    // qDebug() << "base rect mouse press";
    game->musicManager->PlayButtonClicked();
}
