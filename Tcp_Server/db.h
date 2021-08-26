#ifndef DB_H
#define DB_H
#include"user_info.h"
#include"QSqlDatabase"
#include"QString"

class db
{
private:
    db();
    QSqlDatabase db_;
    ~db();
public:
    //静态获取数据库对象
    static db *getDatabaseIntence();
    //把用户信息插入数据库,ture为插入成功，false为插入失败
    bool insertSql(user_info &user);
    //通过用户名在数据库查找，返回true为查找到，用户名已经存在，false为不存在
    bool selectSql(QString username);
    //判断用户名密码是否存在，是否匹配
    //true为匹配成功，密码正确，false为匹配失败，密码错误
    bool loginJudge(QString username,QString password);
};

#endif // DB_H


