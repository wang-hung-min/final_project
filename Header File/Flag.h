#ifndef FLAG_H
#define FLAG_H

#include "Bullet.h"

#include <QObject>
#include <QGraphicsPixmapItem>
#include <QGraphicsScene>

class Flag : public QObject , public QGraphicsPixmapItem
{
    Q_OBJECT
    Q_PROPERTY(bool state READ getState WRITE setState)
public:
    Flag();
    void setState(bool alive);
    bool getState();

public slots:
    void gameOver(); // 檢查有沒有被打到

signals:
    void flagIsDestroyed(Flag *flag, Bullet *bullet);
    // 自己的子彈打到也算

private:
    bool state;
    QTimer *timer;

};

#endif // FLAG_H
