#ifndef MAPRECT_H
#define MAPRECT_H

#include "rect.h"


class MapRect : public Rect
{

public:
    int kind;   //16种地形
    QString style;

    int moveConsume[3]; //移动需要的行动力 [0 1 2] -> [空 陆 海]
    void setMoveConsume();

    MapRect(int kind);


    //显示一些文字
    void showString(QString text);
    QGraphicsTextItem * text = 0;
    void UnshowString();


    //event
    void mousePressEvent(QGraphicsSceneMouseEvent * event);
};

#endif // MAPRECT_H
