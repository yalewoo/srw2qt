#ifndef MENU_H
#define MENU_H

#include <QGraphicsRectItem>
#include <QObject>

#include "button.h"


class Menu : public QObject, public QGraphicsRectItem
{
    Q_OBJECT


public:
    enum Layout{
        horizontal,
        vertical,
    };
    Layout layout;

    Menu(Layout layout, int x, int y);

    void setXY(qreal x, qreal y);


    void mousePressEvent(QGraphicsSceneMouseEvent *event);

    Button * addButton(QString name);
    int buttonNum = 0;

    int x;
    int y;

    int x_index = 0;
    int y_index = 0;

signals:
    void leftButtonClicked();
    void rightButtonClicked();
};

#endif // MENU_H
