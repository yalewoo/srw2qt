#ifndef SCENE_START_H
#define SCENE_START_H

#include "scene.h"

class Button;
class Menu;

class SceneStart : public Scene
{
    Q_OBJECT

    Menu * menu = nullptr;
    Button * btnStart = nullptr;
    Button * btnStartFromSave = nullptr;
public:
    SceneStart();
    void init();

public slots:
    void Start();
    void StartFromSave();
signals:
    void StartClicked();

};

#endif // SCENE_START_H
