#include "login.h"
#include "ui_login.h"
#include "mainwindow.h"
#include "enroll.h"

Login::Login(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Login)
{
    ui->setupUi(this);
    ui->password->setPlaceholderText("输入8-16位");//密码输入框提示信息
    this->setWindowIcon(QIcon(":/image/QQ.png"));//设置左上角图标

    //设置密码显示的格式
    ui->password->setEchoMode(QLineEdit::Password);
    //构建账号补全列表
    QStringList listnumber;
    listnumber <<"1293957879"<<"123456789"<<"2020030421";
//    //构建补全器
//    QCompleter *number = new QCompleter(listnumber);
//    //设置给编译器
//    ui->lineEdit_uset->setCompleter(number);
//    connect(ui->btnOk,SIGNAL(clicked(bool)),this,SLOT(login()));

//    this->ui->registuser->setText("<a style = 'text-decoration:none' href=' '>注册账号</ a>");
//    this->ui->registuser->setOpenExternalLinks(true);

//    this->ui->findpd->setText("<a style=' text-decoration:none' href= 'http://aq.qq.com/v2/uv_aq/html/reset_pwd/pc_reset_pwd_input_account.html'>找回密码</ a>");//超链接
//    this->ui->findpd->setOpenExternalLinks(true);

    this->setWindowTitle("QQ");//设置左上角文字
    this->setWindowFlags(Qt::FramelessWindowHint);//去边框
}

Login::~Login()
{
    delete ui;
}

//登录
void Login::on_login_clicked()
{
    MainWindow *mainwindow = new MainWindow;
    mainwindow->show();//close();
}

void Login::mousePressEvent(QMouseEvent *event)
{
    if(event->button() == Qt::LeftButton)
    {
        mousePressed = true;//按下状态
        mousePoint = event->globalPos() - this->pos();//寻找相对位置
        event->accept();
    }
}

void Login::mouseMoveEvent(QMouseEvent *event)
{
    if(mousePressed && (event->buttons() && Qt::LeftButton))
    {
        this->move(event->globalPos() - mousePoint);
        event->accept();
    }
}

void Login::mouseReleaseEvent(QMouseEvent *event)
{
    mousePressed = false;
}

void Login::on_close_clicked()
{
    this->close();
}

//打开注册界面
void Login::on_enroll_clicked()
{
    Enroll *enroll = new Enroll;
    enroll->show();//close();
}
