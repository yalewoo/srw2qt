#include <QApplication>

#include "config.h"
#include "game.h"

Config *config;
Game * game;

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    config = new Config();

    game = new Game();
    game->start();

    return a.exec();
}
