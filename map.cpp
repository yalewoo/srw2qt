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

void Map::loadLevel(int level)
{
    DataHelper::Map mapdata(config->getMapPathOfLevel(level));
    width = mapdata.width + 2;
    height = mapdata.height + 2;
    qDebug() << width << height;
    // width = 16, height = 20

    map = QVector<QVector<MapRect *> >(width, QVector<MapRect *>(height, 0));
    robots = QVector<QVector<Robot *> >(width, QVector<Robot *>(height, 0));

    for (int i = 0; i < width; ++i)
    {
        for (int j = 0; j < height; ++j)
        {
            qDebug() << i << j;

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
}

void Map::addRobot(Robot * robot, int xPos, int yPos)
{
    robots[xPos][yPos] = robot;
    robots[xPos][yPos]->setxy(xPos,yPos);
    game->scene->add(robots[xPos][yPos]);

    qDebug() << robots[xPos][yPos]->property.robotName << xPos << yPos;
}


void Map::placeRobot(int stage, int round)
{
    for (int i = 0; i < Robot::robots_init.length(); ++i)
    {
        RobotData robotData = Robot::robots_init[i];

        if (stage != robotData.stage || round != robotData.round)
            continue;

        int x = robotData.x;
        int y = robotData.y;
        Robot * robot = new Robot(robotData.robotId);

        robot->robotBehavior = 1;

        qDebug() << robotData.robotId;
        robot->setPilot(robotData.peopleId);
        qDebug() << robot->property.robotName << robot->move;
        addRobot(robot, x, y);
    }

}
void Map::placeEnemy(int stage, int round)
{
    for (int i = 0; i < Robot::enemys_init.length(); ++i)
    {
        EnemyData robotData = Robot::enemys_init[i];

        if (stage != robotData.stage || round != robotData.round)
            continue;

        int x = robotData.x;
        int y = robotData.y;
        Robot * robot = new Robot(robotData.robotId, 1);
        robot->setPilot(robotData.peopleId);
        robot->robotBehavior = robotData.robotBehavior;

        addRobot(robot, x, y);
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
                map[i][j]->showString(QString::number(moveMap[i][j]));
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
                if (canAttack(robot, robot->weapon1, enemy) || canAttack(robot, robot->weapon2, enemy))
                {
                    enemy->setGraphicsEffect(0);
                    enemy->shouldPaintUsingActive = false;
                }
                else if (robot->player != enemy->player)
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

            map[i][j]->UnshowString();

            if (robots[i][j])
            {
                robots[i][j]->setGraphicsEffect(0);
                robots[i][j]->shouldPaintUsingActive = true;
            }
        }
    }
}
int Map::getMoveConsume(Robot *robot, int x, int y)
{
    if (robots[x][y] && robots[x][y]->player != robot->player)
        return 9999;
    else
        return map[x][y]->moveConsume[robot->property.type];
}

QVector<QVector<int> > Map::calculateMoveRange(Robot *robot, int x_start, int y_start, int move_value)
{
    qDebug() << robot->property.robotName << "start searching";

    int xPos = robot->x;
    int yPos = robot->y;

    if (x_start != -1)
        xPos = x_start;
    if (y_start != -1)
        yPos = y_start;

    QVector<QVector<int> > m = QVector<QVector<int> >(width, QVector<int>(height, -99));

    if (move_value == -1)
    {
        qDebug() << robot->move;
        m[xPos][yPos] = robot->move;    //行动力
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
        if (m[x][y-1] < m[x][y] - getMoveConsume(robot, x, y-1))
        {
            m[x][y-1] = m[x][y] - getMoveConsume(robot, x, y-1);
            if (m[x][y-1] >= 0)
            {
                visited.append(Point(x, y-1));
            }
        }

        //down
        if (m[x][y+1] < m[x][y] - getMoveConsume(robot, x, y+1))
        {
            m[x][y+1] = m[x][y] - getMoveConsume(robot, x, y+1);
            if (m[x][y+1] >= 0)
            {
                visited.append(Point(x, y+1));
            }
        }

        //left
        if (m[x-1][y] < m[x][y] - getMoveConsume(robot, x-1, y))
        {
            m[x-1][y] = m[x][y] - getMoveConsume(robot, x-1, y);
            if (m[x-1][y] >= 0)
            {
                visited.append(Point(x-1, y));
            }
        }

        //right
        if (m[x+1][y] < m[x][y] - getMoveConsume(robot, x+1, y))
        {
            m[x+1][y] = m[x][y] - getMoveConsume(robot, x+1, y);
            if (m[x+1][y] >= 0)
            {
                visited.append(Point(x+1, y));
            }
        }

    }
    qDebug() << "search " << debug_i << " times";
    return m;
}


void Map::move(Robot *selectedRobot, int xTo, int yTo)
{
    robot_to_move = selectedRobot;
    dest_x = xTo;
    dest_y = yTo;

    int x = selectedRobot->x;
    int y = selectedRobot->y;
    robots[x][y] = nullptr;
    robots[xTo][yTo] = selectedRobot;

    UnshowMoveRange();   //消除不能移动的格子的灰色效果

    showMoveAnimation(250);

    move_finished = false;
    while (!move_finished)
        QCoreApplication::processEvents(QEventLoop::AllEvents, 100);


}

void Map::showMoveAnimation(int fps)
{
    move_timer = new QTimer(this);
    connect(move_timer, SIGNAL(timeout()), this, SLOT(moveAnimation()));
    move_timer->start(1000/fps);
}
void Map::moveAnimation()
{
    if (robot_to_move->pos().x() < dest_x*32)
    {
        robot_to_move->setPos(robot_to_move->pos().x() + 1, robot_to_move->pos().y());
    }
    else if (robot_to_move->pos().x() > dest_x*32)
    {
        robot_to_move->setPos(robot_to_move->pos().x() - 1, robot_to_move->pos().y());
    }
    else if (robot_to_move->pos().y() < dest_y*32)
    {
        robot_to_move->setPos(robot_to_move->pos().x(), robot_to_move->pos().y() + 1);
    }
    else if (robot_to_move->pos().y() > dest_y*32)
    {
        robot_to_move->setPos(robot_to_move->pos().x(), robot_to_move->pos().y() - 1);
    }
    else
    {
        disconnect(move_timer, SIGNAL(timeout()), this, SLOT(moveAnimation()));
        move_timer->stop();
        delete move_timer;

        robots[dest_x][dest_y]->setxy(dest_x, dest_y);

        //game->canMoveStatus = false;

        move_finished = true;
    }
}

QVector<QVector<int> > Map::calculateAttackRange(Robot *robot, Weapon *weapon)
{
    int xPos = robot->x;
    int yPos = robot->y;
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
                    map[i][j]->showString(QString::number(AttackMap[i][j]));
                    if (robots[i][j] && weapon->firepower[robots[i][j]->property.type] == 0)
                    {
                        QGraphicsColorizeEffect *e1 = new QGraphicsColorizeEffect();
                        e1->setColor(QColor(111,111,111));
                        robots[i][j]->setGraphicsEffect(e1);
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

    QVector<QVector<int> > m2 = calculateMoveRange(selectedRobot, closeEnemy->x, closeEnemy->y, 999);

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

void Map::removeRobot(Robot *robot)
{
    robots[robot->x][robot->y] = 0;

    game->scene->removeItem(robot);

    delete robot;
}
int Map::calcDistance(Robot *robot1, Robot *robot2)
{
    int x1 = robot1->x;
    int y1 = robot1->y;
    int x2 = robot2->x;
    int y2 = robot2->y;

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
    if (robot->player == enemy->player)
        return false;
    if (m[enemy->x][enemy->y] >= 0 && weapon->firepower[enemy->property.type] > 0)
        return true;
    else
        return false;
}

void Map::arrowAnimation()
{
    if (arrow->pos().x() < dest_arrow_x*32)
    {
        arrow->setPos(arrow->pos().x() + 1, arrow->pos().y());
    }
    else if (arrow->pos().x() > dest_arrow_x*32)
    {
        arrow->setPos(arrow->pos().x() - 1, arrow->pos().y());
    }
    else if (arrow->pos().y() < dest_arrow_y*32)
    {
        arrow->setPos(arrow->pos().x(), arrow->pos().y() + 1);
    }
    else if (arrow->pos().y() > dest_arrow_y*32)
    {
        arrow->setPos(arrow->pos().x(), arrow->pos().y() - 1);
    }
    else
    {
        disconnect(move_timer, SIGNAL(timeout()), this, SLOT(arrowAnimation()));
        move_timer->stop();
        delete move_timer;

        move_finished = true;
    }
}
