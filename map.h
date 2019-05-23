#ifndef MAP_H
#define MAP_H

#include "maprect.h"
#include "robot.h"

#include <QObject>

#include "datahelper.h"

class Map : public QObject
{
    Q_OBJECT
public:
    Map(QObject * parent = 0);
    ~Map();

    int width;
    int height;

    QVector<QVector<MapRect *> > map;
    QVector<QVector<Robot *> > robots;

    void loadStage(int level);

    void addRobot(Robot * robot, int xPos, int yPos);
    void removeRobot(Robot *robot);


    void placeRobot(int stage, int round);
    void placeEnemy(int stage, int round);

    void placeRobotRunTime(RunTimeRobotData data);

    //显示可移动范围 不能移动的格子加灰色
    void showMoveRange(Robot * robot);
    QVector<QVector<int> > moveMap; //保存移动范围
    void UnshowMoveRange(); //消除显示可移动范围

    //获取(x,y)格消耗的移动力，考虑敌方机体的影响
    int getMoveConsume(Robot *robot, int x, int y);

    //计算移动范围 返回二维数组表示每个格子是否可移动 move_value=-1时使用robot本身的
    QVector<QVector<int> > calculateMoveRange(Robot * robot, int x_start = -1, int y_start = -1, int move_value = -1);


    //计算格子距离
    int calcDistance(Robot * robot1, Robot * robot2);


    //移动机器人
    void move(Robot * selectedRobot, int xTo, int yTo);
    void showMoveAnimation(int fps);    //显示动画
    int moveAnimationSpeed = 2;
    bool move_finished = false; //动画是否显示完毕
    QTimer * move_timer;
    Robot *robot_to_move;
    int dest_x;
    int dest_y;


    //武器
    QVector<QVector<int> > AttackMap; //保存攻击范围
    QVector<QVector<int> > calculateAttackRange(Robot * robot, Weapon * weapon);
    void showAttackRange(Robot * robot, Weapon * weapon);
    //robot 是否可以攻击enemy
    bool canAttack(Robot * robot, Weapon * weapon, Robot * enemy);

    //显示武器瞄准
    void showAttackGif(int x1, int y1, int x2, int y2);
    int dest_arrow_x;
    int dest_arrow_y;
    QGraphicsPixmapItem * arrow = 0;
    bool isShowingAttackGif = false;

    // 当前所在格子
    int selected_x;
    int selected_y;
    QGraphicsPixmapItem * select_effect = 0;


    //AI
    void AI_move(Robot * selectedRobot);

    // 大地图动画
    QTimer * showTextTimer;
    QGraphicsTextItem * animation_text;
    int showText_x;
    int showText_y;
    void showText(int x, int y, QString s);


public slots:
    void moveAnimation();
    void arrowAnimation();
    void showTextAnimation();
};

#endif // MAP_H
