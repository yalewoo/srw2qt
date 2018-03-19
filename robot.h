#ifndef ROBOT_H
#define ROBOT_H


#include <QGraphicsPixmapItem>

class Robot : public QGraphicsPixmapItem
{
public:
    Robot(int id, QGraphicsItem * parent = 0);

    QString name;
    QString owner;

    bool canMove;

    int kind;


    int moveKind;   //[0 1 2] -> [海 陆 空]
    int moveValue;

    void setxy(int x, int y);
    int x;
    int y;


};

#endif // ROBOT_H
