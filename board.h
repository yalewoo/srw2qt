#ifndef PANEL_H
#define PANEL_H

#include <QGraphicsRectItem>
#include <QObject>

#include "robot.h"

class Board : public QObject, public QGraphicsRectItem
{
    Q_OBJECT
public:
    Board();

    QGraphicsTextItem * text = 0;

    void setString(QString s);

    void showRobot(Robot * robot);
};

#endif // PANEL_H
