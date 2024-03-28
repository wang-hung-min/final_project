#include "Pausescreen.h"
#include <QGraphicsScene>
PauseScreen::PauseScreen(int width, int height)
{
    setRect(0, 0, width, height);
    setBrush(QBrush(QColor(0, 0, 0, 128))); // 半透明的黑色背景
}
void PauseScreen::showPauseMessage()
{
    QGraphicsTextItem *pauseText = new QGraphicsTextItem("Pause");
    QFont font;
    font.setPointSize(40);
    pauseText->setFont(font);
    int x = rect().width() / 2 - pauseText->boundingRect().width() / 2;
    int y = rect().height() / 2 - pauseText->boundingRect().height() / 2;
    pauseText->setPos(x, y);
}
