#include "map.h"

#include "datahelper.h"

#include "config.h"
extern Config *config;

#include "game.h"
#include "point.h"

#include <QCoreApplication>
#include <QGraphicsColorizeEffect>
#include <QTimer>
extern Game * game;

#include "ai.h"

Map::Map(QObject *parent) : QObject(parent)
{

}

Map::~Map()
{
    for (int i = 0; i < width; ++i)
    {
        for (int j = 0; j < height; ++j)
        {
            if (robots[i][j]){
                delete robots[i][j];
            }
            if (map[i][j])
            {
                delete map[i][j];
            }
        }
    }
}

Robot *Map::getCaptain()
{
    for (int i = 0; i < width; ++i)
    {
        for (int j = 0; j < height; ++j)
        {
            if (robots[i][j] && robots[i][j]->m_pilot->id == 54){
                return robots[i][j];
            }
        }
    }
    return 0;

}

void Map::moveToCaptain(Robot *robot, Robot *captain)
{

}

void Map::loadStage(int stage)
{
    DataHelper::Map mapdata(config->getMapPathOfLevel(stage));
    width = mapdata.width + 2;
    height = mapdata.height + 2;
    //qDebug() << width << height;
    // width = 16, height = 20

    map = QVector<QVector<MapRect *> >(width, QVector<MapRect *>(height, 0));
    robots = QVector<QVector<Robot *> >(width, QVector<Robot *>(height, 0));

    for (int i = 0; i < width; ++i)
    {
        for (int j = 0; j < height; ++j)
        {
            //qDebug() << i << j;

            MapRect * rect;
            if (i == 0 || j == 0 || i == width-1 || j == height-1)
            {
                rect = new MapRect(0);
            }
            else
            {
                int type = mapdata[i][j];
                rect = new MapRect(type);
            }

            rect->setxy(i, j);
            game->scene->add(rect);

            map[i][j] = rect;
        }
    }

    if (game->scene->inDebugMode)
        displayXYLabel();
}

void Map::displayXYLabel()
{
    // 显示坐标
    for (int i = 0; i < width; ++i)
    {
        map[i][0]->showString2(QString::number(i));
        map[i][height-1]->showString2(QString::number(i));
    }
    for (int i = 1; i < height-1; ++i)
    {
        map[0][i]->showString2(QString::number(i));
        map[width-1][i]->showString2(QString::number(i));
    }
}

void Map::addRobot(Robot * robot, int xPos, int yPos)
{
    robots[xPos][yPos] = robot;
    robots[xPos][yPos]->setxy(xPos,yPos);
    game->scene->add(robots[xPos][yPos]);

    //qDebug() << robots[xPos][yPos]->property.robotName << xPos << yPos;
}

bool Map::hasAddRobot(int stage, int round, int player)
{
    bool has = false;
    if (player == 0)
    {
        for (int i = 0; i < Robot::robots_init.length(); ++i)
        {
            RobotData robotData = Robot::robots_init[i];

            if (stage != robotData.stage || round != robotData.round)
                continue;
            else {
                has = true;
                break;
            }

        }
    }
    else {
        for (int i = 0; i < Robot::enemys_init.length(); ++i)
        {
            EnemyData robotData = Robot::enemys_init[i];

            if (stage != robotData.stage || round != robotData.round)
                continue;
            else {
                has = true;
                break;
            }

        }
    }

    return has;
}
void Map::placeRobot(int stage, int round, bool wait)
{
    for (int i = 0; i < Robot::robots_init.length(); ++i)
    {
        RobotData robotData = Robot::robots_init[i];

        if (stage != robotData.stage || round != robotData.round)
            continue;

        int x = robotData.x;
        int y = robotData.y;
        Robot * robot = new Robot(robotData.robotId);

        robot->m_robotBehavior = 1;

        //qDebug() << robotData.robotId;
        robot->setPilot(robotData.peopleId);
        robot->m_hp = robot->m_hp_total;
        //qDebug() << robot->property.robotName << robot->move;
        addRobot(robot, x, y);
    }

}
bool Map::placeEnemy(int stage, int round, bool wait)
{
    bool has = false;
    for (int i = 0; i < Robot::enemys_init.length(); ++i)
    {
        EnemyData robotData = Robot::enemys_init[i];

        if (stage != robotData.stage || round != robotData.round)
            continue;

        int x = robotData.x;
        int y = robotData.y;
        Robot * robot = new Robot(robotData.robotId, 1);
        robot->setPilot(robotData.peopleId);
        robot->m_hp = robot->m_hp_total;

        robot->m_level = robotData.robotLevel+1;
        robot->updateLevel();

        robot->m_robotBehavior = robotData.robotBehavior;

        addRobot(robot, x, y);
        has = true;
    }

    return has;
}

void Map::placeRobotRunTime(RunTimeRobotData robotData)
{
    Robot * c = getCaptain();


    int x = robotData.x;
    int y = robotData.y;
    Robot * backupRobot = robots[x][y];
    Robot * robot = new Robot(robotData.robotId, robotData.player);

    robot->m_robotBehavior = robotData.robotBehavior;

    //qDebug() << robotData.robotId;
    robot->setPilot(robotData.peopleId);
    //qDebug() << robot->property.robotName << robot->move;
    addRobot(robot, x, y);



    robot->m_level = robotData.level;
    robot->m_pilot->exp = robotData.exp;
    robot->m_hp = robotData.hp;
    robot->m_pilot->spirit = robotData.sprit;

    robot->m_inMainShip = robotData.inMainShip;

    if (robotData.active != 1)
    {
        robot->setNotActive();
    }
    if (robot->m_inMainShip)
    {
        if (c)
        {
            c->m_passengers.push_back(robot);
            game->scene->map->robots[x][y] = backupRobot;
            game->scene->remove(robot);
        }

    }
}


void Map::showMoveRange(Robot *robot)
{
    moveMap = calculateMoveRange(robot);
    for (int i = 0; i < width; ++i)
    {
        for (int j = 0; j < height; ++j)
        {

            if (moveMap[i][j] >= 0)
            {
                if (game->scene->inDebugMode)
                {
                    map[i][j]->showString(QString::number(moveMap[i][j]));

                }
            }
            else
            {
                //不能移动的格子添加灰色效果
                QGraphicsColorizeEffect *e1 = new QGraphicsColorizeEffect();
                e1->setColor(QColor(111,111,111));
                map[i][j]->setGraphicsEffect(e1);

                if (robots[i][j])
                {
                    robots[i][j]->setGraphicsEffect(e1);
                }
            }


        }
    }

    // 判断地方机器人能否被攻击
    for (int i = 0; i < width; ++i)
    {
        for (int j = 0; j < height; ++j)
        {
            Robot * enemy = robots[i][j];
            if (enemy)
            {
                if (canAttack(robot, robot->m_weapon1, enemy) || canAttack(robot, robot->m_weapon2, enemy))
                {
                    enemy->setGraphicsEffect(0);
                    enemy->m_shouldPaintUsingActive = false;
                }
                else if (robot->m_player != enemy->m_player)
                {
                    //不能移动的格子添加灰色效果
                    QGraphicsColorizeEffect *e1 = new QGraphicsColorizeEffect();
                    e1->setColor(QColor(111,111,111));

                    enemy->setGraphicsEffect(e1);

                }
            }
        }
    }
}
void Map::UnshowMoveRange()
{
    for (int i = 0; i < width; ++i)
    {
        for (int j = 0; j < height; ++j)
        {
            map[i][j]->setGraphicsEffect(0);

            if (i != 0 && i != width - 1 && j != 0 && j != height - 1)
            {
                map[i][j]->UnshowString();

            }

            if (robots[i][j])
            {
                robots[i][j]->setGraphicsEffect(0);
                robots[i][j]->m_shouldPaintUsingActive = true;
            }
        }
    }
}
int Map::getMoveConsume(Robot *robot, int x, int y, bool ignore_robot)
{
    if ((!ignore_robot) && robots[x][y] && robots[x][y]->m_player != robot->m_player)
        return 9999;
    else
        return map[x][y]->m_moveConsume[robot->m_property.type];
}

QVector<QVector<int> > Map::calculateMoveRange(Robot *robot, int x_start, int y_start, int move_value, bool ignore_robots)
{
    //qDebug() << robot->property.robotName << "start searching";

    int xPos = robot->m_x;
    int yPos = robot->m_y;

    if (x_start != -1)
        xPos = x_start;
    if (y_start != -1)
        yPos = y_start;

    QVector<QVector<int> > m = QVector<QVector<int> >(width, QVector<int>(height, -99));

    if (move_value == -1)
    {
        //qDebug() << robot->move;
        m[xPos][yPos] = robot->t_move();    //行动力
    }
    else
        m[xPos][yPos] = move_value;


    QList<Point> visited;
    visited.append(Point(xPos, yPos));

    int debug_i = 0;
    while (!visited.isEmpty())
    {
        ++debug_i;


        Point now = visited.last();
        visited.pop_back();

        //qDebug() << debug_i << now.x << now.y;

        int x = now.x;
        int y = now.y;
        //up
        if (m[x][y-1] < m[x][y] - getMoveConsume(robot, x, y-1, ignore_robots))
        {
            m[x][y-1] = m[x][y] - getMoveConsume(robot, x, y-1, ignore_robots);
            if (m[x][y-1] >= 0)
            {
                visited.append(Point(x, y-1));
            }
        }

        //down
        if (m[x][y+1] < m[x][y] - getMoveConsume(robot, x, y+1, ignore_robots))
        {
            m[x][y+1] = m[x][y] - getMoveConsume(robot, x, y+1, ignore_robots);
            if (m[x][y+1] >= 0)
            {
                visited.append(Point(x, y+1));
            }
        }

        //left
        if (m[x-1][y] < m[x][y] - getMoveConsume(robot, x-1, y,ignore_robots))
        {
            m[x-1][y] = m[x][y] - getMoveConsume(robot, x-1, y,ignore_robots);
            if (m[x-1][y] >= 0)
            {
                visited.append(Point(x-1, y));
            }
        }

        //right
        if (m[x+1][y] < m[x][y] - getMoveConsume(robot, x+1, y,ignore_robots))
        {
            m[x+1][y] = m[x][y] - getMoveConsume(robot, x+1, y,ignore_robots);
            if (m[x+1][y] >= 0)
            {
                visited.append(Point(x+1, y));
            }
        }

    }
    //qDebug() << "search " << debug_i << " times";
    return m;
}


void Map::move(Robot *selectedRobot, int xTo, int yTo)
{
    int x = selectedRobot->m_x;
    int y = selectedRobot->m_y;
    if (game->scene->captain)
    {

    }
    else {
        robots[x][y] = nullptr;
    }
    robots[xTo][yTo] = selectedRobot;

    moveAnimation(selectedRobot, xTo, yTo);
}
void Map::moveAnimation(Robot *selectedRobot, int xTo, int yTo)
{
    robot_to_move = selectedRobot;
    dest_x = xTo;
    dest_y = yTo;

    UnshowMoveRange();   //消除不能移动的格子的灰色效果

    showMoveAnimation(250);

    move_finished = false;
    while (!move_finished)
        QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
}

void Map::showMoveAnimation(int fps)
{
    game->scene->isMovingRobot = true;
    move_timer = new QTimer(this);
    connect(move_timer, SIGNAL(timeout()), this, SLOT(moveAnimation()));
    move_timer->start(1000/fps);
}
void Map::moveAnimation()
{
    //qDebug() << "move speed = " << moveAnimationSpeed;
    if (robot_to_move->pos().x() < dest_x*32)
    {
        int x = robot_to_move->pos().x() + moveAnimationSpeed < dest_x*32 ? robot_to_move->pos().x() + moveAnimationSpeed : dest_x*32;
        robot_to_move->setPos(x, robot_to_move->pos().y());
    }
    else if (robot_to_move->pos().x() > dest_x*32)
    {
        int x = robot_to_move->pos().x() - moveAnimationSpeed > dest_x*32 ? robot_to_move->pos().x() - moveAnimationSpeed : dest_x*32;
        robot_to_move->setPos(x, robot_to_move->pos().y());
    }
    else if (robot_to_move->pos().y() < dest_y*32)
    {
        int y = robot_to_move->pos().y() + moveAnimationSpeed < dest_y*32 ? robot_to_move->pos().y() + moveAnimationSpeed : dest_y*32;
        robot_to_move->setPos(robot_to_move->pos().x(), y);
    }
    else if (robot_to_move->pos().y() > dest_y*32)
    {
        int y = robot_to_move->pos().y() - moveAnimationSpeed > dest_y*32 ? robot_to_move->pos().y() - moveAnimationSpeed : dest_y*32;
        robot_to_move->setPos(robot_to_move->pos().x(), y);
    }
    else
    {
        qDebug() << "move finished";
        game->scene->isMovingRobot = false;
        moveAnimationSpeed = 2;
        disconnect(move_timer, SIGNAL(timeout()), this, SLOT(moveAnimation()));
        move_timer->stop();
        delete move_timer;

        robot_to_move->setxy(dest_x, dest_y);

        //game->canMoveStatus = false;

        move_finished = true;
    }
}

QVector<QVector<int> > Map::calculateAttackRange(Robot *robot, Weapon *weapon)
{
    int xPos = robot->m_x;
    int yPos = robot->m_y;
    QVector<QVector<int> > m = QVector<QVector<int> >(width, QVector<int>(height, -99));
    m[xPos][yPos] = weapon->range;    //行动力


    QList<Point> visited;
    visited.append(Point(xPos, yPos));

    //int debug_i = 0;
    while (!visited.isEmpty())
    {
        //++debug_i;
        //qDebug() << debug_i;

        Point now = visited.last();
        visited.pop_back();

        int x = now.x;
        int y = now.y;
        //up
        if (y-1 >= 0 && m[x][y-1] < m[x][y] - 1)
        {
            m[x][y-1] = m[x][y] - 1;
            if (m[x][y-1] >= 0)
            {
                visited.append(Point(x, y-1));
            }
        }

        //down
        if (y+1 < height && m[x][y+1] < m[x][y] - 1)
        {
            m[x][y+1] = m[x][y] - 1;
            if (m[x][y+1] >= 0)
            {
                visited.append(Point(x, y+1));
            }
        }

        //left
        if (x-1 >= 0 && m[x-1][y] < m[x][y] - 1)
        {
            m[x-1][y] = m[x][y] - 1;
            if (m[x-1][y] >= 0)
            {
                visited.append(Point(x-1, y));
            }
        }

        //right
        if (x+1 < width && m[x+1][y] < m[x][y] - 1)
        {
            m[x+1][y] = m[x][y] - 1;
            if (m[x+1][y] >= 0)
            {
                visited.append(Point(x+1, y));
            }
        }

    }

    if (weapon->range > 1)
    {
        for (int i = 0; i < width; ++i)
        {
            for (int j = 0; j < height; ++j)
            {
                if (m[i][j] == weapon->range - 1)
                    m[i][j] = -99;
            }
        }
    }


    return m;
}
void Map::showCannotAttackRange(Robot *robot)
{
    for (int i = 0; i < width; ++i)
    {
        for (int j = 0; j < height; ++j)
        {
            if (i != robot->m_x || j != robot->m_y)
            {
                //不能移动的格子添加灰色效果
                QGraphicsColorizeEffect *e1 = new QGraphicsColorizeEffect();
                e1->setColor(QColor(111,111,111));
                map[i][j]->setGraphicsEffect(e1);

                if (robots[i][j])
                {
                    robots[i][j]->setGraphicsEffect(e1);
                }
            }
        }
    }
}
void Map::showAttackRange(Robot *robot, Weapon *weapon)
{
    AttackMap = calculateAttackRange(robot, weapon);

    if (weapon->id == 164)
    {
        for (int i = 0; i < width; ++i)
        {
            for (int j = 0; j < height; ++j)
            {
                if (AttackMap[i][j] >= 0)
                {
                    if (robots[i][j])
                    {
                        robots[i][j]->setGraphicsEffect(0);
                        robots[i][j]->update();
                        QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
                    }
                }
                else
                {
                    //不能移动的格子添加灰色效果
                    QGraphicsColorizeEffect *e1 = new QGraphicsColorizeEffect();
                    e1->setColor(QColor(111,111,111));
                    map[i][j]->setGraphicsEffect(e1);

                    if (robots[i][j])
                    {
                        robots[i][j]->setGraphicsEffect(e1);
                    }
                }
            }
        }
    }
    else
    {
        for (int i = 0; i < width; ++i)
        {
            for (int j = 0; j < height; ++j)
            {
                if (AttackMap[i][j] >= 0)
                {
                    if (game->scene->inDebugMode)
                    {
                        map[i][j]->showString(QString::number(AttackMap[i][j]));

                    }
                    if (robots[i][j])
                    {
                        if (weapon->firepower[robots[i][j]->m_property.type] <= 0)
                        {
                            QGraphicsColorizeEffect *e1 = new QGraphicsColorizeEffect();
                            e1->setColor(QColor(111,111,111));
                            robots[i][j]->setGraphicsEffect(e1);
                        }
                        else {
                            robots[i][j]->setGraphicsEffect(0);
                            robots[i][j]->m_shouldPaintUsingActive = false;
                        }

                    }

                }
                else
                {
                    //不能移动的格子添加灰色效果
                    QGraphicsColorizeEffect *e1 = new QGraphicsColorizeEffect();
                    e1->setColor(QColor(111,111,111));
                    map[i][j]->setGraphicsEffect(e1);

                    if (robots[i][j])
                    {
                        robots[i][j]->setGraphicsEffect(e1);
                    }
                }
            }
        }
    }

}

void Map::showAttackGif(int x1, int y1, int x2, int y2)
{
    dest_arrow_x = x2;
    dest_arrow_y = y2;

    arrow = new QGraphicsPixmapItem();
    arrow->setPixmap(QPixmap(config->attack_arrow));
    arrow->setPos(x1 * 32, y1 * 32);
    game->scene->addItem(arrow);

    move_finished = false;

    isShowingAttackGif = true;
    move_timer = new QTimer(this);
    connect(move_timer, SIGNAL(timeout()), this, SLOT(arrowAnimation()));
    move_timer->start(1000/200);


    while (!move_finished)
        QCoreApplication::processEvents(QEventLoop::AllEvents, 100);

    game->scene->removeItem(arrow);
    delete arrow;
    arrow = 0;
}

void Map::AI_move(Robot *selectedRobot)
{
    QVector<QVector<int> > m = calculateMoveRange(selectedRobot, -1, -1, 999);

    Robot * closeEnemy = AI::GetEnemy(this, selectedRobot);
    if (!closeEnemy)
    {
        return;
    }

    m = calculateMoveRange(selectedRobot);

    QVector<QVector<int> > m2 = calculateMoveRange(selectedRobot, closeEnemy->m_x, closeEnemy->m_y, 999);

    int xTo, yTo;
    int value;
    for (int i = 0; i < m.size(); ++i)
    {
        for (int j = 0; j < m[i].size(); ++j)
        {
            if (robots[i][j] == 0 && m[i][j] >= 0)
            {
                xTo = i;
                yTo = j;
                value = m2[i][j];
            }
        }
    }
    for (int i = 0; i < m.size(); ++i)
    {
        for (int j = 0; j < m[i].size(); ++j)
        {
            if (m[i][j] >= 0)
            {
                if (robots[i][j] == 0 && m2[i][j] > value)
                {
                    xTo = i;
                    yTo = j;
                    value = m2[i][j];
                }

            }
        }
    }


    game->scene->deleteMenu();
    move(selectedRobot, xTo, yTo);

}

bool Map::hasAnyRobot(int player)
{
    for (int i = 0; i < width; ++i)
    {
        for (int j = 0; j< height; ++j)
        {
            if (robots[i][j] && robots[i][j]->m_player == player)
            {
                return true;
            }
        }
    }
    return false;
}

void Map::UpdateExpTable(QVector<int> & table)
{
    qDebug() << "table length = " << table.length();
    for (int i = 0; i < width; ++i)
    {
        for (int j = 0; j < height; ++j)
        {
            Robot * robot = robots[i][j];
            if (robot && robot->m_player == 0)
            {
                table[robot->m_pilot->id] = robot->m_pilot->exp;
            }
        }
    }
}
void Map::UpdateRobotLevelUsingExpTable(const QVector<int> & table)
{
    for (int i = 0; i < width; ++i)
    {
        for (int j = 0; j < height; ++j)
        {
            Robot * robot = robots[i][j];
            if (robot && robot->m_player == 0)
            {
                robot->m_pilot->exp = table[robot->m_pilot->id];
                robot->updateLevel();
            }
        }
    }
}

void Map::showText(int x, int y, QString s)
{
    showText_x = x*32;
    showText_y = y*32;
    animation_text = new QGraphicsTextItem();
    animation_text->setPlainText(s);
    animation_text->setPos(x*32, y*32);

    animation_text->setScale(2);
    animation_text->setDefaultTextColor(Qt::white);

    game->scene->add(animation_text);

    showTextTimer = new QTimer();
    connect(showTextTimer, SIGNAL(timeout()), this, SLOT(showTextAnimation()));

    showTextTimer->start(1000/70);

}

void Map::UpdateRobotsAtSupply()
{
    for (int i = 0; i < width; ++i)
    {
        for (int j = 0; j < height; ++j)
        {
            if (map[i][j]->m_kind == 10)
            {
                Robot * robot = robots[i][j];
                if (robot)
                {
                    int hp = robot->m_hp_total * 0.3;
                    robot->m_hp = robot->m_hp + hp > robot->m_hp_total ? robot->m_hp_total : robot->m_hp + hp;
                }
            }
        }
    }
}

void Map::removeRobot(Robot *robot)
{
    robots[robot->m_x][robot->m_y] = 0;

    game->scene->removeItem(robot);

    delete robot;
}
int Map::calcDistance(Robot *robot1, Robot *robot2)
{
    int x1 = robot1->m_x;
    int y1 = robot1->m_y;
    int x2 = robot2->m_x;
    int y2 = robot2->m_y;

    int dx = abs(x1-x2);
    int dy = abs(y1-y2);
    return dx + dy;
}

//robot能不能攻击enemy
bool Map::canAttack(Robot *robot, Weapon *weapon, Robot *enemy)
{
    //修理装置
    if (weapon->id == 164)
    {
        return false;
    }

    QVector<QVector<int> > m = calculateAttackRange(robot, weapon);
    if (robot->m_player == enemy->m_player)
        return false;
    if (m[enemy->m_x][enemy->m_y] >= 0 && weapon->firepower[enemy->m_property.type] > 0)
        return true;
    else
        return false;
}

void Map::arrowAnimation()
{

    if (arrow->pos().x() < dest_arrow_x*32)
    {
        int x = arrow->pos().x() + moveAnimationSpeed < dest_arrow_x*32 ? arrow->pos().x() + moveAnimationSpeed : dest_arrow_x*32;
        arrow->setPos(x, arrow->pos().y());
    }
    else if (arrow->pos().x() > dest_arrow_x*32)
    {
        int x = arrow->pos().x() - moveAnimationSpeed > dest_arrow_x*32 ? arrow->pos().x() - moveAnimationSpeed : dest_arrow_x*32;
        arrow->setPos(x, arrow->pos().y());
    }
    else if (arrow->pos().y() < dest_arrow_y*32)
    {
        int y = arrow->pos().y() + moveAnimationSpeed < dest_arrow_y*32 ? arrow->pos().y() + moveAnimationSpeed : dest_arrow_y*32;
        arrow->setPos(arrow->pos().x(), y);
    }
    else if (arrow->pos().y() > dest_arrow_y*32)
    {
        int y = arrow->pos().y() - moveAnimationSpeed > dest_arrow_y*32 ? arrow->pos().y() - moveAnimationSpeed : dest_arrow_y*32;
        arrow->setPos(arrow->pos().x(), y);
    }
    else
    {
        isShowingAttackGif = false;
        moveAnimationSpeed = 2;

        disconnect(move_timer, SIGNAL(timeout()), this, SLOT(arrowAnimation()));
        move_timer->stop();
        delete move_timer;

        move_finished = true;
    }
}

void Map::showTextAnimation()
{
    animation_text->setPos(animation_text->x(), animation_text->y() - 1);
    if (showText_y - animation_text->y() > 20)
    {
        qDebug() << "show text disappear";
        disconnect(showTextTimer, SIGNAL(timeout()), this, SLOT(showTextAnimation()));
        showTextTimer->stop();
        game->scene->remove(animation_text);
        delete animation_text;
        animation_text = 0;
        delete showTextTimer;
    }
}
