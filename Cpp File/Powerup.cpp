#include "Powerup.h"
#include <QGraphicsScene>
#include <ctime>
#include <random>
Powerup::Powerup(QGraphicsItem *parent)
    : QGraphicsPixmapItem(parent) {
    std::srand(static_cast<unsigned>(std::time(nullptr)));

    //int xPos = std::rand() % 400;
    //int yPos = std::rand() % 300;
    int xPos = 150;
    int yPos = 400;
    setPos(xPos, yPos);
    type = rand() % 6;
    // type = 3;
    setPixmap(generatePixmap().scaled(50,50));
}
QPixmap Powerup::generatePixmap() {
    static const QMap<int, QString> powerupMap = {
        {0, ":/images/Power_up/Grenade.png"},
        {1, ":/images/Power_up/Helmat.png"},
        {2, ":/images/Power_up/Shovel.png"},
        {3, ":/images/Power_up/Star.png"},
        {4, ":/images/Power_up/Tank.png"},
        {5, ":/images/Power_up/Timer.png"}
    };

    if (!powerupMap.contains(type)) {
        qDebug() << "Unknown power-up type!";
        return QPixmap();
    }

    QString imageName = powerupMap.value(type);
    QPixmap pixmap(imageName);
    if (pixmap.isNull()) {
        qDebug() << "Failed to load image: " << imageName;
    }

    return pixmap;
}



