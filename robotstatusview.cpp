#include "robotstatusview.h"


#include <QBrush>

#include "game.h"
extern Game * game;

#include "people.h"
#include "weapon.h"

#include "robot.h"

#include "config.h"
#include "imageresourcemanager.h"
extern Config * config;

#include <QDebug>

RobotStatusView::RobotStatusView(int width, int height)
{



    setOpacity(0);

    setRect(0, 0, width, height);

    QString s = QString("初始");

    text = new QGraphicsTextItem(s, this);
    text->setScale(1.5);
    text->setDefaultTextColor(Qt::black);

    game->scene->add(text);

    pilotImage = new QGraphicsPixmapItem(this);

    robotImage = new QGraphicsPixmapItem(this);

    game->scene->add(pilotImage);
    game->scene->add(robotImage);
}

void RobotStatusView::setString(QString s)
{
    text->setPlainText(s);
}

void RobotStatusView::showPilotImage(Robot *robot)
{
    pilotImage->setPixmap(ImageResourceManager::getPeopleImage(robot->pilot->id));
}

void RobotStatusView::showRobotImage(Robot *robot)
{
    robotImage->setPixmap(ImageResourceManager::getRobotImage(robot->id));
}

void RobotStatusView::showRobot(Robot *robot)
{
    People * pilot = robot->pilot;

    QString s;
    QString tmp;
    static QString t2[2] = {QString("我方"), QString("敌方")};
    s += robot->property.robotName + "\t" + t2[robot->player] +"\n";
    s += QString("水平\t") + QString::number(robot->level) + "\t类型\t";
    static QString table[256] = {QString("空"), QString("陆"), QString("海")};
    s += table[robot->property.type] + "\n机动\t";
    s += QString::number(robot->move) + "\n";
    s += QString("精神\t") + QString::number(pilot->spirit) + "/" + QString::number(pilot->spirit_total);


    s += "\n\n";


    s += QString("强度\t") + QString::number(robot->strength);
    s += "  ";
    s += QString("HP\t") + QString::number(robot->hp) + " / " + QString::number(robot->hp_total);
    s += "\n";
    s += QString("防卫\t") + QString::number(robot->defense);
    s += "  ";
    s += QString("EXP\t") + QString::number(pilot->exp);
    s += "\n";
    s += QString("速度\t") + QString::number(robot->speed);
    s += "  ";
    s += QString("升级还需  ") + QString::number(Robot::exp_update_table[robot->level] - pilot->exp);
    s += "\n\n";

    s+= QString("驾驶员\t") + pilot->property.name + "\n\n";

    Weapon * weapon1 = robot->weapon1;
    Weapon * weapon2 = robot->weapon2;
    if (weapon1)
    {
        s += weapon1->name + QString("\n命中 ") + QString::number(weapon1->hitRadio) + "\t";
        s += QString("射程\t") + QString::number(weapon1->range) + " ";

        int sky = weapon2->firepower[0] == 0 ? 0 :  weapon2->firepower[2] + robot->strength;
        int land = weapon2->firepower[1] == 0 ? 0 :  weapon2->firepower[1] + robot->strength;
        int sea = weapon2->firepower[2] == 0 ? 0 :  weapon2->firepower[0] + robot->strength;

        tmp.sprintf("\n  火力：空 %d\t陆 %d\t海 %d", sky, land, sea);
        s += tmp;
    }
    if (weapon2)
    {
        s += "\n\n";
        s += weapon2->name + QString("\n命中 ") + QString::number(weapon2->hitRadio) + "\t";
        s += QString("射程\t") + QString::number(weapon2->range) + " ";

        int sky = weapon2->firepower[0] == 0 ? 0 :  weapon2->firepower[2] + robot->strength;
        int land = weapon2->firepower[1] == 0 ? 0 :  weapon2->firepower[1] + robot->strength;
        int sea = weapon2->firepower[2] == 0 ? 0 :  weapon2->firepower[0] + robot->strength;

        tmp.sprintf("\n  火力：空 %d\t陆 %d\t海 %d", sky, land, sea);
        s += tmp;
    }








    setString(s);

    showPilotImage(robot);
    showRobotImage(robot);

}

void RobotStatusView::updateXY(int x, int y)
{
    setPos(x, y);
    text->setPos(this->x(), this->y() + 150);
    pilotImage->setPos(this->x() + 220, this->y() + 370);
    robotImage->setPos(this->x() + 200, this->y() + 10);

}
