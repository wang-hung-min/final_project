#include "widget.h"
#include "ui_widget.h"
#include "Scene.h"
#include <QGraphicsPixmapItem>

Widget::Widget(QWidget *parent)//宣告一個QWidget的物件叫parent
    : QWidget(parent)//直接初始化
    , ui(new Ui::Widget), scene(new Scene)//直接初始化
{

    ui->setupUi(this);
    scene->setSceneRect(0,0,800,800);//game's O(0,0)為左上角座標,800跟600為寬跟高
    ui->graphicsView->setScene(scene);//透過graphicsView看scene
    ui->graphicsView->setFixedSize(800,800);//只能看到固定的大小
    ui->graphicsView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);//不給使用者show出上下調整的滾輪
    ui->graphicsView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->graphicsView->setBackgroundBrush(QBrush(QImage(":/images/Homepage/Solid_black.png")));

    // level = new Level(scene->getLevel(), scene->getPlayer());  // 先初始化
    connect(scene, &Scene::changeLevel, [=](){
        level = new Level(scene->getLevel(), scene->getPlayer());   // 根據在 Scene 的選擇設定關卡
        ui->graphicsView->setScene(level);
        ui->graphicsView->setFixedSize(800,800);//只能看到固定的大小
        ui->graphicsView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);//不給使用者show出上下調整的滾輪
        ui->graphicsView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    });

//    connect(level, &Level::loseAndBackToStart, [=](){
//        scene = new Scene();
//        ui->graphicsView->setScene(scene);
//    });
}

Widget::~Widget()
{
    delete level;
    delete ui;
    delete scene;
}

void Widget::on_startGameButton_clicked()
{
    scene->startGame();
    ui->graphicsView->setFocus();
}
