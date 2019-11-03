#ifndef SCENE_H
#define SCENE_H

#include <QGraphicsView>
#include <common.h>

class Scene : public QGraphicsScene
{
public:
    Scene();

    void add(QGraphicsItem * item);
    void remove(QGraphicsItem * item);

    void virtual init();

    PositionAndSize virtual getConversationPosition();
};

#endif // SCENE_H
