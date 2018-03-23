#include "robot.h"

#include "game.h"
extern Game * game;
#include <QDebug>

Robot::Robot(int id, int player, QGraphicsItem *parent):id(id), player(player)
{
    QString filename;
    filename.sprintf("%04x.png", id);

    qDebug() << filename;
    setPixmap(QPixmap(game->workDir + "res/images/robot32/" + filename));

    Attributes();
}

void Robot::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
    {
        if (game->menu == 0 && !game->canMoveStatus)
        {
            game->selectedRobot = this;
            game->originalPosition = Point(x, y);

            game->displayMenu(this);
        }

    }
    else if (event->button() == Qt::RightButton)
    {
        game->cancel();
    }


}

bool Robot::canAttack(Weapon *weapon)
{
    QVector<QVector<int> > m = game->map->calculateAttackRange(this, weapon);
    for (int i = 0; i < game->map->width; ++i)
    {
        for (int j = 0; j < game->map->height; ++j)
        {
            if (m[i][j] >= 0)
            {
                Robot * enemy = game->map->robots[i][j];
                if (enemy && enemy != this && weapon->firepower[enemy->type])
                {
                    return true;
                }
            }
        }
    }
    return false;
}

void Robot::setxy(int xPos, int yPos)
{
    x = xPos;
    y = yPos;

    setPos(32*x, 32*y);
}

void Robot::setxy(Point pos)
{
    setxy(pos.x, pos.y);
}

void Robot::Attributes()
{
    robotName = QString("刚达");
    driverName = QString("大卫");

    hp = 320;

    type = 1;
    move = 6;
    spirit = 40;
    strength = 70;
    defense = 55;
    speed = 72;

    weapon1 = new Weapon(1);
    weapon2 = new Weapon(2);
}
