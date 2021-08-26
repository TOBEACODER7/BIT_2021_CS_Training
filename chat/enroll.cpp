#include "enroll.h"
#include "ui_enroll.h"

Enroll::Enroll(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Enroll)
{
    ui->setupUi(this);
}

Enroll::~Enroll()
{
    delete ui;
}


void Enroll::on_putin_clicked()
{
    this->close();
}
