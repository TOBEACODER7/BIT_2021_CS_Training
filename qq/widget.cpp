#include "widget.h"
#include "ui_widget.h"
#include<QDataStream>
#include<QMessageBox>
#include<QDateTime>
#include<QComboBox>
#include<QColorDialog>
#include<QFileDialog>
Widget::Widget(QWidget *parent ,QString name) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
    myname=name;

    this->port=9999;
    this->udpSocket=new QUdpSocket(this);

    udpSocket->bind(port,QUdpSocket::ShareAddress |QUdpSocket::ReuseAddressHint);

    //
    connect(udpSocket,&QUdpSocket::readyRead,this,&Widget::ReceiveMessage);

    //连接发送按钮
    connect(ui->sendBtn,&QPushButton::clicked,[=](){
        sndMsg(Msg);
    });

    //新用户进入
    sndMsg(UserEnter);

    connect(ui->exitBtn,&QPushButton::clicked,[=](){
        this->close();
    });
    //字体类型
    connect(ui->fontCbx,&QFontComboBox::currentFontChanged,[=](const QFont &font){
        ui->msgTxtEdit->setFontFamily(font.toString());
        ui->msgTxtEdit->setFocus();
    });
    //字体大小
    void (QComboBox:: * sizebtn)(const QString &text)=&QComboBox::currentTextChanged;
    connect(ui->sizeCbx,sizebtn,[=](const QString &text){
        ui->msgTxtEdit->setFontPointSize(text.toDouble());
        ui->msgTxtEdit->setFocus();
    });
    //字体的加粗
    connect(ui->boldTBtn,&QToolButton::clicked,[=](bool checked){
        if(checked)
        {
            ui->msgTxtEdit->setFontWeight(QFont::Bold);
        }
        else {
            ui->msgTxtEdit->setFontWeight(QFont::Normal);
        }
    });
    //字体倾斜
    connect(ui->italicTbtn,&QToolButton::clicked,[=](bool checked){
        ui->msgTxtEdit->setFontItalic(checked);
        ui->msgTxtEdit->setFocus();
    });
    //字体下划线
    connect(ui->underlineTBtn,&QToolButton::clicked,[=](bool checked){
        ui->msgTxtEdit->setFontUnderline(checked);
        ui->msgTxtEdit->setFocus();
    });
    //清空功能
    connect(ui->clearTBtn,&QToolButton::clicked,[=](){
        ui->msgBrowser->clear();
    });
    connect(ui->colorTBtn,&QToolButton::clicked,[=](){
        QColor color=QColorDialog::getColor(color,this);

        ui->msgTxtEdit->setTextColor(color);
    });
    connect(ui->saveTBtn,&QToolButton::clicked,[=](){
        if(ui->msgBrowser->toPlainText().isEmpty())
        {
            QMessageBox::warning(this,"警告","警告！保存内容不能为空!");
            return;
        }
        QString filename=QFileDialog::getSaveFileName(this,"保存聊天记录","聊天记录","(*.txt)");
        if(!filename.isEmpty())
        {
            QFile file(filename);
            file.open(QIODevice::WriteOnly | QFile::Text);
            QTextStream stream(&file);
            stream<<ui->msgBrowser->toPlainText();
            file.close();
        }
    });
}

void Widget::closeEvent(QCloseEvent *e)
{
    emit this->closeWidget();
    sndMsg(UserLeft);
    udpSocket->close();
    udpSocket->destroyed();
    QWidget::closeEvent(e);
}

void Widget::sndMsg(Widget::Msgtype type)
{
    QByteArray array;
    QDataStream stream(&array,QIODevice::WriteOnly);
    stream<<type<<this->getName();
    switch (type)
    {
    case Msg:
        if(ui->msgTxtEdit->toPlainText()=="")
        {
            QMessageBox::warning(this,"警告","发送的聊天内容不能为空!");
            return;
        }
        stream<<this->getMsg();
        break;
    case UserEnter:
        break;
    case UserLeft:
        break;
    }
    //书写报文
    udpSocket->writeDatagram(array.data(),array.size(),QHostAddress::Broadcast,this->port);
}

QString Widget::getName()
{
    return this->myname;
}

QString Widget::getMsg()
{
    QString msg=ui->msgTxtEdit->toHtml();
    ui->msgTxtEdit->clear();
    ui->msgTxtEdit->setFocus();
    return msg;
}
void Widget::userEnter(QString username)
{
    bool IsEmpty=ui->tableWidget->findItems(username,Qt::MatchExactly).isEmpty();

    if(IsEmpty)
    {
        QTableWidgetItem *table=new QTableWidgetItem(username);
        ui->tableWidget->insertRow(0);
        ui->tableWidget->setItem(0,0,table);
        ui->msgBrowser->setTextColor(QColor(Qt::gray));
        ui->msgBrowser->append(username+"已上线");
        ui->userNumLbl->setText(QString("在线人数:%1").arg(ui->tableWidget->rowCount()));
        sndMsg(UserEnter);
    }
}

void Widget::userLeft(QString username, QString time)
{
    bool IsEmpty=ui->tableWidget->findItems(username,Qt::MatchExactly).isEmpty();
    if(!IsEmpty)
    {
        int row=ui->tableWidget->findItems(username,Qt::MatchExactly).first()->row();

        ui->tableWidget->removeRow(row);

        ui->msgBrowser->append(QString("%1用户于%2离开").arg(username).arg(time));

         ui->userNumLbl->setText(QString("在线人数:%1").arg(ui->tableWidget->rowCount()));
    }
}

void Widget::ReceiveMessage()
{
    qint64 size=udpSocket->pendingDatagramSize();
    int mysize= static_cast<int>(size);
    QByteArray *array=new QByteArray(mysize,0);
    udpSocket->readDatagram((*array).data(),size);
    QDataStream stream(array,QIODevice::ReadOnly);
    int mytype;
    QString name,msg;//用户名 聊天内容
    QString time=QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
    stream>>mytype;
    switch (mytype) {
    case Msg:
        stream>>name>>msg;
        ui->msgBrowser->setTextColor(QColor(Qt::blue));
        ui->msgBrowser->append("["+name+"]"+time);
        ui->msgBrowser->append(msg);
        break;
    case UserLeft:
        stream>>name;
        userLeft(name,time);
        break;
    case UserEnter:
        stream>>name;
        userEnter(name);
        break;
    }

}
Widget::~Widget()
{
    delete ui;
}
