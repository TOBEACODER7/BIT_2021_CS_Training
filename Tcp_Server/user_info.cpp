#include "user_info.h"

user_info::user_info(int uno_,
                     QString username_,
                     QString password_,
                     QString name_,
                     QString phone_,
                     QString email_):
                     uno(uno_),
                     username(username_),
                     password(password_),
                     name(name_),
                     phone(phone_),
                     email(email_)

{
        qDebug()<<"user_info 对象创建成功！,已知个数=6";
}

user_info::user_info(int uno_,
                     QString username_,
                     QString password_):
                     uno(uno_),
                     username(username_),
                     password(password_)

{
    name="unkown";
    phone="unkown";
    email="unkown";
    qDebug()<<"user_info 对象创建成功！,已知个数=3";
}

user_info::user_info(int uno_,
                     QString username_,
                     QString password_,
                     QString name_):
                     uno(uno_),
                     username(username_),
                     password(password_),
                     name(name_)
{
    phone="unkown";
    email="unkown";
    qDebug()<<"user_info 对象创建成功！,已知个数=4";
}

user_info::user_info(int uno_,
                     QString username_,
                     QString password_,
                     QString name_,
                     QString phone_):
                     uno(uno_),
                     username(username_),
                     password(password_),
                     name(name_),
                     phone(phone_)

{
    email="unkown";
    qDebug()<<"user_info 对象创建成功！,已知个数=5";
}

int user_info::getUno(){
    return uno;
}
QString user_info::getUsername(){
    return  username;
}
QString user_info::getPassword(){
    return  password;
}
QString user_info::getName(){
    return  name;
}
QString user_info::getPhone(){
    return  phone;
}
QString user_info::getemail(){
    return  email;
}
