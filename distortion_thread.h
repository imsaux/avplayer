#ifndef DISTORTION_THREAD_H
#define DISTORTION_THREAD_H

#include <QThread>
#include <buffer.h>


class distortion_thread: public QThread
{
    Q_OBJECT
public:
    distortion_thread();
    ~distortion_thread();
protected:
    void run();
signals:
    void newFrame1(char*);
};

#endif // DISTORTION_THREAD_H
