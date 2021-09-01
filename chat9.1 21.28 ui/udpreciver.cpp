#include "udpreciver.h"

UdpReciver::UdpReciver(QObject *parent) :
    QObject(parent)
{
    qDebug()<<"进入reciver";
    m_thread = new QThread();
    m_udpSocket = new QUdpSocket();
    QString localhost = "";
    init_port("10.195.203.21", 8192);
    qDebug()<<"Here";
    connect(m_udpSocket, SIGNAL(readyRead()), this, SLOT(readDatagrams()), Qt::DirectConnection);
    this->moveToThread(m_thread);
    m_udpSocket->moveToThread(m_thread);
    m_thread->start();
}
UdpReciver::~UdpReciver()
{
    m_thread->quit();
    m_thread->wait();
    m_thread->deleteLater();
    m_udpSocket->close();
    m_udpSocket->deleteLater();
}
void UdpReciver::init_port(QString tmphost, int tmport)
{
    m_port = tmport;
    m_localhost = tmphost;
    m_udpSocket->bind(QHostAddress(m_localhost), m_port);
}
void UdpReciver::readDatagrams()
{
    QFile final_file;
    qDebug()<<"刚进来";
    QString File_name;
    qDebug()<<"in readDatagrams";
    QHostAddress client_address; //client ip addr
    m_data.clear();
    while(m_udpSocket->hasPendingDatagrams())
    {

        m_data.resize(m_udpSocket->pendingDatagramSize());

        m_udpSocket->readDatagram(m_data.data(), m_data.size(), &client_address);
        if(m_data.contains("&&"))
        {
            int idx1 = m_data.indexOf("&&");
            int idx2 = m_data.indexOf("##");
            File_name = m_data.mid(idx1+2,idx2-idx1-2);
            qDebug()<<"文件名："<<File_name;
            m_data.clear();


            //指定路径输出文件
            final_file.setFileName("D:/Qt/Project/"+File_name);
            qDebug()<<"真实文件名："<<final_file.fileName();
            continue;
        }
        QString strclient_address = client_address.toString();
        deliverInfo(m_data, strclient_address);




        qDebug() << "receive UDP's size:"<< m_data.size();
    }
    if(final_file.exists())
    {
        qDebug()<<"文件存在";
    }
    else
    {
        qDebug()<<"文件不存在";
    }
    QString str(m_data);
    if(!final_file.open(QIODevice::ReadWrite))
    {
        qDebug()<<"打开失败";
    }
    else
    {
        qDebug()<<"打开成功";
    }
    int flag = final_file.write(str.toUtf8(),m_data.size());
    if(flag == -1)
    {
        qDebug()<<"文件写入失败";
    }
    final_file.close();
    //qDebug()<<"这是m_data:"<<m_data;
    m_data.clear();
}
