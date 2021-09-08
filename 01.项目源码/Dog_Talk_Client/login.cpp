#include "login.h"
#include "ui_login.h"
#include "mainwindow.h"
#include "enroll.h"
#include "QGraphicsBlurEffect"

Login::Login(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Login)
{
    ui->setupUi(this);

    //初始化
    LoginInit();
    //构建账号补全列表
    LoadConfig();

    connect(ui->name,SIGNAL(currentTextChanged(QString)),ui->password,SLOT(clear()));
    connect(ui->name,SIGNAL(currentIndexChanged(int)),this,SLOT(SetLogin(int)));

    //连接服务器
    clientSocket = TcpClientSocket::getInstance();
    clientSocket->setServerIP("10.195.136.207");
    clientSocket->setServerTcpPort("6666");
    clientSocket->connectToServer();
    connect(clientSocket,SIGNAL(dealMessage(QString)),this,SLOT(displayMsg(QString)) );
    connect(clientSocket,SIGNAL(login_success()),this,SLOT(success()));
    connect(clientSocket,SIGNAL(login_failure()),this,SLOT(failure()));
    connect(clientSocket,SIGNAL(login_no_name()),this,SLOT(no_name()));

    ui->name->setFocus();
}
//初始化
void Login::LoginInit(){

    //从上往下
       QPropertyAnimation *animation = new QPropertyAnimation(this, "size");
        animation->setDuration(500);
        animation->setStartValue(QSize(520,0));
        animation->setEndValue(QSize(520,400));
        animation->start();

    this->setWindowTitle("DOG");//设置左上角文字
    this->setWindowFlags(Qt::FramelessWindowHint);//去边框
    this->setWindowIcon(QIcon(":/image/dogblue.png"));//设置左上角图标
        ui->password->installEventFilter(this);//设置完后自动调用其eventFilter函数

        //阴影
        QGraphicsBlurEffect *effect = new QGraphicsBlurEffect(this);
                   effect->setBlurRadius(1.5);


                   QGraphicsDropShadowEffect *shadowEffect = new QGraphicsDropShadowEffect(this);
                   shadowEffect->setBlurRadius(20);
                   shadowEffect->setXOffset(2);
                   shadowEffect->setYOffset(2);


                   ui->widget->setGraphicsEffect(shadowEffect);

    //设置密码显示的格式
    ui->password->setPlaceholderText("输入8-16位");//密码输入框提示信息
    ui->password->setClearButtonEnabled(true);
    ui->password->setEchoMode(QLineEdit::Password);
    ui->password->setClearButtonEnabled(true);

    //label不可见
    ui->label->setVisible(false);
    ui->label_2->setVisible(false);

    namelist.clear();
    pwdlist.clear();
    remlist.clear();
}
void Login::ReadInit(QString key, QStringList & value){
    value.clear();
    QString path = "login.ini";

    QSettings *config = new QSettings(path,QSettings::IniFormat);
    QVariant var = config->value(QString("config/")+key);
    value = var.value<QStringList>();

    delete config;
}
void Login::LoadConfig(){
    qDebug()<<"in loadconfig";
    ReadInit("name",namelist);
    ReadInit("pwd",pwdlist);
    ReadInit("rem",remlist);
    qDebug()<<namelist<<pwdlist<<remlist;

    ui->name->addItems(namelist);

    if(namelist.size()>0){
        name = namelist.at(0);
    }
    if(pwdlist.size()>0){
        pwd = pwdlist.at(0);
    }
    if(rem=="true"){
        ui->checkBox->setChecked(true);
        ui->name->setCurrentText(name);
        ui->password->setText(pwd);
    }
}
void Login::SetLogin(int index)
{
    if(pwdlist.size()>=index||index>0){
           pwd = pwdlist.at(index-1);
           rem = remlist.at(index-1);
        if(rem== "true")
        {
            ui->checkBox->setChecked(true);//勾选记住用户名密码
            ui->password->setText(pwd);
        }else{
            ui->checkBox->setChecked(false);
            ui->password->setText("");
        }
    }
}
void Login::WriteInit(QString key, QStringList value){

    QString path = "login.ini";
    //创建配置文件操作对象
    QSettings *config = new QSettings(path, QSettings::IniFormat);
    QVariant variant;
    variant.setValue(value);
    //将信息写入配置文件

    qDebug()<<value<<"!!!"<<endl;
    config->beginGroup("config");
    config->setValue(key, variant);
    config->endGroup();
    delete config;
}

Login::~Login()
{
    delete ui;
}

//登录
void Login::on_login_clicked()
{
    QString name = ui->name->currentText();
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
    if(event->button() == Qt::LeftButton && event->pos().y()>0 && event->pos().y()<160)
    {
        mousePressed = true;//按下状态
        mousePoint = event->globalPos() - this->pos();//寻找相对位置
        event->accept();
    }
}
void Login::mouseMoveEvent(QMouseEvent *event)
{
    if(mousePressed && (event->buttons() && Qt::LeftButton) && event->pos().y()>0 && event->pos().y()<160)
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
    connect ( enroll, SIGNAL (send(QString ,QString )),this, SLOT(receive(QString ,QString )) );
    enroll->show();//close();
}
void Login::success(){
    //存入本地ini
    name = ui->name->currentText();
    pwd = ui->password->text();
    if(namelist.contains(name)){
        for(int i = 0; i<namelist.size(); ++i){
            QString name_ = namelist.at(i);
            if(name_==name){
                namelist.removeAt(i);
                pwdlist.removeAt(i);
                remlist.removeAt(i);
                namelist.push_front(name);
                pwdlist.push_front(pwd);
                remlist.push_front(rem);
            }
        }
    }else{
        namelist.push_front(name);
        if(namelist.size()>5) namelist.pop_back();
        pwdlist.push_front(pwd);
        if(pwdlist.size()>5) pwdlist.pop_back();
        remlist.push_front(rem);
        if(remlist.size()>5) remlist.pop_back();
    }
    qDebug()<<namelist<<" "<<pwdlist<<" "<<remlist;
    WriteInit("name",namelist);
    WriteInit("pwd",pwdlist);
    WriteInit("rem",remlist);

    qDebug()<<"连接成功"<<endl;
//    QMessageBox::information(this,"提示","连接成功！");

    MainWindow *mainwindow = new MainWindow(nullptr,name);
    mainwindow->show();
    this->close();
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
void Login::receive(QString username, QString password)
{
    ui->name->setCurrentText(username);
    ui->password->setText(password);
}

void Login::on_checkBox_clicked()
{
    if(ui->checkBox->isChecked()){
        rem = "true";
    }else{
        rem = "false";
    }
}

void Login::on_name_currentTextChanged(const QString &arg1)
{
    QString name = ui->name->currentText();
    if(name==""){
        ui->label->setText("请输入用户名!");
        ui->label->setVisible(true);
    }
    else{
        ui->label->setVisible(false);
        name = "U#" +name;
        clientSocket->write(name.toUtf8());
    }
}

void Login::on_hide_clicked()
{
    this->showMinimized();
}

bool Login::eventFilter(QObject *target, QEvent *event)
{
    if(target == ui->password)
    {
        if(event->type() == QEvent::KeyPress)//回车键
        {
             QKeyEvent *k = static_cast<QKeyEvent *>(event);
             if(k->key() == Qt::Key_Return)
             {
                 on_login_clicked();
                 return true;
             }
        }
    }
    return QWidget::eventFilter(target,event);
}
