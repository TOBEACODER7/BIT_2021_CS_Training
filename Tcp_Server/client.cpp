#include "client.h"
#include "tcp_server.h"

Client::Client(Tcp_Server *p,QTcpSocket *Socket,int num)
{
    this->Socket = Socket;
    this->parent = p;
    this->location = num;
    this->address = Socket->peerAddress();

}
Client::Client()
{
    this->Socket=nullptr;
    this->parent=nullptr;

    this->location=0;
}
Client::~Client(){}
void Client::slot_Read()
{
    QString str = Socket->readAll();
    str = str + "From" + this->address.toString();
    //qDebug()<<str;
    parent->recvmsg(str);
}
Client::Client(const Client &x)
{
    this->Socket=x.Socket;
    this->address=x.address;
    this->parent=x.parent;
    this->location=x.location;
}
Client& Client::operator=(const Client &x)
{
    this->Socket=x.Socket;
    this->address=x.address;
    this->location=x.location;
    this->parent=x.parent;

}
