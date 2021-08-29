#ifndef TCP_SERVER_H
#define TCP_SERVER_H

#include <QWidget>
#include <QTcpServer>
#include <QTcpSocket>
#include "client.h"
#include "user_info.h"
#include <QMap>
#include "db.h"

namespace Ui {
class Tcp_Server;
}

class Tcp_Server : public QWidget
{
    Q_OBJECT

public:
    explicit Tcp_Server(QWidget *parent = nullptr);
    ~Tcp_Server();
    //void getMsg(QString str);

    //int num[10];//记录服务器中位置是否被连接，若已连接则值为1，否则为NULL
    //Client *Clients[10];//用于存放客户端指针的数组
    QMap<int,Client> map1;
    int location = 0;
    int connect_sum = 0;//记录目前已经连接的客户端总数
    void recvmsg(QString str);
    void client_menu_update();
    void server_menu_update();
    void Timer();
private slots:
    void slot_newconnect();
    void slot_sendmsg(QString str,int send_id,int recv_id);//用于发送信息至客户端
    void slot_disconnect(int location);
    void update_Socket();
    bool WakeHand(QTcpSocket *Socket);

    //void slot_disconnect();

private:
    Ui::Tcp_Server *ui;
    int check_num();//检查目前服务器中剩余的空位，若有空位则返回空位坐标，否则返回0
    QTcpServer *TCP_Server;
    QTcpSocket *TCP_connectSocket;
    user_info *user;
    db *DataBase;
};

#endif // TCP_SERVER_H
