#ifndef POWERUP_H
#define POWERUP_H

#include <QGraphicsPixmapItem>

class Powerup : public QGraphicsPixmapItem {
public:
    Powerup(QGraphicsItem *parent = nullptr);
    int getType() const {
        return type;
    }
    int type ;
public slots:
    void collected();

private:
    QPixmap generatePixmap();

protected:


};

#endif // POWERUP_H
