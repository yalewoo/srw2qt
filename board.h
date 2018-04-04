#ifndef PANEL_H
#define PANEL_H

#include <QGraphicsRectItem>
#include <QObject>

#include "robot.h"

class Board : public QObject, public QGraphicsRectItem
{
    Q_OBJECT
public:
    Board(int width, int height);

    QGraphicsTextItem * text = 0;

    QGraphicsPixmapItem * pilotImage;
    QGraphicsPixmapItem * robotImage;

    void setString(QString s);

    void showPilotImage(Robot * robot);
    void showRobotImage(Robot * robot);

    void showRobot(Robot * robot);
};

#endif // PANEL_H
