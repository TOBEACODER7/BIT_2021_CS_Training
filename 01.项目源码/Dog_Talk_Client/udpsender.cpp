#include "udpsender.h"

UdpSender::UdpSender()
{
    m_Socket = new QUdpSocket();
    initSender("10.194.148.192", 8192);
    //接受者ip
}
UdpSender::~UdpSender()
{
    if (m_Socket != nullptr)
    {
        delete m_Socket;
        m_Socket = nullptr;
    }
}
void UdpSender::initSender(QString desHost, int port)
{
    m_address = desHost;
    m_port = port;
}
void UdpSender::send_file(QByteArray msg)
{
    qDebug()<<"in send file";
    m_Socket->writeDatagram(msg, QHostAddress(m_address), m_port);
}
void UdpSender::recMsg(QByteArray msgInfo)
{
    send_file(msgInfo);
}
