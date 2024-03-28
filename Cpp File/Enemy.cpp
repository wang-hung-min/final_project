#include "Enemy.h"
#include <QGraphicsScene>
#include <vector>
#include "Bullet.h"
#include "Wall.h"
#include "Player.h"
using namespace std;

Enemy::Enemy()
{
    moveSpeed = 20; // Default movement speed
    bulletSpeed=10;
    hits=0;
    vector<int> posVector = {0, 8, 16, 24, 32, 40, 48};
    setPos(posVector[(rand() % 6)] * 15.38 , 20);  // 每格長寬 16*16，全部生成在最上排
    curPos = pos();
    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &Enemy::move);
    timer->start(500); // move and fires every 500 ms
    moveDistance = 0; // 初始移動距離為0
}

void Enemy::move() {
    setTransformOriginPoint(boundingRect().center());
    curPos = pos();
    QPointF nextPos = curPos;
    if (moveDistance < 60 + rand()%400) {
        // 在改變方向後先前進一段距離（步伐數）
        if(currentDirection==Up){
            nextPos += QPointF(0, moveSpeed);
            // setPos(x(), y()+moveSpeed);
            current_tank_Direction=4;
            shootBullet();
            moveDistance += moveSpeed; // 更新移動的距離
        }
        if(currentDirection==Down){
            nextPos += QPointF(0, -moveSpeed);
            // setPos(x(), y()-moveSpeed);
            current_tank_Direction=3;
            shootBullet();
            moveDistance += moveSpeed; // 更新移動的距離
        }
        if(currentDirection==Left){
            nextPos += QPointF(-moveSpeed, 0);
            // setPos(x()-moveSpeed, y());
            current_tank_Direction=1;
            shootBullet();
            moveDistance += moveSpeed; // 更新移動的距離
        }
        if(currentDirection==Right){
            nextPos += QPointF(moveSpeed, 0);
            // setPos(x()+moveSpeed, y());
            current_tank_Direction=2;
            shootBullet();
            moveDistance += moveSpeed; // 更新移動的距離
        }
    } else {  // 步伐數到了之後換方向
        switch (rand()%4) {
        case 0:
            setRotation(-90);
            currentDirection=Right;
            break;
        case 1:
            setRotation(90);
            currentDirection=Left;
            break;
        case 2:
            setRotation(180);
            currentDirection=Down;
            break;
        case 3:
            setRotation(0);
            currentDirection=Up;
            break;}
        moveDistance = 0; // 重置移動距離
    }


    if ( (nextPos.y() <= 0 || nextPos.y() > 800 || nextPos.x() < 0 || nextPos.x() > 800) || checkEnemyWallCollision(nextPos)) {
        // 超出邊界時，或碰到牆了
        setPos(curPos); // 回到原來的位置
    } else{
        setPos(nextPos); // 成功移動
        curPos = nextPos;
    }
}

QPointF Enemy::getCurPos() const
{
    return curPos;
}

void Enemy::setCurPos(QPointF newCurPos)
{
    curPos = newCurPos;
}


void Enemy::shootBullet() {
    Bullet *bullet = new Bullet(bulletSpeed);
    // bullet->setPixmap(QPixmap(":/images/Bullet/bullet.png"));
    switch (currentDirection) {
    case Up://player->x() + player->pixmap().width() / 2 - bullet->pixmap().width() / 2, player->y());
        bullet->setPos(x() + boundingRect().width() / 2 - bullet->boundingRect().width() / 2, y() + boundingRect().height());
        //bullet->setRotation(0);
        break;
    case Down:
        bullet->setPos(x() + boundingRect().width() / 2 - bullet->boundingRect().width() / 2, y() - boundingRect().height());
        //bullet->setRotation(180);
        break;
    case Right:
        //bullet->setRotation(90);
        bullet->setPos(x() + boundingRect().width()+25, y() + boundingRect().width()/2  - bullet->boundingRect().height() /2);
        break;
    case Left:
        // bullet->setRotation(-90);
        bullet->setPos(x() - bullet->boundingRect().width()-25, y() + boundingRect().width()/2  - bullet->boundingRect().height()/2);
        break;
    default:
        break;
    }

    bullet->setCurrentDirection(current_tank_Direction);
    scene()->addItem(bullet);

    QTimer *bulletTimer = new QTimer();
    connect(bulletTimer, &QTimer::timeout, bullet, &Bullet::enemy_move);
    bulletTimer->start(30);
}

bool Enemy::checkEnemyWallCollision(QPointF nextPos){

    // 遍歷所有牆 or 玩家，看是否與 enemy 發生碰撞
    for (QGraphicsItem *item : scene()->items())  // 注意是檢查 this 所在的 scene 裡面的所有 item
    {
        Wall *wall = dynamic_cast<Wall *>(item);
        Player *player = dynamic_cast<Player *>(item);
        // Enemy *ally = dynamic_cast<Enemy *>(item);
        if (wall)
        {
            switch (wall->getTypeIndex())
            {
            case 0:  // brick
            case 1:  // steel
            case 3:  // water
            case 5:  // breakable steel
                // 如果以下情況，阻止 enemy 移動
                // 1. 若往左走小於牆的x
                // 2. 若往右走大於牆的x
                // 3. 若往上走小於牆的y
                // 4. 若往下走大於牆的y
                if (nextPos.x() < wall->pos().x() + wall->boundingRect().width() &&
                    nextPos.x() + this->pixmap().width() > wall->pos().x() &&
                    nextPos.y() < wall->pos().y() + wall->boundingRect().height() &&
                    nextPos.y() + this->pixmap().height() > wall->pos().y())
                {
                    qDebug() << "enemy v.s. wall";
                    return true;
                }
            }
        }
        else if (player){
            // 碰到 player 或 enemy 也不能移動
            if (nextPos.x() < player->pos().x() + player->boundingRect().width() &&
                nextPos.x() + this->pixmap().width() > player->pos().x() &&
                nextPos.y() < player->pos().y() + player->boundingRect().height() &&
                nextPos.y() + this->pixmap().height() > player->pos().y())
            {
                qDebug() << "enemy v.s. player";
                return true;
            }
        }
    }

    return false;
}
