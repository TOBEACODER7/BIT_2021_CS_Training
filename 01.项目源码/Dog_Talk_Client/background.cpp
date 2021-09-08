#include "background.h"
#include "ui_background.h"
#include "mainwindow.h"

BackGround::BackGround(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::BackGround)
{
    ui->setupUi(this);
    this->setWindowTitle("背景图片");//设置左上角文字
    this->setWindowIcon(QIcon(":/image/dogblue.png"));//设置左上角图标
    this->setWindowFlags(Qt::FramelessWindowHint);//去边框

    //淡入淡出
       QPropertyAnimation *animation = new QPropertyAnimation(this, "windowOpacity");
        animation->setDuration(1000);
        animation->setStartValue(0);
        animation->setEndValue(1);
        animation->start();
}

BackGround::~BackGround()
{
    delete ui;
}

void BackGround::mousePressEvent(QMouseEvent *event)
{
    if(event->button() == Qt::LeftButton && event->pos().y()>0 && event->pos().y()<160)
    {
        mousePressed = true;//按下状态
        mousePoint = event->globalPos() - this->pos();//寻找相对位置
        event->accept();
    }
}
void BackGround::mouseMoveEvent(QMouseEvent *event)
{
    if(mousePressed && (event->buttons() && Qt::LeftButton) && event->pos().y()>0 && event->pos().y()<160)
    {
        this->move(event->globalPos() - mousePoint);
        event->accept();
    }
}
void BackGround::mouseReleaseEvent(QMouseEvent *event)
{
    mousePressed = false;
}


void BackGround::on_hide_clicked()
{
    this->showMinimized();
}

void BackGround::on_close_clicked()
{
    this->close();
}

void BackGround::on_bg1_clicked()
{
    emit change1();
}
void BackGround::on_bg2_clicked()
{
    emit change2();
}
void BackGround::on_bg3_clicked()
{
    emit change3();
}
void BackGround::on_bg4_clicked()
{
    emit change4();
}
void BackGround::on_bg5_clicked()
{
    emit change5();
}
void BackGround::on_bg6_clicked()
{
    emit change6();
}
void BackGround::on_bg7_clicked()
{
    emit change7();
}
void BackGround::on_bg8_clicked()
{
    emit change8();
}
void BackGround::on_bg9_clicked()
{
    emit change9();
}

