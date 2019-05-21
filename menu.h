#ifndef MENU_H
#define MENU_H

#include <QGraphicsRectItem>
#include <QObject>

#include "button.h"

class Menu : public QObject, public QGraphicsRectItem
{
    Q_OBJECT
public:
    Menu(qreal x, qreal y, qreal width, qreal height, QColor color, double opacity = 0.5);

    void mousePressEvent(QGraphicsSceneMouseEvent *event);

    Button * addButton(QString name);
    int buttonNum = 0;

    int x;
    int y;

    int x_index = 0;

signals:
    void leftButtonClicked();
    void rightButtonClicked();
};

#endif // MENU_H
