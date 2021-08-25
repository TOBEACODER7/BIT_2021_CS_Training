#include "denglu.h"
#include "ui_denglu.h"
#include<QIcon>
#include<QToolButton>
#include"widget.h"
#include<QMessageBox>
denglu::denglu(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::denglu)
{
    ui->setupUi(this);
    //设置图标
    //路径: 冒号+前缀+路径
    this->setWindowIcon(QIcon(":/images/qq.png"));
    //设置名称
    this->setWindowTitle("qq 2020");
    QList<QString> nameList;
    nameList<<"性感小飞侠"<<"闷骚男"<<"韩国欧巴"<<"神经病一枚"<<"笨笨猪"<<"狗头"<<"蛇精病"<<"尼古拉斯赵四"<<"本山兄弟";
    QStringList iconNameList; //图标资源列表
    iconNameList << "spqy"<< "ymrl" <<"qq" <<"Cherry"<< "dr"
                  <<"jj"<<"lswh"<<"qmnn"<<"spqy";
    QVector< QToolButton *> vector;
    for(int i=0;i<9;i++)
    {
        QToolButton *btn=new QToolButton(this);
        //加载图标
        btn->setIcon(QPixmap(QString(":/images/%1.png").arg(iconNameList[i])));
        //设置图片大小
        btn->setIconSize(QPixmap(QString(":/images/%1.png").arg(iconNameList[i])).size());
        //设置网名
        btn->setText(QString("%1").arg(nameList[i]));
        //设置为透明
        btn->setAutoRaise(true);
        //设置显示格式
        btn->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
        //放到vlayout布局中
        ui->vlayout->addWidget(btn);
        vector.push_back(btn);
        IsShow.push_back(false);

    }
    for(int i=0;i<9;i++)
    {
        connect(vector[i],&QToolButton::clicked,[=](){
            if(IsShow[i])
            {
                QMessageBox::warning(this,"警告","该聊天框已被打开!");
                return;
            }
            IsShow[i]=true;
            Widget *widget=new Widget(nullptr,vector[i]->text());
            widget->setWindowIcon(vector[i]->icon());
            widget->setWindowTitle(vector[i]->text());
            widget->show();
            //关闭时将对应的IsShow变为false;
            connect(widget,&Widget::closeWidget,this,[=](){
                IsShow[i]=false;
            });
        });
    }

}
denglu::~denglu()
{
    delete ui;
}
