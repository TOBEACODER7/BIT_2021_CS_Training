#include "enroll.h"
#include "ui_enroll.h"
#include "QGraphicsBlurEffect"
#include <QValidator>
#include <QDebug>

Enroll::Enroll(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Enroll)
{
    ui->setupUi(this);
    EnrollInit();

    clientSocket = TcpClientSocket::getInstance();

    connect(clientSocket,SIGNAL(dealMessage(QString)),this,SLOT(displayMsg(QString)) );
    connect(clientSocket,SIGNAL(enroll_success()),this,SLOT(success()));
    connect(clientSocket,SIGNAL(enroll_failure()),this,SLOT(failure()));
    connect(clientSocket,SIGNAL(enroll_name_exists()),this,SLOT(name_exists()));

}

Enroll::~Enroll()
{
    delete ui;
}

void Enroll::EnrollInit(){
    this->setWindowTitle("注册");//设置左上角文字
    this->setWindowIcon(QIcon(":/image/dogblue.png"));//设置左上角图标
    this->setWindowFlags(Qt::FramelessWindowHint);//去边框

    //淡入淡出
    QPropertyAnimation *animation = new QPropertyAnimation(this, "windowOpacity");
    animation->setDuration(1000);
    animation->setStartValue(0);
    animation->setEndValue(1);
    animation->start();

    ui->password->setClearButtonEnabled(true);
    ui->name->setClearButtonEnabled(true);
    ui->email->setClearButtonEnabled(true);
    ui->phonenumber->setClearButtonEnabled(true);
    ui->checkpassword->setClearButtonEnabled(true);

    QGraphicsBlurEffect *effect = new QGraphicsBlurEffect(this);
    effect->setBlurRadius(1.5);
    QGraphicsDropShadowEffect *shadowEffect = new QGraphicsDropShadowEffect(this);
    shadowEffect->setBlurRadius(20);
    shadowEffect->setXOffset(2);
    shadowEffect->setYOffset(2);
    ui->widget->setGraphicsEffect(shadowEffect);

    //设置显示的格式
    ui->password->setEchoMode(QLineEdit::Password);
    ui->name_check->setVisible(false);
    ui->pwd_check->setVisible(false);
    ui->email_check->setVisible(false);
    ui->phone_check->setVisible(false);
    //邮箱格式检测
    QRegExp email_exp("^\\w+([-+.]\\w+)*@\\w+([-.]\\w+)*\\.\\w+([-.]\\w+)*$");//正则表达式
    QRegExpValidator *email_val = new QRegExpValidator(email_exp,ui->email);
    ui->email->setValidator(email_val);
    //手机格式检测
    QRegExp phone_exp("^(13[0-9]|14[01456879]|15[0-35-9]|16[2567]|17[0-8]|18[0-9]|19[0-35-9])\\d{8}$");//正则表达式
    QRegExpValidator *phone_val = new QRegExpValidator(phone_exp,ui->phonenumber);
    ui->phonenumber->setValidator(phone_val);


    ui->name->setFocus();
}
//点击登录，发送信息
void Enroll::on_putin_clicked()
{
    QString phone = ui->phonenumber->text();
    QString email = ui->email->text();
    QString name = ui->name->text();
    QString pwd = ui->password->text();
    QString check_pwd = ui->checkpassword->text();

    if(name==""||pwd==""||check_pwd==""){
        QMessageBox::information(this,"提示","信息输入不全\n请补充");
    }
    //检测两次密码一致与否
    else if(pwd!=check_pwd){
        ui->password->clear();
        ui->checkpassword->clear();

        ui->pwd_check->setText("密码不一致!");
        ui->pwd_check->setVisible(true);
    }
    else if(pwd.length()<8){
        ui->pwd_check->setText("密码长度不足");
        ui->pwd_check->setVisible(true);
    }
    else {

        QString info = "R#phone:" + phone +"/email:" + email + "/username:"+ name + "/password:" +pwd;
        qDebug()<<info;
        clientSocket->write(info.toUtf8());
    }
}
void Enroll::success(){
    qDebug()<<"连接成功"<<endl;
    QMessageBox::information(this,"提示","注册成功！");
    QString name = ui->name->text();
    QString pwd = ui->password->text();
    emit send(name,pwd);
    this->close();
}
void Enroll::failure(){
    qDebug()<<"连接失败"<<endl;
    QMessageBox::information(this,"提示","连接失败！请重试");

}
void Enroll::name_exists(){
    ui->name_check->setText("用户名已存在!");
    ui->name_check->setVisible(true);
}
void Enroll::on_name_editingFinished()
{
    QString name = ui->name->text();
    if(name==""){
        ui->name_check->setText("请输入用户名!");
        ui->name_check->setVisible(true);
    }
    else{
        ui->name_check->setVisible(false);
        name = "U#" +name;
        clientSocket->write(name.toUtf8());
    }
}

void Enroll::on_close_btn_clicked()
{
    close();
}
void Enroll::mousePressEvent(QMouseEvent *event)
{
    if(event->button() == Qt::LeftButton && event->pos().y()>0 && event->pos().y()<120)
    {
        mousePressed = true;//按下状态
        mousePoint = event->globalPos() - this->pos();//寻找相对位置
        event->accept();
    }
}
void Enroll::mouseMoveEvent(QMouseEvent *event)
{
    if(mousePressed && (event->buttons() && Qt::LeftButton) && event->pos().y()>0 && event->pos().y()<120)
    {
        this->move(event->globalPos() - mousePoint);
        event->accept();
    }
}
void Enroll::mouseReleaseEvent(QMouseEvent *event)
{
    mousePressed = false;
}

void Enroll::on_email_textEdited(const QString &arg1)
{
    const QValidator *v = ui->email->validator();
    int pos = 0;
    QString email = ui->email->text();
    if (v->validate(email, pos) != QValidator::Acceptable)
    {
        ui->email_check->setText("邮箱格式不正确");
        ui->email_check->setVisible(true);
        return;
    }
    ui->email_check->setVisible(false);
}
void Enroll::on_phonenumber_textEdited(const QString &arg1)
{
    const QValidator *v = ui->phonenumber->validator();
    int pos = 0;
    QString phone = ui->phonenumber->text();
    if (v->validate(phone, pos) != QValidator::Acceptable)
    {
        ui->phone_check->setText("手机格式不正确");
        ui->phone_check->setVisible(true);
        return;
    }
    ui->phone_check->setVisible(false);
}


