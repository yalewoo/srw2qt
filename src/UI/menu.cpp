#include "menu.h"

#include <QBrush>

#include "game.h"

extern Game * game;


Menu::Menu(Menu::Layout layout, int x, int y)
{
    this->layout = layout;
    this->x = x;
    this->y = y;
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

    if (layout == Menu::Layout::horizontal)
    {
            button->setPos(x + x_index + 10, y);
            x_index += button->width + 10;
    }
    else
    {
            button->setPos(x, y + y_index + 10);
            y_index += button->height + 10;

    }

    ++buttonNum;
    return button;
}
