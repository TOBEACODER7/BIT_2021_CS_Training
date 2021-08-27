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
    ui->label->setVisible(false);
    ui->label_2->setVisible(false);
    //连接服务器
    clientSocket = new TcpClientSocket(this);
    clientSocket->connectToHost("10.194.185.14",6666);
    connect(clientSocket,SIGNAL(dealMessage(QString)),this,SLOT(displayMsg(QString)) );
    connect(clientSocket,SIGNAL(login_success()),this,SLOT(success()));
    connect(clientSocket,SIGNAL(login_failure()),this,SLOT(failure()));
    connect(clientSocket,SIGNAL(login_no_name()),this,SLOT(no_name()));
}

Login::~Login()
{
    delete ui;
}

//登录
void Login::on_login_clicked()
{
    QString name = ui->name->text();
    QString pwd = ui->password->text();
    QString info = "L#username:"+ name + "/password:" +pwd;

    ui->label->setVisible(false);
    ui->label_2->setVisible(false);

    if(name==""){
        ui->label->setText("请输入用户名!");
        ui->label->setVisible(true);
    }
    if(pwd==""){
        ui->label_2->setText("请输入密码!");
        ui->label_2->setVisible(true);
    }
    //输入完整，检测信息。
    if(name!=""&&pwd!=""){
        //发送信息
        clientSocket->write(info.toUtf8());
    }


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

void Login::on_enroll_clicked()//打开注册界面
{
    Enroll *enroll = new Enroll;
    enroll->show();//close();
}

void Login::success(){
    qDebug()<<"连接成功"<<endl;
    MainWindow *mainwindow = new MainWindow;
    this->close();
    QMessageBox::information(this,"提示","连接成功！");
    mainwindow->show();
}

void Login::failure(){
    ui->password->clear();
    ui->label->setText("密码错误!");
    ui->label->setVisible(true);
}

void Login::no_name(){
    ui->label->setText("用户名不存在!");
    ui->label->setVisible(true);
}

void Login::on_name_editingFinished()
{
    QString name = ui->name->text();
    if(name==""){
        ui->label->setText("请输入用户名!");
        ui->label->setVisible(true);
    }
    else{
        ui->label->setVisible(false);
        clientSocket->write(name.toUtf8());
    }
}
