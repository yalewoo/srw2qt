#ifndef SCENEMAIN_H
#define SCENEMAIN_H

#include "scene.h"
#include "robotstatusview.h"

#include "ai.h"
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
    Menu * drawMenu(Menu::Layout layout, int x, int y);
    void displayMenu(Robot * robot);    //第一次点击菜单
    void displayMenu2(Robot * robot);   //移动后出现的菜单
    void displayMenu3(Robot * robot, Robot * enemy);   //攻击时出现的菜单
    void displayMenu4();   //舰长起飞专属菜单
    int Menu_x = 10;
    int Menu_y = 710;

    Menu * menu = 0;
    void deleteMenu();

    bool inMoveStatus = false;

    // 选中的机器人
    Robot * selectedRobot = nullptr;
    Point originalPosition;     //选中机器人的原始位置，用于右键取消
    Weapon * selectedWeapon = 0;    //选中的机器

    // 起飞可以取消
    Robot * captain = 0;

    // 结束回合按钮
    Button * next_turn_button = 0;
    int round = 1;

    //显示攻击范围
    void showAttackRange();
    void showAttackRangeAfterMove();
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

    // 动画控制
    bool isMovingRobot = false;

    void attackDone();

    // 剧情画面
    Story * story = 0;
    void showDiagDone();

    // AI
    void AI();  //
    void AICore();

    void win();// 是否胜利

    // 保存进度
    void saveToFile_expTable();
    void LoadFromFile_expTable();
    QVector<int> exp_table;

public slots:
    void robotActionFinished();
    void cancel();  //按下右键后，取消行动
    void next_turn();   //回合结束

    void enable_debug_mode();   //debug模式
    void change_stage();   //debug模式

    void AI_robot();  //AI自动行动

    void attack1(); //使用武器1
    void attack2(); //使用武器2

    void do_attack1(); //使用武器1
    void do_attack2(); //使用武器2

    //debug
    void setActive();   //设置机器人激活状态
    void addExp();

    void transform(int id);

    // 起飞 舰长专属
    void launch(int id);

    void saveToFile();
    void loadFromFile();


    bool canUseSpirit(int id);
    void use_sprit_begin(int id); //所有精神都执行的代码
    void use_sprit_end(int id); //所有精神都执行的代码
    void use_sprit_0();	//毅力
    void use_sprit_1();	//加速s
    void use_sprit_2();	//瞄准s
    void use_sprit_3();	//防守s
    void use_sprit_4();	//强攻s
    void use_sprit_5();	//友情
    void use_sprit_6();	//必杀s
    void use_sprit_7();	//疾风s
    void use_sprit_8();	//回避s
    void use_sprit_9();	//潜力
    void use_sprit_10();	//热血s
    void use_sprit_11();	//情义
    void use_sprit_12();	//传真
    void use_sprit_13();	//援助
    void use_sprit_14();	//怒
    void use_sprit_15();	//祈祷
    void use_sprit_16();	//干扰s+
    void use_sprit_17();	//狂怒
    void use_sprit_18();	//爱心
};

#endif // SCENE_H
