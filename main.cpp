#include <QApplication>

#include "config.h"
#include "game.h"

#include "imageresourcemanager.h"

Config *config;
Game * game;

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    config = new Config();

    ImageResourceManager::init();

    game = new Game();
    game->start();

    return a.exec();
}
