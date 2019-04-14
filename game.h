#ifndef GAME_H
#define GAME_H

#include <QGraphicsView>

#include "robot.h"
#include "map.h"

#include "menu.h"
#include "point.h"

#include "battle.h"


#include "board.h"

#include "music.h"

void waitVariableToBeTrueAllEvents(bool & b);
void waitVariableToBeTrueExcludeUserInput(bool & b);


class Game : public QGraphicsView
{
    Q_OBJECT
public:
    //文件路径
    QString robot_value_path;
    QString weapon_value_path;
    QString people_value_path;
    QString workDir;

    //构造函数
    Game(QWidget * parent = 0);

    //开始游戏
    void start();

    //场景
    QGraphicsScene * scene;
    //地图
    Map * map = 0;
    //资料面板
    Board * board = 0;


    //music
    Music * music_background;
    Music * music_effect;
    Music * music_battle;


    Button * next_turn_button;
    int turn = 1;


    //选中的
    Robot * selectedRobot = 0;  //选中的机器人
    Point originalPosition;     //选中机器人的原始位置，用于右键取消
    Weapon * selectedWeapon = 0;    //选中的机器

    Robot * enemy = 0;  //敌人

    //显示菜单
    Menu * drawMenu(int x, int y, int width, int height, QColor color, double opacity = 1);
    void displayMenu(Robot * robot);    //第一次点击菜单
    void displayMenu2(Robot * robot);   //移动后出现的菜单

    Menu * menu = 0;
    void deleteMenu();

    //战斗画面
    Battle * battle = 0;

    //处于可移动状态
    bool canMoveStatus = false;


    //显示攻击范围
    void showAttackRange();
    void attack(Robot * enemy);

    //攻击结束
    void attackDone();

public slots:
    void move();    //移动
    void finished();    //待命
    void cancel();  //按下右键后，取消行动
    void attack1(); //使用武器1
    void attack2(); //使用武器2

    void AI();  //AI自动行动

    void next_turn();   //回合结束

};

#endif // GAME_H
