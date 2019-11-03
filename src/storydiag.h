#ifndef STORYDIAG_H
#define STORYDIAG_H


#include <QGraphicsRectItem>
#include <QObject>

#include "robot.h"
#include "weapon.h"

#include "common.h"
#include "conversation.h"

class StoryDiag : public QObject, public QGraphicsRectItem
{
    Q_OBJECT
public:
    StoryDiag(QVector<Conversation> conversitions);
    void setSize(int width, int height);
    void setSize(PositionAndSize position);


    bool finished = false;

    QVector<Conversation> conversitions;



    QGraphicsPixmapItem * peopleImage = 0;

    int stage = 0;
    void mousePressEvent(QGraphicsSceneMouseEvent *event);

    void showTalk(int index);

    QGraphicsTextItem * player_text = 0;

    QGraphicsTextItem * battle_text = 0;
};

#endif // STORYDIAG_H
