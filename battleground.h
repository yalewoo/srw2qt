#ifndef BATTLEGROUND_H
#define BATTLEGROUND_H


#include <QGraphicsRectItem>
#include <QObject>

#include "robot.h"
#include "weapon.h"

class BattleGround : public QObject, public QGraphicsRectItem
{
    Q_OBJECT
public:
    BattleGround(Robot *robot, Weapon *weapon, Robot *enemy);
    void setSize(int width, int height);

    bool battlefinished = false;

    //显示一句话
    void showString(QString s);

    void showAttackAnimation(Robot * robot);
    bool move_finished = true; //动画是否显示完毕
    QTimer * move_timer;
    QGraphicsPixmapItem * weapon_image = 0;
    bool move_left;

    //显示hp和rate
    void showHpAndRate();

    QGraphicsPixmapItem * playerRobot;
    QGraphicsPixmapItem * enemyRobot;

    int stage = 0;
    void mousePressEvent(QGraphicsSceneMouseEvent *event);


    Robot *robot;
    Weapon *weapon;
    Robot *enemy;

    Weapon * enemy_weapon;
    Weapon * getEnemyBackWeapon();  //得到反击武器

    static int getDamage(Robot * robot2, Robot * enemy2, Weapon *weapon2);
    static int getMaxDamage(Robot * robot2, Robot * enemy2);


    int getEnemyDamage();
    int getPlayerDamage();

    QGraphicsTextItem * enemy_text = 0;
    QGraphicsTextItem * player_text = 0;

    QGraphicsTextItem * battle_text = 0;

    double enemy_rate = 0;

    double player_rate = 0;

    int fps = 250;

    //计算命中率
    double calcRadio(Robot * robot, Weapon * weapon, Robot * enemy);

    //计算是否命中
    bool prob(double p);

public slots:
    void moveAnimation();
};

#endif // BATTLEGROUND_H
