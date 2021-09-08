#ifndef TALKWINDOW_H
#define TALKWINDOW_H

#include <QWidget>
#include "QMouseEvent"
#include "tcpclientsocket.h"
#include "QPushButton"
#include<QDataStream>
#include<QMessageBox>
#include<QDateTime>
#include<QComboBox>
#include<QColorDialog>
#include<QFileDialog>
#include<QTime>
#include "QGraphicsBlurEffect"
#include "QPropertyAnimation"
#include "udpreciver.h"
#include "udpsender.h"
namespace Ui {
class TalkWindow;
}

class TalkWindow : public QWidget
{
    Q_OBJECT

public:
    explicit TalkWindow(QWidget *parent = nullptr, QString ="",QString ="");
//    explicit TalkWindow(QWidget *parent = nullptr);
    ~TalkWindow();
    void TalkInit();
    QString getName();//获取名字
    QString getMsg();//获取聊天信息
    QFile file;     //需要发送的文件对象
    QString fileName;
    int fileSize;

protected:
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    bool eventFilter(QObject *target, QEvent *event);//事件过滤器

private:
    int ShowMsg(QString,QString);

private slots:
    void on_close_clicked();
    void on_pushButton_clicked();
    void getMsg(QString);

    void on_hide_clicked();
    void on_close2_clicked();
    void file_ok(QString);
    void on_saveTbtn_clicked();

signals:
    void closeWidget();

private:
    Ui::TalkWindow *ui;
    bool mousePressed;//鼠标是否按下
    QPoint mousePoint;//鼠标（x，y轴）拖动自定义的坐标


    UdpSender * udpsender;
    UdpReciver * udpreceiver;

    TcpClientSocket * clientsocket;
    QString friendname;
    QString myname;

};

#endif // TALKWINDOW_H
