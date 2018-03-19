#include "robot.h"

#include <QDebug>

#include "game.h"
extern Game * game;

Robot::Robot(int id, QGraphicsItem *parent)
{
    QString filename;
    filename.sprintf("%04x.png", id);

    qDebug() << filename;
    setPixmap(QPixmap("../srw2QT/res/images/robot32/" + filename));


}

void Robot::setxy(int xPos, int yPos)
{
    x = xPos;
    y = yPos;

    setPos(x*32, y*32);
}
