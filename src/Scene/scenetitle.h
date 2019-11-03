#ifndef SCENETITLE_H
#define SCENETITLE_H

#include "scene.h"


class SceneTitle : public Scene
{
    Q_OBJECT

public:
    SceneTitle();

    void PlayMusic();

    void mousePressEvent(QGraphicsSceneMouseEvent *event);

signals:
    void StartClicked();
};

#endif // SCENETITLE_H
