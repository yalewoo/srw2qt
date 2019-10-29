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
    game->scene->music_effect->setMusicOnce(config->attack_fire_music);

    weapon_image = new QGraphicsPixmapItem(this);
    if (robot->m_player == 0)
    {
        //玩家攻击 从左向右
        move_left = true;
        weapon_image->setPixmap(QPixmap(config->attack_fire_img));
        weapon_image->setPos(this->x() + 250, this->y() + 10);

        move_timer = new QTimer(this);
        connect(move_timer, SIGNAL(timeout()), this, SLOT(moveAnimation()));
        move_timer->start(1000/fps);
    }
    else
    {
        //电脑攻击 从右向左
        move_left = false;
        weapon_image->setPixmap(QPixmap(config->attack_fire_img));
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
    QString path = config->robot_image_path + QString::number(robot->m_id) + QString(".png");
    playerRobot->setPixmap(QPixmap(path));

    enemyRobot = new QGraphicsPixmapItem(this);
    path = config->robot_image_path + QString::number(enemy->m_id) + QString(".png");
    enemyRobot->setPixmap(QPixmap(path));

    int music_id;
    if (robot->m_player == 1)
    {
        music_id = enemy->m_pilot->property.music_id;
        playerRobot->setPos(this->x() + 10, this->y() + 10);
        enemyRobot->setPos(this->x() + 300, this->y() + 10);

    }
    else
    {
        music_id = robot->m_pilot->property.music_id;
        playerRobot->setPos(this->x() + 300, this->y() + 10);
        enemyRobot->setPos(this->x() + 10, this->y() + 10);
    }

    QString s;

    s.sprintf("%X.wav", music_id);
    game->scene->music_battle->setMusicLoop(config->background_music_path_root + s);


    s = robot->m_property.robotName + QString("攻击!");
    battle_text = new QGraphicsTextItem(s, this);
    battle_text->setScale(2.5);
    battle_text->setPos(this->x() + 10, this->y() + 400);

    enemy_weapon = getEnemyBackWeapon();

    s = QString("");
    player_text = new QGraphicsTextItem(s, this);
    player_text->setScale(2.5);
    player_text->setPos(this->x() + 10, this->y() + 300);
    enemy_text = new QGraphicsTextItem(s, this);
    enemy_text->setScale(2.5);
    enemy_text->setPos(this->x() + 10 + 300, this->y() + 300);


    enemy_rate = calcRadio(enemy, enemy_weapon, robot);
    player_rate = calcRadio(robot, weapon, enemy);

    showHpAndRate();


}



void BattleGround::showHpAndRate()
{
    QString s;
    s.sprintf("HP %d/%d\n命中 %.0f %%", robot->m_hp, robot->m_hp_total,player_rate);

    if (robot->m_player == 1)
    {
        player_text->setPlainText(s);
        s.sprintf("HP %d/%d\n命中 %.0f %%", enemy->m_hp, enemy->m_hp_total, enemy_rate);
        enemy_text->setPlainText(s);
    }
    else
    {
        enemy_text->setPlainText(s);
        s.sprintf("HP %d/%d\n命中 %.0f %%", enemy->m_hp, enemy->m_hp_total, enemy_rate);
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
        s = robot->m_pilot->property.name + QString("：打！\n");
        battle_text->setPlainText(s);

        move_finished = false;
        showAttackAnimation(robot);
        while (!move_finished)
            QCoreApplication::processEvents(QEventLoop::AllEvents, 100);


        if (prob(player_rate))
        {
            int damage = getEnemyDamage();
            enemy->m_hp = qMax(enemy->m_hp - damage, 0);
            showHpAndRate();

            s += enemy->m_property.robotName + QString("损坏 ") + QString::number(damage) + "\n" + enemy->m_pilot->property.name + QString(": 被打中了!");
        }
        else
        {
            s += QString("攻击失败!");
        }

        battle_text->setPlainText(s);

        if (enemy->m_hp > 0)
            ++stage;
        else
            stage = 3;
    }
    else if (stage == 1)
    {
        if (enemy_weapon == 0)
        {
            s = enemy->m_pilot->property.name + QString("无力反击\n");
            s += enemy->m_pilot->property.name + QString("：便宜你了!");
            battle_text->setPlainText(s);
        }
        else
        {
            s = enemy->m_pilot->property.name + QString("反击\n");
            battle_text->setPlainText(s);

            move_finished = false;
            showAttackAnimation(enemy);
            while (!move_finished)
                QCoreApplication::processEvents(QEventLoop::AllEvents, 100);

            if (prob(enemy_rate))
            {
                int damage = getPlayerDamage();
                robot->m_hp  = qMax(robot->m_hp - damage, 0);
                showHpAndRate();

                s += robot->m_property.robotName + QString("损坏 ") + QString::number(damage) + "\n" + robot->m_pilot->property.name + QString(": 被打中了!");
            }
            else {
                s += QString("攻击失败!");
            }

            battle_text->setPlainText(s);

        }

        if (robot->m_hp > 0)
            ++stage;
        else
        {
            stage = 3;
        }
    }
    else if (stage == 2)
    {
        if (enemy->m_hp > 0 && robot->m_speed - enemy->m_speed >= 50)
        {
            s = robot->m_property.robotName + QString("再次攻击\n");
            battle_text->setPlainText(s);

            move_finished = false;
            showAttackAnimation(robot);
            while (!move_finished)
                QCoreApplication::processEvents(QEventLoop::AllEvents, 100);


            if (prob(player_rate))
            {
                int damage = getEnemyDamage();
                enemy->m_hp = qMax(enemy->m_hp - damage, 0);
                showHpAndRate();

                s += enemy->m_property.robotName + QString("损坏 ") + QString::number(damage) + "\n" + enemy->m_pilot->property.name + QString(": 被打中了!");
            }
            else
            {
                s += QString("攻击失败!");
            }

            battle_text->setPlainText(s);

            ++stage;
        }
        else if (robot->m_hp >= 0 && enemy->m_speed - robot->m_speed >= 50 && enemy_weapon != 0)
        {
            s = enemy->m_property.robotName + QString("再次反击\n");
            battle_text->setPlainText(s);

            move_finished = false;
            showAttackAnimation(enemy);
            while (!move_finished)
                QCoreApplication::processEvents(QEventLoop::AllEvents, 100);

            if (prob(enemy_rate))
            {
                int damage = getPlayerDamage();
                robot->m_hp  = qMax(robot->m_hp - damage, 0);
                showHpAndRate();

                s += robot->m_property.robotName + QString("损坏 ") + QString::number(damage) + "\n" + robot->m_pilot->property.name + QString(": 被打中了!");
            }
            else {
                s += QString("攻击失败!");
            }
            battle_text->setPlainText(s);

            ++stage;
        }
        else
        {
            game->scene->music_battle->stop();
            battlefinished = true;
        }
    }
    else
    {
        game->scene->music_battle->stop();
        battlefinished = true;
    }
}


Weapon *BattleGround::getEnemyBackWeapon()
{
    if (game->scene->map->canAttack(enemy, enemy->m_weapon1, robot))
    {
        if (game->scene->map->canAttack(enemy, enemy->m_weapon2, robot))
        {
            if (enemy->m_weapon1->firepower[robot->m_property.type] > enemy->m_weapon2->firepower[robot->m_property.type])
                return enemy->m_weapon1;
            else
                return enemy->m_weapon2;
        }
        else
            return enemy->m_weapon1;
    }
    if (game->scene->map->canAttack(enemy, enemy->m_weapon2, robot))
        return enemy->m_weapon2;
    return 0;
}

int BattleGround::getEnemyDamage()
{
    return getDamage(robot, enemy, weapon);
}

int BattleGround::getPlayerDamage()
{
    return getDamage(enemy, robot, enemy_weapon);
}
int BattleGround::getDamage(Robot * robot2, Robot * enemy2, Weapon *weapon2)
{
    int damage = robot2->m_strength + weapon2->firepower[enemy2->m_property.type] - enemy2->m_defense;
    if (robot2->m_spirit[10])
    {
        damage *= 2;
    }
    if (robot2->m_spirit[4])
    {
        damage *= 2;
    }

    if (enemy2->m_spirit[3])
    {
        damage /= 2;
    }
    return damage;
}

int BattleGround::getMaxDamage(Robot *robot2, Robot *enemy2)
{
    int damage1 = getDamage(robot2, enemy2, robot2->m_weapon1);
    int damage2 = getDamage(robot2, enemy2, robot2->m_weapon2);
    return qMax(damage1, damage2);
}


double BattleGround::calcRadio(Robot *robot2, Weapon *weapon2, Robot *enemy2)
{
    if (weapon2 == 0)
        return 0;

    if (enemy2->m_spirit[8])  //使用回避精神
        return 0;

    int base = robot2->m_speed + weapon2->hitRadio - enemy2->m_speed;


    static double typeRadioTable[16] = {1,1,0.8,0.8,0.85,0.9,0.9,0.8,0.8,0.8,0.8,0.8,0.8,0.8,0.8,0.8};
    double typeRadio;
    if (enemy2->m_property.type == 2)
    {
        typeRadio = 1;
    }
    else
    {
        typeRadio = typeRadioTable[game->scene->map->map[enemy2->m_x][enemy2->m_y]->m_kind];
    }



    double distanceRadio;
    int d = game->scene->map->calcDistance(robot2, enemy2);
    if (d <= 1)
    {
        distanceRadio = 1;
    }
    else
    {
        distanceRadio = 1 - 0.05 * (d - 2);
    }

    double res = base * typeRadio * distanceRadio;

    if (robot2->m_spirit[2])
        res += 10;

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
    //qDebug() << newp << p;
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
            weapon_image->setPos(weapon_image->x() - 2, weapon_image->y());
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
            weapon_image->setPos(weapon_image->x() + 2, weapon_image->y());
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
