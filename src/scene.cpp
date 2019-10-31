#include "scene.h"

Scene::Scene()
{

}

void Scene::add(QGraphicsItem *item)
{
    this->addItem(item);
}

void Scene::remove(QGraphicsItem *item)
{
    this->removeItem(item);
}

void Scene::init()
{

}
