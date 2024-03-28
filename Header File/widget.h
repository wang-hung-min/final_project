#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <Scene.h>
#include <Level.h>

QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();

private slots:
    void on_startGameButton_clicked();

private:
    Ui::Widget *ui;
    Scene *scene; //Scene的物件
    Level *level; //Level的物件
};
#endif // WIDGET_H
