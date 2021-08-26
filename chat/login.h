#ifndef LOGIN_H
#define LOGIN_H

#include <QWidget>
#include "QMouseEvent"

namespace Ui {
class Login;
}

class Login : public QWidget
{
    Q_OBJECT

public:
    explicit Login(QWidget *parent = nullptr);
    ~Login();

protected:
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);

private slots:
    void on_login_clicked();

    void on_close_clicked();

    void on_enroll_clicked();

private:
    Ui::Login *ui;
    bool mousePressed;//鼠标是否按下
    QPoint mousePoint;//鼠标（x，y轴）拖动自定义的坐标
};

#endif // LOGIN_H
