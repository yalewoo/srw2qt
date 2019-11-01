#ifndef STORY_H
#define STORY_H


#include "robot.h"
#include "weapon.h"

#include "conversation.h"


class Story
{
    int m_stage;
public:
    Story(int stage);
    void setStage(int stage);

    void showConversition(int turn);

    void execute(int turn);
};

#endif // STORY_H
