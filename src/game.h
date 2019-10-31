#ifndef GAME_H
#define GAME_H


#include <QDebug>
#include <QGraphicsView>

#include "music/musicmanager.h"
#include "scene_main.h"

void waitVariableToBeTrueAllEvents(bool & b);
void waitVariableToBeTrueExcludeUserInput(bool & b);


class SceneStart;

class Game : public QGraphicsView
{
    Q_OBJECT
public:

    MusicManager * musicManager;

    SceneMain * scene;
    SceneStart * sceneStart;

    Game();

    void start();

    void init();
public slots:
    void StartClicked();
    void cancel();



};

#endif // GAME_H
