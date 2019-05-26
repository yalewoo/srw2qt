#include "datahelper.h"
#include "robot.h"

#include "game.h"
#include "imageresourcemanager.h"
#include "scene_main.h"

#include <QCoreApplication>
#include <QGraphicsColorizeEffect>
extern Game * game;

QVector<int> Robot::exp_update_table;
QVector<RobotData> Robot::robots_init;
QVector<EnemyData> Robot::enemys_init;

void Robot::gotExp(int exp)
{
    pilot->exp += exp;
    if (pilot->exp >= exp_update_table[level])
    {
        updateLevel();
    }
}

void Robot::updateLevel()
{
    if (player == 0)
    {
        int i = 1;
        while (pilot->exp >= exp_update_table[i])
        {
            ++i;
        }
        level = i;
    }
    else {

    }


    hp_total = property.robot_hp0 + pilot->property.hp + getLevelPropertyPlus(property.robot_hp_plus, level);
    this->move = property.robot_move0 + pilot->property.move;
    speed = property.robot_speed0 + pilot->property.speed + getLevelPropertyPlus(property.robot_speed_plus, level);
    strength = property.robot_strength0 + pilot->property.strength + getLevelPropertyPlus(property.robot_strength_plus, level);
    defense = property.robot_defense0 + pilot->property.defense + getLevelPropertyPlus(property.robot_defense_plus, level);
}

QVector<int> Robot::canTransform()
{
    QVector<int> lists;
    int id2 = id;


    if ((property.type_original & 0x0c) == 0)
    {
        return lists;
    }
    int index = property.type_original & 0x30;
    while (1)
    {
        RobotProperty prev = DataHelper::getRobotProperty(--id2);
        if ((prev.type_original & 0x0c) == (property.type_original & 0x0c))
        {
            int prev_index = prev.type_original & 0x30;
            if (prev.robot_move0 == 0 || prev_index >= index)
                break;
            lists.append(id2);
        }
        else {
            break;
        }
    }
    id2 = id;
    while (1)
    {
        RobotProperty prev = DataHelper::getRobotProperty(++id2);
        if ((prev.type_original & 0x0c) == (property.type_original & 0x0c))
        {
            int prev_index = prev.type_original & 0x30;
            if (prev.robot_move0 == 0 || prev_index <= index)
                break;
            lists.append(id2);
        }
        else {
            break;
        }
    }
    return lists;
}

void Robot::ChangeId(int targetId)
{
    id = targetId;
    property = DataHelper::getRobotProperty(targetId);

    setImage();
    delete weapon1;
    delete weapon2;

    weapon1 = DataHelper::getWeapon(property.weapon1id);
    weapon2 = DataHelper::getWeapon(property.weapon2id);

    updateLevel();

    // 盖塔变形同时换驾驶员
    if ((property.type_original & 0x0c) == 0x08)
    {
        int exp = pilot->exp;
        if (property.type_original & 0x10)
        {
            delete pilot;
            this->setPilot(8);
        }
        else if (property.type_original & 0x20) {
            delete pilot;
            this->setPilot(9);
        }
        else if (property.type_original & 0x30)
        {
            delete pilot;
            this->setPilot(10);
        }
        this->pilot->exp = exp;
    }


    updateLevel();
}


int Robot::getLevelPropertyPlus(int plusType, int level)
{
    /*  0 +1.5向上取整
        1 +2
        2 +5
        3 +10
        4 麦塔斯速度专用 公式未知
        5 貌似未使用
        6 导弹专属 未知
        7 +2.5向下取整
        8 +3*/
    level = level - 1;
    switch (plusType) {
        case 0:
            return (int)(level * 1.5) + 1;
        case 1:
            return (level * 2);
        case 2:
            return (level * 5);
        case 3:
            return (level * 10);
        case 4:
            return (level * 1.5);
        case 5:
            return (level * 1.5);
        case 6:
            return (level * 1.5);
        case 7:
            return (int)(level * 2.5) - 1;
        case 8:
            return (level * 3);
        default:
            return level * 1.5;
    }
}

Robot::Robot(int id, int player):id(id),player(player)
{
    property = DataHelper::getRobotProperty(id);

    setImage();
    weapon1 = DataHelper::getWeapon(property.weapon1id);
    weapon2 = DataHelper::getWeapon(property.weapon2id);

}


Robot::~Robot()
{
    delete weapon1;
    delete weapon2;
    delete pilot;
}

void Robot::setImage()
{
    setPixmap(ImageResourceManager::getRobotIcon(property.img_id, player));
}

void Robot::setPilot(int peopleId)
{
    pilot = new People(peopleId);

    pilot->spirit_total = pilot->property.spirit_total0;
    pilot->spirit = pilot->spirit_total;

    hp_total = property.robot_hp0 + pilot->property.hp + getLevelPropertyPlus(property.robot_hp_plus, level);
    this->move = property.robot_move0 + pilot->property.move;
    speed = property.robot_speed0 + pilot->property.speed + getLevelPropertyPlus(property.robot_speed_plus, level);
    strength = property.robot_strength0 + pilot->property.strength + getLevelPropertyPlus(property.robot_strength_plus, level);
    defense = property.robot_defense0 + pilot->property.defense + getLevelPropertyPlus(property.robot_defense_plus, level);

}
void Robot::hoverEnterEvent(QGraphicsSceneHoverEvent *event)
{
    Rect::hoverEnterEvent(event);

    game->scene->robotStatus->showRobot(this);
    update();
}

void Robot::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    Rect::mousePressEvent(event);

    if (game->scene->isMovingRobot || game->scene->map->isShowingAttackGif)
    {
        game->scene->map->moveAnimationSpeed = 10;
        //game->scene->map->move_timer->setInterval(1000/60);
        qDebug() << "isMovingRobot return";
        return;
    }

    if (event->button() == Qt::LeftButton)
    {
        if (game->scene->selectedRobot == this && game->scene->selectedWeapon && game->scene->selectedWeapon->id == 164)
        {
            game->scene->music_effect->setMusicOnce(config->button_press_music);
            game->scene->attack(this);
        }
        else if (game->scene->selectedRobot != this)
        {
            if (game->scene->selectedWeapon && game->scene->map->AttackMap[x][y] >= 0)
            {

                game->scene->music_effect->setMusicOnce(config->button_press_music);
                game->scene->attack(this);
            }
            else if (game->scene->selectedRobot && game->scene->map->canAttack(game->scene->selectedRobot, game->scene->selectedRobot->weapon1, this)
                     && game->scene->map->canAttack(game->scene->selectedRobot, game->scene->selectedRobot->weapon2, this))
            {
                game->scene->enemy = this;
                game->scene->deleteMenu();
                game->scene->displayMenu3(game->scene->selectedRobot, this);
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
                if (this->pilot->id != 54)
                {
                    game->scene->robotActionFinished();
                }
                else {
                    // 可以搭载到母舰里
                    if (game->scene->map->moveMap[x][y] >= 0)
                    {
                        this->passengers.push_back(game->scene->selectedRobot);
                        game->scene->selectedRobot->inMainShip = true;

                        game->scene->map->moveAnimation(game->scene->selectedRobot, x, y);
                        game->scene->inMoveStatus  = false;
                        game->scene->remove(game->scene->selectedRobot);
                        game->scene->robotActionFinished();

                    }
                    else {
                        game->scene->robotActionFinished();
                    }
                }

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
            else if (!active && pilot->id == 54)
            {
                game->scene->map->UnshowMoveRange();
                game->scene->selectedRobot = this;
                game->scene->originalPosition = Point(x, y);

                game->scene->displayMenu4();
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
    if (active)
    {
        active = false;
        QGraphicsColorizeEffect *e1 = new QGraphicsColorizeEffect();
        e1->setColor(QColor(66,66,66));
        setGraphicsEffect(e1);
        update();
        QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
    }

}

void Robot::setActive()
{
    if (!active)
    {
        active = true;
        setGraphicsEffect(0);
        update();
        QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
    }

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
    if (shouldPaintUsingActive && (!active))
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
