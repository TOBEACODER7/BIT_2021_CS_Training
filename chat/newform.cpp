#include "newform.h"
#include "ui_newform.h"

NewForm::NewForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::NewForm)
{
    ui->setupUi(this);
}

NewForm::~NewForm()
{
    delete ui;
}
