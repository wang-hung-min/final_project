#include "health.h"

#include <QFont>

Health::Health() {
    // Initialize the score to zero
    health = 3;
    // Draw the score
    setPlainText("Health: " + QString::number(health));
    setDefaultTextColor(Qt::blue);
    setFont(QFont("times", 16)); // name, size
}
void Health::increase() {
    health++;
    setPlainText("Health: " + QString::number(health));
}

void Health::decrease() {
    health--;
    setPlainText("Health: " + QString::number(health));
}

int Health::getHealth() {
    return health;
}
