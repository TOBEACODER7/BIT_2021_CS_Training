#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "newform.h"

MainWindow::MainWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->search->setPlaceholderText("搜索");//搜索输入框提示信息
    this->setWindowFlags(Qt::FramelessWindowHint);//去边框
    this->setWindowTitle("QQ");//设置左上角文字
    this->setWindowIcon(QIcon(":/image/QQ.png"));//设置左上角图标
}

MainWindow::~MainWindow()
{
    delete ui;
}

//更换背景
void MainWindow::on_changebackground_clicked()
{
    this->setStyleSheet("QWidget#widget{background-image:url(:/image/ss.png);}"
                        "QToolButton{border-style:none;}");
}

//换回默认背景
void MainWindow::on_defaultbackground_clicked()
{
    this->setStyleSheet("QWidget#widget{background-image:url(:/image/moon.png);}"
                        "QToolButton{border-style:none;}");
}

//开启新窗口函数
void MainWindow::on_newform_clicked()
{
    NewForm *newform = new NewForm;
    newform->show();//close();
}

//关闭窗口
void MainWindow::on_pushButton_clicked()
{
    this->close();
}


void MainWindow::mousePressEvent(QMouseEvent *event)
{
    if(event->button() == Qt::LeftButton)
    {
        mousePressed = true;//按下状态
        mousePoint = event->globalPos() - this->pos();//寻找相对位置
        event->accept();
    }
}

void MainWindow::mouseMoveEvent(QMouseEvent *event)
{
    if(mousePressed && (event->buttons() && Qt::LeftButton))
    {
        this->move(event->globalPos() - mousePoint);
        event->accept();
    }
}

void MainWindow::mouseReleaseEvent(QMouseEvent *event)
{
    mousePressed = false;
}
