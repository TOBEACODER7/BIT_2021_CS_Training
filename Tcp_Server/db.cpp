#include "user_info.h"
#include"QString"
#include"QDebug"
#include <QSql>
#include <QSqlDatabase>
#include <QDebug>
#include <QSqlError>
#include <QSqlQuery>
#include <QSqlRecord>
#include "db.h"

db::db()
{
    //创建数据库数据库
    QSqlDatabase db_=QSqlDatabase::addDatabase("QMYSQL");
    db_.setHostName("127.0.0.1");
    db_.setDatabaseName("user_info");
    db_.setPort(3306);
    db_.setUserName("root");
    db_.setPassword("ly1426847100!");
    //打开数据库
    if(!db_.open())
    {
         qDebug()<<"Database open fail!Because:"<<db_.lastError().text();
    }

}

db::~db()
{
    db_.close();
}

db * db::getDatabaseIntence(){
    static db mysql;
    return &mysql;
}
//数据库表名为userinfo，里面有username，password，name，phone，email四个char类型的表，建议都设置成非空
bool db::insertSql(user_info &user){
    QString sql=QString("INSERT INTO user_info VALUES('1%','2%','3%','4%','5%')")
            .arg(user.getUsername())
            .arg(user.getPassword())
            .arg(user.getName())
            .arg(user.getPhone())
            .arg(user.getemail());
    QSqlQuery query(db_);
    if(!query.exec(sql)){
        qDebug()<<"Insert fail!Because:"<< query.lastError().text();
        return false;
    }
    qDebug()<<"Insert success!";
    return true;
}

bool db::selectSql(QString username)
{
    QString sql = QString("select username from user_info where name = :name;");
    QSqlQuery query(db_);
    query.prepare(sql);
    query.bindValue(":name",QVariant(username));
    query.exec();
    //查询结束
    if(query.next())
    {
        //匹配成功
        if(username==query.value(0).toString()){
            return true;
        }
    }
    //匹配失败
    return false;
}

bool db::loginJudge(QString username, QString password)
{
    QString sql = QString("select username,password from user_info where name = :name;");
    QSqlQuery query(db_);
    query.prepare(sql);
    query.bindValue(":name",QVariant(username));
    query.exec();
    //查询结束
    if(query.next())
    {
        if(password==query.value(1).toString()){
            //密码正确
            return true;
        }
    }
    //密码错误
    return false;
}
