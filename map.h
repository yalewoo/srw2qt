#ifndef MAP_H
#define MAP_H

#include <QVector>

#include "maprect.h"
#include "point.h"


class Map
{
public:
    Map(int width, int height);

    QVector<QVector<MapRect *> > map;

    void loadMapFromTxt(QString filename);

    int width;
    int height;

    void showMoveRange(int x, int y, int moveValue, int moveKind);

private:
    QVector<MapRect *> map1D;
    QVector<QVector<MapRect *> > map2D;
};

#endif // MAP_H
