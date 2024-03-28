#ifndef PAUSESCREEN_H
#define PAUSESCREEN_H

#include <QGraphicsRectItem>
#include <QGraphicsTextItem>
class PauseScreen : public QGraphicsRectItem
{
public:
    PauseScreen(int width, int height);
    void showPauseMessage();
};

#endif // PAUSESCREEN_H
