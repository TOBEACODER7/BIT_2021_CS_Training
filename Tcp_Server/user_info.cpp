#include "user_info.h"

user_info::user_info(QString username_,
                     QString password_,
                     QString name_="unkown",
                     QString phone_="unkown",
                     QString email_="unkown"):
                     username(username_),
                     password(password_),
                     name(name_),
                     phone(phone_),
                     email(email_)

{
        qDebug()<<"user_info build success！";
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

