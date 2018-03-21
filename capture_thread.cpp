#include "capture_thread.h"
#include "global.h"

#include "libEasyPlayerAPI.h"
#include "inc/Distortion.h"

bool init = true;
int CALLBACK fun_callback( int _channelId, void *_channelPtr, int _frameType, char *pBuf, int isize, int width, int height)
{

    if (width>=0 && height>=0)
    {
        if (init)
        {
            imgheight = height;
            imgwidth = width;
            imgptr = _channelPtr;
            DistortionCreat(imgwidth, imgheight);
            //DistortionSet(imgwidth, imgheight, 1000);
            init = false;
        }
        if (imgheight != height)
        {
            DistortionDelete();
            DistortionCreat(imgwidth, height);
            imgheight = height;
        }
        if (imgwidth != width)
        {
            DistortionDelete();
            DistortionCreat(width, imgheight);
            imgwidth = width;
        }
        QImage img =QImage((const uchar*)pBuf, width, height, QImage::Format_RGB555);
//        QByteArray imageByteArray = QByteArray(pBuf, isize);
//        uchar*  transData = (unsigned char*)imageByteArray.data();
//        QImage img = QImage(transData, width, height, QImage::Format_RGB555);
//        DistortionExport(img1.bits(), width, height, 3);
        imgBuffer->addFrame(img);
    }
    return 0;
}


captureThread::captureThread(): QThread()
{
}

void captureThread::setDistortion(int w, int h, int r)
{
    DistortionSet(w, h, r);
}

void captureThread::saveConfig()
{
    QString iniFilePath = QDir::currentPath() + "/video.ini";

    QSettings settings(iniFilePath,QSettings::IniFormat);
    settings.setValue("video/ratio", imgratio);
}


void captureThread::readConfig()
{
    QString iniFilePath = QDir::currentPath() + "/debug/video.ini";

    QSettings settings(iniFilePath,QSettings::IniFormat);
    ip = settings.value("video/ip").toString();
    user = settings.value("video/user").toString();
    pwd = settings.value("video/pwd").toString();
    ratio = settings.value("video/ratio").toInt();
    imgratio = ratio;
}

void captureThread::run()
{
    connectRTSP();
}



void captureThread::connectRTSP()
{
    EasyPlayer_Init();
    this->readConfig();
    nChannelID = EasyPlayer_OpenStream(ip.toLocal8Bit(), NULL, DISPLAY_FORMAT_RGB555, 0x01, user.toLocal8Bit(), pwd.toLocal8Bit(), NULL, fun_callback, this);
    if (nChannelID > 0)
    {
        EasyPlayer_SetFrameCache(nChannelID, 3);
//        EasyPlayer_SetDecodeType(nChannelID, 1);
    }

}

void captureThread::stopRTSP()
{
    EasyPlayer_CloseStream(nChannelID);
    EasyPlayer_Release();
    saveConfig();
}
