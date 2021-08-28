#include "user_info.h"
#include"QString"
#include"QDebug"
#include <QSql>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QSqlQueryModel>
#include "db.h"


db::db()
{
   qDebug()<<"数据库对象建立!";
}

db::~db()
{
   qDebug()<<"数据库对象释放!";
}



int db::getNum(){

    QSqlDatabase db=QSqlDatabase::addDatabase("QMYSQL");
    db.setHostName("127.0.0.1");
    db.setDatabaseName("user_info");
    db.setPort(3306);
    db.setUserName("root");
    db.setPassword("admin");
    if(!db.open())
    {
         qDebug()<<"数据库在函数getNum打开失败!原因是:"<<db.lastError().text();
    }
    db.close();


    QSqlQuery query(db);
    query.exec ( "SELECT * FROM user_info");
    QSqlQueryModel *queryModel = new QSqlQueryModel();
    queryModel->setQuery(query);
    int nRecordcount = queryModel->rowCount();
    return nRecordcount;
}

bool db::insertSql(user_info &user){


    QSqlDatabase db=QSqlDatabase::addDatabase("QMYSQL");
    db.setHostName("127.0.0.1");
    db.setDatabaseName("user_info");
    db.setPort(3306);
    db.setUserName("root");
    db.setPassword("admin");
    if(!db.open())
    {
         qDebug()<<"数据库在函数 insertSql 打开失败!原因是:"<<db.lastError().text();
    }


    QString sql1=QString("INSERT INTO user_info VALUES('%1','%2','%3','%4');")
            .arg(user.getUno())
            .arg(user.getName())
            .arg(user.getPhone())
            .arg(user.getemail());
    QString sql2=QString("INSERT INTO login_info VALUES('%1','%2','%3');")
            .arg(user.getUno())
            .arg(user.getUsername())
            .arg(user.getPassword());

    QSqlQuery query(db);
    if(!query.exec(sql1)){
        qDebug()<<"插入失败!原因是:"<< query.lastError().text();
        db.close();
        return false;
    }
    if(!query.exec(sql2)){
        qDebug()<<"插入失败!原因是:"<< query.lastError().text();
        db.close();
        return false;
    }
    qDebug()<<"插入成功!";
    db.close();
    return true;
}

bool db::selectSql(QString username)
{

    QSqlDatabase db=QSqlDatabase::addDatabase("QMYSQL");
    db.setHostName("127.0.0.1");
    db.setDatabaseName("user_info");
    db.setPort(3306);
    db.setUserName("root");
    db.setPassword("admin");


    //打开数据库
    if(!db.open())
    {
         qDebug()<<"数据库在函数 selectSql中打开失败!原因是:"<<db.lastError().text();
    }


    QString sql = QString("select username from login_info where username = :name;");
    QSqlQuery query(db);
    query.prepare(sql);
    query.bindValue(":name",QVariant(username));
    query.exec();
    //查询结束
    if(query.next())
    {
        //匹配成功
        qDebug()<<query.value(0).toString();
        if(username==query.value(0).toString()){
            db.close();
            return true;
        }
    }
    //匹配失败
    db.close();
    return false;
}

bool db::loginJudge(QString username, QString password)
{

    QSqlDatabase db=QSqlDatabase::addDatabase("QMYSQL");
    db.setHostName("127.0.0.1");
    db.setDatabaseName("user_info");
    db.setPort(3306);
    db.setUserName("root");
    db.setPassword("admin");


    if(!db.open())
    {
         qDebug()<<"数据库在函数loginJudge打开失败！原因是："<<db.lastError().text();
    }


    QString sql = QString("select username,password from login_info where username = :name;");
    QSqlQuery query(db);
    query.prepare(sql);
    query.bindValue(":name",QVariant(username));
    query.exec();
    //查询结束
    if(query.next())
    {
        qDebug()<<query.value(1).toString();
        if(password==query.value(1).toString()){
            //密码正确
            db.close();
            return true;
        }
    }
    //密码错误
    db.close();
    return false;
}
