#include "talkwindow.h"
#include "ui_talkwindow.h"

TalkWindow::TalkWindow(QWidget *parent, QString name, QString name_f) :
//TalkWindow::TalkWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TalkWindow)
{
    ui->setupUi(this);

    myname=name;
    friendname = name_f;
    clientsocket = TcpClientSocket::getInstance();
    udpsender = new UdpSender;
    udpreceiver = new UdpReciver;
    TalkInit();

    connect(clientsocket,SIGNAL(recvMsg(QString)),this,SLOT(getMsg(QString)));

}

void TalkWindow::TalkInit(){
    this->setWindowIcon(QIcon(":/image/dogblue.png"));//设置左上角图标
    this->setWindowFlags(Qt::FramelessWindowHint);
    ui->name->setText(friendname);
    ui->msgTextEdit->installEventFilter(this);//设置完后自动调用其eventFilter函数

    //淡入淡出
    QPropertyAnimation *animation = new QPropertyAnimation(this, "windowOpacity");
    animation->setDuration(600);
    animation->setStartValue(0);
    animation->setEndValue(1);
    animation->start();
//阴影
    QGraphicsBlurEffect *effect = new QGraphicsBlurEffect(this);
               effect->setBlurRadius(1.5);


    QGraphicsDropShadowEffect *shadowEffect = new QGraphicsDropShadowEffect(this);
    shadowEffect->setBlurRadius(20);
               shadowEffect->setXOffset(2);
               shadowEffect->setYOffset(2);


               ui->widget->setGraphicsEffect(shadowEffect);

    //字体类型
    connect(ui->fontCbx,&QFontComboBox::currentFontChanged,[=](const QFont &font){
        ui->msgTextEdit->setFontFamily(font.toString());
        ui->msgTextEdit->setFocus();
    });
    //字体大小
    void (QComboBox:: * sizebtn)(const QString &text)=&QComboBox::currentTextChanged;
    connect(ui->sizeCbx,sizebtn,[=](const QString &text){
        ui->msgTextEdit->setFontPointSize(text.toDouble());
        ui->msgTextEdit->setFocus();
    });
    //字体的加粗
    connect(ui->boldTbtn,&QToolButton::clicked,[=](bool checked){
        if(checked)
        {
            ui->msgTextEdit->setFontWeight(QFont::Bold);
        }
        else {
            ui->msgTextEdit->setFontWeight(QFont::Normal);
        }
    });
    //字体倾斜
    connect(ui->italicTbtn,&QToolButton::clicked,[=](bool checked){
        ui->msgTextEdit->setFontItalic(checked);
        ui->msgTextEdit->setFocus();
    });
    //字体下划线
    connect(ui->underlineTbtn,&QToolButton::clicked,[=](bool checked){
        ui->msgTextEdit->setFontUnderline(checked);
        ui->msgTextEdit->setFocus();
    });
    //清空功能
    connect(ui->clearTbtn,&QToolButton::clicked,[=](){
        ui->msgBrowser->clear();
    });
    connect(ui->colorTbtn,&QToolButton::clicked,[=](){
        QColor color=QColorDialog::getColor(color,this);

        ui->msgTextEdit->setTextColor(color);
    });
}

TalkWindow::~TalkWindow()
{
    delete ui;
}

void TalkWindow::mousePressEvent(QMouseEvent *event)
{
    if(event->button() == Qt::LeftButton && event->pos().y()>0 && event->pos().y()<50)
    {
        mousePressed = true;//按下状态
        mousePoint = event->globalPos() - this->pos();//寻找相对位置
        event->accept();
    }
}
void TalkWindow::mouseMoveEvent(QMouseEvent *event)
{
    if(mousePressed && (event->buttons() && Qt::LeftButton) && event->pos().y()>0 && event->pos().y()<50)
    {
        this->move(event->globalPos() - mousePoint);
        event->accept();
    }
}
void TalkWindow::mouseReleaseEvent(QMouseEvent *event)
{
    mousePressed = false;
}
void TalkWindow::on_close_clicked()
{
    emit closeWidget();
    this->close();
}

QString TalkWindow::getName()
{
    return this->myname;
}

void TalkWindow::getMsg(QString info)
{
    qDebug()<<info<<"in getMsg";
    if(info.contains("M#1")){
        QMessageBox::information(this,"提示","好友不在线");
    }
    else if(info.contains("M#0")){
        QString name = info.mid(info.indexOf("M#0")+3,info.indexOf(":")-info.indexOf("M#0")-3);
        QString msg = info.mid(info.indexOf(':')+1,info.indexOf("|||")-info.indexOf(':')-1);
        ShowMsg(name,msg);
    }
    else if(info.contains("M#2")){

    }
}
int TalkWindow::ShowMsg(QString name, QString msg){

    QDateTime current_date_time =QDateTime::currentDateTime();
    QString current_date =current_date_time.toString("yyyy.MM.dd hh:mm:ss.zzz ddd");

    QString title = "[" + name + "] " + current_date + "\n";
    ui->msgBrowser->setTextColor("grey");
    ui->msgBrowser->setFontPointSize(10);

    ui->msgBrowser->append(current_date);
    ui->msgBrowser->setAlignment(Qt::AlignCenter);

    if(name==myname){
        ui->msgBrowser->setTextColor("black");
        ui->msgBrowser->setFontPointSize(16);
        ui->msgBrowser->setFontItalic(true);
        ui->msgBrowser->append("[" + name + "]");
        ui->msgBrowser->setAlignment(Qt::AlignRight);
        ui->msgBrowser->setFontPointSize(23);
        ui->msgBrowser->setFontItalic(false);
        ui->msgBrowser->append(msg);
    }
    else{
        ui->msgBrowser->setTextColor("black");
        ui->msgBrowser->setFontPointSize(16);
        ui->msgBrowser->setFontItalic(true);
        ui->msgBrowser->append("[" + name + "]");
        ui->msgBrowser->setAlignment(Qt::AlignLeft);
        ui->msgBrowser->setFontPointSize(23);
        ui->msgBrowser->setFontItalic(false);
        ui->msgBrowser->append(msg);
    }
    qDebug()<<title<< "\n"<<msg;
    return 1;
}
//发送消息
void TalkWindow::on_pushButton_clicked()
{
    QString msg = ui->msgTextEdit->toPlainText();
    ui->msgTextEdit->clear();
    ShowMsg(myname,msg);
    msg = "M#send_user:" + myname + "/recv_user:" + friendname + "|message" + msg+"|||";
    clientsocket->sendMsg(msg);
}

void TalkWindow::on_hide_clicked()
{
    this->showMinimized();
}

bool TalkWindow::eventFilter(QObject *target, QEvent *event)
{
    if(target == ui->msgTextEdit)
    {
        if(event->type() == QEvent::KeyPress)//回车键
        {
             QKeyEvent *k = static_cast<QKeyEvent *>(event);
             if(k->key() == Qt::Key_Return)
             {
                 on_pushButton_clicked();
                 return true;
             }
        }
    }
    return QWidget::eventFilter(target,event);
}


void TalkWindow::on_close2_clicked()
{
    close();
}

void TalkWindow::on_saveTbtn_clicked()
{
    QString filePath = QFileDialog::getOpenFileName(this, "Open", "../");
    if(!filePath.isEmpty())
    {
        QFileInfo info(filePath);
        fileName = info.fileName();
        fileSize = info.size();
        qDebug()<<fileName<<fileSize;

        file.setFileName(filePath);
        if(file.open(QIODevice::ReadOnly))
        {
            ShowMsg(myname,QString("FileName:%1\nFileSize:%2 KB").arg(fileName).arg(fileSize/1024));
            //连接时设置“选择文件”为不可用，“发送文件”可用
            QString str = "&&" + fileName + "##" ;
            udpsender->send_file(str.toLatin1());
        }
        else {
            qDebug()<<"打开文件失败";
        }
    }
    else
    {
        qDebug()<<"文件路径出错";
    }
    while (!file.atEnd()) {
        QByteArray array = file.read(1024*8);
        udpsender->send_file(array);
    }

}
