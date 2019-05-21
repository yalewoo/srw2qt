#include "menu.h"

#include <QBrush>

#include "game.h"

extern Game * game;

Menu::Menu(qreal xPos, qreal yPos, qreal width, qreal height, QColor color, double opacity) : QGraphicsRectItem(xPos, yPos, width, height)
{
    x = xPos;
    y = yPos;
//    QBrush brush;
//    brush.setStyle(Qt::SolidPattern);
//    brush.setColor(color);
//    setBrush(brush);
//    setOpacity(opacity);


}

void Menu::mousePressEvent(QGraphicsSceneMouseEvent *event)
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

Button * Menu::addButton(QString name)
{
    Button * button = new Button(name, this);
    button->setPos(x + x_index + 10, y);
    ++buttonNum;
    x_index += button->width + 10;
    return button;
}
