#ifndef GAME_H
#define GAME_H

#include <QGraphicsView>

#include "robot.h"
#include "map.h"

#include "menu.h"
#include "point.h"

class Game : public QGraphicsView
{
    Q_OBJECT
public:
    Game(QWidget * parent = 0);
    void start();
    QString workDir;

    QGraphicsScene * scene;

    //选中的机器人
    Robot * selectedRobot;
    Point originalPosition;
    Map * map;

    //显示菜单
    Menu * drawMenu(int x, int y, int width, int height, QColor color, double opacity = 1);
    void displayMenu(Robot * robot);    //第一次点击菜单
    void displayMenu2(Robot * robot);   //移动后出现的菜单

    Menu * menu = 0;
    void deleteMenu();


    bool canMoveStatus = false;

public slots:
    void move();    //移动
    void finished();    //待命
    void cancel();  //按下右键后，取消行动
    void attack1(); //使用武器1
};

#endif // GAME_H
