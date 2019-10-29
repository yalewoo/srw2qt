#ifndef MAPRECT_H
#define MAPRECT_H

#include "rect.h"


class MapRect : public Rect
{

public:
    int m_kind;   //16种地形
    QString m_style;

    int m_moveConsume[3]; //移动需要的行动力 [0 1 2] -> [空 陆 海]
    void setMoveConsume();

    MapRect(int kind);
    ~MapRect();


    //显示一些文字
    void showString(QString m_text);
    void showString2(QString m_text);
    QGraphicsSimpleTextItem  * m_text = nullptr;
    void UnshowString();


    //event
    void mousePressEvent(QGraphicsSceneMouseEvent * event);
};

#endif // MAPRECT_H
