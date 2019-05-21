#ifndef GAME_H
#define GAME_H


#include <QDebug>
#include <QGraphicsView>

#include "scene_main.h"

void waitVariableToBeTrueAllEvents(bool & b);
void waitVariableToBeTrueExcludeUserInput(bool & b);




class Game : public QGraphicsView
{
public:

    SceneMain * scene;

    Game();

    void start();

    void init();


};

#endif // GAME_H
