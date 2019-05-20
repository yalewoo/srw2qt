#include "battle.h"
#include "scene_main.h"

SceneMain::SceneMain()
{
    setSceneRect(0,0,1024,768);
}

void SceneMain::init()
{
    next_turn_button = new Button(QString("结束第1回合"));
    next_turn_button->setPos(600, 650);
    add(next_turn_button);
    connect(next_turn_button, SIGNAL(leftButtonClicked()), this, SLOT(next_turn()));

    debug_button = new Button(QString("进入Debug模式"));
    debug_button->setPos(800, 650);
    add(debug_button);
    connect(debug_button, SIGNAL(leftButtonClicked()), this, SLOT(enable_debug_mode()));

    map = new Map();

    stage = 1;

    map->loadLevel(stage);


    robotStatus = new RobotStatusView(200, 800);

    map->placeEnemy(stage,1);

    map->placeRobot(stage,1);

    music_background = new Music();
    music_background->setMusicLoop(config->background_music_path);
    music_battle = new Music();
    music_effect = new Music();

    connect(music_battle, SIGNAL(stateChanged(QMediaPlayer::State)), music_background, SLOT(state_change_slot(QMediaPlayer::State)));

}

Menu * SceneMain::drawMenu(int x, int y, int width, int height, QColor color, double opacity)
{
    //draws a panel at x,y
    Menu * panel = new Menu(x, y, width, height, color, opacity);
    QBrush brush;
    brush.setStyle(Qt::SolidPattern);
    brush.setColor(color);
    panel->setBrush(brush);
    panel->setOpacity(opacity);
    this->add(panel);

    connect(panel, SIGNAL(rightButtonClicked()), this, SLOT(cancel()));

    return panel;
}

void SceneMain::displayMenu(Robot *robot)
{
    int x = robot->pos().x();
    int y = robot->pos().y();
    menu = drawMenu(Menu_x, Menu_y, 0, 0, Qt::darkCyan, 0.5);

    Button * button_ai = menu->addButton(QString("AI行动"));
    connect(button_ai, SIGNAL(leftButtonClicked()), this, SLOT(AI()));

    if (robot->canAttack1())
    {
        Button * button_attack1 = menu->addButton(robot->weapon1->name);
        connect(button_attack1, SIGNAL(leftButtonClicked()), this, SLOT(attack1()));
    }
    if (robot->canAttack2())
    {
        Button * button_attack2 = menu->addButton(robot->weapon2->name);
        connect(button_attack2, SIGNAL(leftButtonClicked()), this, SLOT(attack2()));
    }

    if (inDebugMode)
    {
        if (selectedRobot && selectedRobot->active == false)
        {
            Button * button_set_active = menu->addButton(QString("设置激活状态"));
            connect(button_set_active, SIGNAL(leftButtonClicked()), this, SLOT(setActive()));

        }
    }

    Button * button_ok = menu->addButton(QString("待命"));
    connect(button_ok, SIGNAL(leftButtonClicked()), this, SLOT(robotActionFinished()));
}

void SceneMain::displayMenu2(Robot *robot)
{
    int x = robot->pos().x();
    int y = robot->pos().y();
    menu = drawMenu(Menu_x, Menu_y, 0, 0, Qt::darkCyan, 0.5);

    if (robot->weapon1->range == 1 && robot->canAttack1())
    {
        Button * button_attack1 = menu->addButton(robot->weapon1->name);
        connect(button_attack1, SIGNAL(leftButtonClicked()), this, SLOT(attack1()));
    }
    if (robot->weapon2->range == 1 && robot->canAttack2())
    {
        Button * button_attack2 = menu->addButton(robot->weapon2->name);
        connect(button_attack2, SIGNAL(leftButtonClicked()), this, SLOT(attack2()));
    }


    Button * Button_move = menu->addButton(QString("待命"));
    connect(Button_move, SIGNAL(leftButtonClicked()), this, SLOT(robotActionFinished()));
}

void SceneMain::deleteMenu()
{
    if (menu)
    {
        this->remove(menu);
        delete menu;
        menu = 0;
    }
}

void SceneMain::robotActionFinished()
{
    deleteMenu();

    map->UnshowMoveRange();
    inMoveStatus  = false;



    selectedRobot->setNotActive();

    selectedRobot = nullptr;
    selectedWeapon = nullptr;

}

void SceneMain::cancel()
{
    deleteMenu();

    if (selectedRobot)
        map->move(selectedRobot, originalPosition.x, originalPosition.y);

    map->UnshowMoveRange();
    inMoveStatus  = false;
    selectedRobot = 0;
    selectedWeapon = 0;

}


void SceneMain::next_turn()
{
    ++round;

    map->placeEnemy(stage, round);

    music_background->setMusicLoop(config->background_music_path_enemy);

    for (int i = 1; i < map->width - 1; ++i)
    {
        for (int j = 1; j < map->height - 1; ++j)
        {
            if (map->robots[i][j])
            {
                map->robots[i][j]->setActive();

            }
        }
    }
    for (int i = 1; i < map->width - 1; ++i)
    {
        for (int j = 1; j < map->height - 1; ++j)
        {
            qDebug() << i << j;
            if (map->robots[i][j] && map->robots[i][j]->player == 1)
            {
                Robot * robot = map->robots[i][j];
                if (robot->active)
                {
                    selectedRobot = map->robots[i][j];
                    AI();
                    qDebug() << "AI()";
                }
            }
        }
    }

    for (int i = 1; i < map->width - 1; ++i)
    {
        for (int j = 1; j < map->height - 1; ++j)
        {
            if (map->robots[i][j])
            {
                map->robots[i][j]->setActive();

            }
        }
    }

    QString s;
    s.sprintf("结束第%d回合", round);
    next_turn_button->setText(s);



    music_background->setMusicLoop(config->background_music_path);

    story->showConversition(round);
}

void SceneMain::enable_debug_mode()
{
    inDebugMode = !inDebugMode;

    QString s;
    if (inDebugMode)
    {
        s.sprintf("退出Debug模式", round);

    }
    else {
        s.sprintf("进入Debug模式", round);

    }
    debug_button->setText(s);
}


void SceneMain::AI()
{
    deleteMenu();
    map->UnshowMoveRange();
    inMoveStatus  = false;

    Robot * enemy;
    if (enemy = selectedRobot->canAttack1())
    {
        selectedWeapon = selectedRobot->weapon1;
        attack(enemy);
        return;
    }
    if (enemy = selectedRobot->canAttack2())
    {
        selectedWeapon = selectedRobot->weapon2;
        attack(enemy);
        return;
    }

    if (round >= selectedRobot->robotBehavior)
    {
        map->AI_move(selectedRobot);
    }


    if (selectedRobot->weapon1 && selectedRobot->weapon1->range == 1 && (enemy = selectedRobot->canAttack1()))
    {
        selectedWeapon = selectedRobot->weapon1;
        attack(enemy);
        return;
    }
    else if (selectedRobot->weapon2 && selectedRobot->weapon2->range == 1 && (enemy = selectedRobot->canAttack2()))
    {
        selectedWeapon = selectedRobot->weapon2;
        attack(enemy);
        return;
    }

    selectedRobot->setNotActive();
    selectedRobot = 0;
}
void SceneMain::attack1()
{
    selectedWeapon = selectedRobot->weapon1;
    showAttackRange();
}

void SceneMain::attack2()
{
    selectedWeapon = selectedRobot->weapon2;
    showAttackRange();
}

void SceneMain::setActive()
{
    deleteMenu();

    map->UnshowMoveRange();

    selectedRobot->setActive();

    selectedRobot = 0;
}
void SceneMain::attack(Robot *enemy2)
{
    enemy = enemy2;

    deleteMenu();

    map->UnshowMoveRange();

    map->showAttackGif(selectedRobot->x, selectedRobot->y, enemy->x, enemy->y);



    battle = new Battle(selectedRobot, selectedWeapon, enemy);

}
void SceneMain::showAttackRange()
{
    deleteMenu();

    map->UnshowMoveRange();

    map->showAttackRange(selectedRobot, selectedWeapon);
}

void SceneMain::attackDone()
{
    deleteMenu();

    if (enemy->hp <= 0)
    {
        map->removeRobot(enemy);
    }
    else if (selectedRobot->hp <= 0)
    {
        map->removeRobot(selectedRobot);
    }


    if (selectedRobot)
    {
        selectedRobot->setNotActive();
    }



    map->UnshowMoveRange();
    this->inMoveStatus = false;
    selectedRobot = 0;
    selectedWeapon = 0;

    if (battle)
    {
        delete battle;
        battle = 0;
    }

}

void SceneMain::showDiagDone()
{

}

void SceneMain::showConversition()
{
    story = new Story();

    story->showConversition(1);
}
