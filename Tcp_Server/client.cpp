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
    if(Socket!=nullptr)
    {
        QString str = Socket->readAll();
        str = str + "From" + this->address.toString();
        //int recv_id = this->parent->map1.key(*this);
        int recv_id = this->location;
        //qDebug()<<str;
        parent->recvmsg(str,recv_id);
    }

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
/*
bool Client::operator==(Client x)
{
    if(this->Socket==x.Socket && this->parent==x.parent && this->address==x.address && this->location==x.location)
    {
        return true;
    }
    else
    {
        return false;
    }
}
*/
