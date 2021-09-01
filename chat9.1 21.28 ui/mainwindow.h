#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include "talkwindow.h"
#include "enroll.h"
#include <QWidget>
#include "QMouseEvent"
#include "tcpclientsocket.h"
#include "QPropertyAnimation"
#include <QToolButton>
namespace Ui {
class MainWindow;
}

class MainWindow : public QWidget
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr,QString = "");
    ~MainWindow();

private:
    void MainInit();
    void fresh_friend_list(bool,QStringList);


protected:
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);

private slots:
    void on_changebackground_clicked();
    void on_defaultbackground_clicked();
    void on_newfriend_clicked();
    void add_friend(QString);
    void friend_list(QString);
    void on_close_clicked();
    void on_hide_clicked();
    void on_newform_clicked();


private:
    QToolButton *btn;
    Ui::MainWindow *ui;
    QString my_name;
    QStringList on_friend;
    QStringList off_friend;
    QVector< QToolButton *> on;
    QVector< QToolButton *> off;
    TcpClientSocket *clientSocket;
    QVector<bool> IsShow_on;
    QVector<bool> IsShow_off;
    bool mousePressed;//鼠标是否按下
    QPoint mousePoint;//鼠标（x，y轴）拖动自定义的坐标
};

#endif // MAINWINDOW_H
