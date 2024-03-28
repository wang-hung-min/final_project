#include "Flag.h"
//#include "Bullet.h"
#include <QGraphicsPixmapItem>
#include <QGraphicsScene>
#include <QList>
#include <QTimer>
#include <QDebug>

Flag::Flag()
{
    state = true;
    setPixmap(QPixmap(":/image/flag.png").scaled(30,30));  // 一格wall的4倍大小
    setPos(rand() % int(800 - boundingRect().width()), rand() % int(600 - boundingRect().height()));    //food 位置隨機出現

    // 呼叫 gameOver，使得其可以被吃
    // 檢查速度和移動速度不能差太多，避免位置產生問題（圖片重疊）
    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &Flag::gameOver);
    timer->start(100);
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
    // qDebug() << colliding_items;
//    foreach (QGraphicsItem *item, colliding_items) {
//        // 如果是一個 Bullet 物件，則可以轉型出一個 Bullet pointer
//        Bullet *bullet = dynamic_cast<Bullet*> (item);
//        // 如果錦旗被打，發出一個訊號通知
//        if (bullet) {
//            state = false;
//            //emit: 用來發出 signal 通知 Scene
//            emit hitByEnemy(this, bullet);
//            scene()->removeItem(this);
//            qDebug() << "Game over...";
//            delete this;
//        }
//    }
}

