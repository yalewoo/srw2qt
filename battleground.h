#ifndef BATTLEGROUND_H
#define BATTLEGROUND_H


#include <QGraphicsRectItem>
#include <QObject>

#include "robot.h"
#include "weapon.h"

class BattleGround : public QObject, public QGraphicsRectItem
{
public:
    BattleGround(Robot *robot, Weapon *weapon, Robot *enemy);
    void setSize(int width, int height);

    //显示一句话
    void showString(QString s);

    //显示hp和rate
    void showHpAndRate();

    int stage = 0;
    void mousePressEvent(QGraphicsSceneMouseEvent *event);


    Robot *robot;
    Weapon *weapon;
    Robot *enemy;

    Weapon * enemy_weapon;
    Weapon * getEnemyBackWeapon();  //得到反击武器

    int getEnemyDamage();
    int getPlayerDamage();

    QGraphicsTextItem * enemy_text = 0;
    QGraphicsTextItem * player_text = 0;

    QGraphicsTextItem * battle_text = 0;

    double enemy_rate = 0;

    double player_rate = 0;

    //计算命中率
    double calcRadio(Robot * robot, Weapon * weapon, Robot * enemy);

    //计算是否命中
    bool prob(double p);
};

#endif // BATTLEGROUND_H
