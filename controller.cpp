#include "controller.h"


Controller::Controller()
{
} // Controller constructor

Controller::~Controller()
{
} // Controller destructor

void Controller::setShowThreadDistortion(int w, int h, int r)
{
    _capture_thread->setDistortion(w,h,r);
}

bool Controller::Start()
{
    _capture_thread = new captureThread();
    _show_thread = new showThread();
    _capture_thread->start((QThread::Priority)7);//启动捕获进程
    _show_thread->start((QThread::Priority)7);//启动处理进程
    return true;
}
