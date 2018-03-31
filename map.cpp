#include "map.h"

#include "maprect.h"
#include "game.h"
#include "point.h"

extern Game * game;

#include <cstdlib>
#include <QDebug>

#include <QFile>

#include <QGraphicsColorizeEffect>

#include "people.h"

Map::Map(QString mapfilename)
{
    loadMapFromCsv(mapfilename);


    for (int i = 0; i < width; ++i)
    {
        for (int j = 0; j < height; ++j)
        {
            map[i][j]->setxy(i, j);
            game->scene->addItem(map[i][j]);
        }
    }
}

void Map::loadMapFromTxt(QString filename)
{
    QVector<MapRect *> map1D;
    QVector<QVector<MapRect *> > map2D;

    FILE * fp = fopen(filename.toLatin1().data(), "r");
    if (!fp)
    {
        qDebug() << filename << " open failed";
        return;
    }

    int one = 0;

    fscanf(fp, "%x", &width);
    fscanf(fp, "%x", &height);
    width += 2;
    height += 2;
    robots = QVector<QVector<Robot *> >(width, QVector<Robot *>(height, 0));


    while (fscanf(fp, "%x", &one) == 1)
    {
        int n = one / 16;
        int kind = one % 16;

        for (int i = 0; i <= n; ++i)
        {
            //qDebug() << kind;
            MapRect * rect = new MapRect(kind);
            map1D.append(rect);
            //qDebug() << map1D.size();
        }
    }
    fclose(fp);

    int count = 0;
    QVector<MapRect *> v;
    for (int i = 0; i < height; ++i)
    {
        MapRect * rect = new MapRect(0);
        v.append(rect);
    }
    map2D.append(v);
    for (int i = 1; i < height-1; ++i)
    {
        v.clear();
        MapRect * rect = new MapRect(0);
        v.append(rect);
        for (int j = 1; j < width-1; ++j)
        {
            v.append(map1D[count]);
            ++count;
        }
        rect = new MapRect(0);
        v.append(rect);
        map2D.append(v);
    }
    v.clear();
    for (int i = 0; i < height; ++i)
    {
        MapRect * rect = new MapRect(0);
        v.append(rect);
    }
    map2D.append(v);

    for (int i = 0; i < width; ++i)
    {
        QVector<MapRect *> v;
        for (int j = 0; j < height; ++j)
        {
            v.append(map2D[j][i]);
        }
        map.append(v);
    }


}

void Map::loadMapFromCsv(QString filename)
{
    QVector<MapRect *> map1D;
    QVector<QVector<MapRect *> > map2D;

    QFile file(filename);
    if(!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        qDebug() << filename << " open failed";
        exit(-1);

    }
    QTextStream in(&file);

    QString line = in.readLine();
    QStringList t = line.split(",");

    width = QString(t[1]).toInt() + 2;
    height = QString(t[0]).toInt() + 2;

    robots = QVector<QVector<Robot *> >(width, QVector<Robot *>(height, 0));

    //第一行0
    QVector<MapRect *> v;
    for (int i = 0; i < width; ++i)
    {
        MapRect * rect = new MapRect(0);
        v.append(rect);
    }
    map2D.append(v);

    //中间地图 最左和最右0
    while(!in.atEnd())
    {
        QString line = in.readLine();

        QStringList t = line.split(",");

        v.clear();
        MapRect * rect = new MapRect(0);
        v.append(rect);
        for (int j = 1; j < width-1; ++j)
        {
            MapRect * rect = new MapRect(QString(t[j-1]).toInt());
            v.append(rect);
        }
        rect = new MapRect(0);
        v.append(rect);
        map2D.append(v);
    }


    //最后一行0
    v.clear();
    for (int i = 0; i < width; ++i)
    {
        MapRect * rect = new MapRect(0);
        v.append(rect);
    }
    map2D.append(v);


    for (int i = 0; i < width; ++i)
    {
        QVector<MapRect *> v;
        for (int j = 0; j < height; ++j)
        {
            v.append(map2D[j][i]);
        }
        map.append(v);
    }

    file.close();
}



void Map::placeRobot_init(QString filename, int player)
{
    QFile file(filename);
    if(!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        qDebug() << filename << "open failed";
        exit(-1);

    }
    QTextStream in(&file);

    QString line = in.readLine();

    while(!in.atEnd())
    {
        QString line = in.readLine();

        QStringList t = line.split(",");

        if (t.size() == 0)
            continue;

        int x = QString(t[0]).toInt() + 1;
        int y = QString(t[1]).toInt() + 1;
        People * people = new People(QString(t[2]).toInt(), player);
        Robot * robot = new Robot(QString(t[4]).toInt(), people, QString(t[6]).toInt()+1);

        addRobot(robot, x, y);
    }
}
void Map::placePlayerRobot_init(QString filename)
{
    placeRobot_init(filename, 0);
}
void Map::placeEnemyRobot_init(QString filename)
{
    placeRobot_init(filename, 1);
}

int Map::getMoveConsume(Robot *robot, int x, int y)
{
    if (robots[x][y] && robots[x][y]->player != robot->player)
        return 999;
    else
        return map[x][y]->moveConsume[robot->type];
}

QVector<QVector<int> > Map::calculateMoveRange(Robot *robot)
{
    int xPos = robot->x;
    int yPos = robot->y;
    QVector<QVector<int> > m = QVector<QVector<int> >(width, QVector<int>(height, -99));
    m[xPos][yPos] = robot->move;    //行动力


    QList<Point> visited;
    visited.append(Point(xPos, yPos));

    int debug_i = 0;
    while (!visited.isEmpty())
    {
        ++debug_i;
        qDebug() << debug_i;

        Point now = visited.last();
        visited.pop_back();

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
    return m;
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
            }
        }
    }
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
    for (int i = 0; i < width; ++i)
    {
        for (int j = 0; j < height; ++j)
        {
            if (AttackMap[i][j] >= 0)
            {
                map[i][j]->showString(QString::number(AttackMap[i][j]));
                if (robots[i][j] && weapon->firepower[robots[i][j]->type] == 0)
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

//robot能不能攻击enemy
bool Map::canAttack(Robot *robot, Weapon *weapon, Robot *enemy)
{
    QVector<QVector<int> > m = calculateAttackRange(robot, weapon);
    if (robot->player == enemy->player)
        return false;
    if (m[enemy->x][enemy->y] >= 0 && weapon->firepower[enemy->type] > 0)
        return true;
    else
        return false;
}

void Map::move(Robot *selectedRobot, int xTo, int yTo)
{
    int x = selectedRobot->x;
    int y = selectedRobot->y;
    robots[x][y] = 0;
    robots[xTo][yTo] = selectedRobot;
    robots[xTo][yTo]->setxy(xTo, yTo);
}

void Map::addRobot(Robot * robot, int xPos, int yPos)
{
    robots[xPos][yPos] = robot;
    robots[xPos][yPos]->setxy(xPos,yPos);
    game->scene->addItem(robots[xPos][yPos]);
}
