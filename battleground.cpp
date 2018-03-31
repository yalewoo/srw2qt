#include "battleground.h"

#include <QBrush>

#include "game.h"
extern Game * game;

#include "point.h"

void BattleGround::setSize(int width, int height)
{
    int x = 0;
    int y = 0;

    setRect(x, y, width, height);


    QBrush brush;
    brush.setStyle(Qt::SolidPattern);
    brush.setColor(Qt::gray);
    setBrush(brush);
    setOpacity(0.7);
}

BattleGround::BattleGround(Robot *robot2, Weapon *weapon2, Robot *enemy2):
    robot(robot2),weapon(weapon2),enemy(enemy2)
{
    QString s;
    s = robot->robotName + QString("攻击!");
    battle_text = new QGraphicsTextItem(s, this);
    battle_text->setScale(2);
    battle_text->setPos(this->x() + 10, this->y() + 400);

    enemy_weapon = getEnemyBackWeapon();

    s = QString("");
    player_text = new QGraphicsTextItem(s, this);
    player_text->setScale(1.7);
    player_text->setPos(this->x() + 10, this->y() + 300);
    enemy_text = new QGraphicsTextItem(s, this);
    enemy_text->setScale(1.7);
    enemy_text->setPos(this->x() + 10 + 200, this->y() + 300);

    enemy_rate = calcRadio(enemy, enemy_weapon, robot);
    player_rate = calcRadio(robot, weapon, enemy);

    showHpAndRate();
}



void BattleGround::showHpAndRate()
{
    QString s;
    s.sprintf("HP %d\n命中 %.0f %%", robot->hp, player_rate);
    player_text->setPlainText(s);


    s.sprintf("HP %d\n命中 %.0f %%", enemy->hp, enemy_rate);
    enemy_text->setPlainText(s);
}

void BattleGround::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    QString s;
    if (stage == 0)
    {
        s = robot->driverName + QString("：打！\n");
        battle_text->setPlainText(s);


        int damage = getEnemyDamage();
        enemy->hp -= damage;
        showHpAndRate();

        s += enemy->robotName + QString("损坏") + QString::number(damage) + "\n" + enemy->driverName + QString(": 被打中了!");
        battle_text->setPlainText(s);

        ++stage;
    }
    else if (stage == 1)
    {
        s = enemy->driverName + QString("反击\n");
        battle_text->setPlainText(s);


        int damage = getPlayerDamage();
        robot->hp -= damage;
        showHpAndRate();

        s += robot->robotName + QString("损坏") + QString::number(damage) + "\n" + robot->driverName + QString(": 被打中了!");
        battle_text->setPlainText(s);

        ++stage;
    }
    else
    {
        game->attackDone();
    }
}


Weapon *BattleGround::getEnemyBackWeapon()
{
    if (game->map->canAttack(enemy, enemy->weapon1, robot))
    {
        if (game->map->canAttack(enemy, enemy->weapon2, robot))
        {
            if (enemy->weapon1->firepower[robot->type] > enemy->weapon2->firepower[robot->type])
                return enemy->weapon1;
            else
                return enemy->weapon2;
        }
        else
            return enemy->weapon1;
    }
    if (game->map->canAttack(enemy, enemy->weapon2, robot))
        return enemy->weapon2;
    return 0;
}

int BattleGround::getEnemyDamage()
{
    return robot->strength + weapon->firepower[enemy->type] - enemy->defense;
}

int BattleGround::getPlayerDamage()
{
    return enemy->strength + enemy_weapon->firepower[robot->type] - robot->defense;
}


double BattleGround::calcRadio(Robot *robot, Weapon *weapon, Robot *enemy)
{
    int base = robot->speed + weapon->hitRadio - enemy->speed;


    static double typeRadioTable[16] = {1,1,0.8,0.8,0.85,0.9,0.9,0.8,0.8,0.8,0.8,0.8,0.8,0.8,0.8,0.8};
    double typeRadio;
    if (enemy->type == 2)
    {
        typeRadio = 1;
    }
    else
    {
        typeRadio = typeRadioTable[game->map->map[enemy->x][enemy->y]->kind];
    }



    double distanceRadio;
    int d = game->map->calcDistance(robot, enemy);
    if (d <= 1)
    {
        distanceRadio = 1;
    }
    else
    {
        distanceRadio = 1 - 0.05 * (d - 2);
    }

    double res = base * typeRadio * distanceRadio;
    if (res > 100)
        res = 100;
    return res;
}
