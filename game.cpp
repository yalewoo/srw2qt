#include "game.h"

#include "map.h"

#include "robot.h"

#include "button.h"

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

    workDir = QString("F:/Project/srw2_cpp/");

    selectedRobot = 0;
}

void Game::start()
{
    //test
    map = new Map(16, 20);

    map->addRobot(6,5,0x0001,0);
    map->addRobot(5,5,0x1001,1);



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

    selectedRobot = 0;
}

void Game::cancel()
{
    deleteMenu();

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

void Game::showAttackRange()
{
    deleteMenu();

    map->UnshowMoveRange();

    map->showAttackRange(selectedRobot, selectedWeapon);
}

void Game::attack(Robot *enemy)
{
    battleGround = new BattleGround(selectedRobot, selectedWeapon, enemy);

    battleGround->setSize(map->width*32, map->height*32);
    scene->addItem(battleGround);


}

void Game::attackDone()
{
    deleteMenu();


    map->UnshowMoveRange();
    canMoveStatus = false;
    selectedRobot = 0;
    selectedWeapon = 0;

    if (battleGround)
    {
        scene->removeItem(this->battleGround);
        delete battleGround;
    }

}


