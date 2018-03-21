#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <capture_thread.h>
#include <show_thread.h>

class Controller : public QObject
{
    Q_OBJECT

public:
    Controller();
    ~Controller();
    bool Start();
    void setShowThreadDistortion(int,int,int);
    showThread *_show_thread;
    captureThread *_capture_thread;

};

#endif // CONTROLLER_H
