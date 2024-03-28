#ifndef ENEMY_H
#define ENEMY_H
#include <QObject>
#include <QGraphicsPixmapItem>
#include <QTimer>

class Enemy : public QObject, public QGraphicsPixmapItem
{
    Q_OBJECT
public:
    Enemy();
    enum Direction { Up, Down, Left, Right };
    Direction currentDirection = Up;
    void shootBullet();
    void enemy_move();
    int current_tank_Direction=4;
    enum Tanktype{BASIC, FAST, POWER, ARMOR};
    Tanktype tank_type=BASIC;
    int moveSpeed;
    int bulletSpeed;
    int hits; // 被射擊的次數
    QPointF getCurPos() const;
    void setCurPos(QPointF newCurPos);
    bool checkEnemyWallCollision(QPointF nextPos);

    int getBulletSpeed()
    {
        return bulletSpeed;
    }
    void setBulletSpeed(int shoot)
    {
        bulletSpeed = shoot;
    }
    void setMoveSpeed(int speed)
    {
        moveSpeed = speed;
    }
    int getMoveSpeed()
    {
        return moveSpeed;
    }


public slots:
    void move();

private:
    QPointF curPos;
    QTimer *timer;
    Enemy *enemy;
    int moveDistance;

};

#endif // ENEMY_H
