#include "helpwidget.h"
#include "ui_helpwidget.h"

helpWidget::helpWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::helpWidget)
{
    ui->setupUi(this);
    this->setFixedSize(300, 500);
    helpCodec = QTextCodec::codecForName("GB18030");
    this->setWindowTitle(helpCodec->toUnicode("����"));
    base = 80, diff = 45, label_width = 300, label_height = 25;

    lbLineH1=new QLabel(this);
    lbLineH1->setGeometry(base, diff*1, label_width, label_height);
    lbLineH1->setText(helpCodec->toUnicode("���ƺ���1 ���� 1"));
    lbLineH2=new QLabel(this);
    lbLineH2->setGeometry(base, diff*2, label_width, label_height);
    lbLineH2->setText(helpCodec->toUnicode("���ƺ���2 ���� 2"));
    lbLineV1=new QLabel(this);
    lbLineV1->setGeometry(base, diff*3, label_width, label_height);
    lbLineV1->setText(helpCodec->toUnicode("��������1 ���� 3"));
    lbLineV2=new QLabel(this);
    lbLineV2->setGeometry(base, diff*4, label_width, label_height);
    lbLineV2->setText(helpCodec->toUnicode("��������2 ���� 4"));
    lbSave=new QLabel(this);
    lbSave->setGeometry(base, diff*5, label_width, label_height);
    lbSave->setText(helpCodec->toUnicode("��ͼ ���� S"));
    lbLoadPic=new QLabel(this);
    lbLoadPic->setGeometry(base, diff*6, label_width, label_height);
    lbLoadPic->setText(helpCodec->toUnicode("����ͼƬ ���� F"));
    lbESC=new QLabel(this);
    lbESC->setGeometry(base, diff*7, label_width, label_height);
    lbESC->setText(helpCodec->toUnicode("�л�����Ƶ�� ���� ESC"));
    lbQuit=new QLabel(this);
    lbQuit->setGeometry(base, diff*8, label_width, label_height);
    lbQuit->setText(helpCodec->toUnicode("�˳� ���� Q"));
}

helpWidget::~helpWidget()
{
    delete ui;
}
