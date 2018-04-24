#include "helpwidget.h"
#include "ui_helpwidget.h"

helpWidget::helpWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::helpWidget)
{
    ui->setupUi(this);
    this->setFixedSize(300, 500);
    helpCodec = QTextCodec::codecForName("GB18030");
    this->setWindowTitle(helpCodec->toUnicode("帮助"));
    base = 80, diff = 45, label_width = 300, label_height = 25;

    lbLineH1=new QLabel(this);
    lbLineH1->setGeometry(base, diff*1, label_width, label_height);
    lbLineH1->setText(helpCodec->toUnicode("绘制横线1 —— 1"));
    lbLineH2=new QLabel(this);
    lbLineH2->setGeometry(base, diff*2, label_width, label_height);
    lbLineH2->setText(helpCodec->toUnicode("绘制横线2 —— 2"));
    lbLineV1=new QLabel(this);
    lbLineV1->setGeometry(base, diff*3, label_width, label_height);
    lbLineV1->setText(helpCodec->toUnicode("绘制竖线1 —— 3"));
    lbLineV2=new QLabel(this);
    lbLineV2->setGeometry(base, diff*4, label_width, label_height);
    lbLineV2->setText(helpCodec->toUnicode("绘制竖线2 —— 4"));
    lbSave=new QLabel(this);
    lbSave->setGeometry(base, diff*5, label_width, label_height);
    lbSave->setText(helpCodec->toUnicode("截图 —— S"));
    lbLoadPic=new QLabel(this);
    lbLoadPic->setGeometry(base, diff*6, label_width, label_height);
    lbLoadPic->setText(helpCodec->toUnicode("加载图片 —— F"));
    lbESC=new QLabel(this);
    lbESC->setGeometry(base, diff*7, label_width, label_height);
    lbESC->setText(helpCodec->toUnicode("切换至视频流 —— ESC"));
    lbQuit=new QLabel(this);
    lbQuit->setGeometry(base, diff*8, label_width, label_height);
    lbQuit->setText(helpCodec->toUnicode("退出 —— Q"));
}

helpWidget::~helpWidget()
{
    delete ui;
}
