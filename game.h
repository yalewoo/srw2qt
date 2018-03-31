#ifndef GAME_H
#define GAME_H

#include <QGraphicsView>

#include "robot.h"
#include "map.h"

#include "menu.h"
#include "point.h"

#include "battleground.h"

class Game : public QGraphicsView
{
    Q_OBJECT
public:
    Game(QWidget * parent = 0);
    void start();
    QString workDir;

    QGraphicsScene * scene;

    //选中的机器人
    Robot * selectedRobot = 0;
    Point originalPosition;
    Map * map = 0;

    //使用的武器
    Weapon * selectedWeapon = 0;

    //显示菜单
    Menu * drawMenu(int x, int y, int width, int height, QColor color, double opacity = 1);
    void displayMenu(Robot * robot);    //第一次点击菜单
    void displayMenu2(Robot * robot);   //移动后出现的菜单

    Menu * menu = 0;
    void deleteMenu();

    //战斗画面
    BattleGround * battleGround = 0;


    bool canMoveStatus = false;

    //显示攻击范围
    void showAttackRange();
    void attack(Robot * enemy);

    void attackDone();

public slots:
    void move();    //移动
    void finished();    //待命
    void cancel();  //按下右键后，取消行动
    void attack1(); //使用武器1
    void attack2(); //使用武器2
};

#endif // GAME_H
