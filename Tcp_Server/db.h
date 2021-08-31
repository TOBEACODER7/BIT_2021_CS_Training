#ifndef DB_H
#define DB_H
#include"user_info.h"
#include"QSqlDatabase"
#include"QString"
#include <QSqlQueryModel>

class db
{
private:
public:
    db();
    ~db();
    //把用户信息插入数据库,ture为插入成功，false为插入失败
    bool insertSql(user_info &user);
    //通过用户名在数据库查找，返回true为查找到，用户名已经存在，false为不存在
    bool selectSql(QString username);
    //判断用户名密码是否存在，是否匹配
    bool loginJudge(QString username,QString password);
    //获取记录个数
    int  getNum();
    //只要一注册，就添加一个表，需要提供用户名 如（王岩）表示王岩注册为用户，true表示注册成功
    bool login(QString);
    //添加好友，需要给加好友的人和被加好友的人，如（王岩，王储）表示王岩添加王储为好友，true表示添加成功
    bool addFriend(QString,QString);
    //查找好友是否存在 如（王岩，王储）表示查找王储是否是王岩的好友，true表示是
    bool selectFriend(QString,QString);
    //通过username改变登录状态
    bool changeState (QString,int);
    //通过uno查找登录状态
    int  selectState(int);
    QString getUsernameByUno(int);
    //通过username获取uno
    int getUnoByUsername(QString);
    //通过uno获取password
    QString getPasswordByUno(int);
    //通过uno获取name
    QString getNameByUno(int);
    //通过uno获取phone
    QString getPhoneByUno(int);
    //通过uno获取email
    QString getEmailByUno(int);
};

#endif // DB_H
