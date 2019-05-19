#ifndef ROBOTSTATUSVIEW_H
#define ROBOTSTATUSVIEW_H

#include "robot.h"

#include <QGraphicsPixmapItem>



class RobotStatusView : public QObject, public QGraphicsRectItem
{
    Q_OBJECT
public:
    RobotStatusView(int width, int height);

    QGraphicsTextItem * text = 0;

    QGraphicsPixmapItem * pilotImage;
    QGraphicsPixmapItem * robotImage;

    void setString(QString s);

    void showPilotImage(Robot * robot);
    void showRobotImage(Robot * robot);

    void showRobot(Robot * robot);
};

#endif // ROBOTSTATUSVIEW_H
