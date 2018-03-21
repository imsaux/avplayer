#include "show_thread.h"


#include <global.h>
showThread::showThread():QThread()
{

}

void showThread::run()
{
    while(1)
    {
        QImage img = imgBuffer->getFrame();

        if (img.height()>2048)
        {
            int diff = (img.height() - 2048) / 2;
            img = img.copy(0, diff, img.width(), 2048);
        }
        emit newFrame(img);
    }
}
