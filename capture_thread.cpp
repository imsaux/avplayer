#include "capture_thread.h"
#include "global.h"
#include "libEasyPlayerAPI.h"
bool init = true;
char* ptrBGR2RGB = (char*)malloc(imgwidth*imgheight*3);
void bgr2rgb(char* p, int size)
{
    char tmpValue = 0;
    for(int i = 0; i < size; i+=3)
    {

        tmpValue = p[i];
        p[i] = p[i + 2];
        p[i + 2] = tmpValue;
    }
}

int CALLBACK fun_callback( int _channelId, void *_channelPtr, int _frameType, char *pBuf, int isize, int width, int height)
{
    if (width>=0 && height>=0 && isize>0)
    {
        if (init)
        {
            imgheight = height;
            imgwidth = width;
            if(ptrBGR2RGB!=NULL)
            {
                ptrBGR2RGB = (char*)realloc(ptrBGR2RGB, isize);
                memset(ptrBGR2RGB, 0, isize);
            }
            init = false;
        }
        if (imgheight != height)
        {
            imgheight = height;
        }
        if (imgwidth != width)
        {
            imgwidth = width;
        }
        if (ptrBGR2RGB!=NULL)
        {
            bgr2rgb(pBuf, isize);
            memcpy(ptrBGR2RGB, pBuf, imgwidth*imgheight*3);
            imgBuffer->addFrame2(ptrBGR2RGB);

        }
        else
        {
            qDebug() << "ptrBGR2RGB is NULL";
        }
    }

    return 0;
}


captureThread::captureThread(): QThread()
{
}




void captureThread::run()
{
    connectRTSP();
}



void captureThread::connectRTSP()
{
    EasyPlayer_Init();
    nChannelID = EasyPlayer_OpenStream(rtspip.toLocal8Bit(), NULL, DISPLAY_FORMAT_RGB24_GDI, 0x01, rtspuser.toLocal8Bit(), rtsppwd.toLocal8Bit(), NULL, fun_callback, this);
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
}
