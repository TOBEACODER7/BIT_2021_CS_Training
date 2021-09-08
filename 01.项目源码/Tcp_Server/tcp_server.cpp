#include "tcp_server.h"
#include "ui_tcp_server.h"
#include <QMessageBox>
#include <QDateTime>
#include <QTimer>
#include "user_info.h"
#include <QMap>
#include "db.h"

Tcp_Server::Tcp_Server(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Tcp_Server)
{
    ui->setupUi(this);

    //初始化
    TCP_Server = new QTcpServer();
    TCP_connectSocket = nullptr;

    DataBase = new db();
    for(int i=1;i <= DataBase->getNum(); i++)
    {
        QString name = DataBase->getUsernameByUno(i);
        DataBase->changeState(name,0);
    }

    //调用listen函数监听同时绑定IP和端口号
    if(TCP_Server->listen(QHostAddress::Any,6666))
    {
        this->connect(TCP_Server,SIGNAL(newConnection()),this,SLOT(slot_newconnect()));

        //利用自实现的心跳包机制，定期给服务器发送信息来检测异常断开的情况
        QTimer *timer = new QTimer(this);
        connect(timer,SIGNAL(timeout()),this,SLOT(update_Socket()));
        timer->start(1000);
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

//服务器坑位检测函数，用以检测服务器目前是否还有可用的坑位
/*
int Tcp_Server::check_num()
{
    for(int i=1;i<=9;i++)
    {
        if(this->num[i] == 0)
        {
            return i;
        }
    }
    return 0;
}
*/

void Tcp_Server::slot_newconnect()
{
    TCP_connectSocket = TCP_Server->nextPendingConnection();
    qDebug()<<"有新的客户端接入";
    location++;

    connect_sum++;


    client[location] = new Client(this,TCP_connectSocket,location);

    this->map1.insert(location,*client[location]);

    //服务器列表更新
    this->server_menu_update();


    //客户端列表更新
    this->client_menu_update();

    //当客户端有通信发出时
    connect(map1[location].Socket,SIGNAL(readyRead()),&map1[location],SLOT(slot_Read()));

}

//服务器发送函数，用于反馈异常信息or进行客户端之间通信时作为转发函数
void Tcp_Server::slot_sendmsg(QString str,int send_id,int recv_id)
{
    if(send_id == 0)
    {
        std::string str1 = str.toStdString();
        const char *data = str1.c_str();
        map1.value(recv_id).Socket->write(data);
    }
    else
    {
        std::string str1 = send_id+":"+str.toStdString();
        const char *data = str1.c_str();
        map1.value(recv_id).Socket->write(data);
        //qDebug()<< "sendhanshu";
    }


}

//服务器信息接收函数，用于接收来自客户端的各种信息
void Tcp_Server::recvmsg(QString str,int recv_id)
{
    qDebug()<<"这里是接收到的信息"<<str;
    //格式解析
    //qDebug()<<"接收函数";
    if(str[0] == 'L')
    {
        int idx2 = str.indexOf("/password");
        int idx3 = str.indexOf("From");
        QString msg;
        QString user_name = str.mid(11,idx2-11);
        QString user_pwd = str.mid(idx2+10,idx3-idx2-10);

        if((DataBase->selectSql(user_name)))
        {
            //用户名存在

            if(DataBase->loginJudge(user_name,user_pwd))
            {
                if(DataBase->selectState(user_name)==0)
                {
                    //登录成功
                    msg = "L#1";
                    DataBase->changeState(user_name,recv_id);
                    //DataBase->changeState(user_name,this->location);

                }
                else
                {
                    //该用户已在其他客户端登录
                    msg = "L#2";
                }

            }
            else
            {
                //登录失败
                msg ="L#0";
            }
        }
        else
        {
            qDebug()<<"这里的登录有问题";
            //用户名不存在，无法登录
            msg = "U#1";
        }
        qDebug()<< msg;
        slot_sendmsg(msg,0,recv_id);




        //服务器列表更新
        this->server_menu_update();


        //客户端列表更新
        this->client_menu_update();





    }
    else if(str[0] == 'R')
    {

        int idx2 = str.indexOf("/email");
        int idx3 = str.indexOf("/username");
        int idx4 = str.indexOf("/password");
        int idx5 = str.indexOf("From");
        QString msg;
        QString phone = str.mid(8,11);
        QString email = str.mid(26,idx3-26);
        QString user_name = str.mid(idx3+10,idx4-idx3-10);
        QString user_pwd = str.mid(idx4+10,idx5-idx4-10);

        qDebug()<<"--------------------------------------------------";
        qDebug()<<phone;
        qDebug()<<email;
        qDebug()<<user_name;
        qDebug()<<user_pwd;
        qDebug()<<"--------------------------------------------------";

        if(!DataBase->selectSql(user_name))
        {


            user = new user_info(DataBase->getNum()+1,user_name,user_pwd);
            qDebug()<<DataBase->getNum();
            if(DataBase->insertSql(*user))
            {
                //注册成功
                msg = "R#1";
                if(DataBase->Friend_table(user_name))
                {
                    qDebug()<<user_name<<"好友表注册成功";
                }
                else
                {
                    qDebug()<<user_name<<"好友表注册失败";
                }

            }
            else
            {
                //注册失败（原因？）
                msg = "R#0";
            }
        }
        else
        {
            //用户名已被使用
            msg = "U#0";
        }

        qDebug()<<msg;

        slot_sendmsg(msg,0,recv_id);
    }
    else if(str[0] == 'M')
    {
        qDebug()<<"消息解析";
        int idx2 = str.indexOf("/");
        int idx3 = str.indexOf("|");
        int idx4 = str.indexOf("From");
        QString msg;
        QString send_name = str.mid(12,idx2-12);
        QString recv_name = str.mid(idx2+11,idx3-idx2-11);
        int send_location = DataBase->selectState(send_name);
        int recv_location = DataBase->selectState(recv_name);
        qDebug()<<"$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$";
        qDebug()<<"send_name"<<send_name;
        qDebug()<<"recv_name"<<recv_name;
        qDebug()<<"send_loaction"<<send_location;
        qDebug()<<"recv_location"<<recv_location;
        qDebug()<<"$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$";

        if(DataBase->selectFriend(send_name,recv_name))
        {
            qDebug()<<"两个人是好友，可以发送信息";


            if(recv_location == 0)
            {
                qDebug()<<"该用户离线";
                msg = "M#1";
                slot_sendmsg(msg,0,send_location);
            }
            else
            {
                qDebug()<<"可以发消息";
                msg = "M#0"+send_name+":"+str.mid(idx3+8,idx4-idx3-8);
                slot_sendmsg(msg,send_location,recv_location);
            }

        }
        else
        {
            qDebug()<<"该用户不是你的好友";
            msg = "M#2";
            slot_sendmsg(msg,0,send_location);
        }



        qDebug()<<msg;


    }
    else if(str[0] == 'U')
    {
        int idx1 = str.indexOf("#");
        int idx2 = str.indexOf("From");
        QString user_name = str.mid(idx1+1,idx2-idx1-1);
        qDebug()<<user_name;
        QString msg;
        if(DataBase->selectSql(user_name))
        {
            msg = "U#0";//用户名存在
        }
        else
        {
            msg = "U#1";//用户名不存在
        }
        slot_sendmsg(msg,0,recv_id);
    }

    else if(str[0] == 'F')
    {
        int idx1 = str.indexOf("/");
        int idx2 = str.indexOf("From");
        QString My_name = str.mid(8,idx1-8);
        QString Friend_name = str.mid(idx1+11,idx2-idx1-11);
        qDebug()<<"@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@";
        qDebug()<<My_name;
        qDebug()<<Friend_name;

        QString msg;
        if(DataBase->selectSql(Friend_name))
        {
            //查找的用户存在
            if(DataBase->selectFriend(My_name,Friend_name))
                    {
                        //两个人是好友
                        msg = "F#0";
                    }
                    else
                    {
                        //彼此加为好友（在彼此的好友表中添加姓名）
                        if(DataBase->addFriend(My_name,Friend_name) && DataBase->addFriend(Friend_name,My_name))
                        {
                            //加好友成功
                            msg = "F#1";
                            //服务器列表更新
                            this->server_menu_update();


                            //客户端列表更新
                            this->client_menu_update();



                        }
                        else
                        {
                            //系统错误，加好友失败
                            msg = "F#2";
                        }
                    }
        }
        else
        {
            //查找的用户不存在
            msg = "F#3";
        }

        slot_sendmsg(msg,0,DataBase->selectState(My_name));
        qDebug()<<"##########"<<msg;

    }

    else if(str[0] == 'G')
    {
        int idx1 = str.indexOf("From");
        QString user_name = str.mid(10,idx1-10);
        //服务器列表更新
        this->server_menu_update();


        //客户端列表更新
        this->client_menu_update();

    }
    else if(str[0] == "D")
    {
        int idx2 = str.indexOf("/");
        int idx3 = str.indexOf("|");

        QString msg;
        QString send_name = str.mid(12,idx2-12);
        QString recv_name = str.mid(idx2+11,idx3-idx2-11);
        QString send_ip = map1[DataBase->selectState(send_name)].address.toString();
        QString recv_ip = map1[DataBase->selectState(recv_name)].address.toString();
        slot_sendmsg("!@#$%^&*|"+recv_ip+"|",0,DataBase->selectState(send_name));
        //slot_sendmsg("!@#$%^&*|"+send_ip+"|",0,DataBase->selectState(recv_name));

    }
    //qDebug()<<str[0];
    ui->textBrowser->append(str);
}

//服务器清除“死客户端”函数，用于清除异常断开or正常断开的客户端占用的坑位
void Tcp_Server::slot_disconnect(int location)
{


    for(int i = 1;i <= DataBase->getNum(); i++)
    {
        //通过主键找username，再通过username找state，如果state=location，则state=0
        QString name = DataBase->getUsernameByUno(i);

        if(name != "unknow")
        {
            if(DataBase->selectState(name) == location)
            {
                qDebug()<<"要删除啦！！！！！！";
                DataBase->changeState(name,0);
            }
        }

    }

    map1.remove(location);

    this->connect_sum--;
    this->client_menu_update();
    this->server_menu_update();

}

//刷新更新目前在线的客户端数目
void Tcp_Server::client_menu_update()
{
    //foreach (Client user,map1.values())
    //{

        for(int i=1;i <= DataBase->getNum(); i++)
        {

            QString name = DataBase->getUsernameByUno(i);
            qDebug()<<"有在更新好友列表";
            qDebug()<<"好友的名字是"<<name;
            //说明这个name用户在线
            if(DataBase->selectState(name) != 0)
            {
                QString online_name = "online_name:";
                QString offline_name = "offline_name:";
                qDebug()<<"第一层循环";
                for(int j=1;j<=DataBase->getNum();j++)
                {
                    qDebug()<<"第二层循环";

                    if(i!=j)
                    {
                        qDebug()<<"第一层条件";
                        QString friend_name = DataBase->getUsernameByUno(j);
                        if(DataBase->selectFriend(name,friend_name))
                        {
                            qDebug()<<"第二层条件";
                            if( DataBase->selectState(friend_name) != 0)
                            {
                                online_name+="@"+friend_name;
                                //QString online_name = "online_name:"
                                //slot_sendmsg("online_name:"+friend_name,0,DataBase->selectState(name));
                            }
                            else
                            {
                                offline_name+="@"+friend_name;
                                //slot_sendmsg("offline_name:"+friend_name,0,DataBase->selectState(name));
                            }


                        }
                    }


                }

                slot_sendmsg("%"+online_name+"%",0,DataBase->selectState(name));
                slot_sendmsg("%"+offline_name+"%",0,DataBase->selectState(name));
            }
        }



    //}


}

//服务器中在线客户端列表更新
void Tcp_Server::server_menu_update()
{
    ui->textBrowser->append("已连接数："+QString::number(connect_sum));
    if(connect_sum!=0)
    {
        ui->textBrowser->append("在线ip列表:");


        foreach(Client user,map1.values())
        {
            ui->textBrowser->append("#"+user.Socket->peerAddress().toString());
            if(DataBase->getNum()!=0)
            {
                for(int i=1;i<=DataBase->getNum();i++)
                {
                    qDebug()<<i;
                    QString name = DataBase->getUsernameByUno(i);
                    if(DataBase->selectState(name)!=0)
                    {
                        ui->textBrowser->append("在线用户name:"+name+"用户客户端:"+QString::number(DataBase->selectState(name),10));

                    }
                    else
                    {
                        ui->textBrowser->append("不在线用户name:"+name);
                    }
                }
            }

        }


    }


}

//服务器更新清理函数，用于更新设备连接状态，清理释放过期的SOCKET
void Tcp_Server::update_Socket()
{
    foreach(Client user,map1.values())
    {
        bool flag = WakeHand(user.Socket);
        if(!flag)
        {
            slot_disconnect(user.location);
        }
    }

}

//服务器挥手函数，用以给客户端发送消息检测此客户端是否和服务器正常通信
//(注意在客户端添加消息筛选机制以区分 挥手消息 和 客户端间通信的消息)
bool Tcp_Server::WakeHand(QTcpSocket *Socket)
{
    int ret = Socket->write("");
    if(ret == -1)
    {
        return false;
    }
    else
    {
        return true;
    }
}



