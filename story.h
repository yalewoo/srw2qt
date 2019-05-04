#ifndef STORY_H
#define STORY_H


#include "robot.h"
#include "weapon.h"

#include "conversation.h"


class Story
{
public:
    Story();

    void showConversition(int turn);

    QVector<QVector<Conversation> > conversitions;
};

#endif // STORY_H
