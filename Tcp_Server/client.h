#ifndef CLIENT_H
#define CLIENT_H
#include<QtNetwork>
#include <QObject>
#include <QTcpSocket>
class Tcp_Server;
class Client : public QObject
{
    Q_OBJECT
public:
    QTcpSocket *Socket = nullptr;
    int location;//用于记录当前客户端在服务器中的位置信息
    QHostAddress address;//用于记录客户端的ip地址
    Tcp_Server *parent;//记录其所连接的服务器的指针
    Client();
    Client(const Client &x);
    Client(Tcp_Server *p,QTcpSocket *Socket,int num);
    ~Client();
    Client& operator=(const Client &x);
public slots:
    void slot_Read();
};

#endif // CLIENT_H
