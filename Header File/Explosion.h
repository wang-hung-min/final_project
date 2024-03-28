// Explosion.h
#ifndef EXPLOSION_H
#define EXPLOSION_H

#include <QGraphicsPixmapItem>
#include <QTimer>
#include <QObject>
class Explosion : public QObject,public QGraphicsPixmapItem
{
    Q_OBJECT
public:
    Explosion(QGraphicsItem *parent = nullptr);
    void updatePixmap(QTimer* timer);

public slots:
    void startExplosion1();  // 開始爆炸動畫
    void startExplosion2();
private slots:
    void updateFrame1();  // 更新爆炸動畫的當前幀
    void updateFrame2();

private:
    QTimer *explosionTimer1;
    QTimer *explosionTimer2;
    int currentFrame;  // 目前顯示的幀數
    int totalFrames;   // 總共的幀數
};

#endif // EXPLOSION_H
