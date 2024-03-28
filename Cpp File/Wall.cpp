#include "Wall.h"
#include "Bullet.h"
#include "Enemy.h"
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
        typeIndex = 0;
        setPixmap(QPixmap(":/images/Environment/Brick_Wall.png").scaled(15,15));
        break;
    case 's':
        walltype = Walltype::STEEL;
        typeIndex = 1;
        setPixmap(QPixmap(":/images/Environment/Steel_Wall.png").scaled(15,15));
        break;
    case 't':
        walltype = Walltype::TREE;
        typeIndex = 2;
        setPixmap(QPixmap(":/images/Environment/Trees.png").scaled(14,14));  // 讓草有空隙
        setZValue(1);  // 草會在最上面
        break;
    case 'w':
        walltype = Walltype::WATER;
        typeIndex = 3;
        setPixmap(QPixmap(":/images/Environment/Water.png").scaled(15,15));
        break;
    case 'i':
        walltype = Walltype::ICE;
        typeIndex = 4;
        setPixmap(QPixmap(":/images/Environment/Ice.png").scaled(15,15));
        break;
    case 'c':
        walltype = Walltype::FakeBrick;
        typeIndex = 5;
        setPixmap(QPixmap(":/images/Environment/Steel_Wall.png").scaled(15,15));
        break;
    }

    // 檢查牆有沒有被打到
    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &Wall::broken);
    timer->start(1);
}

void Wall::setState(bool ok)
{
    state = ok;
}

bool Wall::getState()
{
    return state;
}

int Wall::getTypeIndex()
{
    return typeIndex;
}

void Wall::broken() // 考慮 bullet v.s. wall
{
    QList<QGraphicsItem *> colliding_items = collidingItems();
    foreach (QGraphicsItem *item, colliding_items) {
        Bullet *bullet = dynamic_cast<Bullet*> (item);
        if (bullet) {
            emit breakWall(this, bullet);
        }
    }
}


