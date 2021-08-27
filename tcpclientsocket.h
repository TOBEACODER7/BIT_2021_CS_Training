#ifndef TCPCLIENTSOCKET_H
#define TCPCLIENTSOCKET_H

#include <QTcpSocket>

class TcpClientSocket : public QTcpSocket
{
    Q_OBJECT
public:
    explicit TcpClientSocket(QObject *parent = nullptr);

    //设置服务器IP
    void setServerIP(QString serverIP);
    //设置服务器端口
    void setServerTcpPort(QString serverTcpPort);

    //获得IP,端口
    QString getServerIP();
    QString getServerPort();

    //测试链接
    bool testConnect();
    bool connectToServer();

    //获得sever信息
    QString getServerInfo();

signals:
    void dealMessage(QString msg);
    void login_no_name();
    void login_success();
    void login_failure();
    void enroll_failure();
    void enroll_success();

public slots:
    void receiveMessage();

private:
    QString _serverIP;//服务器IP
    int _serverTcpPort;//服务器Port
    QString info;
};

#endif // TCPCLIENTSOCKET_H
