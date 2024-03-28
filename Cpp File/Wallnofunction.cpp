#include "Wall.h"
//#include "Bullet.h"
//#include "Player.h"
//#include "Enemy.h"
#include <QGraphicsPixmapItem>
#include <QGraphicsScene>
#include <QList>
#include <QTimer>
#include <QDebug>

Wall::Wall(char type)
{
    state = true;
    switch(type)
    {
        case 'b':
            walltype = Walltype::BRICK;
            setPixmap(QPixmap(":/image/Brick_Wall.png").scaled(15,15));
            break;
        case 's':
            walltype = Walltype::STEEL;
            setPixmap(QPixmap(":/image/Steel_Wall.png").scaled(15,15));
            break;
        case 't':
            walltype = Walltype::TREE;
            setPixmap(QPixmap(":/image/Trees.png").scaled(15,15));
            break;
        case 'w':
            walltype = Walltype::WATER;
            setPixmap(QPixmap(":/image/Water.png").scaled(15,15));
            break;
        case 'i':
            walltype = Walltype::ICE;
            setPixmap(QPixmap(":/image/Ice.png").scaled(15,15));
            break;
    }
    setPos(rand() % int(800 - boundingRect().width()), rand() % int(600 - boundingRect().height()));    //位置隨機出現

    // 檢查牆有沒有被打到
//    timer = new QTimer(this);
//    connect(timer, &QTimer::timeout, this, &Wall::broken);
//    timer->start(100);
}

void Wall::setState(bool ok)
{
    state = ok;
}

bool Wall::getState()
{
    return state;
}

//void Wall::freezeTank()
//{
//    if (walltype == Walltype::BRICK){

//    }
//    else if (walltype == Walltype::STEEL){

//    }
//    else if (walltype == Walltype::TREE){

//    }
//    else if (walltype == Walltype::WATER){

//    }
//    else if (walltype == Walltype::ICE){

//    }
//}

//void Wall::broken() // 考慮 bullet v.s. wall
//{
//    // 先取得兩者相碰的清單
//    QList<QGraphicsItem *> colliding_items = collidingItems();
//    // qDebug() << colliding_items;
//    foreach (QGraphicsItem *item, colliding_items) {
//        // 如果是一個 Bullet 物件，則可以轉型出一個 Bullet pointer
//        Bullet *bullet = dynamic_cast<Bullet*> (item);
//        // 如果錦旗被打，發出一個訊號通知
//        if (bullet) {
//            if (walltype == Walltype::BRICK){
//                state = false;
//                emit breakWall(this, bullet); // 提醒 Scene
//                scene()->removeItem(this);
//                qDebug() << "Brick is hit...";
//                delete this;
//            }
//            else if (walltype == Walltype::STEEL){
//                // 這邊要讓子彈消失
//                scene()->removeItem(bullet);
//                delete bullet;
//            }
//            else if (walltype == Walltype::TREE){
//                // 子彈可以穿過去
//                this->setZValue(1);
//            }
//            else if (walltype == Walltype::WATER){
//                bullet->setZValue(1);
//                // 子彈可以穿過去
//            }
//            else if (walltype == Walltype::ICE){
//                bullet->setZValue(1);
//                // 子彈可以穿過去
//            }
//        }
//    }
//}

