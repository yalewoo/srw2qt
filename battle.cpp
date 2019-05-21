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
        int hpp = qMin(robot->hp_total / 2, enemy->hp_total - enemy->hp);
        enemy->hp += hpp;

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
