#ifndef MAPRECT_H
#define MAPRECT_H

#include <QGraphicsPixmapItem>

class MapRect : public QGraphicsPixmapItem
{
public:
    MapRect(int kind, QGraphicsItem * parent = 0);

    QString style;
    int kind;

    //[0 1 2] -> [海 陆 空]
    int moveConsume[3];

    void setMoveConsume();
};

#endif // MAPRECT_H
