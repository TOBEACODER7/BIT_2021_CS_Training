#ifndef BACKGROUND_H
#define BACKGROUND_H

#include <QWidget>
#include <QMouseEvent>

namespace Ui {
class BackGround;
}

class BackGround : public QWidget
{
    Q_OBJECT

public:
    explicit BackGround(QWidget *parent = nullptr);
    ~BackGround();

private slots:


    void on_hide_clicked();
    void on_close_clicked();
    void on_bg1_clicked();
    void on_bg2_clicked();
    void on_bg3_clicked();
    void on_bg4_clicked();
    void on_bg5_clicked();
    void on_bg6_clicked();
    void on_bg7_clicked();
    void on_bg8_clicked();
    void on_bg9_clicked();

signals:
    void change1();
    void change2();
    void change3();
    void change4();
    void change5();
    void change6();
    void change7();
    void change8();
    void change9();

protected:
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);

private:
    Ui::BackGround *ui;
    bool mousePressed;//鼠标是否按下
    QPoint mousePoint;//鼠标（x，y轴）拖动自定义的坐标
};

#endif // BACKGROUND_H
