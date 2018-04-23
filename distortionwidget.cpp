#include "distortionwidget.h"
#include "ui_distortionwidget.h"

bool _init=true;
char* show_img = (char*)malloc(imgwidth*imgheight*3);

distortionWidget::distortionWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::distortionWidget)
{
    initWidget();
}

distortionWidget::~distortionWidget()
{
    delete ui;
}

void distortionWidget::initWidget()
{
    ui->setupUi(this);
    uint base = 20, diff = 25, btn_width = 50, btn_height = 25;

    distorationCodec = QTextCodec::codecForName("GB18030");
    this->setFixedSize(800, 645);
    this->setWindowTitle(distorationCodec->toUnicode("����У��"));

    lpic = new QLabel(this);
    lpic->setGeometry(0, 0, 800, 600);

    pbRefresh = new QPushButton(this);
    pbRefresh->setText(distorationCodec->toUnicode("ˢ��"));
    pbRefresh->setGeometry(base + (btn_width + diff)*1, 610, btn_width, btn_height);
    connect(pbRefresh, SIGNAL(clicked()), this, SLOT(distoration_refresh()));


//    pbToFile = new QPushButton(this);
//    pbToFile->setText(distorationCodec->toUnicode("��ͼ"));
//    pbToFile->setGeometry(base + (btn_width + diff)*2, 610, btn_width, btn_height);
//    connect(pbToFile, SIGNAL(clicked()), this, SLOT(distoration_tofile()));

    lRatioInfo = new QLabel(this);
    lRatioInfo->setGeometry(base + (btn_width + diff)*8, 610, btn_width, btn_height);
    lRatioInfo->setNum(imgratio);

    slider = new QSlider(this);
    slider->setMaximum(1000);
    slider->setMinimum(-1000);
    slider->setSingleStep(50);
    slider->setOrientation(Qt::Horizontal);
    slider->setTickPosition(QSlider::TicksLeft);
    slider->setTickInterval(50);
    connect(slider, SIGNAL(valueChanged(int)), this, SLOT(updateSlider(int)));
    slider->setGeometry(base + (btn_width + diff)*4, 610, 300, btn_height);
    slider->setValue(imgratio);


}


void distortionWidget::distoration_tofile()
{
    try
    {
        dp->runD(map, (char*)img.bits(), img.width(), img.height(), 3);
        if (img.height()>2048)
        {

            int diff = (img.height() - 2048) / 2;

            img = img.copy(0, diff, img.width(), 2048);
        }
        img.save(QTime::currentTime().toString("hhmmsszzz")+".jpg");
    }
    catch(...)
    {
        if (img.height()>2048)
        {

            int diff = (img.height() - 2048) / 2;

            img = img.copy(0, diff, img.width(), 2048);
        }
        img.save(QTime::currentTime().toString("hhmmsszzz")+".jpg");
    }

}

void distortionWidget::distoration_refresh()
{
    updateImg();
    showImg();
}

void distortionWidget::updateImg()
{
    char* pimg = imgBuffer->getFrame2();
    memcpy(show_img, pimg, imgwidth*imgheight*3);
    img = QImage((uchar*)show_img, imgwidth, imgheight, QImage::Format_RGB888);
    dp->setD(map, imgwidth, imgheight, imgratio);
    if(_init)
    {
        _init = false;
    }
}

void distortionWidget::showImg()
{
    try
    {
        dp->runD(map, (char*)img.bits(), img.width(), img.height(), 3);
        lpic->setPixmap(QPixmap::fromImage(img.scaled(800, 600)));
    }
    catch(...)
    {
        lpic->setPixmap(QPixmap::fromImage(img.scaled(800, 600)));
    }
}




void distortionWidget::updateSlider(int p)
{
    imgratio = p;
    lRatioInfo->setNum(p);
    this->update();
}
