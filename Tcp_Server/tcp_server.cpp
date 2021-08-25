#include "tcp_server.h"
#include "ui_tcp_server.h"
#include <QMessageBox>
#include <QDateTime>
#include <QTimer>

Tcp_Server::Tcp_Server(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Tcp_Server)
{
    ui->setupUi(this);

    //初始化
    TCP_Server = new QTcpServer();
    TCP_connectSocket = nullptr;
    memset(num,0,sizeof (num));

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
    connect_sum++;
    ui->textBrowser->append("已连接数："+QString::number(connect_sum)+"最新连接ip："+TCP_connectSocket->peerAddress().toString());
    Client *client = new Client(this,TCP_connectSocket,location);
    num[location] = 1;
    this->Clients[location] = client;
    /*
    for(int i=1;i<=connect_sum;i++)
    {
        QString a = QString::number(i,10);//int转换为QString
        slot_sendmsg("在线用户"+a+':'+"location:"+QString::number(Clients[i]->location,10)+'/'+"ip_address:"+Clients[i]->Socket->peerAddress().toString(),TCP_connectSocket);
    }
    */
    this->menu_update();
    //当客户端有通信发出时
    connect(Clients[location]->Socket,SIGNAL(readyRead()),Clients[location],SLOT(slot_Read()));
    //当客户端断开连接时，让出服务器的位置（未实现）
    connect(Clients[location]->Socket,SIGNAL(error(QAbstractSocket::SocketError)),this,SLOT(slot_disconnect(location)));

}

//服务器发送函数，用于反馈异常信息or进行客户端之间通信时作为转发函数
void Tcp_Server::slot_sendmsg(QString str,QTcpSocket *Socket)
{
    std::string str1 = str.toStdString();
    const char *data = str1.c_str();
    Socket->write(data);

}

//服务器信息接收函数，用于接收来自客户端的各种信息
void Tcp_Server::recvmsg(QString str)
{
    //格式解析
    if(str[0] == 'L')
    {
        int idx2 = str.indexOf("/");
        QString user_name = str.mid(11,idx2-11);
        QString user_pwd = str.mid(idx2+10,-1);
    }
    else if(str[0] == 'R')
    {
        int idx2 = str.indexOf("/");
        QString user_name = str.mid(11,idx2-11);
        QString user_pwd = str.mid(idx2+10,-1);
    }
    else if(str[0] == 'M')
    {
        int idx2 = str.indexOf("/");
        int recv_user = str.mid(12,idx2-12).toInt();
        QString msg = str.mid(idx2+9,-1);
        slot_sendmsg(msg,Clients[recv_user]->Socket);
    }

    ui->textBrowser->append(str);
}

//服务器清除“死客户端”函数，用于清除异常断开or正常断开的客户端占用的坑位
void Tcp_Server::slot_disconnect(int location)
{
    QString a = "error!!";
    qDebug()<<a;
    this->Clients[location] = nullptr;
    this->num[location] = 0;
    this->connect_sum--;
    this->menu_update();

}

//刷新更新目前在线的客户端数目
void Tcp_Server::menu_update()
{
    for(int i=1;i<=9;i++)
    {
        if(num[i]!=0)
        {
            slot_sendmsg("在线用户location:"+QString::number(Clients[i]->location,10)+'/'+"ip_address:"+Clients[i]->Socket->peerAddress().toString(),TCP_connectSocket);
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
    int ret = Socket->write("wake hands");
    if(ret == -1)
    {
        return false;
    }
    else
    {
        return true;
    }
}



