#ifndef STORY_H
#define STORY_H


#include "robot.h"
#include "weapon.h"

#include "conversation.h"
#include "scene/scene.h"


class Story
{
    int m_stage;
    Scene * scene;
public:
    Story(int stage, Scene * scene);
    void setStage(int stage);

    void showConversition(int turn);

    void execute(int turn);
};

#endif // STORY_H
