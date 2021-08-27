#ifndef USER_INFO_H
#define USER_INFO_H
#include"QString"
#include"QDebug"

//建立用户信息的对象，用于储存用户信息
//分别为：序号,用户名，密码，姓名，电话号码，电子邮箱
//其中，序号，用户名和密码不能为空，其他可以为空，默认值为“unknown”
//public中的方法为获取信息的途径

class user_info
{
public:
    user_info(int,QString,QString,QString,QString,QString);
    user_info(int,QString,QString,QString,QString);
    user_info(int,QString,QString,QString);
    user_info(int,QString,QString);
    QString getUsername();
    QString getPassword();
    QString getName();
    QString getPhone();
    QString getemail();
    int     getUno();
private:
    int uno;
    QString username;
    QString password;
    QString name;
    QString phone;
    QString email;
};

#endif // USER_INFO_H
