#ifndef PLAYER_H
#define PLAYER_H

#include "Bullet.h"
#include "Enemy.h"
#include <QObject>
#include <QGraphicsPixmapItem>
#include <QTimer>
class Player :  public QGraphicsPixmapItem
{
public:
    Player(int who);
    int current_player_Direction=3;

    QPointF getCurPos() const;
    void setCurPos(QPointF newCurPos);

private:
    QPointF curPos;
};

#endif // PLAYER_H
