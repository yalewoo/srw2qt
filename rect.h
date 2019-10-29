#ifndef RECT_H
#define RECT_H

#include <QGraphicsPixmapItem>

class Rect : public QGraphicsPixmapItem
{
public:
    int m_x;
    int m_y;
    Rect(QGraphicsPixmapItem * parent = 0);
    void setxy(int x, int y);

    //event
    void hoverEnterEvent(QGraphicsSceneHoverEvent *event);
    void hoverLeaveEvent(QGraphicsSceneHoverEvent *event);

    void mousePressEvent(QGraphicsSceneMouseEvent * event);
};

#endif // RECT_H
