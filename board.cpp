#include "board.h"

#include <QBrush>

#include "game.h"
extern Game * game;

#include "people.h"
#include "weapon.h"

#include <QDebug>

Board::Board(int width, int height)
{
    QPen pen;
    pen.setWidth(2);
    pen.setColor(Qt::black);
    setPen(pen);

    QBrush brush;
    brush.setStyle(Qt::SolidPattern);
    brush.setColor(Qt::lightGray);
    setBrush(brush);




    setOpacity(0);

    setRect(0, 0, width, height);
    setPos(580, 0);

    QString s = QString("初始");

    text = new QGraphicsTextItem(s, this);
    text->setScale(1.5);
    text->setPos(this->x(), this->y() + 150);

    game->scene->addItem(text);

    pilotImage = new QGraphicsPixmapItem(this);
    pilotImage->setPos(this->x() + 220, this->y() + 370);

    robotImage = new QGraphicsPixmapItem(this);
    robotImage->setPos(this->x() + 200, this->y() + 10);

    game->scene->addItem(pilotImage);
    game->scene->addItem(robotImage);
}

void Board::setString(QString s)
{
    text->setPlainText(s);
}

void Board::showPilotImage(Robot *robot)
{
    QString path = game->workDir + QString("res/images/people/") + QString::number(robot->pilot->id) + QString(".png");
    pilotImage->setPixmap(QPixmap(path));
    //qDebug() << path;
}

void Board::showRobotImage(Robot *robot)
{
    QString path = game->workDir + QString("res/images/robotImg/") + QString::number(robot->id) + QString(".png");
    robotImage->setPixmap(QPixmap(path));
    //qDebug() << path;
}

void Board::showRobot(Robot *robot)
{
    People * pilot = robot->pilot;

    QString s;
    QString tmp;
    static QString t2[2] = {QString("我方"), QString("敌方")};
    s += robot->robotName + "\t" + t2[robot->player] +"\n";
    s += QString("水平\t") + QString::number(robot->level) + "\t类型\t";
    static QString table[256] = {QString("空"), QString("陆"), QString("海")};
    s += table[robot->type] + "\n机动\t";
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

    s+= QString("驾驶员\t") + pilot->name + "\n\n";

    Weapon * weapon1 = robot->weapon1;
    Weapon * weapon2 = robot->weapon2;
    if (weapon1)
    {
        s += weapon1->name + QString("\n命中 ") + QString::number(weapon1->hitRadio) + "\t";
        s += QString("射程\t") + QString::number(weapon1->range) + " ";
        tmp.sprintf("\n  火力：空 %d\t陆 %d\t海 %d", weapon1->firepower[2] + robot->strength, weapon1->firepower[1] + robot->strength, weapon1->firepower[0] + robot->strength);
        s += tmp;
    }
    if (weapon2)
    {
        s += "\n\n";
        s += weapon2->name + QString("\n命中 ") + QString::number(weapon2->hitRadio) + "\t";
        s += QString("射程\t") + QString::number(weapon2->range) + " ";
        tmp.sprintf("\n  火力：空 %d\t陆 %d\t海 %d", weapon2->firepower[2] + robot->strength, weapon2->firepower[1] + robot->strength, weapon2->firepower[0] + robot->strength);
        s += tmp;
    }








    setString(s);

    showPilotImage(robot);
    showRobotImage(robot);

}
