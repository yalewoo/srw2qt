#ifndef GAME_H
#define GAME_H

#include <QGraphicsView>
#include <QGraphicsScene>
#include <QMouseEvent>

#include "maprect.h"
#include "map.h"

class Game : public QGraphicsView
{
    Q_OBJECT
public:
    Game(QWidget * parent = 0);
    void start();


    QGraphicsScene * scene;
    Map * map;
};

#endif // GAME_H
