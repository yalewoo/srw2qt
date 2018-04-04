#include "button.h"

#include <QBrush>

#include "game.h"

extern Game * game;

Button::Button(QString name, QGraphicsItem *parent, int width, int height) : QGraphicsRectItem(parent)
{
    //draw the rect
    setRect(0, 0, width, height);
    QBrush brush;
    brush.setStyle(Qt::SolidPattern);
    brush.setColor(Qt::darkCyan);
    setBrush(brush);

    //draw the text
    text = new QGraphicsTextItem(name, this);
    int xPos = rect().width()/2 - text->boundingRect().width()/2;
    int yPos = rect().height()/2 - text->boundingRect().height()/2;
    text->setPos(xPos, yPos);

    //allow responding to hover events
    setAcceptHoverEvents(true);


    game->connect(this, SIGNAL(rightButtonClicked()), game, SLOT(cancel()));
}

void Button::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
    {
        emit leftButtonClicked();
    }
    else if (event->button() == Qt::RightButton)
    {
        emit rightButtonClicked();
    }

}

void Button::hoverEnterEvent(QGraphicsSceneHoverEvent *event)
{
    //change color to cyan
    QBrush brush;
    brush.setStyle(Qt::SolidPattern);
    brush.setColor(Qt::cyan);
    setBrush(brush);
}

void Button::hoverLeaveEvent(QGraphicsSceneHoverEvent *event)
{
    //change color to dark cyan
    QBrush brush;
    brush.setStyle(Qt::SolidPattern);
    brush.setColor(Qt::darkCyan);
    setBrush(brush);
}

void Button::setText(QString s)
{
    text->setPlainText(s);
}
