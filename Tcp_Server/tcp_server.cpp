#include "tcp_server.h"
#include "ui_tcp_server.h"
#include <QMessageBox>
#include <QDateTime>

Tcp_Server::Tcp_Server(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Tcp_Server)
{
    ui->setupUi(this);

    //初始化
    TCP_Server = new QTcpServer();
    TCP_connectSocket = nullptr;
    connect(ui->pushButton,SIGNAL(clicked()),this,SLOT(slot_sendmsg()));

    //调用listen函数监听同时绑定IP和端口号
    if(TCP_Server->listen(QHostAddress::LocalHost,6666))
    {
        this->connect(TCP_Server,SIGNAL(newConnection()),this,SLOT(slot_newconnect()));

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
int Tcp_Server::check_num()
{
    for(int i=1;i<=2;i++)
    {
        if(this->num[i] == NULL)
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
    connect(Clients[location]->Socket,SIGNAL(readyRead()),Clients[location],SLOT(slot_Read()));


}
void Tcp_Server::slot_sendmsg(QString str,QTcpSocket *Socket)
{
    std::string str1 = str.toStdString();
    const char *data = str1.c_str();
    Socket->write(data);

}
void Tcp_Server::recvmsg(QString str)
{
    ui->textBrowser->append(str);
}
