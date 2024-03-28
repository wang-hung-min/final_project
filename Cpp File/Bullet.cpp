#include "Bullet.h"
#include "Enemy.h"
#include <QGraphicsScene>
#include <QList>
#include <QGraphicsRectItem>
#include "player.h"
#include "player2.h"

Bullet::Bullet(int speed): bullet_speed(speed), originalBulletSpeed(speed)
{
    setPixmap(QPixmap(":/images/Bullet/bullet.png").scaled(12,28));
    moveHelper();

    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &Bullet::move);
    timer->start(50); // fires every 50ms
    bullet_speed=speed;
}

void Bullet::move() {
    // If the bullet collides with an enemy, destroy both
    QList<QGraphicsItem *> colliding_items = collidingItems();
    foreach (QGraphicsItem *item, colliding_items) {
        Enemy * enemy = dynamic_cast<Enemy*>(item);
        if (enemy) {
            emit bulletHitsEnemy(this, enemy);
            return; // important
        }
    }

    moveHelper();

    if (y() + boundingRect().height() < 0 ||y() + boundingRect().height() >800
        ||x() + boundingRect().height() < 0||x() + boundingRect().height() >800) {
        // Remove the bullet from the scene first
        scene()->removeItem(this);
        delete this;
        // qDebug() << "Bullet deleted...";
    }
}

void Bullet::moveHelper(){
    setTransformOriginPoint(boundingRect().center());
    if(current_player_Direction == 1){
        setRotation(-90);
        setPos(x()-bullet_speed, y());
    }
    else if(current_player_Direction == 2){
        setRotation(90);
        setPos(x()+bullet_speed, y());
    }
    else if(current_player_Direction == 3){
        setRotation(0);
        setPos(x(), y()-bullet_speed);
    }
    else if(current_player_Direction == 4){
        setRotation(180);
        setPos(x(), y()+bullet_speed);
    }
}


void Bullet::enemy_move() {
    // 檢查enemy的子弹有沒有撞到東西
    QList<QGraphicsItem *> colliding_items = collidingItems();
    foreach (QGraphicsItem *item, colliding_items) {
        Player * player = dynamic_cast<Player*>(item);
        if (player) {
            //emit bulletHitsEnemy(this, enemy);
            //scene()->removeItem(player);
            scene()->removeItem(this);
            //delete player;
            delete this;
            return; // important
        }
    }

    QList<QGraphicsItem *> colliding_enemy_items = collidingItems();
    foreach (QGraphicsItem *item, colliding_enemy_items) {
        Enemy * enemy = dynamic_cast<Enemy*>(item);
        if (enemy) {
            qDebug() << "Collided with item:" << item;
            scene()->removeItem(this);
            delete this;
            //emit bulletHitsEnemy(this, enemy);
            return;
        }
    }

    QList<QGraphicsItem *> colliding_player2 = collidingItems();
    foreach (QGraphicsItem *item, colliding_player2) {
        Player2 * player2 = dynamic_cast<Player2*>(item);
        if (player2) {
            scene()->removeItem(this);
            delete this;
            return;
        }
    }

}

void Bullet::setState(bool exist)
{
    state = exist;
}

bool Bullet::getState()
{
    return state;
}
