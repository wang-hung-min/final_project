#include "Explosion.h"

Explosion::Explosion(QGraphicsItem *parent) : QGraphicsPixmapItem(parent), currentFrame(0), totalFrames(16)
{
    // Set the initial image for the first explosion
    setPixmap(QPixmap(":/images/Effect/explosion1"));

    // Set up the first explosion timer
    explosionTimer1 = new QTimer(this);
    connect(explosionTimer1, &QTimer::timeout, this, &Explosion::updateFrame1);

    // Set up the second explosion timer
    explosionTimer2 = new QTimer(this);
    connect(explosionTimer2, &QTimer::timeout, this, &Explosion::updateFrame2);
}

void Explosion::startExplosion1()
{
    // Start the first explosion timer
    explosionTimer1->start(500);  // Adjust this number to control the animation speed
}

void Explosion::startExplosion2()
{
    // Set the initial image for the second explosion
    setPixmap(QPixmap(":/images/Effect/explosion2"));

    // Start the second explosion timer
    explosionTimer2->start(500);  // Adjust this number to control the animation speed
}

void Explosion::updateFrame1()
{
    // Update the first explosion animation frame
    currentFrame = (currentFrame + 1) % totalFrames;
    updatePixmap(explosionTimer1);
}

void Explosion::updateFrame2()
{
    // Update the second explosion animation frame
    currentFrame = (currentFrame + 1) % totalFrames;
    updatePixmap(explosionTimer2);
}

void Explosion::updatePixmap(QTimer* timer)
{
    // Calculate the width of each frame in the sequence image
    int frameWidth = pixmap().width() / totalFrames;

    // Set the displayed portion based on the current frame
    setPixmap(pixmap().copy(currentFrame * frameWidth, 0, frameWidth, pixmap().height()));

    // If the animation is finished, stop the timer
    if (currentFrame == totalFrames - 1) {
        timer->stop();
    }
}
