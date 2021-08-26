#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QWidget>
#include "QMouseEvent"

namespace Ui {
class MainWindow;
}

class MainWindow : public QWidget
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

protected:
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);

private slots:
    void on_changebackground_clicked();

    void on_defaultbackground_clicked();

    void on_newform_clicked();

    void on_pushButton_clicked();

private:
    Ui::MainWindow *ui;
    bool mousePressed;//鼠标是否按下
    QPoint mousePoint;//鼠标（x，y轴）拖动自定义的坐标
};

#endif // MAINWINDOW_H
