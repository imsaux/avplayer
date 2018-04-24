#include <widget.h>
#include <ui_widget.h>

bool dw_init_bit=true;
char* finalImg=(char*)malloc(imgwidth*imgheight*3);

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{    
    ui->setupUi(this);
    codec = QTextCodec::codecForName("GB18030");
    this->setWindowTitle(codec->toUnicode("图像采集处理工具"));
    this->setWindowState(Qt::WindowMaximized);
    controller = new Controller;
    winwidth = QApplication::desktop()->availableGeometry().width();
    winheight = QApplication::desktop()->availableGeometry().height() - 50;
    readConfig();
    map= dp->initD(imgwidth, imgheight, imgratio);
    dw=new distortionWidget();
    hw=new helpWidget();

    initWidget();
    startMonitor();
}

void Widget::initWidget()
{
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

    QPushButton *pb_show_help = new QPushButton;
    pb_show_help->setText(codec->toUnicode("帮助"));
    pb_show_help->setParent(this);
    pb_show_help->setGeometry(base + (btn_width + diff)*5, winheight, btn_width, btn_height);
    connect(pb_show_help, SIGNAL(clicked()), this, SLOT(showHelpWidget()));

    QPushButton *pb_show_distoration = new QPushButton;
    pb_show_distoration->setText(codec->toUnicode("畸变校正设置"));
    pb_show_distoration->setParent(this);
    pb_show_distoration->setGeometry(base + (btn_width + diff)*4, winheight, btn_width, btn_height);
    connect(pb_show_distoration, SIGNAL(clicked()), this, SLOT(showDistorationDialog()));
//    connect(pb_drag_v2,SIGNAL(clicked()),this,SLOT(dragV2()));

    QCheckBox *cb_distortion = new QCheckBox(this);
    cb_distortion->setGeometry(base + (btn_width + diff)*6, winheight, btn_width, btn_height);
    cb_distortion->setText(codec->toUnicode("开启校正"));
    cb_distortion->setChecked(isOpenDistortion);
    connect(cb_distortion, SIGNAL(stateChanged(int)), this, SLOT(cbChange(int)));
//    lbSliderInfo = new QLabel(this);
//    lbSliderInfo->setGeometry(base + (btn_width + diff)*4, winheight, btn_width, btn_height);
//    lbSliderInfo->setText(codec->toUnicode("校正系数：0"));



//    QLabel *lb = new QLabel(this);
//    lb->setWordWrap(true);
//    lb->setText(codec->toUnicode("ESC - 还原\n  F - 加载图片\n  S - 截图\n  Q - 关闭\n  1 - 横线1\n  2 - 横线2\n  3 - 竖线1\n  4 - 竖线2"));
//    lb->setGeometry(width+funcStart, 580, funcWidth, 300);
}

void Widget::cbChange(int state)
{
    if (state==Qt::CheckState::Unchecked)
    {
        isOpenDistortion = false;
    }
    else if(state==Qt::CheckState::Checked)
    {
        isOpenDistortion = true;
    }
}

void Widget::showDistorationDialog()
{
    dw->show();
}


void Widget::showHelpWidget()
{
    hw->show();
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

void Widget::closeEvent(QCloseEvent *event)
{
    dw->close();
    hw->close();
}

Widget::~Widget()
{
    map=NULL;
    if (controller->_capture_thread->isRunning())
    {
        controller->_capture_thread->exit();
    }
    if (controller->_distortion_thread->isRunning())
    {
        controller->_distortion_thread->exit();
    }
    dp->delD(map);
    saveConfig();
    delete ui;
}

void Widget::startMonitor()
{
    if(controller->Start())     //参数的意义见Imagebuffer的定义
    {
        //将处理线程的“产生新的一帧”信号和GUI线程的“更新帧”槽连接
        connect(controller->_distortion_thread,SIGNAL(newFrame1(char*)),this,SLOT(updateFrame1(char*)));
//        connect(controller->_toimg_thread,SIGNAL(newFrame2(char*)),this,SLOT(updateFrame2(char*)));
//        connect(controller->_show_thread,SIGNAL(newFrame(QImage)),this,SLOT(updateFrame(QImage)));
    }
    // Display error dialog if camera connection is unsuccessful
    else
    {
        QMessageBox::warning(this,"ERROR:","Could not connect to camera.");
    }
}

void Widget::updateFrame1(char* p)
{
    memcpy(finalImg, p, imgwidth*imgheight*3);
    img = QImage((uchar*)finalImg, imgwidth, imgheight, QImage::Format_RGB888);
    plb->setShowImage(img);
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
        QString _path = itOpenFile();
        itReadFile(_path, 0);
        qDebug() << "加载图片 -> " << _path;

    }
    else if (e->key()==Qt::Key_S)
    {
        this->plb->screenShot();
//        showDistorationDialog();
        qDebug() << "screenshot";
    }
    else if (e->key()==Qt::Key_R)
    {
        plb->resetImg();
        qDebug() << "刷新";
    }
    else if (e->key()==Qt::Key_Q)
    {
        close();
    }
    else if (e->key()==Qt::Key_Escape) //
    {
        plb->setOpMode(OP_MODE_CREATE);
        plb->setLineMode(LINE_NONE);
        plb->setDisplayMode(DISPLAY_RTSP);
        qDebug() << "切换至视频流";
        plb->resetImg();

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
    else if(e->key()==Qt::Key_F1)
    {
        showHelpWidget();
    }

}

void Widget::readConfig()
{
    QDir tmpPath("./video.ini");
    QSettings settings(tmpPath.absolutePath(),QSettings::IniFormat);
    rtspip = settings.value("video/ip").toString();
    rtspuser = settings.value("video/user").toString();
    rtsppwd = settings.value("video/pwd").toString();
    imgratio = settings.value("video/ratio").toInt();
}



void Widget::saveConfig()
{
    QDir tmpPath("./video.ini");
    QSettings settings(tmpPath.absolutePath(),QSettings::IniFormat);
    settings.setValue("video/ratio", imgratio);
    qDebug()<<"imgratio >>" << imgratio;
    qDebug() <<"path >>" << tmpPath.absolutePath();
}
