#ifndef BULLET_H
#define BULLET_H
#include "enemy.h"
#include <QObject>
#include <QGraphicsPixmapItem>
#include <QTimer>

class Bullet : public QObject, public QGraphicsPixmapItem
{
    Q_OBJECT
public:
    Bullet(int speed);
    void moveEnemyBullets();
    void enemy_move();
    void setCurrentDirection(int direction) {
        current_player_Direction = direction;
    }
    void setState(bool exist);
    bool getState();
    int bullet_speed;

    void setBulletSpeed(int speed)
    {
        bullet_speed= speed;
    }
    int getBulletSpeed()
    {
        return bullet_speed;
    }

    int getOriginalBulletSpeed() const
    {
        return originalBulletSpeed;
    }

    void moveHelper();

public slots:
    void move();

signals:
    void bulletHitsEnemy(Bullet *bullet, Enemy *Enemy);

private:
    QTimer *timer;
    Enemy *enemy;
    int current_player_Direction;
    int current_tank_Direction;
    bool state = true;
    int originalBulletSpeed;
};

#endif // BULLET_H
