#ifndef LEVEL_H
#define LEVEL_H

#include <vector>
#include <QTimer>
#include <QObject>
#include <QGraphicsScene>
#include <QGraphicsPixmapItem>
#include <QSettings>
#include <QMediaPlayer>
#include <QAudioOutput>
#include <QGraphicsTextItem>
#include <QFont>
#include <QGraphicsItemGroup>
#include <QGraphicsEllipseItem>
#include <QTimer>
#include <QRandomGenerator>

#include "Bullet.h"
#include "Health.h"
#include "Player.h"
#include "Score.h"
#include "Scene.h"
#include "Wall.h"
#include "Powerup.h"
#include "Enemy.h"
#include "Explosion.h"
// #include "Pausescreen.h"


using namespace std;

class Level : public QGraphicsScene
{
    Q_OBJECT
public:
    Level(int =0, int =0); //接收scene傳來的level跟number of players
    void setPlayer(int number);
    void generateMap(int level);
    void saveGame(); //目前不知怎麼寫
    void showSurvivor(int remains);     //生成畫面右邊的坦克剩餘圖
    void showGameover();//最後再寫
    void setTankRemainsVector(const vector<vector<char>>& tankRemains);
    const vector<vector<char>>& getTankRemainsVector() const;

    int current_player_Direction=3;
    int current_player2_Direction=3;
    void enemy_move();
    void checkPlayerWallCollision(Player *player, int playerDir);
    void checkEnemyWallCollision(Player *player);
    char tankType ;
    void checkPowerupCollision(); //確定玩家有拿到道具
    void handlePowerupCollision(); //觸發指定道具效果

    void togglePause();//暫停鍵esc可以讓所有敵人,子彈,玩家無法移動
    void pauseAllBullets();//停止所有子彈，速度=0
    void resumeAllBullets();//恢復所有子彈，速度=原來速度
    void setOpacityForScene(qreal opacity);//暫停畫面為所有場景的透明度0.5

    void resetShootStatus();//讓子彈不能連射

    void handleStarEffect();//處理星星效果

    void handleTimerEffect();//處理敵人停止
    void resumeEnemyMovement();//恢復敵人速度
    void pauseTimer();//暫停時間
    void resumeTimer();//恢復時間
    void setInitialEnemySpeed();//讓敵人速度為0

    void handleGrenadeEffect();//處理DELETE所有敵人

    void handleShovelEffect();//處理保護旗子

    void handleTankEffect();//處理多一條命

    void handleHelmatEffect();//處理無敵
    void handleBulletHitPlayer(Player *player);//當子彈碰到玩家會被打死，生命減少，復活在初始位置
    void checkBulletPlayerCollision();//確認玩家有碰到子彈



public slots:
    void generateEnemy(int level);
    void increaseScore(Bullet *bullet, Enemy *enemy);
    void hitOnWall(Wall *wall, Bullet *bullet);



    void togglePlayerOpacity()
    {
        if (player&&invincible) {
            player->setOpacity(player->opacity() == 1.0 ? 0.5 :1.0);
        }
    }
    void setInvincibe()
    {
        invincible=false;
    }
signals:
    void handleBulletCreation(Bullet *bullet);
    void loseAndBackToStart();

private:
    int level;    //等級
    int number;//player人數
    int number_1;
    int remains;
    int bestScore=0;//用來算歷史分數的分數

    vector<vector<char>> tanks;
    vector<vector<char>> matrix;
    QGraphicsPixmapItem *survivors;

    Player *player;
    Player *player2;

    Score *score;
    Health *health;
    Scene* scene;
    QList<int> keys;
    void slotTimeOut1();
    Powerup powerup;
    QSettings settings;


    bool invincible = false;
    bool isTimerPaused=false;
    bool isEnemyMovementFrozen=false;
    bool BulletFired = false;
    bool isPaused = false;
    bool isSlidingOnIce = false;


    QTimer *timer;
    QTimer* keyRespondTimer1;
    QTimer *spawnTimer;
    QTimer* invincibleTimer;
    QTimer* effectTimer;
    QTimer* checkTimer;
    QTimer *shootTimer;
    QTimer *powerupTimer;
    QTimer *gameOverTimer;
    QGraphicsPixmapItem *pauseScreen;
    QVector<qreal> originalEnemySpeeds;
    QVector<qreal> originalEnemyShoot;
    QList<Enemy*> enemyList;
    int star;//powerup star
    int removedEnemyCount = 0;

    Explosion *explosion;
    QMediaPlayer *explosionSound;
    QMediaPlayer *startSound;
    QMediaPlayer *shootingSound;
    QMediaPlayer *ArmorSound;
    QMediaPlayer *powerupSound;
    QMediaPlayer *powerupGenerateSound;
    QMediaPlayer *moveSound;
    QMediaPlayer *stopSound;
    QMediaPlayer *gameoverSound;

    QAudioOutput* musicOutput;
    QAudioOutput* musicOutput1;
    QAudioOutput* musicOutput2;
    QAudioOutput* musicOutput3;
    QAudioOutput* musicOutput4;
    QAudioOutput* musicOutput5;
    QAudioOutput* musicOutput6;
    QAudioOutput* musicOutput7;
    QAudioOutput *musicOutput8;


protected:
    void keyPressEvent (QKeyEvent *event)override ;
    void keyReleaseEvent (QKeyEvent *event);
};

#endif // LEVEL_H
