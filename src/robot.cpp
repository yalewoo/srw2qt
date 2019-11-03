#include "datareader/datahelper.h"
#include "robot.h"

#include "game.h"
#include "datareader/imageresourcemanager.h"
#include "common.h"
#include "scene_main.h"

#include <QCoreApplication>
#include <QGraphicsColorizeEffect>
extern Game * game;

QVector<int> Robot::exp_update_table;
QVector<RobotData> Robot::robots_init;
QVector<EnemyData> Robot::enemys_init;

void Robot::gotExp(int exp)
{
    m_pilot->exp += exp;
    if (m_pilot->exp >= exp_update_table[m_level])
    {
        updateLevel();
    }
}

void Robot::updateLevel()
{
    if (m_player == 0)
    {
        int i = 1;
        while (m_pilot->exp >= exp_update_table[i])
        {
            ++i;
        }
        m_level = i;
    }
    else {

    }


    m_hp_total = m_property.robot_hp0 + m_pilot->property.hp + getLevelPropertyPlus(m_property.robot_hp_plus, m_level);
    this->m_move = m_property.robot_move0 + m_pilot->property.move;
    m_speed = m_property.robot_speed0 + m_pilot->property.speed + getLevelPropertyPlus(m_property.robot_speed_plus, m_level);
    m_strength = m_property.robot_strength0 + m_pilot->property.strength + getLevelPropertyPlus(m_property.robot_strength_plus, m_level);
    m_defense = m_property.robot_defense0 + m_pilot->property.defense + getLevelPropertyPlus(m_property.robot_defense_plus, m_level);
}



void Robot::AddPassenger(Robot *passenger)
{
    m_passengers.push_back(passenger);
    passenger->m_inMainShip = true;
}
void Robot::DeletePassenger(Robot *passenger)
{
    int i = 0;
    for ( ; i < m_passengers.length(); ++i)
    {
        if (m_passengers[i] == passenger)
        {
            break;
        }
    }
    if (i < m_passengers.length())
    {
        m_passengers.remove(i);
    }
    passenger->m_inMainShip = false;
}

QVector<int> Robot::canTransform()
{
    QVector<int> lists;
    int id2 = m_id;


    if ((m_property.type_original & 0x0c) == 0)
    {
        return lists;
    }
    int index = m_property.type_original & 0x30;
    while (1)
    {
        RobotProperty prev = DataHelper::getRobotProperty(--id2);
        if ((prev.type_original & 0x0c) == (m_property.type_original & 0x0c))
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
    id2 = m_id;
    while (1)
    {
        RobotProperty prev = DataHelper::getRobotProperty(++id2);
        if ((prev.type_original & 0x0c) == (m_property.type_original & 0x0c))
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
    m_id = targetId;
    m_property = DataHelper::getRobotProperty(targetId);

    setImage();
    delete m_weapon1;
    delete m_weapon2;

    m_weapon1 = DataHelper::getWeapon(m_property.weapon1id);
    m_weapon2 = DataHelper::getWeapon(m_property.weapon2id);

    updateLevel();

    // 盖塔变形同时换驾驶员
    if ((m_property.type_original & 0x0c) == 0x08)
    {
        int exp = m_pilot->exp;
        if (m_property.type_original & 0x10)
        {
            delete m_pilot;
            this->setPilot(8);
        }
        else if (m_property.type_original & 0x20) {
            delete m_pilot;
            this->setPilot(9);
        }
        else if (m_property.type_original & 0x30)
        {
            delete m_pilot;
            this->setPilot(10);
        }
        this->m_pilot->exp = exp;
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

Robot::Robot(int id, int player):m_id(id),m_player(player)
{
    m_property = DataHelper::getRobotProperty(id);

    setImage();
    m_weapon1 = DataHelper::getWeapon(m_property.weapon1id);
    m_weapon2 = DataHelper::getWeapon(m_property.weapon2id);

}


Robot::~Robot()
{
    delete m_weapon1;
    delete m_weapon2;
    delete m_pilot;
}

void Robot::setImage()
{
    setPixmap(ImageResourceManager::getRobotIcon(m_property.img_id, m_player));
}

void Robot::setPilot(int peopleId)
{
    m_pilot = new People(peopleId);

    m_pilot->spirit_total = m_pilot->property.spirit_total0;
    m_pilot->spirit = m_pilot->spirit_total;

    m_hp_total = m_property.robot_hp0 + m_pilot->property.hp + getLevelPropertyPlus(m_property.robot_hp_plus, m_level);
    this->m_move = m_property.robot_move0 + m_pilot->property.move;
    m_speed = m_property.robot_speed0 + m_pilot->property.speed + getLevelPropertyPlus(m_property.robot_speed_plus, m_level);
    m_strength = m_property.robot_strength0 + m_pilot->property.strength + getLevelPropertyPlus(m_property.robot_strength_plus, m_level);
    m_defense = m_property.robot_defense0 + m_pilot->property.defense + getLevelPropertyPlus(m_property.robot_defense_plus, m_level);

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

    // 显示动画时按下鼠标动画加速
    if (game->scene->isMovingRobot || game->scene->map->isShowingAttackGif)
    {
        game->scene->map->moveAnimationSpeed = 10;
        //game->scene->map->move_timer->setInterval(1000/60);
        qDebug() << "isMovingRobot return";
        return;
    }

    if (event->button() == Qt::LeftButton)
    {


        // 给自己修理加hp
        if (game->scene->selectedRobot == this && game->scene->selectedWeapon && game->scene->selectedWeapon->id == 164)
        {
            game->musicManager->PlayButtonClicked();
            game->scene->attack(this);
        }
        // 点击的不是自己时
        else if (game->scene->selectedRobot != this)
        {
            //使用选择的武器攻击
            if (game->scene->selectedWeapon && game->scene->map->AttackMap[m_x][m_y] >= 0)
            {

                game->musicManager->PlayButtonClicked();
                game->scene->attack(this);
            }
            //两个武器都能攻击时，显示菜单让玩家选择武器
            else if (game->scene->selectedRobot && game->scene->map->canAttack(game->scene->selectedRobot, game->scene->selectedRobot->m_weapon1, this)
                     && game->scene->map->canAttack(game->scene->selectedRobot, game->scene->selectedRobot->m_weapon2, this))
            {
                game->scene->enemy = this;
                game->scene->deleteMenu();
                game->scene->displayMenu3(game->scene->selectedRobot, this);
            }
            // 只有武器1能攻击到时自动使用武器1
            else if (game->scene->selectedRobot && game->scene->map->canAttack(game->scene->selectedRobot, game->scene->selectedRobot->m_weapon1, this))
            {
                game->scene->selectedWeapon = game->scene->selectedRobot->m_weapon1;
                game->musicManager->PlayButtonClicked();
                game->scene->attack(this);
            }
            // 只有武器2能攻击到时自动使用武器2
            else if (game->scene->selectedRobot && game->scene->map->canAttack(game->scene->selectedRobot, game->scene->selectedRobot->m_weapon2, this))
            {
                game->scene->selectedWeapon = game->scene->selectedRobot->m_weapon2;
                game->musicManager->PlayButtonClicked();
                game->scene->attack(this);
            }
            else if (game->scene->selectedRobot)
            {
                // 点击我方非母舰其他机器人表示动作结束
                if (this->m_pilot->id != 54)
                {
                    game->scene->robotActionFinished();
                }
                else { // this->pilot->id == 54
                    // 可以搭载到母舰里
                    if (game->scene->map->moveMap[m_x][m_y] >= 0)
                    {
                        // this是母舰
                        this->m_passengers.push_back(game->scene->selectedRobot);
                        game->scene->selectedRobot->m_inMainShip = true;

                        game->scene->map->moveAnimation(game->scene->selectedRobot, m_x, m_y);
                        game->scene->inMoveStatus  = false;
                        game->scene->remove(game->scene->selectedRobot);
                        game->scene->robotActionFinished();

                    }
                    // 距离不够 无法搭载
                    else {
                        game->scene->robotActionFinished();
                    }
                }

            }
            // 点击机体显示移动范围和菜单
            else if (game->scene->inDebugMode || m_active)
            {
                game->scene->map->UnshowMoveRange();
                game->scene->selectedRobot = this;
                game->scene->originalPosition = Point(m_x, m_y);
                game->scene->map->showMoveRange(this);
                game->scene->inMoveStatus = true;
                game->scene->displayMenu(this);
            }
            // 母舰即使移动过了也可以起飞
            else if (!m_active && m_pilot->id == 54)
            {
                game->scene->map->UnshowMoveRange();
                game->scene->selectedRobot = this;
                game->scene->originalPosition = Point(m_x, m_y);

                game->scene->displayMenu4();
            }
        }
        //  点击自己表示行动结束（无动作）
        else
        {
            game->scene->robotActionFinished();
        }
    }
    // 右键取消
    else if (event->button() == Qt::RightButton)
    {
        game->scene->cancel();
    }
}

void Robot::setNotActive()
{
    if (m_active)
    {
        m_active = false;
        QGraphicsColorizeEffect *e1 = new QGraphicsColorizeEffect();
        e1->setColor(QColor(66,66,66));
        setGraphicsEffect(e1);
        update();
        QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
    }

}

void Robot::setActive()
{
    if (!m_active)
    {
        m_active = true;
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
                if (enemy && enemy != this && enemy->m_player != this->m_player && weapon->firepower[enemy->m_property.type])
                {
                    return enemy;
                }

                //修理装置
                if (weapon->id == 164 && enemy && enemy->m_player == this->m_player && enemy->m_hp < enemy->m_hp_total)
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
    if (m_shouldPaintUsingActive && (!m_active))
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
