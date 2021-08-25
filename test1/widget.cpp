#include "widget.h"
#include "ui_widget.h"
#include "form.h"

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
}

Widget::~Widget()
{
    delete ui;
}

//开启新窗口函数
void Widget::on_toolButton_6_clicked()
{
    Form *form = new Form;
    form->show();//close();
}

void Widget::on_toolButton_8_clicked()
{
    Form *form = new Form;
    form->close();
}

//更换背景
void Widget::on_toolButton_4_clicked()
{
    this->setStyleSheet("QWidget#widget{background-image:url(:/image/ss.png);}"
                        "QToolButton{border-style:none;}");
}

void Widget::on_toolButton_5_clicked()
{
    this->setStyleSheet("QWidget#widget{background-image:url(:/image/moon.png);}"
                        "QToolButton{border-style:none;}");
}
