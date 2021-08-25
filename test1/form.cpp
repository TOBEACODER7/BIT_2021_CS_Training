#include "form.h"
#include "ui_form.h"
#include "widget.h"

Form::Form(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Form)
{
    ui->setupUi(this);
    ui->lineEdit->setPlaceholderText("输入用户名/手机号/邮箱");//用户名输入框提示信息
    ui->lineEdit_2->setPlaceholderText("输入8-16位");//密码输入框提示信息
}

Form::~Form()
{
    delete ui;
}

void Form::on_pushButton_clicked()
{
    Widget *widget = new Widget;
    widget->show();//close();
}
