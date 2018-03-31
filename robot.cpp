#include "robot.h"

#include "game.h"
extern Game * game;
#include <QDebug>

#include <QFile>

#include "people.h"

#include <qmath.h>

int Robot::exp_update_table[128] = {99999};


Robot::Robot(int id, People *pilot2, int level2, QGraphicsItem *parent):id(id), pilot(pilot2),level(level2), QGraphicsPixmapItem(parent)
{
    get_exp_update_table(game->workDir + "input/value/exp_table.txt");

    setAcceptHoverEvents(true);

    player = pilot->player;

    getAttributes();

    pilot->spirit = pilot->spirit_total;
    hp = hp_total;

    if (player == 1)
    {
        exp = exp_dievalue;
    }
    else
    {

    }
}

void Robot::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
    {
        if (game->menu == 0 && !game->canMoveStatus && !game->selectedWeapon)
        {
            game->selectedRobot = this;
            game->originalPosition = Point(x, y);

            game->displayMenu(this);
        }
        else if (game->selectedWeapon)
        {
            game->attack(this);
        }

    }
    else if (event->button() == Qt::RightButton)
    {
        game->cancel();
    }


}

void Robot::hoverEnterEvent(QGraphicsSceneHoverEvent *event)
{
    game->board->showRobot(this);

    update();
}

void Robot::get_exp_update_table(QString filename)
{
    QFile file(filename);
    if(!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        qDebug() << filename << "open failed";
        exit(-1);

    }
    QTextStream in(&file);

    while(!in.atEnd())
    {
        QString line = in.readLine();

        QStringList t = line.split(":");

        int le = QString(t[0]).toInt();
        int value = QString(t[1]).toInt();

        exp_update_table[le] = value;
    }

    file.close();
}

void Robot::calcLevelValue()
{
    int * v[5];
    v[0] = &robot_hp;
    v[1] = &robot_strength;
    v[2] = &robot_defense;
    v[3] = &robot_speed;
    v[4] = &(pilot->spirit_total);

    int v0[5] = {robot_hp0, robot_strength0, robot_defense0, robot_speed0, pilot->spirit_total0};

    int ps[5] = {robot_hp_plus,robot_strength_plus,robot_defense_plus,robot_speed_plus,pilot->spirit_increase};

    for (int i = 0; i < 5; ++i)
    {
        switch(ps[i])
        {
        case 0:
            *(v[i]) = v0[i] + qCeil(1.5*(level-1));
            break;
        case 1:
            *(v[i]) = v0[i] + 2*(level-1);
            break;
        case 2:
            *(v[i]) = v0[i] + 5*(level-1);
            break;
        case 3:
            *(v[i]) = v0[i] + 10*(level-1);
            break;
        case 4:
            *(v[i]) = v0[i] + 4*(level-1);
            break;
        case 5:
            *(v[i]) = v0[i] + 0*(level-1);
            break;
        case 6:
            *(v[i]) = v0[i] + 0*(level-1);
            break;
        case 7:
            *(v[i]) = v0[i] + qFloor(2.5*(level-1));
            break;
        case 8:
            *(v[i]) = v0[i] + 3*(level-1);
            break;

        }
    }


}

//攻击范围内有没有可以攻击的机体
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
                if (enemy && enemy != this && enemy->player != this->player && weapon->firepower[enemy->type])
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

void Robot::getAttributes()
{
    QFile file(game->robot_value_path);
    if(!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        qDebug() << game->robot_value_path << "open failed";
        exit(-1);

    }
    QTextStream in(&file);

    while(!in.atEnd())
    {
        QString line = in.readLine();

        QStringList t = line.split(",");

        QString thisid = t.at(0);
        int int_id = thisid.toInt();
        if (id == int_id)
        {
            robotName = QString(t.at(1));
            //driverName = QString("大卫");
            type_original = QString(t[3]).toInt();
            type = type_original & 0x3;

            robot_hp0 = QString(t[19]).toInt();
            robot_move = QString(t[6]).toInt();
            robot_speed0 = QString(t[7]).toInt();
            robot_strength0 = QString(t[8]).toInt();
            robot_defense0 = QString(t[9]).toInt();

            robot_hp_plus = QString(t[18]).toInt();
            robot_strength_plus = QString(t[16]).toInt();
            robot_defense_plus = QString(t[17]).toInt();
            robot_speed_plus = QString(t[15]).toInt();

            exp_dievalue = QString(t[13]).toInt();
            money = QString(t[10]).toInt();


            weapon1 = new Weapon(QString(t[24]).toInt());
            weapon2 = new Weapon(QString(t[25]).toInt());


            //机体图标
            int img_id;
            QString filename;
            if (player == 0)
            {
                img_id = QString(t[20]).toInt();
                filename.sprintf("1/%d.png", img_id);
            }
            else
            {
                img_id = QString(t[20]).toInt() - 32;
                filename.sprintf("1enemy/%d.png", img_id);
            }




            qDebug() << filename;
            setPixmap(QPixmap(game->workDir + "res/images/robot32/" + filename));

            break;
        }
    }

    calcLevelValue();

    hp_total = robot_hp + pilot->hp;
    move = robot_move + pilot->move;
    speed = robot_speed + pilot->speed;
    strength = robot_strength + pilot->strength;
    defense = robot_defense + pilot->defense;



    file.close();
}
