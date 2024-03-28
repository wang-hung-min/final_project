#include "score.h"

#include <QFont>

Score::Score() {
    // Initialize the score to zero
    score = 0;
    // Draw the score
    setPlainText("Score: " + QString::number(score));
    setDefaultTextColor(Qt::blue);
    setFont(QFont("times", 16)); // name, size
}

void Score::increase(int points) {
    score+=points;
    setPlainText("Score: " + QString::number(score));
}

int Score::getScore() {
    return score;
}
