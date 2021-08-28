#include "client.h"
#include "tcp_server.h"

Client::Client(Tcp_Server *p,QTcpSocket *Socket,int num)
{
    this->Socket = Socket;
    this->parent = p;
    this->location = num;
    this->address = Socket->peerAddress();

}
Client::~Client(){}
void Client::slot_Read()
{
    QString str = Socket->readAll();
    str = str + "From" + this->address.toString();
    //qDebug()<<str;
    parent->recvmsg(str);
}
