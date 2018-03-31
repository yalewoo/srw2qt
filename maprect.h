#ifndef MAPRECT_H
#define MAPRECT_H

#include <QGraphicsPixmapItem>
#include <QGraphicsSceneMouseEvent>

class MapRect : public QGraphicsPixmapItem
{
public:
    MapRect(int kind, QGraphicsItem * parent = 0);

    QString style;  //风格ABCD
    int kind;   //16种地形

    int moveConsume[3]; //移动需要的行动力 [0 1 2] -> [空 陆 海]
    void setMoveConsume();

    int x;
    int y;
    void setxy(int x, int y);

    //event
    void mousePressEvent(QGraphicsSceneMouseEvent * event);

    //显示一些文字
    void showString(QString text);
    QGraphicsTextItem * text = 0;
    void UnshowString();
};

#endif // MAPRECT_H
