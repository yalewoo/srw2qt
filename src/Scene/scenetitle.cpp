#include "scenetitle.h"

#include <qgraphicsitem.h>
#include <QString>


#include "game.h"
extern Game * game;


SceneTitle::SceneTitle()
{
    QGraphicsPixmapItem * item = new QGraphicsPixmapItem();

    QString s = "第一章";
    QGraphicsSimpleTextItem * m_text = new QGraphicsSimpleTextItem(s);
    //game->scene->addItem(text);

    QFont font = m_text->font();
    font.setPixelSize(26);  // 像素大小
    font.setBold(true);
    m_text->setFont(font);
    m_text->setBrush(QBrush(QColor(255, 255, 255)));
//    text->setBrush(QBrush(QColor(0, 0,0)));

    m_text->setPen(QPen(QColor(0, 255, 255)));

    add(m_text);
}

void SceneTitle::PlayMusic()
{
    game->musicManager->PlayOnceMusicNonStopBackground("92.wav");
}

void SceneTitle::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    emit StartClicked();
}
