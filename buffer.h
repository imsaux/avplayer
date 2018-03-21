#ifndef BUFFER_H
#define BUFFER_H

#include <QMutex>
#include <QQueue>
#include <QSemaphore>
#include <QImage>
#include <QDebug>

class buffer
{
public:
    buffer(int size, bool dropFrame);
    void addFrame(const QImage& img);
    QImage getFrame();
    void clearBuffer();
    int getSizeOfImageBuffer();
private:
    QMutex imageQueueProtect;
    QQueue<QImage> imageQueue;//Ö¡»º³åÇø¶ÓÁÐ
    QSemaphore *freeSlots;
    QSemaphore *usedSlots;
    QSemaphore *clearBuffer1;
    QSemaphore *clearBuffer2;
    int bufferSize;
    bool dropFrame;
};
#endif // BUFFER_H
