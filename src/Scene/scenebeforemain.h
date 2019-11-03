#ifndef SCENEBEFOREMAIN_H
#define SCENEBEFOREMAIN_H

#include "scene.h"

class SceneBeforeMain : public Scene
{
    Q_OBJECT

public:
    SceneBeforeMain();
    void showConversation();

    PositionAndSize getConversationPosition();

};

#endif // SCENEBEFOREMAIN_H
