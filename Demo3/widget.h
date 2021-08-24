#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QTcpSocket>

namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = nullptr);
    ~Widget();
private slots:
    void recvMsg();
    void on_pushButton_clicked();

private:
    Ui::Widget *ui;
    QTcpSocket *clientSocket;
};

#endif // WIDGET_H
