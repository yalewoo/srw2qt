#include <QApplication>

#include "game.h"

Game * game;

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    game = new Game();
    game->start();
    game->show();

    return a.exec();
}
