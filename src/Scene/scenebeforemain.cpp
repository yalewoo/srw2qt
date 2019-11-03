#include "scenebeforemain.h"

#include "../DataReader/config.h"

#include <qgraphicsitem.h>
extern Config *config;

#include "game.h"
extern Game * game;

SceneBeforeMain::SceneBeforeMain()
{
    QGraphicsPixmapItem * item = new QGraphicsPixmapItem();
    QPixmap * mapImage = new QPixmap(config->start_map_image_path);

    item->setPixmap(*mapImage);
    item->setScale(0.7);
    add(item);
}

void SceneBeforeMain::showConversation()
{
    game->story->execute(0);
}

PositionAndSize SceneBeforeMain::getConversationPosition()
{
    return PositionAndSize(0,600,500, 200);
}
