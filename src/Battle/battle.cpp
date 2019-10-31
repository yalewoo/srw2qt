#include "battle.h"

#include "game.h"
extern Game * game;

#include "battleground.h"
#include "qmath.h"

#include <QCoreApplication>



Battle::Battle(Robot *robot, Weapon *weapon, Robot *enemy)
{
    if (weapon->id == 164)  //修理武器
    {
        int hpp = qMin(robot->m_hp_total / 2, enemy->m_hp_total - enemy->m_hp);
        enemy->m_hp += hpp;
        QString text = "+" + QString::number(hpp);
        game->scene->map->showText(enemy->m_x, enemy->m_y, text);

        game->scene->robotStatus->showRobot(enemy);

        game->scene->attackDone();

    }
    else
    {
        BattleGround * battleGround = new BattleGround(robot, weapon, enemy);

        battleGround->setSize(game->scene->map->width*32, game->scene->map->height*32);
        game->scene->addItem(battleGround);

        waitVariableToBeTrueAllEvents(battleGround->battlefinished);


        game->scene->attackDone();
        game->scene->removeItem(battleGround);
        delete battleGround;
    }
}
