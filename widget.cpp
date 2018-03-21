#include <widget.h>
#include <ui_widget.h>


Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
    codec = QTextCodec::codecForName("GB18030");
    this->setWindowTitle(codec->toUnicode("数据采集处理工具"));
    this->setWindowState(Qt::WindowMaximized);
    controller = new Controller;
    winwidth = QApplication::desktop()->availableGeometry().width();
    winheight = QApplication::desktop()->availableGeometry().height() - 50;

//    qDebug() << "this->geometry()";
//    qDebug() << winwidth;
//    qDebug() << winheight;
    initWidget();
    startMonitor();
}

void Widget::initWidget()
{
//    qDebug() << "this->initWidget()";
//    qDebug() << winwidth;
//    qDebug() << winheight;
    plb = new pLabel(this, (int)winwidth, (int)winheight);
    uint base = 20, diff = 25, btn_width = 75, btn_height = 25;
    QPushButton *pb_drag_h1 = new QPushButton;
    pb_drag_h1->setText(codec->toUnicode("拖动横线1"));
    pb_drag_h1->setParent(this);
    pb_drag_h1->setGeometry(base, winheight, btn_width, btn_height);
    connect(pb_drag_h1,SIGNAL(clicked()),this,SLOT(dragH1()));
    QPushButton *pb_drag_h2 = new QPushButton;
    pb_drag_h2->setText(codec->toUnicode("拖动横线2"));
    pb_drag_h2->setParent(this);
    pb_drag_h2->setGeometry(base + (btn_width + diff)*1, winheight, btn_width, btn_height);
    connect(pb_drag_h2,SIGNAL(clicked()),this,SLOT(dragH2()));
    QPushButton *pb_drag_v1 = new QPushButton;
    pb_drag_v1->setText(codec->toUnicode("拖动竖线1"));
    pb_drag_v1->setParent(this);
    pb_drag_v1->setGeometry(base + (btn_width + diff)*2, winheight, btn_width, btn_height);
    connect(pb_drag_v1,SIGNAL(clicked()),this,SLOT(dragV1()));
    QPushButton *pb_drag_v2 = new QPushButton;
    pb_drag_v2->setText(codec->toUnicode("拖动竖线2"));
    pb_drag_v2->setParent(this);
    pb_drag_v2->setGeometry(base + (btn_width + diff)*3, winheight, btn_width, btn_height);
    connect(pb_drag_v2,SIGNAL(clicked()),this,SLOT(dragV2()));

    lbSliderInfo = new QLabel(this);
    lbSliderInfo->setGeometry(base + (btn_width + diff)*4, winheight, btn_width, btn_height);
    lbSliderInfo->setText(codec->toUnicode("校正系数：0"));

    slider = new QSlider(this);
    slider->setMaximum(1000);
    slider->setMinimum(-1000);
    slider->setSingleStep(10);
    slider->setOrientation(Qt::Horizontal);
    slider->setTickPosition(QSlider::TicksLeft);
    slider->setTickInterval(50);
    connect(slider, SIGNAL(valueChanged(int)), this, SLOT(updateSlider()));
    slider->setGeometry(base + (btn_width + diff)*5, winheight, 150, btn_height);
    slider->setValue(imgratio);


//    QLabel *lb = new QLabel(this);
//    lb->setWordWrap(true);
//    lb->setText(codec->toUnicode("ESC - 还原\n  F - 加载图片\n  S - 截图\n  Q - 关闭\n  1 - 横线1\n  2 - 横线2\n  3 - 竖线1\n  4 - 竖线2"));
//    lb->setGeometry(width+funcStart, 580, funcWidth, 300);
}

void Widget::updateSlider()
{
    imgratio = slider->value();
    try {
        controller->setShowThreadDistortion(imgwidth, imgheight, imgratio);
        char _toStr[10];
        sprintf(_toStr, "%d", imgratio);
        lbSliderInfo->setText(codec->toUnicode("ratio:") + codec->toUnicode(_toStr));
    }
    catch(...)
    {
        imgratio = 0;
        lbSliderInfo->setText(codec->toUnicode("ratio: 0"));
    }
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
    delete ui;
}

void Widget::startMonitor()
{
    if(controller->Start())     //参数的意义见Imagebuffer的定义
    {
        //将处理线程的“产生新的一帧”信号和GUI线程的“更新帧”槽连接
        connect(controller->_show_thread,SIGNAL(newFrame(QImage)),this,SLOT(updateFrame(QImage)));
    }
    // Display error dialog if camera connection is unsuccessful
    else
    {
        QMessageBox::warning(this,"ERROR:","Could not connect to camera.");
    }
}

void Widget::updateFrame(const QImage &frame)
{
    plb->setShowImage(frame);
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
                  plb->setPixmap(QPixmap::fromImage((*img).scaled(winwidth,winheight)));
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
        plb->setDisplayMode(DISPLAY_IMAGE);
//        EasyPlayer_CloseStream(nChannelID);
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
        startMonitor();
        qDebug() << "刷新";
    }
    else if (e->key()==Qt::Key_Q)
    {

//        ui->statusBar->showMessage(QString("Quit"));
        qDebug() << "Quit";
//        EasyPlayer_CloseStream(nChannelID);
        close();
    }
    else if (e->key()==Qt::Key_Escape) //
    {
        plb->setOpMode(OP_MODE_CREATE);
        plb->setLineMode(LINE_NONE);
        plb->setDisplayMode(DISPLAY_RTSP);
        qDebug() << "切换至视频流";
        startMonitor();

    }
    else if (e->key()==Qt::Key_1) //
    {
        qDebug() << "横线1";
        plb->setLineMode(LINE_H1);
    }
    else if (e->key()==Qt::Key_2) //
    {
        qDebug() << "横线2";
        plb->setLineMode(LINE_H2);
    }

    else if (e->key()==Qt::Key_3) //
    {
        qDebug() << "竖线1";
        plb->setLineMode(LINE_V1);
    }

    else if (e->key()==Qt::Key_4) //
    {
        qDebug() << "竖线2";
        plb->setLineMode(LINE_V2);
    }

}
