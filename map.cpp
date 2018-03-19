#include "map.h"

#include <QDebug>

#include "game.h"
extern Game * game;

#include <QGraphicsColorizeEffect>

Map::Map(int width, int height) : width(width+2), height(height+2)
{
    loadMapFromTxt("../srw2QT/input/map1.txt");
    qDebug() << "ok";
}

void Map::loadMapFromTxt(QString filename)
{
    FILE * fp = fopen(filename.toLatin1().data(), "r");

    int one = 0;

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

    for (int i = 0; i < width; ++i)
    {
        for (int j = 0; j < height; ++j)
        {
            map[i][j]->setPos(32*i, 32*j);
            game->scene->addItem(map[i][j]);
        }
    }
}

void Map::showMoveRange(int xPos, int yPos, int moveValue, int moveKind)
{
    QVector<QVector<int> > m = QVector<QVector<int> >(width, QVector<int>(height, -99));
    m[xPos][yPos] = moveValue;

    QList<Point> visited;
    visited.append(Point(xPos, yPos));

    while (!visited.isEmpty())
    {
        Point now = visited.last();
        visited.pop_back();

        int x = now.x;
        int y = now.y;
        //up
        if (m[x][y-1] < 0)
        {
            m[x][y-1] = m[x][y] - map[x][y-1]->moveConsume[moveKind];
            if (m[x][y-1] >= 0)
            {
                visited.append(Point(x, y-1));
            }
        }

        //down
        if (m[x][y+1] < 0)
        {
            m[x][y+1] = m[x][y] - map[x][y+1]->moveConsume[moveKind];
            if (m[x][y+1] >= 0)
            {
                visited.append(Point(x, y+1));
            }
        }

        //left
        if (m[x-1][y] < 0)
        {
            m[x-1][y] = m[x][y] - map[x-1][y]->moveConsume[moveKind];
            if (m[x-1][y] >= 0)
            {
                visited.append(Point(x-1, y));
            }
        }

        //right
        if (m[x+1][y] < 0)
        {
            m[x+1][y] = m[x][y] - map[x+1][y]->moveConsume[moveKind];
            if (m[x+1][y] >= 0)
            {
                visited.append(Point(x+1, y));
            }
        }



    }

    for (int i = 0; i < width; ++i)
    {
        for (int j = 0; j < height; ++j)
        {
            if (m[i][j] > 0)
            {
                QGraphicsColorizeEffect *e1 = new QGraphicsColorizeEffect();
                e1->setColor(QColor(0,0,192));
                map[i][j]->setGraphicsEffect(e1);
            }
        }
    }
}
