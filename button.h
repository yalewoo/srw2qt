#ifndef BUTTON_H
#define BUTTON_H

#include <QGraphicsRectItem>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsSceneHoverEvent>
#include <QObject>

class Button : public QObject, public QGraphicsRectItem
{
    Q_OBJECT
public:
    Button(QString name, QGraphicsItem * parent = 0);

    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void hoverEnterEvent(QGraphicsSceneHoverEvent *event);
    void  hoverLeaveEvent(QGraphicsSceneHoverEvent *event);

signals:
    void leftButtonClicked();
    void rightButtonClicked();

private:
    QGraphicsTextItem * text;
};

#endif // BUTTON_H
