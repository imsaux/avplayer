#include "distortion_thread.h"
#include <global.h>

char*distortionImg = (char*)malloc(imgwidth*imgheight*3);
char*distortionImg_return = (char*)malloc(imgwidth*imgheight*3);
int tmp=imgratio;

distortion_thread::distortion_thread():QThread()
{
}

distortion_thread::~distortion_thread()
{
}


void distortion_thread::run()
{
    while(1)
    {
        char* p = imgBuffer->getFrame2();
        memcpy(distortionImg, p, imgwidth*imgheight*3);
        if (isOpenDistortion)
        {
            dp->runD(map, distortionImg, imgwidth, imgheight, 3);
        }
        memcpy(distortionImg_return, distortionImg,imgwidth*imgheight*3);
        emit newFrame1(distortionImg_return);
    }
}
