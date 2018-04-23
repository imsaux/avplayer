#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <capture_thread.h>
#include <distortion_thread.h>

class Controller : public QObject
{
    Q_OBJECT

public:
    Controller();
    ~Controller();
    bool Start();
    captureThread *_capture_thread;
    distortion_thread *_distortion_thread;
//    BGR2RGB_thread *_bgr2rgb_thread;
//    TOIMG_thread *_toimg_thread;

};

#endif // CONTROLLER_H
