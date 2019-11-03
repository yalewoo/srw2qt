#ifndef GAME_H
#define GAME_H


#include <QDebug>
#include <QGraphicsView>

#include "music/musicmanager.h"
#include "scene_main.h"

void waitVariableToBeTrueAllEvents(bool & b);
void waitVariableToBeTrueExcludeUserInput(bool & b);
void waitForTimeAllEvents(int ms);

class SceneStart;
class SceneBeforeMain;
class SceneTitle;

class Game : public QGraphicsView
{
    Q_OBJECT
public:

    MusicManager * musicManager;

    SceneMain * scene;
    SceneStart * sceneStart;
    SceneBeforeMain * sceneBeforeMain;
    SceneTitle * sceneTitle;

    Game();

    void start();

    void init();

    // 剧情画面
    Story * story = 0;
    int stage;

public slots:
    void showSceneBeforeMain();
    void showSceneTitle();
    void showSceneMain();

    void cancel();



};

#endif // GAME_H
