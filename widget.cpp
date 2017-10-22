#include "widget.h"
#include "ui_widget.h"
#include <plabel.h>
#include <QDebug>
#include <QWidget>
#include <QPainter>
#include <QPaintEvent>
#include <QPushButton>
#include "libEasyPlayerAPI.h"
#include <QDesktopWidget>
#include <QFileDialog>
#include <QTextCodec>
#include <QSlider>
#include <QSettings>
#include <QMessageBox>

int CALLBACK fun_callback( int _channelId, void *_channelPtr, int _frameType, char *pBuf, int bufSize, int width, int height)
{
    pLabel *pThis = (pLabel *)_channelPtr;
    QImage tmpImg = QImage((const uchar*)pBuf, width, height, QImage::Format_RGB555);
    if (height>2048)
    {
        int diff = (height - 2048) / 2;
        pThis->setShowImage(tmpImg.copy(0, diff, width, 2048));
    }
    else
    {
        pThis->setShowImage(tmpImg);
    }
    return 0;
}

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    QDesktopWidget desktop;
    ui->setupUi(this);
    codec = QTextCodec::codecForName("GB18030");
    readConfig();
    this->setWindowTitle(codec->toUnicode("视频图像处理工具"));
    width = desktop.availableGeometry().width() * 0.8;  // 显示宽度
    height = desktop.availableGeometry().height() * 0.8;  //显示高度
    funcRange = desktop.availableGeometry().width() * 0.1;  //功能区宽度
    float diffx = (desktop.availableGeometry().width() - width)/2; // 窗体起始位置
    float diffy = (desktop.availableGeometry().height() - height)/2; // 窗体起始位置
    this->setGeometry((int)diffx,(int)diffy,(int)(width+funcRange),(int)height);

    funcWidth = funcRange * 0.7;
    funcStart = (funcRange - funcWidth) / 2;
    nChannelID = 0;
    initWidget();
    EasyPlayer_Init();
    start();
}

void Widget::initWidget()
{
    plb = new pLabel(this, (int)width, (int)height);

    QPushButton *pb_drag_h1 = new QPushButton;
    pb_drag_h1->setText(codec->toUnicode("拖动横线1"));
    pb_drag_h1->setParent(this);
    pb_drag_h1->setGeometry(width+funcStart, 50, funcWidth, 25);
    connect(pb_drag_h1,SIGNAL(clicked()),this,SLOT(dragH1()));
    QPushButton *pb_drag_h2 = new QPushButton;
    pb_drag_h2->setText(codec->toUnicode("拖动横线2"));
    pb_drag_h2->setParent(this);
    pb_drag_h2->setGeometry(width+funcStart, 80, funcWidth, 25);
    connect(pb_drag_h2,SIGNAL(clicked()),this,SLOT(dragH2()));
    QPushButton *pb_drag_v1 = new QPushButton;
    pb_drag_v1->setText(codec->toUnicode("拖动竖线1"));
    pb_drag_v1->setParent(this);
    pb_drag_v1->setGeometry(width+funcStart, 110, funcWidth, 25);
    connect(pb_drag_v1,SIGNAL(clicked()),this,SLOT(dragV1()));
    QPushButton *pb_drag_v2 = new QPushButton;
    pb_drag_v2->setText(codec->toUnicode("拖动竖线2"));
    pb_drag_v2->setParent(this);
    pb_drag_v2->setGeometry(width+funcStart, 140, funcWidth, 25);
    connect(pb_drag_v2,SIGNAL(clicked()),this,SLOT(dragV2()));

    lbSliderInfo = new QLabel(this);
    lbSliderInfo->setGeometry(width+funcStart, 170, funcWidth, 25);
    lbSliderInfo->setText(codec->toUnicode("矫正系数：0"));


    slider = new QSlider(this);
    slider->setValue(ratio);
    slider->setMaximum(1000);
    slider->setMinimum(-1000);
    slider->setSingleStep(10);
    slider->setOrientation(Qt::Vertical);
    slider->setTickPosition(QSlider::TicksLeft);
    slider->setTickInterval(100);
    connect(slider, SIGNAL(valueChanged(int)), this, SLOT(updateSlider()));
    slider->setGeometry(width+funcStart+funcWidth*0.5, 200, 20, funcWidth*2.5);


    QLabel *lb = new QLabel(this);
    lb->setWordWrap(true);
    lb->setText(codec->toUnicode("ESC - 还原\n F - 加载图片\n S - 截图\n Q - 关闭\n 1 - 横线1\n 2 - 横线2\n 3 - 竖线1\n 4 - 竖线2"));
    lb->setGeometry(width+funcStart, 380, funcWidth, 300);
}

void Widget::updateSlider()
{
    ratio = slider->value();
    char _toStr[10];
    sprintf(_toStr, "%d", ratio);
    lbSliderInfo->setText(codec->toUnicode("矫正系数：") + codec->toUnicode(_toStr));

}

void Widget::saveConfig()
{
    QString iniFilePath = QDir::currentPath() + "/debug/video.ini";

    QSettings settings(iniFilePath,QSettings::IniFormat);
    settings.setValue("video/ratio", ratio);
}


void Widget::readConfig()
{
    QString iniFilePath = QDir::currentPath() + "/debug/video.ini";

    QSettings settings(iniFilePath,QSettings::IniFormat);
    ip = settings.value("video/ip").toString();
    user = settings.value("video/user").toString();
    pwd = settings.value("video/pwd").toString();
    ratio = settings.value("video/ratio").toInt();

}

void Widget::dragH1()
{
    this->plb->dragLine(LINE__H1);
}
void Widget::dragH2()
{
    this->plb->dragLine(LINE__H2);
}
void Widget::dragV1()
{
    this->plb->dragLine(LINE__V1);
}
void Widget::dragV2()
{
    this->plb->dragLine(LINE__V2);
}

Widget::~Widget()
{
    EasyPlayer_Release();
    qDebug() << ratio;
    saveConfig();
    delete ui;
}

void Widget::start()
{
//    HWND hwnd = (HWND)this->plb->winId();
//    nChannelID = EasyPlayer_OpenStream(ip.toLocal8Bit(), NULL, DISPLAY_FORMAT_RGB555, 0x01, user.toLocal8Bit(), pwd.toLocal8Bit(), NULL, fun_callback, this->plb);
    nChannelID = EasyPlayer_OpenStream(ip.toLocal8Bit(), NULL, DISPLAY_FORMAT_RGB555, 0x01, user.toLocal8Bit(), pwd.toLocal8Bit(), NULL, fun_callback, this->plb);
    if (nChannelID > 0)
    {
        EasyPlayer_SetFrameCache(nChannelID, 300);
    }
}

QString Widget::itOpenFile()
{
     QString filename=QFileDialog::getOpenFileName(NULL,
                                              tr("选择图像"),
                                              ".",
                                              tr("Images (*.png *.jpg)"));
     return filename;
}


int Widget::itReadFile(QString filepath, int mode)
{
    try
    {
        if(mode==0)
        {
            // pic

            if(filepath.isEmpty())
              {
                   return FUNCTION_FAIL;
              }
              else
              {
                  QImage* img=new QImage;

                  if(! ( img->load(filepath) ) ) //加载图像
                  {
                      QMessageBox::information(this,
                                               tr("打开图像失败"),
                                               tr("打开图像失败!"));
                      delete img;
                      return FUNCTION_FAIL;
                  }
                  this->plb->setPixmap(QPixmap::fromImage((*img).scaled(width,height)));
              }
        }
        else if(mode==1)
        {
            //txt
        }
        return FUNCTION_OK;
    }
    catch(...)
    {
        return FUNCTION_FAIL;
    }

}



void Widget::keyPressEvent(QKeyEvent *e)
{
    if (e->key()==Qt::Key_F) //load pic
    {
        this->plb->setDisplayMode(DISPLAY_IMAGE);
        EasyPlayer_CloseStream(nChannelID);
        QString _path = itOpenFile();
        itReadFile(_path, 0);
        qDebug() << "加载图片 -> " << _path;

    }
    else if (e->key()==Qt::Key_S)
    {
        this->plb->screenShot();
        qDebug() << "screenshot";
    }
    else if (e->key()==Qt::Key_R)
    {
        qDebug() << "刷新";
    }
    else if (e->key()==Qt::Key_Q)
    {

//        ui->statusBar->showMessage(QString("Quit"));
        qDebug() << "Quit";
        EasyPlayer_CloseStream(nChannelID);
        close();
    }
    else if (e->key()==Qt::Key_Escape) //
    {
        this->plb->setOpMode(OP_MODE_CREATE);
        this->plb->setLineMode(LINE_NONE);
        this->plb->setDisplayMode(DISPLAY_RTSP);
        qDebug() << "切换至视频流";
        start();

    }
    else if (e->key()==Qt::Key_1) //
    {
        qDebug() << "横线1";
        this->plb->setLineMode(LINE_H1);
    }
    else if (e->key()==Qt::Key_2) //
    {
        qDebug() << "横线2";
        this->plb->setLineMode(LINE_H2);
    }

    else if (e->key()==Qt::Key_3) //
    {
        qDebug() << "竖线1";
        this->plb->setLineMode(LINE_V1);
    }

    else if (e->key()==Qt::Key_4) //
    {
        qDebug() << "竖线2";
        this->plb->setLineMode(LINE_V2);
    }

}
