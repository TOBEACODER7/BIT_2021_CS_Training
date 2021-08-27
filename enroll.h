#ifndef ENROLL_H
#define ENROLL_H

#include <QWidget>
#include <QMessageBox>
#include "tcpclientsocket.h"
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

private slots:
    void success();//登录成功
    void failure();//登录失败
    void on_putin_clicked();
    void on_name_editingFinished();

private:
    Ui::Enroll *ui;
    TcpClientSocket *clientSocket;
};

#endif // ENROLL_H
