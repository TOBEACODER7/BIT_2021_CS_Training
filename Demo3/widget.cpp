#include "widget.h"
#include "ui_widget.h"
#include <QDebug>

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);

    clientSocket = new QTcpSocket(this);
    clientSocket->connectToHost("127.0.0.1",6666);//在本机测试用ip"127.0.0.1"本地ip地址
    //clientSocket->write("1234567890");//写函数，写个字符串发送给主机
    connect(clientSocket,SIGNAL(readyRead()),this,SLOT(recvMsg()));//服务器发过来信息，槽函数关联
}

Widget::~Widget()
{
    delete ui;
}

void Widget::recvMsg()
{
    QByteArray buf = clientSocket->readAll();
    ui->lineEdit->setText(buf);

}

void Widget::on_pushButton_clicked()
{
    QString str = ui->lineEdit->text();
    clientSocket->write(str.toUtf8());//用函数将string形式转换为wirte所识别的样式，to开头的函数就是转换函数
}
