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
    db.setDatabaseName("qtsql");
    db.setPort(3306);
    db.setUserName("root");
    db.setPassword("123456");
    if(!db.open())
    {
         qDebug()<<"数据库在函数getNum打开失败!原因是:"<<db.lastError().text();
    }

    QString sql=QString("SELECT * FROM user_info");
    QSqlQuery query(db);
    if(!query.exec(sql)){
        qDebug()<<"获取数据个数失败！原因是:"<< query.lastError().text();
        db.close();
        return 0;
    }
    int num=query.size();
    qDebug()<<"数据库行数:"<<num;
    db.close();
    return num;
}

bool db::login(QString username)
{
    QSqlDatabase db=QSqlDatabase::addDatabase("QMYSQL");
    db.setHostName("127.0.0.1");
    db.setDatabaseName("qtsql");
    db.setPort(3306);
    db.setUserName("root");
    db.setPassword("123456");
    if(!db.open())
    {
         qDebug()<<"数据库在函数login打开失败!原因是:"<<db.lastError().text();
         db.close();
         return false;
    }
    QString sql=QString("create table %1(Friends varchar(255) not null,primary key(Friends));").arg(username  );
    QSqlQuery query(db);
    query.prepare(sql);
//  query.bindValue(":name",QVariant(username));
    if(!query.exec()){
        qDebug()<<"注册表建立失败！原因是:"<< query.lastError().text();
        db.close();
        return false;
    }
    db.close();
    return true;
}

bool db::addFriend(QString user,QString friends){
    QSqlDatabase db=QSqlDatabase::addDatabase("QMYSQL");
    db.setHostName("127.0.0.1");
    db.setDatabaseName("qtsql");
    db.setPort(3306);
    db.setUserName("root");
    db.setPassword("123456");
    if(!db.open())
    {
         qDebug()<<"数据库在函数addFriend打开失败!原因是:"<<db.lastError().text();
         db.close();
         return false;
    }
    QString sql=QString("insert into %1 value('%2')").arg(user).arg(friends);
    QSqlQuery query(db);
    query.prepare(sql);
    if(!query.exec()){
        qDebug()<<"加好友失败！原因是:"<< query.lastError().text();
        db.close();
        return false;
    }
    db.close();
    return true;
}

bool db::selectFriend(QString user, QString friends)
{
        QSqlDatabase db=QSqlDatabase::addDatabase("QMYSQL");
        db.setHostName("127.0.0.1");
        db.setDatabaseName("qtsql");
        db.setPort(3306);
        db.setUserName("root");
        db.setPassword("123456");
        //打开数据库
        if(!db.open())
        {
             qDebug()<<"数据库在函数selectFriend中打开失败!原因是:"<<db.lastError().text();
        }
        QString sql = QString("select friends from %1 where friends = '%2';").arg(user).arg(friends);
        QSqlQuery query(db);
        query.prepare(sql);
        query.exec();
        //查询结束
        qDebug()<<query.value(1).toString();
        if(query.next())
        {
            //匹配成功
            qDebug()<<query.value(0).toString();
            if(friends==query.value(0).toString()){
                db.close();
                return true;
            }
        }
        //匹配失败
        db.close();
        return false;

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
