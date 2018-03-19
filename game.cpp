#include "game.h"

#include "robot.h"

Game::Game(QWidget *parent) : QGraphicsView(parent)
{
    //set up the screen
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    setFixedSize(1024, 768);

    scene = new QGraphicsScene();
    scene->setSceneRect(0,0,1024,768);
    setScene(scene);



}

void Game::start()
{
    //test code
    MapRect * mr = new MapRect(1);
    mr->setPos(16,16);
    scene->addItem(mr);

    map = new Map(16,20);

    //test code
    Robot * robot = new Robot(1);
    robot->setxy(4,5);
    scene->addItem(robot);

    map->showMoveRange(robot->x, robot->y, 5, 1);
}
