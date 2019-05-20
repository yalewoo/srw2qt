#include "datahelper.h"
#include "robot.h"

#include "game.h"
#include "scene_main.h"

#include <QCoreApplication>
#include <QGraphicsColorizeEffect>
extern Game * game;

QVector<int> Robot::exp_update_table;
QVector<RobotData> Robot::robots_init;
QVector<EnemyData> Robot::enemys_init;

Robot::Robot(int id, int player):id(id),player(player)
{
    property = DataHelper::getRobotProperty(id);

    setImage();
    weapon1 = DataHelper::getWeapon(property.weapon1id);
    weapon2 = DataHelper::getWeapon(property.weapon2id);

}

void Robot::setImage()
{
    //机体图标
    QString filename;
    if (player == 0)
    {
        filename.sprintf("1/%d.png", property.img_id);
    }
    else
    {
        filename.sprintf("1enemy/%d.png", property.img_id  - 32);
    }
qDebug() << filename;

    setPixmap(QPixmap(config->robot_icon_path + filename));
}

void Robot::setPilot(int peopleId)
{
    pilot = new People(peopleId);

    pilot->spirit_total = pilot->property.spirit_total0;
    pilot->spirit = pilot->spirit_total;

    hp_total = property.robot_hp0 + pilot->property.hp;
    this->move = property.robot_move0 + pilot->property.move;
    speed = property.robot_speed0 + pilot->property.speed;
    strength = property.robot_strength0 + pilot->property.strength;
    defense = property.robot_defense0 + pilot->property.defense;

    hp = hp_total;

}
void Robot::hoverEnterEvent(QGraphicsSceneHoverEvent *event)
{
    Rect::hoverEnterEvent(event);

    game->scene->robotStatus->showRobot(this);
    update();
}

void Robot::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
    {
        if (game->scene->selectedRobot != this)
        {
            if (game->scene->selectedWeapon && game->scene->map->AttackMap[x][y] >= 0)
            {
                game->scene->music_effect->setMusicOnce(config->button_press_music);
                game->scene->attack(this);
            }
            else if (game->scene->selectedRobot && game->scene->map->canAttack(game->scene->selectedRobot, game->scene->selectedRobot->weapon1, this))
            {
                game->scene->selectedWeapon = game->scene->selectedRobot->weapon1;
                game->scene->music_effect->setMusicOnce(config->button_press_music);
                game->scene->attack(this);
            }
            else if (game->scene->selectedRobot && game->scene->map->canAttack(game->scene->selectedRobot, game->scene->selectedRobot->weapon2, this))
            {
                game->scene->selectedWeapon = game->scene->selectedRobot->weapon2;
                game->scene->music_effect->setMusicOnce(config->button_press_music);
                game->scene->attack(this);
            }
            else if (game->scene->selectedRobot)
            {
                game->scene->robotActionFinished();
            }
            else if (game->scene->inDebugMode || active)
            {
                game->scene->map->UnshowMoveRange();
                game->scene->selectedRobot = this;
                game->scene->originalPosition = Point(x, y);
                game->scene->map->showMoveRange(this);
                game->scene->inMoveStatus = true;
                game->scene->displayMenu(this);
            }
        }
        else
        {
            game->scene->robotActionFinished();
        }
    }
    else if (event->button() == Qt::RightButton)
    {
        game->scene->cancel();
    }
}

void Robot::setNotActive()
{
    active = false;
    QGraphicsColorizeEffect *e1 = new QGraphicsColorizeEffect();
    e1->setColor(QColor(66,66,66));
    setGraphicsEffect(e1);
    update();
    QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
}

void Robot::setActive()
{
    active = true;
    setGraphicsEffect(0);
    update();
    QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
}

//攻击范围内有没有可以攻击的机体
Robot * Robot::canAttack(Weapon *weapon)
{
    if (weapon == 0)
        return 0;

    QVector<QVector<int> > m = game->scene->map->calculateAttackRange(this, weapon);
    for (int i = 0; i < game->scene->map->width; ++i)
    {
        for (int j = 0; j < game->scene->map->height; ++j)
        {
            if (m[i][j] >= 0)
            {
                Robot * enemy = game->scene->map->robots[i][j];
                if (enemy && enemy != this && enemy->player != this->player && weapon->firepower[enemy->property.type])
                {
                    return enemy;
                }

                //修理装置
                if (weapon->id == 164 && enemy && enemy->player == this->player && enemy->hp < enemy->hp_total)
                {
                    return enemy;
                }
            }
        }
    }
    return 0;
}

void Robot::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    if (!active)
    {
        QGraphicsColorizeEffect *e1 = new QGraphicsColorizeEffect();
        e1->setColor(QColor(66,66,66));
        setGraphicsEffect(e1);
    }
    else
    {

    }
    QGraphicsPixmapItem::paint(painter, option, widget);
}
