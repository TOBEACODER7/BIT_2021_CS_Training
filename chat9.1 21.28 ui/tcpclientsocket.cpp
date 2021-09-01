#include "tcpclientsocket.h"
#include <QHostAddress>
#include <QtDebug>
#include <QMessageBox>

TcpClientSocket* TcpClientSocket:: clientsocket =nullptr;

TcpClientSocket::TcpClientSocket(QObject *parent) :
    QTcpSocket(parent)
{
    //有可读信息后，需要用receiveMessage进行处理
    connect(this, SIGNAL(readyRead()),this, SLOT(receiveMessage()));

}

//设置IP
void TcpClientSocket::setServerIP(QString serverIP)
{
    _serverIP = serverIP;
}

//设置Port
void TcpClientSocket::setServerTcpPort(QString serverTcpPort)
{
    _serverTcpPort = serverTcpPort.toInt();
}

//获得IP
QString TcpClientSocket::getServerIP()
{
    return _serverIP;
}

//获得Port
QString TcpClientSocket::getServerPort()
{
    return QString::number(_serverTcpPort);
}

//测试服务器是否能够连接，成功后关闭连接
bool TcpClientSocket::testConnect()
{
    disconnectFromHost();
    connectToHost(QHostAddress(this->_serverIP),this->_serverTcpPort);
    bool ret = waitForConnected();//等待连接

    if(ret) qDebug()<<"Connected!";

    disconnectFromHost();//断开连接，返回ret
    return ret;
}

//连接服务器，返回true表示连接完成，否则未连接。
bool TcpClientSocket::connectToServer()
{
    //初始化ret,且立即断开所有链接
    bool ret = true;
    abort();

    //ret检测是否连接
    if( (ret = testConnect()) == false )
    {
        return ret;
    }
    connectToHost(QHostAddress(this->_serverIP),this->_serverTcpPort);
    return ret;
}

//收到来自服务器的消息，用于判断接下来的操作
void TcpClientSocket::receiveMessage(){
    QByteArray buf = this->readAll();
    qDebug()<<"in receiveMessage ，对info判断:";
    QString info = buf;
    qDebug()<<info;
    if(info==""){
        emit dealMessage(info);
    }
    else if(info[0]=='L'){
        if(info[2]=='1'){
            emit login_success();
        }
        else if(info[2]=="0"){
            emit login_failure();
        }
    }
    else if(info=="R#0"){
        emit enroll_failure();
    }
    else if(info=="R#1"){
        emit enroll_success();
    }
    else if(info[0]=='U'){//用户不存在
        if(info[2]=='1'){
            emit login_no_name();
        }
        else if(info[2]=='0'){//用户存在
            emit enroll_name_exists();
        }
    }
    else if(info.contains("F")){
        emit friend_add(info);
    }
    else if(info.contains("M")){
        emit recvMsg(info);
    }
    else if(info.contains("%")||info.contains("offline")||info.contains("online")){
        emit return_friend_list(info);
    }
}

int TcpClientSocket::sendMsg(QString msg){
    qDebug()<<msg<<"in socket sendMsg";
    write(msg.toUtf8());
    return 1;
}

TcpClientSocket* TcpClientSocket::getInstance(){
    qDebug()<<"in get instance"<<endl;
    if(clientsocket==nullptr){
        clientsocket = new TcpClientSocket;
    }
    return clientsocket;
}
