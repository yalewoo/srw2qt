#include "game.h"

#include "map.h"

#include "robot.h"

#include "button.h"

#include "people.h"

#include <QCoreApplication>


void waitVariableToBeTrueAllEvents(bool & b)
{
    while (!b)
        QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
}
void waitVariableToBeTrueExcludeUserInput(bool & b)
{
    while (!b)
        QCoreApplication::processEvents(QEventLoop::ExcludeUserInputEvents, 100);
}

Game::Game(QWidget *parent)
{
    //设置不显示滚动条
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    //设置视图固定大小
    setFixedSize(1024, 768);

    //新建场景，大小1024*768
    scene = new QGraphicsScene();
    scene->setSceneRect(0,0,1024,768);
    //将scene和View绑定
    setScene(scene);

    workDir = QString("C:/code/srw2_cpp/");
    robot_value_path = workDir + "input/value/robot.csv";
    weapon_value_path = workDir + "input/value/weapon.csv";
    people_value_path = workDir + "input/value/people.csv";




}

void Game::start()
{
    board = new Board(200, 800);
    scene->addItem(board);

    next_turn_button = new Button(QString("结束第1回合"), 0, 150, 50);
    next_turn_button->setPos(600, 650);
    scene->addItem(next_turn_button);
    connect(next_turn_button, SIGNAL(leftButtonClicked()), this, SLOT(next_turn()));

    //test
    map = new Map(workDir + "input/map/map1.csv");

    map->placePlayerRobot_init(workDir + "input/stage/robot_init.csv", 1);
    map->placeEnemyRobot_init(workDir + "input/stage/enemy_init.csv", 1);


    music_background = new Music();
    music_background->setMusicLoop(workDir + "/res/music/87.wav");
    music_battle = new Music();
    music_effect = new Music();

    connect(music_battle, SIGNAL(stateChanged(QMediaPlayer::State)), music_background, SLOT(state_change_slot(QMediaPlayer::State)));


}



Menu * Game::drawMenu(int x, int y, int width, int height, QColor color, double opacity)
{
    //draws a panel at x,y
    Menu * panel = new Menu(x, y, width, height, color, opacity);
    QBrush brush;
    brush.setStyle(Qt::SolidPattern);
    brush.setColor(color);
    panel->setBrush(brush);
    panel->setOpacity(opacity);
    scene->addItem(panel);

    connect(panel, SIGNAL(rightButtonClicked()), this, SLOT(cancel()));

    return panel;
}

void Game::displayMenu(Robot *robot)
{
    int x = robot->pos().x();
    int y = robot->pos().y();
    menu = drawMenu(x+20, y, 50, 120, Qt::darkCyan, 0.5);

    Button * button_move = menu->addButton(QString("移动"));
    connect(button_move, SIGNAL(leftButtonClicked()), this, SLOT(move()));

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

    Button * button_ok = menu->addButton(QString("待命"));
    connect(button_ok, SIGNAL(leftButtonClicked()), this, SLOT(finished()));
}

void Game::displayMenu2(Robot *robot)
{
    int x = robot->pos().x();
    int y = robot->pos().y();
    menu = drawMenu(x+20, y, 50, 120, Qt::darkCyan, 0.5);

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
    connect(Button_move, SIGNAL(leftButtonClicked()), this, SLOT(finished()));
}

void Game::deleteMenu()
{
    if (menu)
    {
        scene->removeItem(menu);
        delete menu;
        menu = 0;
    }
}



void Game::move()
{
   deleteMenu();

   map->UnshowMoveRange();
   map->showMoveRange(selectedRobot);

   canMoveStatus = true;
}

void Game::finished()
{
    deleteMenu();

    selectedRobot->setNotActive();

    selectedRobot = 0;
}

void Game::cancel()
{
    deleteMenu();

    if (selectedRobot)
        map->move(selectedRobot, originalPosition.x, originalPosition.y);

    map->UnshowMoveRange();
    canMoveStatus = false;
    selectedRobot = 0;
    selectedWeapon = 0;

}

void Game::attack1()
{
    selectedWeapon = selectedRobot->weapon1;
    showAttackRange();
}

void Game::attack2()
{
    selectedWeapon = selectedRobot->weapon2;
    showAttackRange();
}

void Game::AI()
{

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


    map->AI_move(selectedRobot);


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

void Game::next_turn()
{
    music_background->setMusicLoop(workDir + "/res/music/88.wav");

    QList<Robot *> & robots = map->player_robots;
    for (int i = 0; i < robots.size(); ++i)
    {
        robots[i]->setActive();
    }
    QCoreApplication::processEvents(QEventLoop::AllEvents, 100);


    QList<Robot *> & enemys = map->enemy_robots;
    for (int i = 0; i < enemys.size(); ++i)
    {
        selectedRobot = enemys[i];
        AI();
    }

    for (int i = 0; i < enemys.size(); ++i)
    {
        enemys[i]->setActive();

    }


    QCoreApplication::processEvents(QEventLoop::AllEvents);

    QString s;
    s.sprintf("结束第%d回合", ++turn);
    next_turn_button->setText(s);



    music_background->setMusicLoop(workDir + "/res/music/87.wav");
}



void Game::showAttackRange()
{
    deleteMenu();

    map->UnshowMoveRange();

    map->showAttackRange(selectedRobot, selectedWeapon);
}

void Game::attack(Robot *enemy2)
{
    enemy = enemy2;

    map->UnshowMoveRange();

    map->showAttackGif(selectedRobot->x, selectedRobot->y, enemy->x, enemy->y);



    battle = new Battle(selectedRobot, selectedWeapon, enemy);

}

void Game::attackDone()
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
    canMoveStatus = false;
    selectedRobot = 0;
    selectedWeapon = 0;

    if (battle)
    {
        delete battle;
        battle = 0;
    }

}


