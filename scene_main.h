#ifndef SCENEMAIN_H
#define SCENEMAIN_H

#include "scene.h"
#include "robotstatusview.h"

#include "battle.h"
#include "map.h"
#include "menu.h"
#include "music.h"
#include "point.h"
#include "story.h"

class SceneMain : public Scene
{
    Q_OBJECT
public:
    Map * map = 0;
    RobotStatusView * robotStatus = 0;

    SceneMain();

    void init();

    int stage = 0;  //关数

    void loadStage(int stage, bool useDefaultRobot = true);

    //显示菜单
    Menu * drawMenu(int x, int y, int width, int height, QColor color, double opacity = 1);
    void displayMenu(Robot * robot);    //第一次点击菜单
    void displayMenu2(Robot * robot);   //移动后出现的菜单
    int Menu_x = 10;
    int Menu_y = 710;

    Menu * menu = 0;
    void deleteMenu();

    bool inMoveStatus = false;

    // 选中的机器人
    Robot * selectedRobot = nullptr;
    Point originalPosition;     //选中机器人的原始位置，用于右键取消
    Weapon * selectedWeapon = 0;    //选中的机器

    // 结束回合按钮
    Button * next_turn_button = 0;
    int round = 1;

    //显示攻击范围
    void showAttackRange();
    void attack(Robot * enemy);

    Robot * enemy = 0;  //敌人

    // 调试按钮
    Button * debug_button = 0;
    bool inDebugMode = false;
    Button * changeStageButton = 0;

    // Save and load
    Button * saveButton = 0;
    Button * loadButton = 0;

    //music
    Music * music_background = 0;
    Music * music_effect = 0;
    Music * music_battle = 0;

    //战斗画面
    Battle * battle = 0;

    void attackDone();

    // 剧情画面
    Story * story = 0;
    void showDiagDone();
    void showConversition();

public slots:
    void robotActionFinished();
    void cancel();  //按下右键后，取消行动
    void next_turn();   //回合结束

    void enable_debug_mode();   //debug模式
    void change_stage();   //debug模式

    void AI();  //AI自动行动

    void attack1(); //使用武器1
    void attack2(); //使用武器2

    void setActive();   //设置机器人激活状态

    void saveToFile();
    void loadFromFile();
};

#endif // SCENE_H
