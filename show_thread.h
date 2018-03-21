#ifndef SHOW_THREAD_H
#define SHOW_THREAD_H


#include <QThread>
#include <buffer.h>

class showThread: public QThread
{
    Q_OBJECT
public:
    showThread();
protected:
    void run();
signals:
    void newFrame(const QImage &frame);
//    void newName(const QString &name);
};

#endif // SHOW_THREAD_H
