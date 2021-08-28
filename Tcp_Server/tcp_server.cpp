#include "tcp_server.h"
#include "ui_tcp_server.h"
#include <QMessageBox>
#include <QDateTime>
#include <QTimer>
#include "user_info.h"
#include "db.h"

Tcp_Server::Tcp_Server(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Tcp_Server)
{
    ui->setupUi(this);

    //初始化
    TCP_Server = new QTcpServer();
    TCP_connectSocket = nullptr;
    memset(num,0,sizeof (num));

    DataBase = new db();

    //用于服务器主动通过输入框发送给客户端信息(正式项目不需要)
    //connect(ui->pushButton,SIGNAL(clicked()),this,SLOT(slot_sendmsg()));

    //调用listen函数监听同时绑定IP和端口号
    if(TCP_Server->listen(QHostAddress::Any,6666))
    {
        this->connect(TCP_Server,SIGNAL(newConnection()),this,SLOT(slot_newconnect()));

        //利用自实现的心跳包机制，定期给服务器发送信息来检测异常断开的情况
        QTimer *timer = new QTimer(this);
        connect(timer,SIGNAL(timeout()),this,SLOT(update_Socket()));
        timer->start(1000);
    }
    else
    {
        QMessageBox::critical(this,"error","IP绑定错误");
    }
}

Tcp_Server::~Tcp_Server()
{
    delete ui;
}

//服务器坑位检测函数，用以检测服务器目前是否还有可用的坑位
int Tcp_Server::check_num()
{
    for(int i=1;i<=9;i++)
    {
        if(this->num[i] == 0)
        {
            return i;
        }
    }
    return 0;
}

void Tcp_Server::slot_newconnect()
{
    TCP_connectSocket = TCP_Server->nextPendingConnection();
    qDebug()<<"有新的客户端接入";
    int location = this->check_num();
    if(location == 0)
    {
        slot_sendmsg("无法连接已满的服务器",TCP_connectSocket);
        TCP_connectSocket->disconnectFromHost();
        qDebug()<<"服务器连接已满，并已阻止连接";

        return;
    }
    num[location] = 1;
    connect_sum++;
    Client *client = new Client(this,TCP_connectSocket,location);
    this->Clients[location] = client;

    //服务器列表更新
    this->server_menu_update();
    /*ui->textBrowser->append("已连接数："+QString::number(connect_sum));
    ui->textBrowser->append("在线ip列表:");
    for(int i=1;i<=9;i++)
    {
        if(num[i]!=0)
        {
            ui->textBrowser->append("#"+Clients[i]->Socket->peerAddress().toString());
        }
    }*/

    //客户端列表更新
    this->client_menu_update();

    //当客户端有通信发出时
    connect(Clients[location]->Socket,SIGNAL(readyRead()),Clients[location],SLOT(slot_Read()));

    //当客户端断开连接时，让出服务器的位置（用心跳包机制代替77）
    //connect(Clients[location]->Socket,SIGNAL(error(QAbstractSocket::SocketError)),this,SLOT(slot_disconnect(location)));

}

//服务器发送函数，用于反馈异常信息or进行客户端之间通信时作为转发函数
void Tcp_Server::slot_sendmsg(QString str,QTcpSocket *Socket)
{
    std::string str1 = str.toStdString();
    const char *data = str1.c_str();
    int num1 = Socket->write(data);
    //qDebug()<< "sendhanshu";


}

//服务器信息接收函数，用于接收来自客户端的各种信息
void Tcp_Server::recvmsg(QString str)
{

    //格式解析
    if(str[0] == 'L')
    {
        int idx2 = str.indexOf("/");
        QString msg;
        QString user_name = str.mid(11,idx2-11);
        QString user_pwd = str.mid(idx2+10,-1);

        if((DataBase->selectSql(user_name)))
        {
            //用户名存在

            if(DataBase->loginJudge(user_name,user_pwd))
            {
                //登录成功
                msg = "L#1";
            }
            else
            {
                //登录失败
                msg ="L#0";
            }
        }
        else
        {
            //用户名不存在，无法登录
            msg = "L#3";
        }
        qDebug()<< msg;
    }
    else if(str[0] == 'R')
    {

        int idx2 = str.indexOf("/");
        QString msg;
        QString user_name = str.mid(11,idx2-11);
        QString user_pwd = str.mid(idx2+10,-1);

        if(!DataBase->selectSql(user_name))
        {

            user = new user_info(DataBase->getNum()+1,user_name,user_pwd);
            if(DataBase->insertSql(*user))
            {
                //注册成功
                msg = "R#1";
            }
            else
            {
                //注册失败（原因？）
                msg = "R#0";
            }
        }
        else
        {
            //用户名已被使用
            msg = "R#3";
        }
        qDebug()<<msg;
    }
    else if(str[0] == 'M')
    {
        int idx2 = str.indexOf("/");
        QString msg;
        int recv_user = str.mid(12,idx2-12).toInt();
        msg = str.mid(idx2+9,-1);
        //qDebug()<<"识别到Message";
        slot_sendmsg(msg,Clients[recv_user]->Socket);
        qDebug()<<msg;
    }
    //qDebug()<<str[0];
    ui->textBrowser->append(str);
}

//服务器清除“死客户端”函数，用于清除异常断开or正常断开的客户端占用的坑位
void Tcp_Server::slot_disconnect(int location)
{
    QString a = "error!!";
    //qDebug()<<a;
    this->Clients[location] = nullptr;
    this->num[location] = 0;
    this->connect_sum--;
    this->client_menu_update();
    this->server_menu_update();

}

//刷新更新目前在线的客户端数目
void Tcp_Server::client_menu_update()
{
    for(int j=1;j<=9;j++)
    {
        if(num[j]!=0)
        {
            for(int i=1;i<=9;i++)
            {
                if(num[i]!=0)
                {
                    slot_sendmsg("在线用户location:"+QString::number(Clients[i]->location,10),Clients[j]->Socket);
                    slot_sendmsg("ip_address:"+Clients[i]->Socket->peerAddress().toString(),Clients[j]->Socket);
                }

            }
        }
    }

}

//服务器中在线客户端列表更新
void Tcp_Server::server_menu_update()
{
    ui->textBrowser->append("已连接数："+QString::number(connect_sum));
    if(connect_sum!=0)
    {
        ui->textBrowser->append("在线ip列表:");
        for(int i=1;i<=9;i++)
        {
            if(num[i]!=0)
            {
                ui->textBrowser->append("#"+Clients[i]->Socket->peerAddress().toString());
            }
        }
    }


}

//服务器更新清理函数，用于更新设备连接状态，清理释放过期的SOCKET
void Tcp_Server::update_Socket()
{
    for(int i=1;i<=9;i++)
    {
        if(num[i]!=0)
        {
            bool flag = WakeHand(Clients[i]->Socket);
            if(!flag)
            {
                slot_disconnect(i);
            }
        }
    }
}

//服务器挥手函数，用以给客户端发送消息检测此客户端是否和服务器正常通信
//(注意在客户端添加消息筛选机制以区分 挥手消息 和 客户端间通信的消息)
bool Tcp_Server::WakeHand(QTcpSocket *Socket)
{
    int ret = Socket->write("");
    if(ret == -1)
    {
        return false;
    }
    else
    {
        return true;
    }
}



