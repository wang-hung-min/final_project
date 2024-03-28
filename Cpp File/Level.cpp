#include "Wall.h"
#include "Flag.h"
#include "Level.h"
#include <QDebug>
#include "Bullet.h"
#include "Enemy.h"
#include <QGraphicsRectItem>
#include <QKeyEvent>
#include <QGraphicsPixmapItem>
#include "Player.h"

Level::Level(int level, int number)
    : remains(20),
    tanks({{'f', 'f', 'f', 'f', 'a', 'a', 'a', 'a', 'a', 'a', 'a', 'a', 'b', 'b', 'b', 'b', 'p', 'p', 'p', 'p'},
           {'p', 'p', 'p', 'p', 'f', 'f', 'f', 'f', 'f', 'f', 'a', 'a', 'a', 'a', 'a', 'a', 'a', 'a', 'b', 'b'}})
{
    musicOutput = new QAudioOutput();
    musicOutput->setVolume(50);
    musicOutput1 = new QAudioOutput();
    musicOutput1->setVolume(100);
    musicOutput2 = new QAudioOutput();
    musicOutput2->setVolume(100);
    musicOutput3 = new QAudioOutput();
    musicOutput3->setVolume(100);
    musicOutput4 = new QAudioOutput();
    musicOutput4->setVolume(100);
    musicOutput5 = new QAudioOutput();
    musicOutput5->setVolume(100);
    musicOutput6 = new QAudioOutput();
    musicOutput6->setVolume(100);
    musicOutput7 = new QAudioOutput();
    musicOutput7->setVolume(100);
    musicOutput8 = new QAudioOutput();
    musicOutput8->setVolume(100);

    explosionSound = new QMediaPlayer();
    explosionSound->setAudioOutput(musicOutput1);
    explosionSound->setSource(QUrl("qrc:/sound/explosion.mp3"));
    startSound = new QMediaPlayer();
    startSound->setAudioOutput(musicOutput);
    startSound->setSource(QUrl("qrc:/sound/Start.mp3"));
    startSound->play();
    shootingSound = new QMediaPlayer();
    shootingSound->setAudioOutput(musicOutput2);
    shootingSound->setSource(QUrl("qrc:/sound/shoot.wav"));
    ArmorSound = new QMediaPlayer();
    ArmorSound->setAudioOutput(musicOutput3);
    ArmorSound->setSource(QUrl("qrc:/sound/armortank.mp3"));
    powerupSound = new QMediaPlayer();
    powerupSound->setAudioOutput(musicOutput4);
    powerupSound->setSource(QUrl("qrc:/sound/powerup.mp3"));
    powerupGenerateSound = new QMediaPlayer();
    powerupGenerateSound->setAudioOutput(musicOutput5);
    powerupGenerateSound->setSource(QUrl("qrc:/sound/powerup_generate.mp3"));
    moveSound = new QMediaPlayer();
    moveSound->setAudioOutput(musicOutput6);
    moveSound->setSource(QUrl("qrc:/sound/move.mp3"));
    stopSound = new QMediaPlayer();
    stopSound->setAudioOutput(musicOutput7);
    stopSound->setSource(QUrl("qrc:/sound/stop.mp3"));
    gameoverSound = new QMediaPlayer();
    gameoverSound->setAudioOutput(musicOutput8);
    gameoverSound->setSource(QUrl("qrc:/sound/GameOver.mp3"));

    star = 2;
    invincibleTimer = new QTimer(this);
    effectTimer = new QTimer(this);
    checkTimer = new QTimer(this);
    pauseScreen = nullptr;
    QTimer::singleShot(4000, this, [=]()
                       { moveSound->play(); });

    QTimer::singleShot(4000, this, [=]()
                       { stopSound->stop(); });

    number_1 = number;
    generateMap(level);
    setPlayer(number); // 決定幾個玩家並生成在初始位置
    showSurvivor(remains);
    setTankRemainsVector(tanks);

    // 定時生成enemy(call generateEnemy)
    spawnTimer = new QTimer(this);
    generateEnemy(level); // 先生成一個初始敵人
    connect(spawnTimer, &QTimer::timeout, this, [this, level]()
            { generateEnemy(level); });
    isTimerPaused = false;
    spawnTimer->start(8000); // 2000 改成 8000
    // Create the score
    score = new Score();
    addItem(score);
    // Create the health
    health = new Health();
    health->setPos(health->x(), health->y() + 25);
    addItem(health);

    keyRespondTimer1 = new QTimer(this);
    connect(keyRespondTimer1, &QTimer::timeout, this, &Level::slotTimeOut1);

    connect(effectTimer, &QTimer::timeout, this, &Level::togglePlayerOpacity);
    connect(invincibleTimer, &QTimer::timeout, this, &Level::setInvincibe);
    connect(invincibleTimer, &QTimer::timeout, this, [=](){
        effectTimer->stop();
    });
    checkTimer->start(50);
    connect(checkTimer, &QTimer::timeout, this, [=](){
        if (!invincible)
            checkBulletPlayerCollision();
    });
    shootTimer = new QTimer(this);
    connect(shootTimer, &QTimer::timeout, this, &Level::resetShootStatus);

    bestScore = settings.value("bestScore", 0).toInt();
    addItem(&powerup);
}

void Level::setPlayer(int number)
{
    if (number == 1)
    {
        player = new Player(1);
        player->setPos(246, 738);
        player->setCurPos(player->pos());
        addItem(player);
        // qDebug() << "addItem(player);";

        invincible = true;
        // qDebug() << "invincible=true;";
        player->setOpacity(0.5);
        // qDebug() << "player->setOpacity(0.5);";
        effectTimer->start(500);
        if (invincible)
            invincibleTimer->start(3000);
        player->setOpacity(1);
    }
    else
    {
        player = new Player(1);
        player->setPos(246, 738);
        player->setCurPos(player->pos());
        addItem(player);
        player2 = new Player(2);
        player2->setPos(492, 738); // 根據您的需求設定初始位置
        player->setCurPos(player2->pos());
        addItem(player2);

        invincible = true;
        qDebug() << "invincible=true;";
        player->setOpacity(0.5);
        qDebug() << "player->setOpacity(0.5);";
        effectTimer->start(500);
        if (invincible)
            invincibleTimer->start(3000);
    }
}

void Level::increaseScore(Bullet *bullet, Enemy *enemy)
{

    removeItem(bullet);
    delete bullet;

    int enemyScore = 0;

    if (tankType == 'a')
    { // 如果是 Armor Tank
        enemy->hits++;
        qDebug() << "Armor Tank hits: " << enemy->hits;

        // 檢查是否被射擊了四次
        if (enemy->hits >= 4)
        {
            explosion = new Explosion();
            explosion->setPos(enemy->pos());

            explosionSound->play();
            addItem(explosion);
            explosion->startExplosion1();

            removeItem(enemy);
            delete enemy;
            enemyScore = 400; // 根據 Armor Tank 的分數設定
            removedEnemyCount++;
        }
        ArmorSound->play();
    }
    else
    {
        // 如果是其他坦克類型，直接刪除
        explosion = new Explosion();
        explosion->setPos(enemy->pos());
        explosionSound->play();
        qDebug() << " explosion";
        addItem(explosion);
        explosion->startExplosion1();
        removeItem(enemy);
        delete enemy;
        removedEnemyCount++;
        // 根據不同的敵人類型設定分數
        switch (tankType)
        {
        case 'b':
            enemyScore = 100;
            break;
        case 'f':
            enemyScore = 200;
            break;
        case 'p':
            enemyScore = 300;
            break;
        default:
            qDebug() << "Unknown enemy type!";
            break;
        }
    }
    if (removedEnemyCount == 1 || 4 || 11 | 18)
    {
        powerup.setZValue(1000);
        powerupGenerateSound->play();
        addItem(&powerup);
    }

    score->increase(enemyScore);
    // 檢查是否破紀錄
    if (score->getScore() > bestScore)
    {
        bestScore = score->getScore();
        settings.setValue("bestScore", bestScore);
        qDebug() << "   bestScore = score->getScore()";
    }

    // 更新遊戲場景中的分數顯示
    // qDebug() <<" settings.setValue bestScore)";
    // if (scene) {
    //     scene->setBestScore(bestScore);
    //  } else {
    //       qDebug() << "Error: Invalid scene pointer!";
    //   }
}
void Level::hitOnWall(Wall *wall, Bullet *bullet)
{
    // qDebug() << wall->getTypeIndex();
    if (bullet->opacity() == 0)
    {
        switch (wall->getTypeIndex())
        {
        case 0:
            qDebug() << "invisible 0" << wall->getTypeIndex();
            removeItem(bullet);
            delete bullet;
            removeItem(wall);
            delete wall;
            break;
        default:
            qDebug() << "invisible else" << wall->getTypeIndex();
            removeItem(bullet);
            delete bullet;
            break;
        }
    }
    else
    {
        switch (wall->getTypeIndex())
        {
        case 0:
            qDebug() << "0" << wall->getTypeIndex();
            removeItem(bullet);
            delete bullet;
            removeItem(wall);
            delete wall;
            break;
        case 1:
            qDebug() << "1" << wall->getTypeIndex();
            removeItem(bullet);
            delete bullet;
            break;
        case 5:
            qDebug() << "5" << wall->getTypeIndex();
            removeItem(bullet);
            delete bullet;
            removeItem(wall);
            delete wall;
            break;
        default:
            break;
        }
    }
}

void Level::keyReleaseEvent(QKeyEvent *event)
{
    if (!event->isAutoRepeat())
    {
        keys.removeAll(event->key());
    }
    if (keys.isEmpty())
    {
        keyRespondTimer1->stop();
    }
    if (isPaused)
    {
        moveSound->stop();
        stopSound->stop();
    }
    else
    {
        moveSound->stop();
        stopSound->play();
    }
}

void Level::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Escape)
    {
        togglePause();
        moveSound->stop();
        stopSound->stop();
    }
    else
    {
        moveSound->play();
        stopSound->stop();
    }
    if (event->key() == Qt::Key_Space && !BulletFired && !isPaused)
    {
        Bullet *bullet = new Bullet(10);
        if (star >= 1)
            bullet->setBulletSpeed(30);

        BulletFired = true; // 空白建間格
        shootTimer->start(800);
        shootingSound->play();
        connect(bullet, &Bullet::bulletHitsEnemy, this, &Level::increaseScore);
        // bullet->setPos(player->pos());
        switch (player->current_player_Direction)
        {
        case 3: // up
            bullet->setPos(player->x() + player->pixmap().width() / 2 - bullet->pixmap().width() / 2, player->y() - 24);
            break;
        case 4: // down
            bullet->setPos(player->x() + player->pixmap().width() / 2 - bullet->pixmap().width() / 2, player->y() + player->pixmap().height() - bullet->pixmap().height() + 24);
            break;
        case 1: // left
            bullet->setPos(player->x() - 15, player->y() + player->pixmap().height() / 2 - bullet->pixmap().height() / 2);
            break;
        case 2: // right
            bullet->setPos(player->x() + player->pixmap().width() - bullet->pixmap().width() + 15, player->y() + player->pixmap().height() / 2 - bullet->pixmap().height() / 2);
            break;
        default:
            break;
        }
        if (star >= 3)
        {
            Bullet *vbullet = new Bullet(30);
            vbullet->setOpacity(0);
            vbullet->setPos(bullet->x() + (player->current_player_Direction == 2 ? 1 : (player->current_player_Direction == 1 ? -1 : 0)),
                            bullet->y() + (player->current_player_Direction == 4 ? 1 : (player->current_player_Direction == 3 ? -1 : 0)));
            vbullet->setCurrentDirection(player->current_player_Direction);
            addItem(vbullet);
        }
        bullet->setCurrentDirection(player->current_player_Direction);
        addItem(bullet);
        if (star >= 2)
        {
            Bullet *secondBullet = new Bullet(30);
            // Position the second bullet in the same direction as the first bullet
            secondBullet->setPos(bullet->x() + (player->current_player_Direction == 2 ? 10 : (player->current_player_Direction == 1 ? -10 : 0)),
                                 bullet->y() + (player->current_player_Direction == 4 ? 10 : (player->current_player_Direction == 3 ? -10 : 0)));

            secondBullet->setCurrentDirection(player->current_player_Direction);
            addItem(secondBullet);
            if (star >= 3)
            {
                Bullet *vbullet2 = new Bullet(30);
                vbullet2->setOpacity(0);
                vbullet2->setPos(secondBullet->x() + (player->current_player_Direction == 2 ? 1 : (player->current_player_Direction == 1 ? -1 : 0)),
                                 secondBullet->y() + (player->current_player_Direction == 4 ? 1 : (player->current_player_Direction == 3 ? -1 : 0)));
                vbullet2->setCurrentDirection(player->current_player_Direction);
                addItem(vbullet2);
            }
        }
    }
    if (number_1 == 2)
    {
        if (event->key() == Qt::Key_Q)
        {
            Bullet *bullet = new Bullet(10);
            connect(bullet, &Bullet::bulletHitsEnemy, this, &Level::increaseScore);
            // bullet->setPos(player->pos());

            switch (player2->current_player_Direction)
            {
            case 3:
                bullet->setPos(player2->x() + player2->pixmap().width() / 2 - bullet->pixmap().width() / 2, player2->y());
                break;
            case 4:
                bullet->setPos(player2->x() + player2->pixmap().width() / 2 - bullet->pixmap().width() / 2, player2->y() + player2->pixmap().height() - bullet->pixmap().height());
                break;
            case 1:
                bullet->setPos(player2->x(), player2->y() + player2->pixmap().height() / 2 - bullet->pixmap().height() / 2);
                break;
            case 2:
                bullet->setPos(player2->x() + player2->pixmap().width() - bullet->pixmap().width(), player2->y() + player2->pixmap().height() / 2 - bullet->pixmap().height() / 2);
                break;
            default:
                break;
            }
            bullet->setCurrentDirection(player2->current_player_Direction);
            addItem(bullet);
        }
    }

    if (!event->isAutoRepeat())
    {
        keys.append(event->key());
    }
    if (!keyRespondTimer1->isActive())
    {
        keyRespondTimer1->start(4);
    }
}

void Level::slotTimeOut1()
{
    if (!isPaused)
    {
        // player->keyPressEvent1(keys); // 這裡處理player1的按鍵事件
        foreach (int key, keys)
        {
            QPointF pos = player->pos(); // Assuming 'player' is the current instance
            player->setTransformOriginPoint(player->boundingRect().center());
            QPointF nextpos = pos;

            if (key == Qt::Key_Left)
            {
                player->current_player_Direction = 1;
                player->setRotation(-90); // 左邊
                // nextpos = pos + QPointF(-15, 0);
                nextpos = pos + QPointF(-2.5, 0);
            }
            else if (key == Qt::Key_Right)
            {
                player->current_player_Direction = 2; // 改變方向為右邊
                player->setRotation(90);              // 右邊
                // nextpos = pos + QPointF(15, 0);
                nextpos = pos + QPointF(2.5, 0);
            }
            else if (key == Qt::Key_Up)
            {
                player->current_player_Direction = 3;
                player->setRotation(0); // 上面
                // nextpos = pos + QPointF(0, -15);
                nextpos = pos + QPointF(0, -2.5);
            }
            else if (key == Qt::Key_Down)
            {
                player->current_player_Direction = 4;
                player->setRotation(180); // 下面
                // nextpos = pos + QPointF(0, 15);
                nextpos = pos + QPointF(0, 2.5);
            }

            player->setCurPos(nextpos);
            checkPlayerWallCollision(player, player->current_player_Direction);
            if ( nextpos.y() <= 0 || nextpos.y() > 760 || nextpos.x() < 0 || nextpos.x() > 750 ){
                player->setPos(pos); // 超出邊界時回到原來的位置
            } else{
                player->setPos(player->getCurPos()); // 成功移動
            }


            if (number_1 == 2)
            {                                 // 這裡處理 player2
                QPointF pos = player2->pos(); // Assuming 'player' is the current instance
                player2->setTransformOriginPoint(player2->boundingRect().center());
                QPointF nextpos = pos;

                if (key == Qt::Key_A)
                {
                    player2->current_player_Direction = 1;
                    player2->setRotation(-90); // 左邊
                    // nextpos = pos + QPointF(-15, 0);
                    nextpos = pos + QPointF(-2.5, 0);
                }
                else if (key == Qt::Key_D)
                {
                    player2->current_player_Direction = 2; // 改變方向為右邊
                    player2->setRotation(90);              // 右邊
                    // nextpos = pos + QPointF(15, 0);
                    nextpos = pos + QPointF(2.5, 0);
                }
                else if (key == Qt::Key_W)
                {
                    player2->current_player_Direction = 3;
                    player2->setRotation(0); // 上面
                    // nextpos = pos + QPointF(0, -15);
                    nextpos = pos + QPointF(0, -2.5);
                }
                else if (key == Qt::Key_S)
                {
                    player2->current_player_Direction = 4;
                    player2->setRotation(180); // 下面
                    // nextpos = pos + QPointF(0, 15);
                    nextpos = pos + QPointF(0, 2.5);
                }

                player2->setCurPos(nextpos);
                checkPlayerWallCollision(player2, player2->current_player_Direction);
                if ( nextpos.y() <= 0 || nextpos.y() > 760 || nextpos.x() < 0 || nextpos.x() > 750 ){
                    player2->setPos(pos); // 超出邊界時回到原來的位置
                } else{
                    player2->setPos(player2->getCurPos()); // 成功移動
                }
            }
            checkPowerupCollision();
        }
    }
}
void Level::generateMap(int level)
{
    // map 19
    vector<string> data;
    if (level == 1)
    {
        data = {
                "****bbbb****bbbb****bbbb****bbbb****bbbb****bbbb****",
                "****bbbb****bbbb****bbbb****bbbb****bbbb****bbbb****",
                "****bbbb****bbbb****bbbb****bbbb****bbbb****bbbb****",
                "****bbbb****bbbb****bbbb****bbbb****bbbb****bbbb****",
                "****bbbb****bbbb****bbbb****bbbb****bbbb****bbbb****",
                "****bbbb****bbbb****bbbb****bbbb****bbbb****bbbb****",
                "****bbbb****bbbb****bbbb****bbbb****bbbb****bbbb****",
                "****bbbb****bbbb****bbbb****bbbb****bbbb****bbbb****",
                "****bbbb****bbbb****bbbb****bbbb****bbbb****bbbb****",
                "****ssss****ssss****ssss****ssss****ssss****ssss****",
                "****ssss****ssss****ssss****ssss****ssss****ssss****",
                "****************************************************",
                "****************************************************",
                "****************bbbb************bbbb****************",
                "****************bbbb************bbbb****************",
                "bbbb****bbbb****bbbb************bbbb****bbbb****bbbb",
                "bbbb****bbbb****bbbb************bbbb****bbbb****bbbb",
                "bbbb****bbbbbbbbbbbb****bbbb****bbbbbbbbbbbb****bbbb",
                "bbbb****bbbbbbbbbbbb****bbbb****bbbbbbbbbbbb****bbbb",
                "bbbb****bbbb****bbbb****bbbb****bbbb****bbbb****bbbb",
                "bbbb****bbbb****bbbb****bbbb****bbbb****bbbb****bbbb",
                "ssss****ssss****ssss****ssss****ssss****ssss****ssss",
                "ssss****ssss****ssss****ssss****ssss****ssss****ssss",
                "****************ssss************ssss****************",
                "****************ssss************ssss****************",
                "tttttttt********bbbb****tttt****bbbb********tttttttt",
                "tttttttt********bbbb****tttt****bbbb********tttttttt",
                "tttttttt********bbbb****tttt****bbbb********tttttttt",
                "tttttttt********bbbb****tttt****bbbb********tttttttt",
                "ttttttttttttttttbbbbbbbbttttbbbbbbbbtttttttttttttttt",
                "ttttttttttttttttbbbbbbbbttttbbbbbbbbtttttttttttttttt",
                "ttttttttttttttttbbbb****tttt****bbbbtttttttttttttttt",
                "ttttttttttttttttbbbb****tttt****bbbbtttttttttttttttt",
                "tttttttttttttttttttttttttttttttttttttttttttttttttttt",
                "tttttttttttttttttttttttttttttttttttttttttttttttttttt",
                "tttttttttttttttttttttttttttttttttttttttttttttttttttt",
                "tttttttttttttttttttttttttttttttttttttttttttttttttttt",
                "****************bbbbttttttttttttbbbb****************",
                "****************bbbbttttttttttttbbbb****************",
                "bbbb****bbbb****bbbbttttttttttttbbbb****bbbb****bbbb",
                "bbbb****bbbb****bbbbttttttttttttbbbb****bbbb****bbbb",
                "****bbbb****bbbb********tttt********bbbb****bbbb****",
                "****bbbb****bbbb********tttt********bbbb****bbbb****",
                "****bbbb****bbbb********tttt********bbbb****bbbb****",
                "****bbbb****bbbb********************bbbb****bbbb****",
                "****bbbb****bbbb********************bbbb****bbbb****",
                "****bbbb****bbbb****bbbbbbbbbbbb****bbbb****bbbb****",
                "****bbbb****bbbb****bbbbbbbbbbbb****bbbb****bbbb****",
                "****bbbb****bbbb****bbbbf***bbbb****bbbb****bbbb****",
                "****bbbb****bbbb****bbbb****bbbb****bbbb****bbbb****",
                "********************bbbb****bbbb********************",
                "********************bbbb****bbbb********************"};
    }
    else
    {
        // map 28
        data = {
                "****************************************************",
                "****************************************************",
                "****************************************************",
                "****************************************************",
                "****************bbbb****bbbb************************",
                "****************bbbb****bbbb************************",
                "****************bbbb****bbbb************************",
                "****************bbbb****bbbb************************",
                "tttt********ttttbbbbttttbbbbtttt********tttt********",
                "tttt********ttttbbbbttttbbbbtttt********tttt********",
                "tttt********ttttbbbbttttbbbbtttt********tttt********",
                "tttt********ttttbbbbttttbbbbtttt********tttt********",
                "bbbbttttttttbbbbbbbbiiiibbbbbbbbttttttttbbbbtttt****",
                "bbbbttttttttbbbbbbbbiiiibbbbbbbbttttttttbbbbtttt****",
                "bbbbttttttttbbbbbbbbiiiibbbbbbbbttttttttbbbbtttt****",
                "bbbbttttttttbbbbbbbbiiiibbbbbbbbttttttttbbbbtttt****",
                "bbbbbbbbbbbbbbbbssssiiiissssbbbbbbbbbbbbbbbbtttt****",
                "bbbbbbbbbbbbbbbbssssiiiissssbbbbbbbbbbbbbbbbtttt****",
                "bbbbbbbbbbbbbbbbssssiiiissssbbbbbbbbbbbbbbbbtttt****",
                "bbbbbbbbbbbbbbbbssssiiiissssbbbbbbbbbbbbbbbbtttt****",
                "wwwwwwwwwwwwbbbbbbbbiiiibbbbbbbbwwwwwwwwwwwwtttt****",
                "wwwwwwwwwwwwbbbbbbbbiiiibbbbbbbbwwwwwwwwwwwwtttt****",
                "wwwwwwwwwwwwbbbbbbbbiiiibbbbbbbbwwwwwwwwwwwwtttt****",
                "wwwwwwwwwwwwbbbbbbbbiiiibbbbbbbbwwwwwwwwwwwwtttt****",
                "wwwwbbbbbbbbbbbbbbbbiiiibbbbbbbbbbbbbbbbwwwwwwwwtttt",
                "wwwwbbbbbbbbbbbbbbbbiiiibbbbbbbbbbbbbbbbwwwwwwwwtttt",
                "wwwwbbbbbbbbbbbbbbbbiiiibbbbbbbbbbbbbbbbwwwwwwwwtttt",
                "wwwwbbbbbbbbbbbbbbbbiiiibbbbbbbbbbbbbbbbwwwwwwwwtttt",
                "bbbbbbbbbbbbwwwwiiiiiiiiiiiiwwwwbbbbbbbbbbbbtttttttt",
                "bbbbbbbbbbbbwwwwiiiiiiiiiiiiwwwwbbbbbbbbbbbbtttttttt",
                "bbbbbbbbbbbbwwwwiiiiiiiiiiiiwwwwbbbbbbbbbbbbtttttttt",
                "bbbbbbbbbbbbwwwwiiiiiiiiiiiiwwwwbbbbbbbbbbbbtttttttt",
                "bbbbbbbbwwwwwwwwwwwwiiiiwwwwwwwwwwwwbbbbbbbbwwwwwwww",
                "bbbbbbbbwwwwwwwwwwwwiiiiwwwwwwwwwwwwbbbbbbbbwwwwwwww",
                "bbbbbbbbwwwwwwwwwwwwiiiiwwwwwwwwwwwwbbbbbbbbwwwwwwww",
                "bbbbbbbbwwwwwwwwwwwwiiiiwwwwwwwwwwwwbbbbbbbbwwwwwwww",
                "ttttwwwwwwwwttttttttttttttttttttwwwwwwwwttttwwwwtttt",
                "ttttwwwwwwwwttttttttttttttttttttwwwwwwwwttttwwwwtttt",
                "ttttwwwwwwwwttttttttttttttttttttwwwwwwwwttttwwwwtttt",
                "ttttwwwwwwwwttttttttttttttttttttwwwwwwwwttttwwwwtttt",
                "****tttttttt********************tttttttt****tttt****",
                "****tttttttt********************tttttttt****tttt****",
                "****tttttttt********************tttttttt****tttt****",
                "****tttttttt********************tttttttt****tttt****",
                "****************************************************",
                "****************************************************",
                "********************bbbbbbbbbbbb********************",
                "********************bbbbbbbbbbbb********************",
                "********************bbbbf***bbbb********************",
                "********************bbbb****bbbb********************",
                "********************bbbb****bbbb********************",
                "********************bbbb****bbbb********************"};
    }

    //存成二維陣列
    for (const string& row : data) {
        vector<char> rowVector(row.begin(), row.end());
        matrix.push_back(rowVector);
    }

    float i = 0;
    float j = 0;

    // setPos
    for (const auto &rowVector : matrix)
    {
        i = 0;
        for (char element : rowVector)
        {
            if (element != '*')
            {
                if (element == 'f')
                {
                    Flag *flag = new Flag();
                    connect(flag, &Flag::flagIsDestroyed, [=](){
                        showGameover();
                    });
                    addItem(flag);
                    flag->setPos(370, 740);
                }
                else
                {
                    Wall *wall = new Wall(element);
                    addItem(wall);
                    wall->setPos(i, j);
                    connect(wall, &Wall::breakWall, this, &Level::hitOnWall);
                }
            }
            i += 800 / 52; // 這裡還要再根據圖片大小做調整
        }
        j += 800 / 52; // 這裡還要再根據圖片大小做調整
    }
}

void Level::setTankRemainsVector(const vector<vector<char>> &tankRemains)
{
    tanks = tankRemains;
}

const vector<vector<char>> &Level::getTankRemainsVector() const
{
    return tanks;
}

void Level::generateEnemy(int level)
{

    // 用connect,當timeout就生成一隻tank
    getTankRemainsVector();

    // 並將其從vector裡刪除
    if (!tanks[level - 1].empty() && !isPaused)
    {

        tankType = tanks[level - 1].front();
        Enemy *enemy = new Enemy();

        switch (tankType)
        {
        case 'f':
            enemy->setPixmap(QPixmap(":/images/Enemies/Fast_Tank.png").scaled(40, 40));
            enemy->moveSpeed = 30;
            enemy->bulletSpeed = 20;
            break;
        case 'a':
            enemy->setPixmap(QPixmap(":/images/Enemies/Armor_Tank.png").scaled(40, 40));
            enemy->moveSpeed = 20;
            enemy->bulletSpeed = 20;
            // tankType='a';
            break;
        case 'b':
            enemy->setPixmap(QPixmap(":/images/Enemies/Basic_Tank.png").scaled(40, 40));
            enemy->moveSpeed = 10;
            enemy->bulletSpeed = 10;
            break;
        case 'p':
            enemy->setPixmap(QPixmap(":/images/Enemies/Power_Tank.png").scaled(40, 40));
            enemy->moveSpeed = 30;
            enemy->bulletSpeed = 30;
            break;
        default:
            break;
        }
        addItem(enemy);
        enemyList.append(enemy);
        qDebug() << "delete picture...";
        tanks[level - 1].erase(tanks[level - 1].begin());
        setTankRemainsVector(tanks);
    }
    else
    {
        qDebug() << "finish deleting";
        spawnTimer->stop();
    }

    if (isEnemyMovementFrozen)
    {
        setInitialEnemySpeed();
    }
}
void Level::checkPlayerWallCollision(Player *player, int playerDir)
{
    QPointF pos = player->pos();
    QPointF nextpos = player->getCurPos(); // 玩家下一步的位置

    // 遍歷所有牆，看是否與玩家發生碰撞
    for (QGraphicsItem *item : items())
    {
        Wall *wall = dynamic_cast<Wall *>(item);
        Enemy *enemy = dynamic_cast<Enemy *>(item);
//        Player *ally = dynamic_cast<Player *>(item);
        if (wall)
        {
            switch (wall->getTypeIndex())
            {
            case 4:  // ice：從 nextPos 再往前滑
                if (nextpos.x() < wall->pos().x() + wall->boundingRect().width() &&
                    nextpos.x() + player->pixmap().width() > wall->pos().x() &&
                    nextpos.y() < wall->pos().y() + wall->boundingRect().height() &&
                    nextpos.y() + player->pixmap().height() > wall->pos().y()){
                    if (playerDir == 1)
                        player->setCurPos(nextpos + QPointF(-2, 0));
                    if (playerDir == 2)
                        player->setCurPos(nextpos + QPointF(2, 0));
                    if (playerDir == 3)
                        player->setCurPos(nextpos + QPointF(0, -2));
                    if (playerDir == 4)
                        player->setCurPos(nextpos + QPointF(0, 2));
                }
                break;

            case 0:  // brick
            case 1:  // steel
            case 3:  // water
            case 5:  // breakable steel
                // 如果以下情況，阻止玩家移動
                // 1. 若往左走小於牆的x
                // 2. 若往右走大於牆的x
                // 3. 若往上走小於牆的y
                // 4. 若往下走大於牆的y
                if (nextpos.x() < wall->pos().x() + wall->boundingRect().width() &&
                    nextpos.x() + player->pixmap().width() > wall->pos().x() &&
                    nextpos.y() < wall->pos().y() + wall->boundingRect().height() &&
                    nextpos.y() + player->pixmap().height() > wall->pos().y())
                {
                    player->setCurPos(pos); // 玩家位置還原當前位置
                    return;
                }
            }
        }
        if (enemy){  // 不能穿過敵人
            if (nextpos.x() < enemy->pos().x() + enemy->boundingRect().width() &&
                nextpos.x() + player->pixmap().width() > enemy->pos().x() &&
                nextpos.y() < enemy->pos().y() + enemy->boundingRect().height() &&
                nextpos.y() + player->pixmap().height() > enemy->pos().y())
            {
                player->setCurPos(pos); // 玩家位置還原當前位置
                return;
            }
        }
    }
}

// 還要寫個刪除所有圖片的
void Level::showSurvivor(int remains)
{
    int x = 740;
    int y = 60;
    // 生成初始ramaining survivors，共20台，10*2
    for (int i = 1; i <= remains / 2; i++)
    {
        for (int j = 1; j <= 2; j++)
        {
            survivors = new QGraphicsPixmapItem(QPixmap(":/images/Enemies/Remains.png").scaled(40,40));
            survivors->setPos(x, y);
            addItem(survivors);
            x += 10;
            y += 40;
        }
        x -= 20;
    }
    // 處理最後一排(mod為奇數時進入)
    for (int k = 1; k <= remains % 2; k++)
    {
        y += 40;
        survivors = new QGraphicsPixmapItem(QPixmap(":/images/Enemies/Remains.png").scaled(40,40));
        survivors->setPos(x, y);
        addItem(survivors);
    }
}
void Level::checkPowerupCollision()
{
    QList<QGraphicsItem *> collidingItems = player->collidingItems();

    for (QGraphicsItem *item : collidingItems)
    {
        if (item == &powerup)
        {
            handlePowerupCollision();
            break;
        }
    }
}

void Level::handlePowerupCollision()
{
    switch (powerup.type)
    {
    case 0:
        handleGrenadeEffect();
        break;
    case 1:
        handleHelmatEffect();
        break;
    case 2:
        handleShovelEffect();
        break;
    case 3:
        handleStarEffect();
        break;
    case 4:
        handleTankEffect();
        break;
    case 5:
        handleTimerEffect();
        break;
    default:
        qDebug() << "Unknown power-up type!";
        break;
    }
    powerupSound->play();
    removeItem(&powerup);
}
void Level::togglePause()
{
    isPaused = !isPaused;

    if (isPaused)
    {
        this->pauseScreen = new QGraphicsPixmapItem(QPixmap(":/images/Homepage/pause.png"));
        int x = (width() - this->pauseScreen->pixmap().width()) / 2;
        int y = (height() - this->pauseScreen->pixmap().height()) / 2;
        this->pauseScreen->setPos(x, y);

        this->pauseScreen->setOpacity(0.5);
        addItem(this->pauseScreen);
        setInitialEnemySpeed();
        pauseAllBullets();
        setOpacityForScene(0.5); // 調整場景透明度
    }
    else
    {
        removeItem(this->pauseScreen);
        resumeAllBullets();
        resumeEnemyMovement();
        setOpacityForScene(1.0); // 恢復場景透明度
    }
}
void Level::pauseAllBullets()
{
    QList<QGraphicsItem *> allItems = items();
    for (QGraphicsItem *item : allItems)
    {
        Bullet *bullet = dynamic_cast<Bullet *>(item);
        if (bullet)
        {
            bullet->setBulletSpeed(0);
        }
    }
}
void Level::resumeAllBullets()
{
    QList<QGraphicsItem *> allItems = items();
    for (QGraphicsItem *item : allItems)
    {
        Bullet *bullet = dynamic_cast<Bullet *>(item);
        if (bullet)
        {
            // 根據你的需求，這裡可能需要將速度設置為原來的值
            bullet->setBulletSpeed(bullet->getOriginalBulletSpeed());
        }
    }
}
void Level::setOpacityForScene(qreal opacity)
{
    QList<QGraphicsItem *> allItems = items();
    for (QGraphicsItem *item : allItems)
    {
        item->setOpacity(opacity);
    }
}
void Level::resetShootStatus()
{
    BulletFired = false;
    shootTimer->stop();
}
void Level::handleStarEffect()
{
    qDebug() << " handleStarEffect()";
    star++;
    if (star >= 3)
    {
        QList<QGraphicsItem *> allItems = items();
        for (QGraphicsItem *item : allItems)
        {
            Wall *wall = dynamic_cast<Wall *>(item);
            if (wall && wall->getState() && wall->getTypeIndex() == 1)
            {
                // Store the current position
                QPointF wallPos = wall->pos();

                // Remove the existing wall
                removeItem(wall);

                // Create a new wall with the same position
                Wall *newWall = new Wall('c');

                qDebug() << newWall->getTypeIndex();
                // Set the position of the new wall
                newWall->setPos(wallPos);

                // Add the new wall to the scene
                addItem(newWall);
                connect(newWall, &Wall::breakWall, this, &Level::hitOnWall);
            }
        }
    }
}
void Level::handleTimerEffect()
{
    qDebug() << " handleTimerEffect()";
    pauseTimer();
    setInitialEnemySpeed();                                      // Freeze enemy movement
    QTimer::singleShot(5000, this, &Level::resumeEnemyMovement); // Resume enemy movement after 5 seconds
}
void Level::resumeEnemyMovement()
{
    isEnemyMovementFrozen = false;

    int i = 0;
    for (Enemy *enemy : enemyList)
    {
        enemy->setMoveSpeed(originalEnemySpeeds[i]);
        enemy->setBulletSpeed(originalEnemyShoot[i]);
        ++i;
    }
}
void Level::pauseTimer()
{
    isTimerPaused = true;
    spawnTimer->stop();
}

void Level::resumeTimer()
{
    isTimerPaused = false;
    spawnTimer->start();
}
void Level::setInitialEnemySpeed()
{
    isEnemyMovementFrozen = true;

    originalEnemySpeeds.clear();
    for (Enemy *enemy : enemyList)
    {

        originalEnemyShoot.append(enemy->getMoveSpeed());
        originalEnemySpeeds.append(enemy->getMoveSpeed());
        enemy->setMoveSpeed(0);
        enemy->setBulletSpeed(0);
    }
}
void Level::handleGrenadeEffect()
{
    qDebug() << "All enemies removed!";

    QList<QGraphicsItem *> allItems = items();

    for (QGraphicsItem *item : allItems)
    {
        if (dynamic_cast<Enemy *>(item))
        {
            explosion = new Explosion();
            explosion->setPos(item->pos());

            explosionSound->play();
            addItem(explosion);
            explosion->startExplosion1();
            removeItem(item);
            delete item;
        }
    }
}
void Level::handleShovelEffect()
{
    QVector<QPair<int, int>> positionsToModify = {
                                                  {48, 20},
                                                  {49, 20},
                                                  {50, 20},
                                                  {51, 20},
                                                  {48, 21},
                                                  {49, 21},
                                                  {50, 21},
                                                  {51, 21},
                                                  {48, 22},
                                                  {49, 22},
                                                  {50, 22},
                                                  {51, 22},
                                                  {48, 23},
                                                  {49, 23},
                                                  {50, 23},
                                                  {51, 23},
                                                  {48, 28},
                                                  {49, 28},
                                                  {50, 28},
                                                  {51, 28},
                                                  {48, 29},
                                                  {49, 29},
                                                  {50, 29},
                                                  {51, 29},
                                                  {48, 30},
                                                  {49, 30},
                                                  {50, 30},
                                                  {51, 30},
                                                  {48, 31},
                                                  {49, 31},
                                                  {50, 31},
                                                  {51, 31},
                                                  };
    for (int row = 46; row <= 47; ++row)
    {
        for (int col = 20; col <= 31; ++col)
        {
            positionsToModify.append({row, col});
        }
    }
    for (auto position : positionsToModify)
    {
        int row = position.first;
        int col = position.second;
        qDebug() << "positionsToModify );" << row;
        qDebug() << "positionsToModify );" << col;
        if (row >= 0 && row < matrix.size() && col >= 0 && col < matrix[row].size())
        {
            QList<QGraphicsItem *> itemList = items(QRectF(col * 15, row * 15, 15, 15), Qt::IntersectsItemBoundingRect);
            for (QGraphicsItem *item : itemList)
            {

                if (Wall *wall = dynamic_cast<Wall *>(item))
                {
                    removeItem(wall);
                    delete wall;
                }
            }
        }
    }
    for (auto position : positionsToModify)
    {
        int row = position.first;
        int col = position.second;

        if (row >= 0 && row < matrix.size() && col >= 0 && col < matrix[row].size())
        {
            Wall *wall = new Wall('s');
            addItem(wall);
            wall->setPos(col * 15, row * 15);
            connect(wall, &Wall::breakWall, this, &Level::hitOnWall);
        }
    }

    QTimer::singleShot(5000, [=](){
       for (auto position : positionsToModify)
       {
           int row = position.first;
           int col = position.second;
           qDebug() << "positionsToModify );"<<row;
           qDebug() << "positionsToModify );"<<col;
           if (row >= 0 && row < matrix.size() && col >= 0 && col < matrix[row].size())
           {
               QList<QGraphicsItem*> itemList = items(QRectF(col * 15,row * 15, 15, 15), Qt::IntersectsItemBoundingRect);
               for (QGraphicsItem* item : itemList) {

                   if (Wall* wall = dynamic_cast<Wall*>(item))
                   {
                       removeItem(wall);
                       delete wall;
                   }
               }
           }
       }
       for (auto position : positionsToModify)
       {
           int row = position.first;
           int col = position.second;

           Wall *wall = new Wall('s');
           addItem(wall);
           wall->setPos(col*15 , row*15);
           connect(wall, &Wall::breakWall, this, &Level::hitOnWall);
       } });

    // qDebug() << "Wall *wall = new Wall('b');;";
}
void Level::handleTankEffect()
{
    health->increase();
}
void Level::handleHelmatEffect()
{
    invincible = true;
    qDebug() << "invincible=true;";
    player->setOpacity(0.5);
    qDebug() << "player->setOpacity(0.5);";
    effectTimer->start(500);
    if (invincible)
        invincibleTimer->start(3000);
}
void Level::handleBulletHitPlayer(Player *player)
{
    explosion = new Explosion();
    explosion->setPos(player->pos());

    explosionSound->play();
    addItem(explosion);
    explosion->startExplosion1();
    removeItem(player);
    setPlayer(number);
    health->decrease();
    if (health->getHealth() <= 0){
        showGameover();
    }
}

void Level::checkBulletPlayerCollision()
{
    // 获取场景中的所有子弹和玩家
    QList<QGraphicsItem *> collidingItems = player->collidingItems();

    for (QGraphicsItem *item : collidingItems)
    {
        // 檢查碰撞的對象是否是子彈
        Bullet *bullet = dynamic_cast<Bullet *>(item);

        if (bullet)
        {
            // 處理子彈擊中玩家的邏輯
            handleBulletHitPlayer(player);
            removeItem(bullet); // 移除子彈
            delete bullet;
            break; // 一旦處理了碰撞，就退出循環
        }
    }
}

void Level::showGameover(){
    QGraphicsPixmapItem *gameOverPixmap = new QGraphicsPixmapItem(QPixmap(":/images/Homepage/Gameover.png").scaled(800, 800));
    gameOverPixmap->setZValue(2);

    explosionSound->stop();
    shootingSound->stop();
    ArmorSound->stop();
    powerupSound->stop();
    powerupGenerateSound->stop();
    moveSound->stop();
    stopSound->stop();

    // 停掉所有音樂之後，播放gameoverSound
    gameOverTimer = new QTimer();
    connect(gameOverTimer, &QTimer::timeout, [=](){
        addItem(gameOverPixmap);
        gameoverSound->play();
        emit loseAndBackToStart();  // 還沒寫完切回主畫面
    });
    gameOverTimer->start(2000);
}
