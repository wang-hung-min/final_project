#include "Player.h"
#include <QGraphicsRectItem>
#include <QKeyEvent>
#include <QGraphicsPixmapItem>
#include <QGraphicsOpacityEffect>

Player::Player(int who)
{
    if (who == 1)
        setPixmap(QPixmap(":/images/Players/Player1.png").scaled(40,40));
    else if(who == 2)
        setPixmap(QPixmap(":/images/Players/Player2.png").scaled(40,40));
}

QPointF Player::getCurPos() const
{
    return curPos;
}

void Player::setCurPos(QPointF newCurPos)
{
    curPos = newCurPos;
}
