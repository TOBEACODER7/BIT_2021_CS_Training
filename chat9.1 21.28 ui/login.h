#ifndef LOGIN_H
#define LOGIN_H

#include <QPropertyAnimation>
#include <QSettings>
#include <QWidget>
#include <QMouseEvent>
#include "tcpclientsocket.h"

namespace Ui {
class Login;
}

class Login : public QWidget
{
    Q_OBJECT

public:
    explicit Login(QWidget *parent = nullptr);
    ~Login();

private:
    void LoginInit();
    void LoadConfig();
    void LoginData();
    void ReadInit(QString,QStringList&);
    void WriteInit(QString,QStringList);
protected:
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    bool eventFilter(QObject *target, QEvent *event);//事件过滤器

private slots:
    void SetLogin(int);
    void on_login_clicked();//登录按钮
    void on_close_clicked();//关闭按钮
    void on_enroll_clicked();//注册界面
    void success();//登录成功
    void failure();//登录失败
    void no_name();//用户不存在


    void receive(QString suername,QString passward);

    void on_checkBox_clicked();

    void on_name_currentTextChanged(const QString &arg1);

    void on_hide_clicked();

private:
    Ui::Login *ui;
    bool mousePressed;//鼠标是否按下
    QPoint mousePoint;//鼠标（x，y轴）拖动自定义的坐标
    TcpClientSocket *clientSocket;

    QString pwd;
    QStringList pwdlist;
    QString name;
    QStringList namelist;
    QString rem;
    QStringList remlist;
};

#endif // LOGIN_H
