#include "Flag.h"
#include "Bullet.h"
#include <QGraphicsPixmapItem>
#include <QGraphicsScene>
#include <QList>
#include <QTimer>
#include <QDebug>

Flag::Flag()
{
    state = true;
    setPixmap(QPixmap(":/images/Flag/flag.png").scaled(45,45));  // 一格wall的4倍大小

    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &Flag::gameOver);
    timer->start(1);
}

void Flag::setState(bool alive)
{
    state = alive;
}

bool Flag::getState()
{
    return state;
}

void Flag::gameOver()
{
    // 先取得兩者相碰的清單
    QList<QGraphicsItem *> colliding_items = collidingItems();
    foreach (QGraphicsItem *item, colliding_items) {
        // 如果是一個 Bullet 物件，則可以轉型出一個 Bullet pointer
        Bullet *bullet = dynamic_cast<Bullet*> (item);
        // 如果錦旗被打，發出一個訊號通知
        if (bullet) {
            state = false;
            setPixmap(QPixmap(":/images/Flag/diedflag.png").scaled(45,45));  // 一格wall的4倍大小
            emit flagIsDestroyed(this, bullet);
            qDebug() << "Game over...";
        }
    }
}

