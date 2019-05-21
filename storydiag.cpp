#include "storydiag.h"


#include <QBrush>

#include "game.h"
extern Game * game;

#include "point.h"

#include "imageresourcemanager.h"
#include "people.h"

#include <QTime>
#include <QDebug>
#include <QTimer>

#include <QtMath>



void StoryDiag::setSize(int width, int height)
{
    int x = 0;
    int y = 0;

    setRect(x, y, width, height);


    QBrush brush;
    brush.setStyle(Qt::SolidPattern);
    brush.setColor(Qt::gray);
    setBrush(brush);
    setOpacity(0.85);
}

StoryDiag::StoryDiag(QVector<Conversation> conversitions):conversitions(conversitions)
{
    showTalk(stage);

}



void StoryDiag::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    ++stage;
    if (stage >= conversitions.length())
    {
        finished = true;
        return;
    }
    showTalk(stage);
}

void StoryDiag::showTalk(int index)
{
    if (!peopleImage)
        peopleImage = new QGraphicsPixmapItem(this);

    qDebug() << conversitions[index].people;
    peopleImage->setPixmap(ImageResourceManager::getPeopleImage(conversitions[index].people));

    peopleImage->setPos(this->x() + 10, this->y() + 600);

    QString s;

    s = QString(conversitions[index].says);
    if (!player_text)
        player_text = new QGraphicsTextItem(s, this);
    player_text->setPlainText(s);
    player_text->setScale(1.7);
    player_text->setPos(this->x() + 80, this->y() + 600);
}
