#include "enroll.h"
#include "ui_enroll.h"

#include <QDebug>

Enroll::Enroll(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Enroll)
{
    ui->setupUi(this);
    ui->password->setPlaceholderText("输入8-16位");//密码输入框提示信息
    this->setWindowIcon(QIcon(":/image/QQ.png"));//设置左上角图标

    //设置密码显示的格式
    ui->password->setEchoMode(QLineEdit::Password);
    ui->textLabel->setVisible(false);
    clientSocket = new TcpClientSocket(this);
    clientSocket->connectToHost("10.194.177.196",6666);
    this->setWindowTitle("QQ");//设置左上角文字
    this->setWindowFlags(Qt::FramelessWindowHint);//去边框
    connect(clientSocket,SIGNAL(dealMessage(QString)),this,SLOT(displayMsg(QString)) );
    connect(clientSocket,SIGNAL(enroll_success()),this,SLOT(success()));
    connect(clientSocket,SIGNAL(enroll_failure()),this,SLOT(failure()));
}

Enroll::~Enroll()
{
    delete ui;
}

//点击登录，发送信息
void Enroll::on_putin_clicked()
{
    QString name = ui->name->text();
    QString pwd = ui->password->text();
    QString check_pwd = ui->checkpassword->text();

    //检测两次密码一致与否
    if(pwd!=check_pwd){
        ui->password->clear();
        ui->checkpassword->clear();

        ui->textLabel->setText("密码不一致!");
        ui->textLabel->setVisible(true);
    }
    else {//发送消息到服务器
        QString info = "L#username:"+ name + "/password:" +pwd;
        clientSocket->write(info.toUtf8());
    }
}
void Enroll::success(){
    if(clientSocket->connectToServer())
        qDebug()<<"连接成功"<<endl;
    else {
        qDebug()<<"连接失败"<<endl;
    }
    QMessageBox::information(this,"提示","注册成功！");
    this->close();
}
void Enroll::failure(){

}
void Enroll::on_name_editingFinished()
{
    QString name = ui->name->text();
    if(name==""){
        ui->textLabel_2->setText("请输入用户名!");
        ui->textLabel_2->setVisible(true);
    }
    else{
        ui->textLabel_2->setVisible(false);
        clientSocket->write(name.toUtf8());
    }
}
