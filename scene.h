#ifndef SCENE_H
#define SCENE_H

#include <QGraphicsView>

class Scene : public QGraphicsScene
{
public:
    Scene();

    void add(QGraphicsItem * item);
    void remove(QGraphicsItem * item);

    void virtual init();
};

#endif // SCENE_H
