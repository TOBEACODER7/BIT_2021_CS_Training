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

//注册完帐号后生成该用户好友列表
bool db::Friend_table(QString username)
{
    QSqlDatabase db=QSqlDatabase::addDatabase("QMYSQL");
    db.setHostName("127.0.0.1");
    db.setDatabaseName("user_info");
    db.setPort(3306);
    db.setUserName("root");
    db.setPassword("admin");
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
    db.setDatabaseName("user_info");
    db.setPort(3306);
    db.setUserName("root");
    db.setPassword("admin");
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
        db.setDatabaseName("user_info");
        db.setPort(3306);
        db.setUserName("root");
        db.setPassword("admin");
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


    QString sql1=QString("INSERT INTO user_info VALUES(%1,'%2','%3','%4');")
            .arg(user.getUno())
            .arg(user.getName())
            .arg(user.getPhone())
            .arg(user.getemail());
    QString sql2=QString("INSERT INTO login_info VALUES(%1,'%2','%3');")
            .arg(user.getUno())
            .arg(user.getUsername())
            .arg(user.getPassword());
    QString sql3=QString("INSERT INTO state_info VALUES('%1','%2',0);")
            .arg(user.getUno())
            .arg(user.getUsername());

    QSqlQuery query(db);
    if(!query.exec(sql1)){
        qDebug()<<"user_info插入失败!原因是:"<< query.lastError().text();
        db.close();
        return false;
    }
    if(!query.exec(sql2)){
        qDebug()<<"login_info插入失败!原因是:"<< query.lastError().text();
        db.close();
        return false;
    }
    if(!query.exec(sql3)){
        qDebug()<<"state_info插入失败!原因是:"<< query.lastError().text();
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
bool db::changeState(QString username,int state){
    QSqlDatabase db=QSqlDatabase::addDatabase("QMYSQL");
    db.setHostName("127.0.0.1");
    db.setDatabaseName("user_info");
    db.setPort(3306);
    db.setUserName("root");
    db.setPassword("admin");
    //打开数据库
    if(!db.open())
    {
         qDebug()<<"数据库在函数changeState中打开失败!原因是:"<<db.lastError().text();
    }
    QString sql = QString("UPDATE state_info SET state=%1 WHERE username='%2';").arg(state).arg(username);
    QSqlQuery query(db);
    query.prepare(sql);
    if(!query.exec()){
        qDebug()<<"改变状态失败！原因是:"<< query.lastError().text();
        db.close();
        return false;
    }
    db.close();
    return true;
}
int db::selectState(QString username)
{
    int state;
    QSqlDatabase db=QSqlDatabase::addDatabase("QMYSQL");
    db.setHostName("127.0.0.1");
    db.setDatabaseName("user_info");
    db.setPort(3306);
    db.setUserName("root");
    db.setPassword("admin");
    //打开数据库
    if(!db.open())
    {
         qDebug()<<"数据库在函数selectState中打开失败!原因是:"<<db.lastError().text();
    }
    QString sql = QString("select state from state_info where username='%1';").arg(username);
    QSqlQuery query(db);
    query.prepare(sql);
    if(!query.exec()){
        qDebug()<<"查找状态失败！原因是:"<< query.lastError().text();
        db.close();
        return -1;
    }
    query.next();
    state=query.value(0).toInt();
    db.close();
    return state;
}
QString db::getUsernameByUno(int uno)
{
    QSqlDatabase db=QSqlDatabase::addDatabase("QMYSQL");
    db.setHostName("127.0.0.1");
    db.setDatabaseName("user_info");
    db.setPort(3306);
    db.setUserName("root");
    db.setPassword("admin");
    if(!db.open())
    {
         qDebug()<<"数据库在函数getUsernameByUno打开失败!原因是:"<<db.lastError().text();
    }

    QString sql=QString("SELECT username FROM login_info where uno=%1;").arg(uno);
    QSqlQuery query(db);
    if(!query.exec(sql)){
        qDebug()<<"通过uno查询username失败！原因是:"<< query.lastError().text();
        db.close();
        return "unkown";
    }
    query.next();
    QString username=query.value(0).toString();
    qDebug()<<uno<<"对应的username是:"<<username;
    db.close();
    return username;
}

bool db::clearState(){
    int num=getNum();
    for(int i=0;i<num;i++){
        qDebug()<<"username是"<<getUsernameByUno(i)<<"的清零结果是："<<changeState(getUsernameByUno(i),0);
    }
    return true;
}

bool db::changeUsername(int uno,QString new_username)
{
    QString username=getUsernameByUno(uno);
    QSqlDatabase db=QSqlDatabase::addDatabase("QMYSQL");
    db.setHostName("127.0.0.1");
    db.setDatabaseName("qtsql");
    db.setPort(3306);
    db.setUserName("root");
    db.setPassword("123456");
    //打开数据库
    if(!db.open())
    {
         qDebug()<<"数据库在函数changeUseranme中打开失败!原因是:"<<db.lastError().text();
    }
    QString sql1 = QString("UPDATE login_info SET username=%1 WHERE uno=%2;").arg(new_username).arg(uno);
    QString sql2 = QString("UPDATE online_info SET username=%1 WHERE uno=%2;").arg(new_username).arg(uno);
    QString sql3 = QString("rename table `%1` to `%2`;").arg(username).arg(new_username);
    QSqlQuery query(db);
    if(!query.exec(sql1)){
        qDebug()<<"改变login_info中username失败！原因是:"<< query.lastError().text();
        db.close();
        return false;
    }
    if(!query.exec(sql2)){
        qDebug()<<"改变online_info中username失败！原因是:"<< query.lastError().text();
        db.close();
        return false;
    }
    if(!query.exec(sql3)){
        qDebug()<<"改变friendtable名字失败！原因是:"<< query.lastError().text();
        db.close();
        return false;
    }
    db.close();
    if(!usedUno(uno)){
        return false;
    }
    qDebug()<<"改变username成功!";
    return true;
}
bool db::changePassword(int uno, QString new_password){
    QSqlDatabase db=QSqlDatabase::addDatabase("QMYSQL");
    db.setHostName("127.0.0.1");
    db.setDatabaseName("qtsql");
    db.setPort(3306);
    db.setUserName("root");
    db.setPassword("123456");
    //打开数据库
    if(!db.open())
    {
         qDebug()<<"数据库在函数changePassword中打开失败!原因是:"<<db.lastError().text();
    }
    QString sql = QString("UPDATE login_info SET password=%1 WHERE uno=%2;").arg(new_password).arg(uno);
    QSqlQuery query(db);
    if(!query.exec(sql)){
        qDebug()<<"改变login_info中password失败！原因是:"<< query.lastError().text();
        db.close();
        return false;
    }
    db.close();
    if(!usedUno(uno)){
        return false;
    }
    qDebug()<<"改变password成功！";
    return true;
}
bool db::changeName(int uno, QString new_name){
    QSqlDatabase db=QSqlDatabase::addDatabase("QMYSQL");
    db.setHostName("127.0.0.1");
    db.setDatabaseName("qtsql");
    db.setPort(3306);
    db.setUserName("root");
    db.setPassword("123456");
    //打开数据库
    if(!db.open())
    {
         qDebug()<<"数据库在函数changeName中打开失败!原因是:"<<db.lastError().text();
    }
    QString sql = QString("UPDATE user_info SET name='%1' WHERE uno=%2;").arg(new_name).arg(uno);
    QSqlQuery query(db);
    if(!query.exec(sql)){
        qDebug()<<"改变user_info中name失败！原因是:"<< query.lastError().text();
        db.close();
        return false;
    }
    db.close();
    if(!usedUno(uno)){
        return false;
    }
    qDebug()<<"改变name成功！";
    return true;
}
bool db::changePhone(int uno, QString new_phone){
    QSqlDatabase db=QSqlDatabase::addDatabase("QMYSQL");
    db.setHostName("127.0.0.1");
    db.setDatabaseName("qtsql");
    db.setPort(3306);
    db.setUserName("root");
    db.setPassword("123456");
    //打开数据库
    if(!db.open())
    {
         qDebug()<<"数据库在函数changePhone中打开失败!原因是:"<<db.lastError().text();
    }
    QString sql = QString("UPDATE user_info SET phone='%1' WHERE uno=%2;").arg(new_phone).arg(uno);
    QSqlQuery query(db);
    if(!query.exec(sql)){
        qDebug()<<"改变user_info中phone失败！原因是:"<< query.lastError().text();
        db.close();
        return false;
    }
    db.close();
    if(!usedUno(uno)){
        return false;
    }
    qDebug()<<"改变phone成功！";
    return true;
}
bool db::changeEmail(int uno, QString new_email){
    QSqlDatabase db=QSqlDatabase::addDatabase("QMYSQL");
    db.setHostName("127.0.0.1");
    db.setDatabaseName("qtsql");
    db.setPort(3306);
    db.setUserName("root");
    db.setPassword("123456");
    //打开数据库
    if(!db.open())
    {
         qDebug()<<"数据库在函数changeEmail中打开失败!原因是:"<<db.lastError().text();
    }
    QString sql = QString("UPDATE user_info SET email='%1' WHERE uno=%2;").arg(new_email).arg(uno);
    QSqlQuery query(db);
    if(!query.exec(sql)){
        qDebug()<<"改变user_info中email失败！原因是:"<< query.lastError().text();
        db.close();
        return false;
    }
    db.close();
    if(!usedUno(uno)){
        return false;
    }
    qDebug()<<"改变email成功！";
    return true;
}

int db::getUnoByUsername(QString username)
{
    QSqlDatabase db=QSqlDatabase::addDatabase("QMYSQL");
    db.setHostName("127.0.0.1");
    db.setDatabaseName("qtsql");
    db.setPort(3306);
    db.setUserName("root");
    db.setPassword("123456");
    if(!db.open())
    {
         qDebug()<<"数据库在函数getUnoByUsername打开失败!原因是:"<<db.lastError().text();
    }

    QString sql=QString("SELECT uno FROM login_info where username='%1';").arg(username);
    QSqlQuery query(db);
    if(!query.exec(sql)){
        qDebug()<<"通过username查询uno失败！原因是:"<< query.lastError().text();
        db.close();
        return -1;
    }
    if(query.next()){
        int uno=query.value(0).toInt();
        qDebug()<<username<<"对应的uno是:"<<uno;
        db.close();
        return uno;
    }
    db.close();
    return -1;
}
QString db::getPasswordByUno(int uno)
{
    QSqlDatabase db=QSqlDatabase::addDatabase("QMYSQL");
    db.setHostName("127.0.0.1");
    db.setDatabaseName("qtsql");
    db.setPort(3306);
    db.setUserName("root");
    db.setPassword("123456");
    if(!db.open())
    {
         qDebug()<<"数据库在函数getPasswordByUno打开失败!原因是:"<<db.lastError().text();
    }

    QString sql=QString("SELECT password FROM login_info where uno=%1;").arg(uno);
    QSqlQuery query(db);
    if(!query.exec(sql)){
        qDebug()<<"通过uno查询password失败！原因是:"<< query.lastError().text();
        db.close();
        return "unknown";
    }
    if(query.next()){
        QString password=query.value(0).toString();
        qDebug()<<uno<<"对应的password是:"<<password;
        db.close();
        return password;
    }
    db.close();
    return "unknown";
}
QString db::getNameByUno(int uno){
    QSqlDatabase db=QSqlDatabase::addDatabase("QMYSQL");
    db.setHostName("127.0.0.1");
    db.setDatabaseName("qtsql");
    db.setPort(3306);
    db.setUserName("root");
    db.setPassword("123456");
    if(!db.open())
    {
         qDebug()<<"数据库在函数getNameByUno打开失败!原因是:"<<db.lastError().text();
    }

    QString sql=QString("SELECT name FROM user_info where uno=%1;").arg(uno);
    QSqlQuery query(db);
    if(!query.exec(sql)){
        qDebug()<<"通过uno查询name失败！原因是:"<< query.lastError().text();
        db.close();
        return "unknown";
    }
    if(query.next()){
        QString name=query.value(0).toString();
        qDebug()<<uno<<"对应的name是:"<<name;
        db.close();
        return name;
    }
    db.close();
    return "unknown";
}
QString db::getPhoneByUno(int uno){
    QSqlDatabase db=QSqlDatabase::addDatabase("QMYSQL");
    db.setHostName("127.0.0.1");
    db.setDatabaseName("qtsql");
    db.setPort(3306);
    db.setUserName("root");
    db.setPassword("123456");
    if(!db.open())
    {
         qDebug()<<"数据库在函数getPhoneByUno打开失败!原因是:"<<db.lastError().text();
    }

    QString sql=QString("SELECT phone FROM user_info where uno=%1;").arg(uno);
    QSqlQuery query(db);
    if(!query.exec(sql)){
        qDebug()<<"通过uno查询phone失败！原因是:"<< query.lastError().text();
        db.close();
        return "unknown";
    }
    if(query.next()){
        QString phone=query.value(0).toString();
        qDebug()<<uno<<"对应的phone是:"<<phone;
        db.close();
        return phone;
    }
    db.close();
    return "unknown";
}
QString db::getEmailByUno(int uno){
    QSqlDatabase db=QSqlDatabase::addDatabase("QMYSQL");
    db.setHostName("127.0.0.1");
    db.setDatabaseName("qtsql");
    db.setPort(3306);
    db.setUserName("root");
    db.setPassword("123456");
    if(!db.open())
    {
         qDebug()<<"数据库在函数getEmailByUno打开失败!原因是:"<<db.lastError().text();
    }

    QString sql=QString("SELECT email FROM user_info where uno=%1;").arg(uno);
    QSqlQuery query(db);
    if(!query.exec(sql)){
        qDebug()<<"通过uno查询email失败！原因是:"<< query.lastError().text();
        db.close();
        return "unknown";
    }
    if(query.next()){
        QString email=query.value(0).toString();
        qDebug()<<uno<<"对应的email是:"<<email;
        db.close();
        return email;
    }
    db.close();
    return "unknown";
}

bool db::deleteUSer(int uno)
{
    QString username=getUsernameByUno(uno);
    QSqlDatabase db=QSqlDatabase::addDatabase("QMYSQL");
    db.setHostName("127.0.0.1");
    db.setDatabaseName("qtsql");
    db.setPort(3306);
    db.setUserName("root");
    db.setPassword("123456");
    if(!db.open())
    {
         qDebug()<<"数据库在函数deleteUSer打开失败!原因是:"<<db.lastError().text();
         db.close();
         return false;
    }
    QString sql1=QString("delete FROM user_info where uno=%1").arg(uno);
    QString sql2=QString("delete FROM login_info where uno=%1").arg(uno);
    QString sql3=QString("delete FROM online_info where uno=%1").arg(uno);
    QString sql4=QString("drop table %1;").arg(username);
    QString sql5=QString("delete FROM used_uno where usednum=%1").arg(uno);
    QSqlQuery query(db);
    if(!query.exec(sql1)){
        qDebug()<<"user_info删除用户失败！原因是:"<< query.lastError().text();
        db.close();
        return false;
    }
    if(!query.exec(sql2)){
        qDebug()<<"login_info删除用户失败！原因是:"<< query.lastError().text();
        db.close();
        return false;
    }
    if(!query.exec(sql3)){
        qDebug()<<"online_info删除用户失败！原因是:"<< query.lastError().text();
        db.close();
        return false;
    }
    if(!query.exec(sql4)){
        qDebug()<<"删除friendtable失败！原因是:"<< query.lastError().text();
        db.close();
        return false;
    }
    if(!query.exec(sql5)){
        qDebug()<<"删除used_uno失败！原因是:"<< query.lastError().text();
        db.close();
        return false;
    }
    qDebug()<<"用户："<<username<<"已经注销";
    db.close();
    return true;
}
bool db::usedUno(int uno){
    QSqlDatabase db=QSqlDatabase::addDatabase("QMYSQL");
    db.setHostName("127.0.0.1");
    db.setDatabaseName("qtsql");
    db.setPort(3306);
    db.setUserName("root");
    db.setPassword("123456");
    if(!db.open())
    {
         qDebug()<<"数据库在函数usedUno中打开失败!原因是:"<<db.lastError().text();
    }
    QString sql = QString("select usednum from used_uno where usednum = %1;").arg(uno);
    QSqlQuery query(db);
    query.prepare(sql);
    query.exec();
    //查询结束
    if(query.next())
    {
        //匹配成功
        qDebug()<<query.value(0).toInt();
        if(uno==query.value(0).toInt()){
            db.close();
            qDebug()<<"uno:"<<uno<<"已存在！";
            return true;
        }
    }
    //匹配失败
    db.close();
    return false;
}
