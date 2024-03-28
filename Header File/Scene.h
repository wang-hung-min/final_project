#ifndef SCENE_H
#define SCENE_H

#include <QGraphicsScene>
#include <QTimer>
#include <QGraphicsPixmapItem>

class Scene : public QGraphicsScene
{
    Q_OBJECT
public:
    explicit Scene(QObject *parent = nullptr);
    ~Scene();
    void startGame();
    bool getGameOn() const;
    void setGameOn(bool newGameOn);
    void setPlayer(int newplayer);
    int getPlayer();
    void setLevel();
    int getLevel();
    int number;
    void setBestScore(int bestScore)
    {
        score = bestScore;
    }
signals:
    void changeLevel();

private:
    bool isChooseStage = false;
    bool gameOn;
    int score;
    int bestScore;
    //int player;
    int level;
    QPointF pos ;
    //在QGraphicsScene中就有PixmapItem, TextItem
    QGraphicsPixmapItem *battleCityBigWord;
    QGraphicsPixmapItem *onePlayer;
    QGraphicsPixmapItem *twoPlayer;
    QGraphicsPixmapItem *levelChose;
    QGraphicsPixmapItem *playerChose;
    QGraphicsTextItem *scoreTextItem;
    QGraphicsTextItem *levelTextItem;

    // QGraphicsScene interface
protected:
    void keyPressEvent(QKeyEvent *event);
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
};

#endif // SCENE_H
