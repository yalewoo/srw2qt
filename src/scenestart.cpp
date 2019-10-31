#include "ui/button.h"
#include "ui/menu.h"
#include "scenestart.h"

SceneStart::SceneStart()
{

}

void SceneStart::init()
{
    menu = new Menu(Menu::Layout::vertical, 300, 300);

    btnStart = menu->addButton("开始新游戏");
    connect(btnStart, SIGNAL(leftButtonClicked()), this, SLOT(Start()));

    btnStartFromSave = menu->addButton("继续游戏");
    connect(btnStartFromSave, SIGNAL(leftButtonClicked()), this, SLOT(StartFromSave()));

    this->add(menu);
}

void SceneStart::Start()
{
    emit StartClicked();
}

void SceneStart::StartFromSave()
{

}

