#include "game.h"

#include "datahelper.h"
#include "scene_main.h"

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

Game::Game()
{
    // init view settings
    init();


}

void Game::start()
{
    // Load data from file
    Robot::enemys_init = DataHelper::EnemyRobotInit(config->enemy_init_path);
    Robot::robots_init = DataHelper::RobotInit(config->robot_init_path);

    Robot::exp_update_table = DataHelper::getExpUpdateTable();



    scene = new SceneMain();


    scene->init();

    setScene(scene);


    this->show();



    //scene->showConversition();

}

void Game::init()
{
    //设置不显示滚动条
    //setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    //setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    //设置视图固定大小
    //setFixedSize(1024, 768);

    setAlignment(Qt::AlignLeft | Qt::AlignTop);
    this->resize(1024, 768);
}
