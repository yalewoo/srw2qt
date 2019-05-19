#include "rect.h"

#include "config.h"
extern Config * config;

Rect::Rect(QGraphicsPixmapItem * parent) : QGraphicsPixmapItem(parent)
{

}

void Rect::setxy(int xPos, int yPos)
{
    x = xPos;
    y = yPos;

    int rectWidth = config->rectWidth;
    setPos(rectWidth*x, rectWidth*y);
}
