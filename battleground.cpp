#include <QCoreApplication>
#include <QEventLoop>


#include "battleground.h"

#include <QBrush>

#include "game.h"
extern Game * game;

#include "point.h"

#include "people.h"

#include <QTime>
#include <QDebug>
#include <QTimer>

#include <QtMath>

void BattleGround::setSize(int width, int height)
{
    int x = 0;
    int y = 0;

    setRect(x, y, width, height);


    QBrush brush;
    brush.setStyle(Qt::SolidPattern);
    brush.setColor(Qt::gray);
    setBrush(brush);
    setOpacity(0.85);
}

void BattleGround::showAttackAnimation(Robot *robot)
{
    game->music_effect->setMusicOnce(game->workDir + "res/wav/huoqiu.mp3");

    weapon_image = new QGraphicsPixmapItem(this);
    if (robot->player == 0)
    {
        //玩家攻击 从左向右
        move_left = true;
        weapon_image->setPixmap(QPixmap(game->workDir + "/res/images/weapon/fire.png"));
        weapon_image->setPos(this->x() + 250, this->y() + 10);

        move_timer = new QTimer(this);
        connect(move_timer, SIGNAL(timeout()), this, SLOT(moveAnimation()));
        move_timer->start(1000/fps);
    }
    else
    {
        //电脑攻击 从右向左
        move_left = false;
        weapon_image->setPixmap(QPixmap(game->workDir + "/res/images/weapon/fire.png"));
        weapon_image->setPos(this->x() + 100, this->y() + 10);

        move_timer = new QTimer(this);
        connect(move_timer, SIGNAL(timeout()), this, SLOT(moveAnimation()));
        move_timer->start(1000/fps);
    }
}

BattleGround::BattleGround(Robot *robot2, Weapon *weapon2, Robot *enemy2):
    robot(robot2),weapon(weapon2),enemy(enemy2)
{
    playerRobot = new QGraphicsPixmapItem(this);
    QString path = game->workDir + QString("res/images/robotImg/") + QString::number(robot->id) + QString(".png");
    playerRobot->setPixmap(QPixmap(path));

    enemyRobot = new QGraphicsPixmapItem(this);
    path = game->workDir + QString("res/images/robotImg/") + QString::number(enemy->id) + QString(".png");
    enemyRobot->setPixmap(QPixmap(path));

    int music_id;
    if (robot->player == 1)
    {
        music_id = enemy->pilot->music_id;
        playerRobot->setPos(this->x() + 10, this->y() + 10);
        enemyRobot->setPos(this->x() + 300, this->y() + 10);

    }
    else
    {
        music_id = robot->pilot->music_id;
        playerRobot->setPos(this->x() + 300, this->y() + 10);
        enemyRobot->setPos(this->x() + 10, this->y() + 10);
    }

    QString s;

    s.sprintf("res/music/%X.wav", music_id);
    qDebug() << s;
    s = game->workDir + s;
    game->music_battle->setMusicLoop(s);


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
    s.sprintf("HP %d/%d\n命中 %.0f %%", robot->hp, robot->hp_total,player_rate);

    if (robot->player == 1)
    {
        player_text->setPlainText(s);
        s.sprintf("HP %d/%d\n命中 %.0f %%", enemy->hp, enemy->hp_total, enemy_rate);
        enemy_text->setPlainText(s);
    }
    else
    {
        enemy_text->setPlainText(s);
        s.sprintf("HP %d/%d\n命中 %.0f %%", enemy->hp, enemy->hp_total, enemy_rate);
        player_text->setPlainText(s);
    }


}

void BattleGround::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    if (!move_finished)
        return;

    QString s;
    if (stage == 0)
    {
        s = robot->pilot->name + QString("：打！\n");
        battle_text->setPlainText(s);

        move_finished = false;
        showAttackAnimation(robot);
        while (!move_finished)
            QCoreApplication::processEvents(QEventLoop::AllEvents, 100);


        if (prob(player_rate))
        {
            int damage = getEnemyDamage();
            enemy->hp = qMax(enemy->hp - damage, 0);
            showHpAndRate();

            s += enemy->robotName + QString("损坏") + QString::number(damage) + "\n" + enemy->pilot->name + QString(": 被打中了!");
        }
        else
        {
            s += QString("攻击失败!");
        }

        battle_text->setPlainText(s);

        if (enemy->hp > 0)
            ++stage;
        else
            stage = 3;
    }
    else if (stage == 1)
    {
        if (enemy_weapon == 0)
        {
            s = enemy->pilot->name + QString("无力反击\n");
            s += enemy->pilot->name + QString("：便宜你了!");
            battle_text->setPlainText(s);
        }
        else
        {
            s = enemy->pilot->name + QString("反击\n");
            battle_text->setPlainText(s);

            move_finished = false;
            showAttackAnimation(enemy);
            while (!move_finished)
                QCoreApplication::processEvents(QEventLoop::AllEvents, 100);

            if (prob(enemy_rate))
            {
                int damage = getPlayerDamage();
                robot->hp  = qMax(robot->hp - damage, 0);
                showHpAndRate();

                s += robot->robotName + QString("损坏") + QString::number(damage) + "\n" + robot->pilot->name + QString(": 被打中了!");
            }
            else {
                s += QString("攻击失败!");
            }

            battle_text->setPlainText(s);

        }

        if (robot->hp > 0)
            ++stage;
        else
        {
            stage = 3;
        }
    }
    else if (stage == 2)
    {
        if (enemy->hp > 0 && robot->speed - enemy->speed >= 50)
        {
            s = robot->robotName + QString("再次攻击\n");
            battle_text->setPlainText(s);

            move_finished = false;
            showAttackAnimation(robot);
            while (!move_finished)
                QCoreApplication::processEvents(QEventLoop::AllEvents, 100);


            if (prob(player_rate))
            {
                int damage = getEnemyDamage();
                enemy->hp = qMax(enemy->hp - damage, 0);
                showHpAndRate();

                s += enemy->robotName + QString("损坏") + QString::number(damage) + "\n" + enemy->pilot->name + QString(": 被打中了!");
            }
            else
            {
                s += QString("攻击失败!");
            }

            battle_text->setPlainText(s);

            ++stage;
        }
        else if (robot->hp >= 0 && enemy->speed - robot->speed >= 50 && enemy_weapon != 0)
        {
            s = enemy->robotName + QString("再次反击\n");
            battle_text->setPlainText(s);

            move_finished = false;
            showAttackAnimation(enemy);
            while (!move_finished)
                QCoreApplication::processEvents(QEventLoop::AllEvents, 100);

            if (prob(enemy_rate))
            {
                int damage = getPlayerDamage();
                robot->hp  = qMax(robot->hp - damage, 0);
                showHpAndRate();

                s += robot->robotName + QString("损坏") + QString::number(damage) + "\n" + robot->pilot->name + QString(": 被打中了!");
            }
            else {
                s += QString("攻击失败!");
            }
            battle_text->setPlainText(s);

            ++stage;
        }
        else
        {
            game->music_battle->stop();
            battlefinished = true;
        }
    }
    else
    {
        game->music_battle->stop();
        battlefinished = true;
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


double BattleGround::calcRadio(Robot *robot2, Weapon *weapon2, Robot *enemy2)
{
    if (weapon2 == 0)
        return 0;

    int base = robot2->speed + weapon2->hitRadio - enemy2->speed;


    static double typeRadioTable[16] = {1,1,0.8,0.8,0.85,0.9,0.9,0.8,0.8,0.8,0.8,0.8,0.8,0.8,0.8,0.8};
    double typeRadio;
    if (enemy2->type == 2)
    {
        typeRadio = 1;
    }
    else
    {
        typeRadio = typeRadioTable[game->map->map[enemy2->x][enemy2->y]->kind];
    }



    double distanceRadio;
    int d = game->map->calcDistance(robot2, enemy2);
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

bool BattleGround::prob(double p)
{
    //return true;


    qsrand(QTime::currentTime().msec());
    int r = qrand() % 100;
    double newp = r;
    qDebug() << newp << p;
    if (newp < p)
        return true;
    else
        return false;
}

void BattleGround::moveAnimation()
{
    if (move_left)
    {
        if (weapon_image->x() > 100)
            weapon_image->setPos(weapon_image->x() - 1, weapon_image->y());
        else
        {
            move_finished = true;
            disconnect(move_timer, SIGNAL(timeout()), this, SLOT(moveAnimation()));
            move_timer->stop();
            delete move_timer;

            game->scene->removeItem(weapon_image);
            delete weapon_image;
        }
    }
    else
    {
        if (weapon_image->x() < 250)
            weapon_image->setPos(weapon_image->x() + 1, weapon_image->y());
        else
        {
            move_finished = true;
            disconnect(move_timer, SIGNAL(timeout()), this, SLOT(moveAnimation()));
            move_timer->stop();
            delete move_timer;

            game->scene->removeItem(weapon_image);
            delete weapon_image;
        }
    }



}
