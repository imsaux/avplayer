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
    void addFrame1(const QImage& img);
    QImage getFrame1();
    char* getFrame2();
    void addFrame2(char*);
    void clearBuffer();
    int getSizeOfImageBuffer1();
    int getSizeOfImageBuffer2();
private:
    QMutex imageQueueProtect;
    QQueue<QImage> imageQueue1;//帧缓冲区队列
    QQueue<char*> imageQueue2;//帧缓冲区队列
    QSemaphore *freeSlots;
    QSemaphore *usedSlots;
    QSemaphore *clearBuffer1;
    QSemaphore *clearBuffer2;
    int bufferSize;
    bool dropFrame;
};
#endif // BUFFER_H
