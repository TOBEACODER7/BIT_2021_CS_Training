#ifndef ENROLL_H
#define ENROLL_H

#include <QWidget>
#include <QMessageBox>
#include <QMouseEvent>
#include "tcpclientsocket.h"
#include "QPropertyAnimation"
namespace Ui {
class Enroll;
}

class Enroll : public QWidget
{
    Q_OBJECT

public:
    explicit Enroll(QWidget *parent = nullptr);
    ~Enroll();
    bool CheckPwd();

protected:
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);

private slots:
    void EnrollInit();

    void success();//登录成功
    void failure();//登录失败
    void name_exists();//用户名存在

    void on_putin_clicked();//注册
    void on_close_btn_clicked();//关闭返回

    void on_name_editingFinished();
    void on_email_textEdited(const QString &arg1);
    void on_phonenumber_textEdited(const QString &arg1);



signals:
    void send(QString b,QString c);//发送用户名和密码到登录界面

private:
    Ui::Enroll *ui;
    bool mousePressed;//鼠标是否按下
    QPoint mousePoint;//鼠标（x，y轴）拖动自定义的坐标
    TcpClientSocket *clientSocket;
};

#endif // ENROLL_H
