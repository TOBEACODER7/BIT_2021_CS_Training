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
};

#endif // DB_H
