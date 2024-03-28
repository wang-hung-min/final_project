#ifndef WALL_H
#define WALL_H
//#include "Bullet.h"
//#include "Player.h"
//#include "Enemy.h"

#include <QObject>
#include <QGraphicsPixmapItem>
#include <QGraphicsScene>
#include <QGraphicsItem>

class Wall : public QObject , public QGraphicsPixmapItem
{
    Q_OBJECT
    Q_PROPERTY(bool state READ getState WRITE setState)
public:
    Wall(char type);
    void setState(bool ok);
    bool getState();
    void freezeTank(); // 根據不同牆壁，坦克遇到會有不同反應

public slots:
//    void broken();

signals:
//    void breakWall(Wall *wall, Bullet *bullet);

private:
    enum Walltype{BRICK, STEEL, TREE, WATER, ICE};
    bool state;
    Walltype walltype;
    QTimer *timer;
};

#endif // WALL_H
