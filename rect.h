#ifndef RECT_H
#define RECT_H

#include <QGraphicsPixmapItem>

class Rect : public QGraphicsPixmapItem
{
public:
    int x;
    int y;
    Rect(QGraphicsPixmapItem * parent = 0);
    void setxy(int x, int y);
};

#endif // RECT_H
